-- preparatory stuff
env = require('test_run')
test_run = env.new()

test_run:cmd("restart server default with cleanup=True")
box.schema.user.grant('guest', 'replication')
_ = box.schema.space.create('test')
_ = box.space.test:create_index('pk')
--
-- reopen xlog
--
test_run:cmd("restart server default")
box.error.injection.set("ERRINJ_WAL_MEM_IGNORE", true)
box.space.test ~= nil
-- insert some stuff
-- 
box.space.test:auto_increment{'before snapshot'}
--
-- this snapshot will go to the replica
--
box.internal.wal_rotate() box.snapshot()
-- 
-- create a replica, let it catch up somewhat
--
test_run:cmd("create server replica with rpl_master=default, script='xlog/replica.lua'")
test_run:cmd("start server replica")
test_run:cmd("switch replica")
box.space.test:select{}
-- 
-- stop replica, restart the master, insert more stuff
-- which will make it into an xlog only
--
test_run:cmd("switch default")
test_run:cmd("stop server replica")
test_run:cmd("restart server default")
box.error.injection.set("ERRINJ_WAL_MEM_IGNORE", true)
box.space.test:auto_increment{'after snapshot'}
box.space.test:auto_increment{'after snapshot - one more row'}
--
-- save snapshot and remove xlogs
-- 
box.snapshot() box.internal.wal_rotate()
fio = require('fio')
glob = fio.pathjoin(box.cfg.wal_dir, '*.xlog')
files = fio.glob(glob)
for _, file in pairs(files) do fio.unlink(file) end
--
-- make sure the server has some xlogs, otherwise the
-- replica doesn't discover the gap in the logs
--
box.space.test:auto_increment{'after snapshot and restart'}
box.space.test:auto_increment{'after snapshot and restart - one more row'}
--  
--  check that panic is true
--
box.cfg{force_recovery=false}
box.cfg.force_recovery
-- 
-- try to start the replica, ha-ha
-- (replication should fail, some rows are missing)
--
test_run:cmd("start server replica")
test_run:cmd("switch replica")
fiber = require('fiber')
while box.info.replication[1].upstream.status ~= "stopped" do fiber.sleep(0.001) end
box.info.replication[1].upstream.status
box.info.replication[1].upstream.message
box.space.test:select{}
--
--
test_run:cmd("switch default")
box.error.injection.set("ERRINJ_WAL_MEM_IGNORE", false)
test_run:cmd("stop server replica")
test_run:cmd("cleanup server replica")
--
-- cleanup
box.space.test:drop()
box.schema.user.revoke('guest', 'replication')
