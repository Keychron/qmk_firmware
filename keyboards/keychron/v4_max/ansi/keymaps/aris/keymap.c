/* Copyright 2024 @ Keychron (https://www.keychron.com)
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

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "keychron_modtap_extras.h"

enum layers {
    MAC_BASE,
    WIN_BASE,
    _space,
    _esc,
    _FN,
    _FN1,
    _FN_stock,
    _FN1_stock,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // mac here is actually win stock, and win is my modified layer
    [MAC_BASE] = LAYOUT_ansi_61(
        KC_ESC,  KC_1,    KC_2, KC_3,    KC_4,    KC_5,    KC_6,   KC_7,    KC_8,    KC_9,     KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,  KC_Q,    KC_W, KC_E,    KC_R,    KC_T,    KC_Y,   KC_U,    KC_I,    KC_O,     KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_CAPS, KC_A,    KC_S, KC_D,    KC_F,    KC_G,    KC_H,   KC_J,    KC_K,    KC_L,     KC_SCLN, KC_QUOT, KC_ENT,
                 KC_LSFT, KC_Z, KC_X,    KC_C,    KC_V,    KC_B,   KC_N,    KC_M,    KC_COMM,  KC_DOT,  KC_SLSH, KC_RSFT,
                                KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_RALT, MO(_FN), MO(_FN1), KC_RCTL
    ),
    [WIN_BASE] = LAYOUT_ansi_61(
        KC_ESC,          KC_1,         KC_2,         KC_3,         KC_4,          KC_5,    KC_6,              KC_7,         KC_8,          KC_9,         KC_0,            KC_MINS,          KC_EQL,  KC_BSPC,
        KC_TAB,          RSG_T(KC_Q),  LSA_T(KC_W),  LCA_T(KC_E),  LCS_T(KC_R),   KC_T,    KC_Y,              LCS_T(KC_U),  LCA_T(KC_I),   LSA_T(KC_O),  RSG_T(KC_P),     KC_LBRC,          KC_RBRC, KC_BSLS,
        LT(_esc,KC_ESC), RGUI_T(KC_A), LALT_T(KC_S), LSFT_T(KC_D), LCTL_T(KC_F),  KC_G,    KC_H,              LCTL_T(KC_J), LSFT_T(KC_K),  LALT_T(KC_L), RGUI_T(KC_SCLN), LT(_esc,KC_QUOT), KC_ENT,
                         KC_LSFT,      KC_Z,         KC_X,         LT(_FN1,KC_C), KC_V,    KC_B,              KC_N,         LT(_FN1,KC_M), KC_COMM,      KC_DOT,          KC_SLSH,          KC_RSFT,
                                                     KC_LCTL,      KC_LGUI,       KC_LALT, LT(_space,KC_SPC), KC_RALT,      MO(_FN),       MO(_FN1),     KC_RCTL
    ),

    [_space] = LAYOUT_ansi_61(
        KC_TRNS, KC_BRID, KC_BRIU, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_MUTE, KC_VOLD, KC_VOLU, KC_DEL,
        KC_TRNS, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, KC_GRV,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_MINS, KC_EQL,  KC_TRNS,
                 KC_TRNS, KC_CAPS, KC_DEL,  CW_TOGG, KC_BSPC, KC_NO,   KC_TAB,  KC_ENT,  KC_LBRC, KC_RBRC, KC_BSLS, KC_TRNS,
                                   KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_esc]   = LAYOUT_ansi_61(
        KC_TRNS, KC_BRID, KC_BRIU, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,        KC_NO,   KC_NO,   KC_MUTE, KC_VOLD, KC_VOLU, KC_DEL,
        KC_TRNS, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR,      KC_ASTR, KC_LPRN, KC_RPRN, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, KC_TILD, KC_HOME,      KC_PGDN, KC_PGUP, KC_END,  KC_UNDS, KC_PLUS, KC_TRNS,
                 KC_TRNS, KC_CAPS, KC_DEL,  CW_TOGG, KC_BSPC, BAT_LVL, LSFT(KC_TAB), KC_ENT,  KC_LCBR, KC_RCBR, KC_PIPE, KC_TRNS,
                                   KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,   KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_FN]    = LAYOUT_ansi_61(
        KC_TILD, KC_F1,   KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,  KC_F12,  _______,
        _______, BT_HST1, BT_HST2,  BT_HST3, P2P4G,   _______, _______, _______, KC_APP,  KC_SCRL, KC_INS,   KC_PGUP, KC_HOME, _______,
        RGB_TOG, RGB_MOD, RGB_VAI,  RGB_HUD, RGB_SAI, RGB_SPI, _______, _______, KC_UP,   KC_PSCR, KC_PGDN,  KC_END,  _______,
                 _______, RGB_RMOD, RGB_VAD, RGB_HUI, RGB_SAD, RGB_SPD, NK_TOGG, KC_LEFT, KC_DOWN, KC_RIGHT, _______, _______,
                                    _______, _______, _______, _______, _______, _______, _______, _______
    ),

    [_FN1]       = LAYOUT_ansi_61(
        KC_NO,   KC_BRID, KC_BRIU, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_MUTE, KC_VOLD, KC_VOLU, KC_NO,
        KC_TRNS, KC_NO,   MS_BTN3, MS_UP,   MS_WHLU, KC_NO,   KC_NO,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_NO,   KC_NO,   KC_TRNS,
        RGB_TOG, MS_BTN2, MS_LEFT, MS_DOWN, MS_RGHT, KC_NO,   KC_NO,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_NO,   KC_TRNS,
                 KC_TRNS, KC_NO,   MS_BTN4, MS_BTN5, MS_WHLD, BAT_LVL, KC_NO,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS,
                                   KC_TRNS, KC_TRNS, KC_TRNS, MS_BTN1, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_FN_stock]  = LAYOUT_ansi_61(
        KC_GRV,  KC_BRID, KC_BRIU,  KC_TASK, KC_FILE, RGB_VAD, RGB_VAI, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,  KC_VOLD, KC_VOLU, KC_DEL,
        _______, BT_HST1, BT_HST2,  BT_HST3, P2P4G,   _______, _______, _______, KC_APP,  KC_SCRL, KC_INS,   KC_PGUP, KC_HOME, _______,
        RGB_TOG, RGB_MOD, RGB_VAI,  RGB_HUD, RGB_SAI, RGB_SPI, _______, _______, KC_UP,   KC_PSCR, KC_PGDN,  KC_END,  _______,
                 _______, RGB_RMOD, RGB_VAD, RGB_HUI, RGB_SAD, RGB_SPD, NK_TOGG, KC_LEFT, KC_DOWN, KC_RIGHT, _______, _______,
                                    _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [_FN1_stock] = LAYOUT_ansi_61(
        KC_TILD, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                 _______, _______, _______, _______, _______, BAT_LVL, _______, _______, _______, _______, _______, _______,
                                   _______, _______, _______, _______, _______, _______, _______, _______
    ),
};



const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
LAYOUT_ansi_61(
        'L', 'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R',
             'L', 'L', 'L', 'L', 'L', '*', 'R', 'R', 'R', 'R', 'R', 'R',
                       'L', 'L', 'L', '*', 'R', 'R', 'R', 'R'
    );

// clang-format on
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }
    return true;
}

