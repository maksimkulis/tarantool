-- txn.lua (internal file)
-- luacheck: ignore box

box.begin = function()
    local res, err = box.txn.begin()
    if res == nil then
        error(err)
    end
end

box.is_in_txn = function()
    return box.txn.is_in_txn()
end

box.savepoint = function()
    local res, err = box.txn.savepoint()
    if res == nil then
        error(err)
    end
    return { csavepoint = res, txn_id = box.txn.txn_id() }
end

box.rollback_to_savepoint = function(savepoint)
    if savepoint == nil then
        error("Usage: txn:rollback to savepoint(savepoint)")
    end

    if savepoint.txn_id ~= box.txn.txn_id() then
        box.error(box.error.NO_SUCH_SAVEPOINT)
    end

    local res, err = box.txn.rollback_to_savepoint(savepoint.csavepoint)
    if res == nil then
            error(err)
    end
end

local function atomic_tail(status, ...)
    if not status then
        box.rollback()
        error((...), 2)
     end
     box.commit()
     return ...
end

box.atomic = function(fun, ...)
    box.begin()
    return atomic_tail(pcall(fun, ...))
end

-- box.commit yields, so it's defined as Lua/C binding
-- box.rollback yields as well
