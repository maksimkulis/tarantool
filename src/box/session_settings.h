#pragma once
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

/**
 * Identifiers of all session setings. The identifier of the
 * option is equal to its place in the sorted list of session
 * options.
 *
 * It is IMPORTANT that these options are sorted by name. If this
 * is not the case, the result returned by the _session_settings
 * space iterator will not be sorted properly.
 */
enum {
	sql_session_setting_BEGIN = 0,
	SQL_SESSION_SETTING_DEFAULT_ENGINE = sql_session_setting_BEGIN,
	SQL_SESSION_SETTING_DEFER_FOREIGN_KEYS,
	SQL_SESSION_SETTING_FULL_COLUMN_NAMES,
	SQL_SESSION_SETTING_FULL_METADATA,
	SQL_SESSION_SETTING_PARSER_DEBUG,
	SQL_SESSION_SETTING_RECURSIVE_TRIGGERS,
	SQL_SESSION_SETTING_REVERSE_UNORDERED_SELECTS,
	SQL_SESSION_SETTING_SELECT_DEBUG,
	SQL_SESSION_SETTING_VDBE_DEBUG,
	sql_session_setting_END = SQL_SESSION_SETTING_VDBE_DEBUG,
	session_setting_MAX,
};

struct session_setting_metadata {
	unsigned field_type;
	unsigned mask;
};

struct session_setting {
	const char *name;
	struct session_setting_metadata metadata;
	/**
	 * Get a MessagePack encoded pair [name, value] for a
	 * setting having index @a id. Index is from the settings
	 * array.
	 */
	void (*get)(int id, const char **mp_pair, const char **mp_pair_end);
	/**
	 * Set value of a setting by a given @a id from a
	 * MessagePack encoded buffer. Note, it is not a pair, but
	 * just value.
	 */
	int (*set)(int id, const char *mp_value);
};

extern struct session_setting session_settings[];
