#include <lauxlib.h>
#include <lua.h>
#include <luaconf.h>
#include <luajit.h>
#include <lualib.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper.h"
#include "parsing.h"

#include "../animation.h"
#include "../bar_manager.h"
#include "../display.h"

// Assumes that on the the stack there is (-2 : key, -1: value);
// TODO: Support sketchybar's specification of booleans
inline static bool parse_bar_key_value_pair(lua_State *L) {
    bool needs_refresh = false;

    const char *key = lua_tostring(L, -2);

    if (key_is(key, PROPERTY_MARGIN)) {
        process_animation(L, &g_bar_manager);
        ANIMATE(bar_manager_set_margin, &g_bar_manager, g_bar_manager.margin,
                lua_tointeger(L, -1));
    } else if (key_is(key, PROPERTY_YOFFSET)) {
        process_animation(L, &g_bar_manager);
        ANIMATE(bar_manager_set_y_offset, &g_bar_manager,
                g_bar_manager.background.y_offset, lua_tointeger(L, -1));
    } else if (key_is(key, PROPERTY_BLUR_RADIUS)) {
        process_animation(L, &g_bar_manager);
        ANIMATE(bar_manager_set_background_blur, &g_bar_manager,
                g_bar_manager.blur_radius, lua_tointeger(L, -1));
    } else if (key_is(key, PROPERTY_FONT_SMOOTHING)) {
        needs_refresh = bar_manager_set_font_smoothing(&g_bar_manager,
                                                       lua_toboolean(L, -1));
    } else if (key_is(key, PROPERTY_SHADOW)) {
        needs_refresh =
            bar_manager_set_shadow(&g_bar_manager, lua_toboolean(L, -1));
    } else if (key_is(key, PROPERTY_NOTCH_WIDTH)) {
        process_animation(L, &g_bar_manager);
        ANIMATE(bar_manager_set_notch_width, &g_bar_manager,
                g_bar_manager.notch_width, lua_tointeger(L, -1));
    } else if (key_is(key, PROPERTY_NOTCH_OFFSET)) {
        process_animation(L, &g_bar_manager);
        ANIMATE(bar_manager_set_notch_offset, &g_bar_manager,
                g_bar_manager.notch_offset, lua_tointeger(L, -1));
    } else if (key_is(key, PROPERTY_HIDDEN)) {
        uint32_t adid = 0;
        if (lua_isboolean(L, -1)) {
            needs_refresh = bar_manager_set_hidden(&g_bar_manager, adid,
                                                   lua_toboolean(L, -1));
        } else {
            adid = display_arrangement(display_active_display_id());

            if (adid > 0 && adid <= g_bar_manager.bar_count) {
                needs_refresh = bar_manager_set_hidden(
                    &g_bar_manager, adid,
                    !g_bar_manager.bars[adid - 1]->hidden);
            } else {
                printf("No bar on display %u \n", adid);
            }
        }
    } else if (key_is(key, PROPERTY_TOPMOST)) {
        if (lua_isboolean(L, -1)) {
            needs_refresh = bar_manager_set_topmost(
                &g_bar_manager, TOPMOST_LEVEL_ALL, lua_toboolean(L, -1));
        } else {
            needs_refresh = bar_manager_set_topmost(&g_bar_manager,
                                                    TOPMOST_LEVEL_WINDOW, true);
        }
    } else if (key_is(key, PROPERTY_STICKY)) {
        needs_refresh =
            bar_manager_set_sticky(&g_bar_manager, lua_toboolean(L, -1));
    } else if (key_is(key, PROPERTY_DISPLAY)) {
        uint32_t display_pattern = 0;

        if (lua_isstring(L, -1)) {
            const char *display = lua_tostring(L, -1);

            if (key_is(display, ARGUMENT_DISPLAY_MAIN)) {
                display_pattern = DISPLAY_MAIN_PATTERN;
            } else if (key_is(display, ARGUMENT_DISPLAY_ALL)) {
                display_pattern = DISPLAY_ALL_PATTERN;
            } else {
                luaL_error(L, "Invalid key value pair");
            }
        } else {
            // Iterate over array of ints
            lua_pushnil(L);
            while (lua_next(L, -2)) {
                display_pattern |= 1 << (lua_tointeger(L, -1) - 1);
                lua_pop(L, 1);
            }
        }
        needs_refresh =
            bar_manager_set_displays(&g_bar_manager, display_pattern);
    } else if (key_is(key, PROPERTY_POSITION)) {
        const char *position = lua_tostring(L, -1);
        if (position[0] != '\0') {
            needs_refresh =
                bar_manager_set_position(&g_bar_manager, position[0]);
        }
    } else if (key_is(key, PROPERTY_HEIGHT)) {
        process_animation(L, &g_bar_manager);
        ANIMATE(bar_manager_set_bar_height, &g_bar_manager,
                g_bar_manager.background.bounds.size.height,
                lua_tointeger(L, -1));
    } else {
        needs_refresh =
            parse_background_key_value_pair(&g_bar_manager.background, key, L);
    }

    return needs_refresh;
}

LFUN(lua_bar_set) {
    g_bar_manager.animator.interp_function = '\0';
    g_bar_manager.animator.duration = 0;
    bar_manager_freeze(&g_bar_manager);
    bool bar_needs_refresh = false;

    int arg_num = lua_gettop(L);

    if (arg_num == 2) {
        bar_needs_refresh = parse_bar_key_value_pair(L);
    } else {
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            lua_pushvalue(L, -2);
            lua_insert(L, -2);

            bar_needs_refresh |= parse_bar_key_value_pair(L);
            lua_pop(L, 2);
        }
    }

    if (bar_needs_refresh) {
        if (g_bar_manager.bar_needs_resize)
            bar_manager_resize(&g_bar_manager);

        g_bar_manager.bar_needs_update = true;
    }

    animator_lock(&g_bar_manager.animator);
    bar_manager_unfreeze(&g_bar_manager);
    bar_manager_refresh(&g_bar_manager, false);

    return 0;
}

static const luaL_Reg lua_bar_funcs[] = {{"set", lua_bar_set}, {NULL, NULL}};

void load_lua_bar_lib(lua_State *L) { luaL_register(L, "bar", lua_bar_funcs); }
