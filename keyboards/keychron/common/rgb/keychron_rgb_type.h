/* Copyright 2024 ~ 2025 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "color.h"

enum {
    PER_KEY_RGB_SOLID,
    PER_KEY_RGB_BREATHING,
    PER_KEY_RGB_REACTIVE_SIMPLE,
    PER_KEY_RGB_REACTIVE_MULTI_WIDE,
    PER_KEY_RGB_REACTIVE_SPLASH,
    PER_KEY_RGB_MAX,
};

typedef struct PACKED {
    uint8_t  effect;
    uint8_t hue;
    uint8_t sat;
    uint8_t speed;
    uint32_t time;
} effect_config_t;

typedef union {
    uint8_t raw;
    struct {
        bool    num_lock : 1;
        bool    caps_lock : 1;
        bool    scroll_lock : 1;
        bool    compose : 1;
        bool    kana : 1;
        uint8_t reserved : 3;
    };
} os_led_t;

// TODO:
// typedef struct PACKED HSV2 {
//     uint8_t h;
//     uint8_t s;
//     uint8_t v;
// } HSV2;

typedef struct PACKED {
    os_led_t disable;
    HSV  hsv;
} os_indicator_config_t;
