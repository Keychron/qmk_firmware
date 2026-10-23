/* Copyright 2024 ~ 2026 @ Keychron (https://www.keychron.com)
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

#include QMK_KEYBOARD_H
#include "keychron_common.h"

enum layers {
    BASE,
    FN,
    L2,
    L3,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_tenkey_27(
        KC_MUTE,  KC_ESC,   KC_DEL,   KC_TAB,   KC_BSPC,
        MC_1,     KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        MC_2,     KC_P7,    KC_P8,    KC_P9,    KC_PPLS,
        MC_3,     KC_P4,    KC_P5,    KC_P6,
        MC_4,     KC_P1,    KC_P2,    KC_P3,    KC_PENT,
        MO(FN),   KC_P0,              KC_PDOT          ),

    [FN] = LAYOUT_tenkey_27(
        UG_TOGG,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,
        _______,  UG_NEXT,  UG_VALU,  UG_HUEU,  _______,
        _______,  UG_PREV,  UG_VALD,  UG_HUED,  _______,
        _______,  UG_SATU,  UG_SPDU,  KC_MPRV,
        _______,  UG_SATD,  UG_SPDD,  KC_MPLY,  _______,
        _______,  UG_TOGG,            KC_MNXT          ),

    [L2] = LAYOUT_tenkey_27(
        _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,
        _______,  _______,            _______          ),

    [L3] = LAYOUT_tenkey_27(
        _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,
        _______,  _______,            _______          )
};

// clang-format on
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
    [L2]   = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [L3]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
};
#endif // ENCODER_MAP_ENABLE

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case BASE:
            break;
        case FN:
            break;
        case L2:
            break;
        case L3:
            break;
    }
    return state;
}
