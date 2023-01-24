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

#include "sendstring_spanish.h"
#include QMK_KEYBOARD_H

// clang-format off
enum layers{
  QWERTY,
  COLEMAK,
  SYMBOLS,
  NUMPAD,
  FUNCTIONS,
  MACROS,
};

enum custom_keycodes {
  MC_TEST = SAFE_RANGE,
  MC_RUT,
  MC_PHONE,
  MC_EMAIL,
  AF_SKY_PMF,
  AF_SKY_VCHR,
  CSTM_QUOT,
  CSTM_SCLN,
  CSTM_QUES,
  CSTM_DOT,
  CSTM_COMM,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[QWERTY] = LAYOUT_ansi_68(
     KC_ESC,   ES_1,     ES_2,     ES_3,     ES_4,     ES_5,     ES_6,     ES_7,     ES_8,     ES_9,     ES_0,     ES_MINS,  ES_EQL,   KC_BSPC, BL_STEP,
     KC_TAB,   ES_Q,     ES_W,     ES_E,     ES_R,     ES_T,     ES_Y,     ES_U,     ES_I,     ES_O,     ES_P,     ES_LBRC,  ES_RBRC,  ES_BSLS, KC_HOME,
     KC_CAPS,  ES_A,     ES_S,     ES_D,     ES_F,     ES_G,     ES_H,     ES_J,     ES_K,     ES_L,     ES_SCLN,  ES_QUOT,            KC_ENT,  KC_PGUP,
     KC_LSFT,  ES_Z,     ES_X,     ES_C,     ES_V,     ES_B,     ES_N,     ES_M,     ES_COMM,  ES_DOT,   ES_SLSH,  KC_RSFT,  KC_UP,    KC_PGDN,
     KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                       KC_RALT,MO(NUMPAD),MO(FUNCTIONS),  KC_LEFT,  KC_DOWN, KC_RGHT),

[COLEMAK] = LAYOUT_ansi_68(
     MO(MACROS),   ES_1,     ES_2,     ES_3,     ES_4,     ES_5,     ES_6,     ES_7,     ES_8,     ES_9,     ES_0,     ES_MINS,  ES_EQL,   KC_BSPC, BL_STEP,
     LT(NUMPAD, KC_TAB),   ES_Q,     ES_W,     ES_F,     ES_P,     ES_B,     ES_J,     ES_L,     ES_U,     ES_Y,     CSTM_SCLN,    ES_ACUT,    KC_NO,    ES_BSLS, KC_PSCR,
     CTL_T(KC_ESC),  ES_A,     ES_R,     ES_S,     ES_T,     ES_G,     ES_M,     ES_N,     ES_E,     ES_I,     ES_O,     CSTM_QUOT,            KC_ENT,  KC_PGUP,
     KC_LSFT,  ES_X,     ES_C,     ES_D,     ES_V,     ES_Z,     ES_K,     ES_H,     CSTM_COMM,   CSTM_DOT,   CSTM_QUES,  KC_RSFT,  KC_UP,    KC_PGDN,
     KC_NO,  KC_LGUI,  ALT_T(KC_BSPC),                      KC_SPC,                       LT(SYMBOLS, KC_ENT),MO(FUNCTIONS),KC_NO,  KC_LEFT,  KC_DOWN, KC_RGHT),

[SYMBOLS] = LAYOUT_ansi_68(
     KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    BL_TOGG,
     KC_TRNS,  ES_TILD,  ES_DLR,   ES_LBRC,  ES_RBRC,  ES_CIRC,  KC_NO,    ES_PERC,  ES_PLUS,  ES_MINS,  ES_ASTR,  KC_NO,    KC_NO,    KC_NO,    KC_NO,
     KC_TRNS,  ES_GRV,   ES_UNDS,  ES_LCBR,  ES_RCBR,  ES_PIPE,  KC_NO,    ES_LPRN,  ES_RPRN,  ES_EQL,  KC_NO,    KC_NO,              KC_NO,    KC_NO,
     KC_TRNS,  ES_AT,    ES_AMPR,   ES_DOT,  ES_SLSH,  ES_BSLS,  ES_EXLM,  ES_LABK,  ES_RABK,  ES_HASH,  KC_NO,    KC_NO,    KC_NO,    KC_NO,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                      KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

[NUMPAD] = LAYOUT_ansi_68(
     KC_NO,    KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,    KC_F12,    KC_NO,    BL_TOGG,
     KC_TRNS,  KC_HOME,  KC_UP,    KC_END,   KC_PGUP,  KC_NO,    ES_SLSH,  ES_7,     ES_8,     ES_9,     ES_ASTR,  ES_MINS,    KC_NO,  KC_NO,  KC_NO,
     KC_TRNS,  KC_LEFT,  KC_DOWN,  KC_RIGHT, KC_BSPC,  KC_NO,    KC_NO,  ES_4,     ES_5,     ES_6,     ES_PLUS,  ES_COMM,  KC_TRNS,            KC_TRNS,
     KC_TRNS,  KC_NO,    KC_DEL,   KC_PGDN,  KC_NO,    KC_NO,    ES_0,     ES_1,     ES_2,     ES_3,     ES_DOT,    KC_RSFT,    KC_NO,    KC_NO,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                      KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

[FUNCTIONS] = LAYOUT_ansi_68(
     ES_TILD,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_CALC,    KC_F6,    KC_MPRV,  KC_MPLY,    KC_MNXT,    KC_MUTE,   KC_VOLD,   KC_VOLU,   KC_TRNS,  BL_TOGG,
     KC_TRNS,  BT_HST1,  BT_HST2,  BT_HST3,  KC_TRNS,  DT_PRNT,  DT_DOWN,  DT_UP,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  BAT_LVL,  KC_TRNS,  DB_TOGG,  QK_RBT,  QK_MAKE,  QK_BOOT,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                      KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),
[MACROS] = LAYOUT_ansi_68(
  KC_TRNS, MC_TEST, MC_RUT,  MC_PHONE, MC_EMAIL, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, AF_SKY_PMF, AF_SKY_VCHR, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
};

bool is_shift_pressed = false;

char first_name[8] = "Alberto";
char last_name[5] = "Test";
char rut[10] = "111111111";
char phone[10] = "321232123";
char email[27] = "user_acid49@skyairline.com";
char voucher_code[7] = "F7PSJN";
char voucher_pin[7] = "7MBMT2";

void sky_autofill_passenger_form(void) {
  SEND_STRING(first_name);
  SEND_STRING(SS_TAP(X_TAB));
  SEND_STRING(last_name);
  SEND_STRING(SS_TAP(X_TAB));
  SEND_STRING(SS_TAP(X_DOWN));
  SEND_STRING(SS_TAP(X_ENTER));
  SEND_STRING(SS_TAP(X_TAB));
  SEND_STRING(SS_TAP(X_DOWN));
  SEND_STRING(SS_TAP(X_ENTER));
  SEND_STRING(SS_TAP(X_TAB));
  for (int i = 0; i < 26; i++) {
    SEND_STRING(SS_TAP(X_DOWN));
  }
  SEND_STRING(SS_TAP(X_ENTER));
  SEND_STRING(SS_TAP(X_TAB));
  SEND_STRING(SS_TAP(X_TAB));
  SEND_STRING(rut);
  SEND_STRING(SS_TAP(X_TAB));
  SEND_STRING(SS_TAP(X_TAB));
  SEND_STRING(phone);
  SEND_STRING(SS_TAP(X_TAB));
  SEND_STRING(email);
  SEND_STRING(SS_TAP(X_TAB));
  SEND_STRING(SS_TAP(X_ENTER));
  SEND_STRING(SS_TAP(X_DOWN));
  SEND_STRING(SS_TAP(X_ENTER));
}

void sky_autofill_voucher_form(void) {
  SEND_STRING(voucher_code);
  SEND_STRING(SS_TAP(X_TAB));
  SEND_STRING(voucher_pin);
  SEND_STRING(SS_TAP(X_TAB));
  SEND_STRING(SS_TAP(X_ENTER));
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case KC_LSFT:
    case KC_RSFT:
      is_shift_pressed = record->event.pressed;
      break;
    case ES_GRV:
    case ES_CIRC:
        if (!record->event.pressed) {
          unregister_code16(keycode);
          tap_code(KC_SPC);
          return false;
        }
      break;
    case CSTM_QUOT:
        if (record->event.pressed) {
          SEND_STRING(is_shift_pressed ? "\"" : "'");
          return false;
        }
      break;
    case CSTM_SCLN:
        if (record->event.pressed) {
          SEND_STRING(is_shift_pressed ? ":" : ";");
          return false;
        }
      break;
    case CSTM_DOT:
        if (record->event.pressed) {
          SEND_STRING(is_shift_pressed ? "/" : ".");
          return false;
        }
      break;
    case CSTM_COMM:
        if (record->event.pressed) {
          SEND_STRING(is_shift_pressed ? "_" : ",");
          return false;
        }
      break;
    case CSTM_QUES:
        if (record->event.pressed) {
          SEND_STRING(is_shift_pressed ? "?" : "-");
          return false;
        }
      break;
    case MC_TEST:
        if (record->event.pressed) {
          SEND_STRING(last_name);
          return false;
        }
        break;
    case MC_RUT:
        if (record->event.pressed) {
          SEND_STRING(rut);
          return false;
        }
        break;
    case MC_PHONE:
        if (record->event.pressed) {
          SEND_STRING(phone);
          return false;
        }
        break;
    case MC_EMAIL:
        if (record->event.pressed) {
          SEND_STRING(email);
          return false;
        }
        break;
    case AF_SKY_PMF:
        if (record->event.pressed) {
          sky_autofill_passenger_form();
          return false;
        }
        break;
    case AF_SKY_VCHR:
        if (record->event.pressed) {
          sky_autofill_voucher_form();
          return false;
        }
        break;
    }
    return true;
};

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        case KC_A...KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;
        case KC_1...KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_LSFT:
        case CSTM_COMM:
        case KC_UNDS:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}