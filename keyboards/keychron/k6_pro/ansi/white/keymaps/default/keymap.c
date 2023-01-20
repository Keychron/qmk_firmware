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

#include QMK_KEYBOARD_H

LEADER_EXTERNS();

// clang-format off
enum layers{
  QWERTY,
  COLEMAK,
  SYMBOLS,
  NUMPAD,
  FUNCTIONS,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[QWERTY] = LAYOUT_ansi_68(
     KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC, BL_STEP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS, KC_HOME,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,  KC_PGUP,
     KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,    KC_PGDN,
     KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                       KC_RALT,MO(NUMPAD),MO(FUNCTIONS),  KC_LEFT,  KC_DOWN, KC_RGHT),

[COLEMAK] = LAYOUT_ansi_68(
     KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC, BL_STEP,
     LT(NUMPAD, KC_TAB),   KC_Q,     KC_W,     KC_F,     KC_P,     KC_B,     KC_J,     KC_L,     KC_U,     KC_Y,     KC_SCLN,    QK_LEAD,    KC_NO,    KC_BSLS, KC_PSCR,
     CTL_T(KC_ESC),  KC_A,     KC_R,     KC_S,     KC_T,     KC_G,     KC_M,     KC_N,     KC_E,     KC_I,     KC_O,     KC_QUOT,            KC_ENT,  KC_PGUP,
     KC_LSFT,  KC_X,     KC_C,     KC_D,     KC_V,     KC_Z,     KC_K,     KC_H,     KC_COMM,   KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,    KC_PGDN,
     KC_NO,  KC_LGUI,  ALT_T(KC_BSPC),                      KC_SPC,                       LT(SYMBOLS, KC_ENT),MO(FUNCTIONS),KC_NO,  KC_LEFT,  KC_DOWN, KC_RGHT),

[SYMBOLS] = LAYOUT_ansi_68(
     KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    BL_TOGG,
     KC_TRNS,  KC_TILD,  KC_DLR,   KC_LBRC,  KC_RBRC,  KC_CIRC,  KC_NO,    KC_PERC,  KC_PLUS,  KC_MINS,  KC_ASTR,  KC_NO,    KC_NO,    KC_NO,    KC_NO,
     KC_TRNS,  KC_GRV,   KC_UNDS,  KC_LCBR,  KC_RCBR,  KC_PIPE,  KC_NO,    KC_LPRN,  KC_RPRN,  KC_EQL,  KC_NO,    KC_NO,              KC_NO,    KC_NO,
     KC_TRNS,  KC_AT,    KC_AMPR,   KC_DOT,  KC_PSLS,  KC_BSLS,  KC_EXLM,  KC_LABK,  KC_RABK,  KC_HASH,  KC_NO,    KC_NO,    KC_NO,    KC_NO,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                      KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

[NUMPAD] = LAYOUT_ansi_68(
     KC_NO,    KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,    KC_F12,    KC_NO,    BL_TOGG,
     KC_TRNS,  KC_HOME,  KC_UP,    KC_END,   KC_PGUP,  KC_NO,    KC_SLSH,  KC_7,     KC_8,     KC_9,     KC_ASTR,  KC_MINS,    KC_NO,  KC_NO,  KC_NO,
     KC_TRNS,  KC_LEFT,  KC_DOWN,  KC_RIGHT, KC_BSPC,  KC_NO,    KC_NO,  KC_4,     KC_5,     KC_6,     KC_PLUS,  KC_COMM,  KC_TRNS,            KC_TRNS,
     KC_TRNS,  KC_NO,    KC_DEL,   KC_PGDN,  KC_NO,    KC_NO,    KC_0,     KC_1,     KC_2,     KC_3,     KC_DOT,    KC_RSFT,    KC_NO,    KC_NO,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                      KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

[FUNCTIONS] = LAYOUT_ansi_68(
     KC_TILD,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_CALC,    KC_F6,    KC_MPRV,  KC_MPLY,    KC_MNXT,    KC_MUTE,   KC_VOLD,   KC_VOLU,   KC_TRNS,  BL_TOGG,
     KC_TRNS,  BT_HST1,  BT_HST2,  BT_HST3,  KC_TRNS,  DT_PRNT,  DT_DOWN,  DT_UP,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  BAT_LVL,  KC_TRNS,  KC_TRNS,  QK_RBT,  KC_TRNS,  QK_BOOT,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                      KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),
};

void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
     leading = false;
    leader_end();

    SEQ_ONE_KEY(KC_A) {
      SEND_STRING(SS_DOWN(X_LALT));
      SEND_STRING(SS_TAP(X_P1)SS_TAP(X_P6)SS_TAP(X_P0));
      SEND_STRING(SS_UP(X_LALT));
    }
    SEQ_TWO_KEYS(KC_LSFT, KC_A) {
      SEND_STRING(SS_DOWN(X_LALT));
      SEND_STRING(SS_TAP(X_P0)SS_TAP(X_P1)SS_TAP(X_P9)SS_TAP(X_P3));
      SEND_STRING(SS_UP(X_LALT));
    }
    SEQ_ONE_KEY(KC_E) {
      SEND_STRING(SS_DOWN(X_LALT));
      SEND_STRING(SS_TAP(X_P1)SS_TAP(X_P3)SS_TAP(X_P0));
      SEND_STRING(SS_UP(X_LALT));
    }
    SEQ_TWO_KEYS(KC_LSFT, KC_E) {
      SEND_STRING(SS_DOWN(X_LALT));
      SEND_STRING(SS_TAP(X_P0)SS_TAP(X_P2)SS_TAP(X_P0)SS_TAP(X_P1));
      SEND_STRING(SS_UP(X_LALT));
    }
    SEQ_ONE_KEY(KC_I) {
      SEND_STRING(SS_DOWN(X_LALT));
      SEND_STRING(SS_TAP(X_P1)SS_TAP(X_P6)SS_TAP(X_P1));
      SEND_STRING(SS_UP(X_LALT));
    }
    SEQ_TWO_KEYS(KC_LSFT, KC_I) {
      SEND_STRING(SS_DOWN(X_LALT));
      SEND_STRING(SS_TAP(X_P0)SS_TAP(X_P2)SS_TAP(X_P0)SS_TAP(X_P5));
      SEND_STRING(SS_UP(X_LALT));
    }
    SEQ_ONE_KEY(KC_O) {
      SEND_STRING(SS_DOWN(X_LALT));
      SEND_STRING(SS_TAP(X_P1)SS_TAP(X_P6)SS_TAP(X_P2));
      SEND_STRING(SS_UP(X_LALT));
    }
    SEQ_TWO_KEYS(KC_LSFT, KC_O) {
      SEND_STRING(SS_DOWN(X_LALT));
      SEND_STRING(SS_TAP(X_P0)SS_TAP(X_P2)SS_TAP(X_P1)SS_TAP(X_P1));
      SEND_STRING(SS_UP(X_LALT));
    }
    SEQ_ONE_KEY(KC_U) {
      SEND_STRING(SS_DOWN(X_LALT));
      SEND_STRING(SS_TAP(X_P1)SS_TAP(X_P6)SS_TAP(X_P3));
      SEND_STRING(SS_UP(X_LALT));
    }
    SEQ_TWO_KEYS(KC_LSFT, KC_U) {
      SEND_STRING(SS_DOWN(X_LALT));
      SEND_STRING(SS_TAP(X_P0)SS_TAP(X_P2)SS_TAP(X_P1)SS_TAP(X_P8));
      SEND_STRING(SS_UP(X_LALT));
    }
    SEQ_ONE_KEY(KC_N) {
      SEND_STRING(SS_DOWN(X_LALT));
      SEND_STRING(SS_TAP(X_P1)SS_TAP(X_P6)SS_TAP(X_P4));
      SEND_STRING(SS_UP(X_LALT));
    }
    SEQ_TWO_KEYS(KC_LSFT, KC_A) {
      SEND_STRING(SS_DOWN(X_LALT));
      SEND_STRING(SS_TAP(X_P1)SS_TAP(X_P6)SS_TAP(X_P5));
      SEND_STRING(SS_UP(X_LALT));
    }
  }
}