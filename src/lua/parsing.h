#pragma once

#include "helper.h"

#include "../animation.h"
#include "../bar_manager.h"

static inline bool
parse_background_key_value_pair(struct background *background, const char *key,
                                lua_State *L) {
    bool needs_refresh = false;

    if (key_is(key, PROPERTY_DRAWING)) {
        needs_refresh =
            background_set_enabled(background, lua_toboolean(L, -1));
    } else if (key_is(key, PROPERTY_HEIGHT)) {
        process_animation(L, &g_bar_manager);
        ANIMATE(background_set_height, background,
                background->bounds.size.height, lua_tointeger(L, -1));
    } else if (key_is(key, PROPERTY_CORNER_RADIUS)) {
        process_animation(L, &g_bar_manager);
        ANIMATE(background_set_corner_radius, background,
                background->corner_radius, lua_tointeger(L, -1));
    } else if (key_is(key, PROPERTY_BORDER_WIDTH)) {
        process_animation(L, &g_bar_manager);
        ANIMATE(background_set_border_width, background,
                background->border_width, lua_tointeger(L, -1));
    } else if (key_is(key, PROPERTY_COLOR)) {
        process_animation(L, &g_bar_manager);

        const char *color_string = lua_tostring(L, -1);

        if (color_string[0] == '#') {
            color_string += 1;
        }

        uint32_t color = strtoul(color_string, NULL, 16);

        ANIMATE_BYTES(background_set_color, background, background->color.hex,
                      color);
    } else if (key_is(key, PROPERTY_BORDER_COLOR)) {
        process_animation(L, &g_bar_manager);

        const char *color_string = lua_tostring(L, -1);

        if (color_string[0] == '#') {
            color_string += 1;
        }

        uint32_t color = strtoul(color_string, NULL, 16);

        ANIMATE_BYTES(background_set_border_color, background,
                      background->border_color.hex, color);
    } else if (key_is(key, PROPERTY_PADDING_LEFT)) {
        process_animation(L, &g_bar_manager);
        ANIMATE(background_set_padding_left, background,
                background->padding_left, lua_tointeger(L, -1));
    } else if (key_is(key, PROPERTY_PADDING_RIGHT)) {
        process_animation(L, &g_bar_manager);
        ANIMATE(background_set_padding_right, background,
                background->padding_right, lua_tointeger(L, -1));
    } else if (key_is(key, PROPERTY_YOFFSET)) {
        process_animation(L, &g_bar_manager);
        ANIMATE(background_set_yoffset, background, background->y_offset,
                lua_tointeger(L, -1));
    }

    return needs_refresh;
}
