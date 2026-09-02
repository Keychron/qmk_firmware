/* Copyright 2025 ~ 2026 @ Keychron (https://www.keychron.com)
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "keychron.h"
#include "factory_test.h"

static uint32_t factory_reset_timer = 0;
static uint8_t  factory_reset_state = 0;
static uint8_t  keys_released       = 0;

void keyboard_post_init_kb(void) {
    keychron_common_init();
    keyboard_post_init_user();
}

void c100_8k_factory_timer_start(void) {
    factory_reset_timer = timer_read32();
}

static inline void c100_8k_factory_timer_check(void) {
    if (timer_elapsed32(factory_reset_timer) > 4000) {
        factory_reset_timer = 0;
        if (factory_reset_state == 0x03) {
            keys_released = 0x01 | 0x02;
            factory_reset();
        }
        factory_reset_state = 0;
    }
}

void keychron_task_kb(void) {
    if (factory_reset_timer) c100_8k_factory_timer_check();
}

bool process_record_keychron_kb(uint16_t keycode, keyrecord_t *record) {
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;

    bool is_prev_corner = (row == 0) && (col == 0 || col == MATRIX_COLS - 1);
    bool is_next_corner = (row == MATRIX_ROWS - 1) && (col == 0 || col == MATRIX_COLS - 1);

    if (is_prev_corner || is_next_corner) {
        uint8_t bit = is_prev_corner ? 0x02 : 0x01;
        if (record->event.pressed) {
            factory_reset_state |= bit;
            if (factory_reset_state == 0x03) {
                c100_8k_factory_timer_start();
                return false;
            }
        } else {
            factory_reset_state &= ~bit;
            factory_reset_timer = 0;
            if (keys_released & bit) {
                keys_released &= ~bit;
                if (keycode >= QK_BACKLIGHT_ON && keycode <= RGB_MODE_TWINKLE) return false;
            }
        }
    }

    return true;
}
