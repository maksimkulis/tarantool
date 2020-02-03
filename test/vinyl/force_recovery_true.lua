#!/usr/bin/env tarantool

local fr = tonumber(arg[1])
box.cfg ({
    listen = os.getenv("LISTEN"),
    replication = os.getenv("MASTER"),
    vinyl_memory = 128 * 1024 * 1024,
    force_recovery = (fr == 1),
})

require('console').listen(os.getenv('ADMIN'))
