#include "lauxlib.h"
#include "lua.h"
#include <stdio.h>

#include "../bar_manager.h"

// static inline void print_non_table_lua_type(lua_State *L) {
//     int t = lua_type(L, -1);
//     switch (t) {

//     case LUA_TSTRING: /* strings */
//         printf("'%s'", lua_tostring(L, -1));
//         break;

//     case LUA_TBOOLEAN: /* booleans */
//         printf(lua_toboolean(L, -1) ? "true" : "false");
//         break;

//     case LUA_TNUMBER: /* numbers */
//         printf("%g", lua_tonumber(L, -1));
//         break;

//     default: /* other values */
//         printf("%s", lua_typename(L, t));
//         break;
//     }
// }

// static inline void print_lua_table(lua_State *L) {
//     int indentation = 1;
//     printf("{\n");
//     lua_pushnil(L);
//     while (lua_next(L, -2)) {
//         printf("%*s", indentation * 2, "");
//         lua_pushvalue(L, -2);
//         lua_insert(L, -2);

//         const char *key = lua_tostring(L, -2);
//         printf("[%s] = ", key);

//         if (lua_istable(L, -1)) {
//             ++indentation;
//             print_lua_table(L);
//         } else {
//             print_non_table_lua_type(L);
//         }

//         printf("\n");
//         lua_pop(L, 2);
//     }
// }

// void dump_lua_stack(lua_State *L) {
//     int i;
//     int top = lua_gettop(L);
//     for (i = 1; i <= top; i++) { /* repeat for each level */
//         if (lua_istable(L, i)) {
//             print_lua_table(L);
//         } else {
//             print_non_table_lua_type(L);
//         }
//         printf("\n");
//     }
//     printf("\n"); /* end the listing */
// }

void dump_lua_stack(lua_State *L) {
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++) { /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {

        case LUA_TSTRING: /* strings */
            printf("`%s'", lua_tostring(L, i));
            break;

        case LUA_TBOOLEAN: /* booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;

        case LUA_TNUMBER: /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;

        default: /* other values */
            printf("%s", lua_typename(L, t));
            break;
        }
        printf("  "); /* put a separator */
    }
    printf("\n"); /* end the listing */
}

void process_animation(lua_State *L, struct bar_manager *bm) {
    if (lua_istable(L, -1)) {
        lua_getfield(L, -1, "curve");
        bm->animator.interp_function = lua_tostring(L, -1)[0];
        lua_pop(L, 1);

        lua_getfield(L, -1, "duration");
        bm->animator.duration = luaL_checkint(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "value");
        lua_remove(L, -2);
    }
}
void env_vars_to_lua_table(lua_State *L, struct env_vars *env_vars) {
    if (!env_vars) {
        lua_createtable(L, 0, 0);
    } else {
        lua_createtable(L, 0, env_vars->count);
        for (int i = 0; i < env_vars->count; ++i) {
            struct key_value_pair *pair = env_vars->vars[i];
            lua_pushstring(L, pair->key);
            lua_pushstring(L, pair->value);
            lua_rawset(L, -3);
        }
    }
}
