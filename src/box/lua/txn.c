/*
 * Copyright 2010-2019, Tarantool AUTHORS, please see AUTHORS file.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * 1. Redistributions of source code must retain the above
 *    copyright notice, this list of conditions and the
 *    following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * <COPYRIGHT HOLDER> OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdint.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <tarantool_ev.h>

#include "box/box.h"
#include "box/schema.h"
#include "box/txn.h"

#include "box/lua/txn.h"

#include "lua/utils.h"
#include "lua/trigger.h"


static uint32_t CTID_STRUCT_TXN_SAVEPOINT_REF = 0;


static int
lbox_txn_begin(struct lua_State *L)
{
	int res = box_txn_begin();
	if (res == -1)
		return luaT_push_nil_and_error(L);

	lua_pushnumber(L, 0);
	return 1;
}

static int
lbox_txn_is_in_txn(struct lua_State *L)
{
	bool res = box_txn();
	lua_pushboolean(L, res);
	return 1;
}

static int
lbox_txn_id(struct lua_State *L)
{
	int64_t res = box_txn_id();

	if (res == -1)
		return luaT_push_nil_and_error(L);

	lua_pushnumber(L, res);
	return 1;
}

struct txn_savepoint*
luaT_check_txn_savepoint(struct lua_State *L, int idx)
{
	if (lua_type(L, idx) != LUA_TCDATA)
		return NULL;

	uint32_t cdata_type;
	struct txn_savepoint **sp_ptr = luaL_checkcdata(L, idx, &cdata_type);

	if (sp_ptr == NULL || cdata_type != CTID_STRUCT_TXN_SAVEPOINT_REF)
		return NULL;

	return *sp_ptr;
}

static int 
lbox_txn_rollback_to_savepoint(struct lua_State *L)
{
	struct txn_savepoint *sp;
	if (lua_gettop(L) != 1
	    || (sp = luaT_check_txn_savepoint(L, 1)) == NULL) {
		 luaL_error(L, "Usage: txn:rollback to savepoint(savepoint)");
	}

	int rc = box_txn_rollback_to_savepoint(sp);
	if (rc != 0)
		return luaT_push_nil_and_error(L);

	lua_pushnumber(L, 0);
	return 1;
}

static int
lbox_txn_savepoint(struct lua_State *L)
{
	struct txn_savepoint *sp = box_txn_savepoint();
	if (sp == NULL)
		return luaT_push_nil_and_error(L);

	lua_pushlightuserdata(L, sp);
	*(struct txn_savepoint **)luaL_pushcdata(L, CTID_STRUCT_TXN_SAVEPOINT_REF) = sp;
	return 1;
}

static const struct luaL_Reg lbox_txn_lib[] = {
	{ "begin",			lbox_txn_begin			},
	{ "is_in_txn",			lbox_txn_is_in_txn		},
	{ "txn_id",			lbox_txn_id			},
	{ "savepoint",			lbox_txn_savepoint		},
	{ "rollback_to_savepoint",	lbox_txn_rollback_to_savepoint	},
	{ NULL,				NULL				}
};

void
box_lua_txn_init(struct lua_State *L)
{
	luaL_cdef(L, "struct txn_savepoint;");
	CTID_STRUCT_TXN_SAVEPOINT_REF = luaL_ctypeid(L, "struct txn_savepoint&");

	luaL_register_module(L, "box.txn", lbox_txn_lib);
	lua_pop(L, 1);
}
