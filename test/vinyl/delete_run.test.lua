test_run = require('test_run').new()
fiber = require('fiber')

-- Test 1
test_run:cmd("create server test with script='vinyl/force_recovery_true.lua'")
test_run:cmd("start server test with args='1'")
test_run:cmd("switch test")

test = box.schema.space.create('test', {engine='vinyl'})
_ = test:create_index('pk')
for i = 1, 10 do test:insert{i, i + 100} box.snapshot() end
test:select()

fio = require ('fio')
for _, f in pairs(fio.glob(box.cfg.vinyl_dir .. '/' .. test.id .. '/0/*26*')) do fio.unlink(f) end

test_run:cmd('switch default')
test_run:cmd('restart server test with args="1"')
test_run:cmd('switch test')
box.cfg.force_recovery
box.space.test:select()

test_run:cmd('switch default')
test_run:cmd('restart server test with args="0"')
test_run:cmd('switch test')
box.cfg.force_recovery
box.space.test:select()

test_run:cmd('switch default')
test_run:cmd('stop server test')
test_run:cmd('cleanup server test')
test_run:cmd('delete server test')

-- Test 2
test_run = require('test_run').new()
test_run:cmd("create server test with script='vinyl/force_recovery_true.lua'")
test_run:cmd("start server test with args='1'")
test_run:cmd("switch test")

test = box.schema.space.create('test', {engine='vinyl'})
_ = test:create_index('pk')
_ = test:insert{1, "123"}
test:select()
box.snapshot()

fio = require ('fio')
for _, f in pairs(fio.glob(box.cfg.vinyl_dir .. '/' .. test.id .. '/0/*.index')) do fio.unlink(f) end

test_run:cmd('switch default')
test_run:cmd('restart server test with args="1"')
test_run:cmd('switch test')
box.space.test:select()

test_run:cmd('switch default')
test_run:cmd('restart server test with args="0"')
test_run:cmd('switch test')
box.space.test:select()

test_run:cmd('switch default')
test_run:cmd('stop server test')
test_run:cmd('cleanup server test')
test_run:cmd('delete server test')

-- Test3

test_run = require('test_run').new()
test_run:cmd("create server test with script='vinyl/force_recovery_true.lua'")
test_run:cmd("start server test")
test_run:cmd("switch test")

test = box.schema.space.create('test', {engine='vinyl'})
_ = test:create_index('pk')
_ = test:insert{1, "123"}
test:select(1)
box.snapshot()

fio = require ('fio')
for _, f in pairs(fio.glob(box.cfg.vinyl_dir .. '/' .. test.id .. '/0/*.run')) do fio.unlink(f) end

test_run:cmd('switch default')
test_run:cmd('restart server test with args="1"')
test_run:cmd('switch test')
box.space.test:select()

test_run:cmd('switch default')
test_run:cmd('restart server test with args="0"')
test_run:cmd('switch test')
box.cfg.force_recovery
box.space.test:select()
test_run:cmd('switch default')
test_run:cmd('stop server test')
test_run:cmd('cleanup server test')
test_run:cmd('delete server test')
