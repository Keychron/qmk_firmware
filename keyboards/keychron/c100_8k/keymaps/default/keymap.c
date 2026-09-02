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

#include QMK_KEYBOARD_H
#include "keychron_common.h"

enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_tkl_ansi(
        UG_PREV,  KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     UG_PREV,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        UG_NEXT,  KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     UG_NEXT),

    [MAC_FN] = LAYOUT_tkl_ansi(
        UG_PREV,  KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     UG_PREV,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        UG_NEXT,  KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     UG_NEXT),

    [WIN_BASE] = LAYOUT_tkl_ansi(
        UG_PREV,  KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     UG_PREV,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        UG_NEXT,  KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     UG_NEXT),

    [WIN_FN] = LAYOUT_tkl_ansi(
        UG_PREV,  KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     UG_PREV,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,
        UG_NEXT,  KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     KC_1,     UG_NEXT),
};
