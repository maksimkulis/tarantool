#!/usr/bin/env tarantool

box.cfg({
    listen              = os.getenv("LISTEN"),
    replication         = os.getenv("MASTER"),
    memtx_memory        = 107374182,
    wal_mode            = 'none',
    replication_timeout = 0.1,
    replication_connect_timeout = 0.5,
})

require('console').listen(os.getenv('ADMIN'))
