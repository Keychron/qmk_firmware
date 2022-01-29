/* Copyright 2021 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "q1.h"
#include "test.c"

#ifdef DIP_SWITCH_ENABLE

bool dip_switch_update_kb(uint8_t index, bool active) {
    if (!dip_switch_update_user(index, active)) { return false;}
    if (index == 0) {
        default_layer_set(1UL << (active ? 2 : 0));
    }
    return true;
}

#endif


#if defined(RGB_MATRIX_ENABLE) && defined(CAPS_LOCK_LED_INDEX)

#ifndef CAPS_LOCK_LED_COLOR
    #define CAPS_LOCK_LED_COLOR RGB_WHITE //white
#endif


#define CAPS_LOCK_BRIGHTNESS 0xFF
#ifdef RGB_MATRIX_MAXIMUM_BRIGHTNESS
    #undef CAPS_LOCK_BRIGHTNESS
    #define CAPS_LOCK_BRIGHTNESS RGB_MATRIX_MAXIMUM_BRIGHTNESS
#endif

__attribute__((weak))
void rgb_matrix_indicators_user(void) {
    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(CAPS_LOCK_LED_INDEX, CAPS_LOCK_LED_COLOR);
    }
}

#endif
