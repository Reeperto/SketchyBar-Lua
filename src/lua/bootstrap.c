#include <lauxlib.h>
#include <lua.h>
#include <luajit.h>
#include <lualib.h>

#include "lbar.h"
#include "lutil.h"

lua_State *create_lua_state() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    load_lua_bar_lib(L);
    load_lua_util_lib(L);

    lua_settop(L, 0);

    return L;
}
