#pragma once

#include "helper.h"
#include "lua.h"
#include <lauxlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <uuid/uuid.h>

#include "../bar_manager.h"

extern struct bar_manager g_bar_manager;

// animate(value, curve, duration)
LFUN(luautil_animate) {
    lua_createtable(L, 0, 3); // [value, curve, duration, {}]

    lua_insert(L, -2); // [value, curve, {}, duration]
    lua_pushstring(L, "duration");
    lua_insert(L, -2);   // [value, curve, {}, "duration", duration]
    lua_settable(L, -3); // [value, curve, {duration}];

    lua_insert(L, -2); // [value, {duration}, curve];
    lua_pushstring(L, "curve");
    lua_insert(L, -2);   // [value, {duration}, "curve", curve];
    lua_settable(L, -3); // [value, {duration, curve}];

    lua_insert(L, -2); // [{duration, curve}, value];
    lua_pushstring(L, "value");
    lua_insert(L, -2);   // [{duration, curve}, "value", value]
    lua_settable(L, -3); // [{duration, curve, value}]

    return 1;
}

// XXX: Remove, just for testing uuid and registry system
LFUN(luautil_set_bar_script) {
    char *name = strdup(lua_tostring(L, 1));
    int index = bar_manager_get_item_index_for_name(&g_bar_manager, name);

    struct bar_item *item = g_bar_manager.bar_items[index];

    uuid_t uuid;
    uuid_generate(uuid);

    // bar_item_set_script(item, uuid);
    uuid_copy(item->lua_script, uuid);

    lua_pushlightuserdata(L, item->lua_script);
    lua_insert(L, -2);

    lua_rawset(L, LUA_REGISTRYINDEX);

    return 0;
}

// XXX: Remove, just for testing uuid and registry system
LFUN(luautil_set_bar_click_script) {
    char *name = strdup(lua_tostring(L, 1));
    int index = bar_manager_get_item_index_for_name(&g_bar_manager, name);

    struct bar_item *item = g_bar_manager.bar_items[index];

    uuid_t uuid;
    uuid_generate(uuid);

    uuid_copy(item->lua_click_script, uuid);

    lua_pushlightuserdata(L, item->lua_click_script);
    lua_insert(L, -2);

    lua_rawset(L, LUA_REGISTRYINDEX);

    return 0;
}

LFUN(luautil_add_hello) {
    struct bar_item *item = bar_manager_create_item(&g_bar_manager);
    bar_item_set_name(item, "hello");
    bar_item_set_position(item, "left");
    bar_item_needs_update(item);

    return 0;
}

static const luaL_Reg lua_util_funcs[] = {
    {"animate", luautil_animate},
    {"script", luautil_set_bar_script},
    {"click_script", luautil_set_bar_click_script},
    {"add", luautil_add_hello},
    {NULL, NULL}};

void load_lua_util_lib(lua_State *L) {
    luaL_register(L, "util", lua_util_funcs);
}
