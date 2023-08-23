#pragma once

#include "lauxlib.h"
#include "lua.h"
#include <stdio.h>

#include "../bar_manager.h"
#include "../misc/env_vars.h"

#define LFUN(n) static int n(lua_State *L)

void dump_lua_stack(lua_State *L);
void process_animation(lua_State *L, struct bar_manager *bm);
void env_vars_to_lua_table(lua_State *L, struct env_vars *env_vars);
