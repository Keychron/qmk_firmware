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
  MACROS,
};

enum custom_keycodes {
  MC_TEST = SAFE_RANGE,
  MC_RUT,
  MC_PHONE,
  MC_EMAIL,
  AF_SKY_PMF,
  AF_SKY_VCHR,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[QWERTY] = LAYOUT_ansi_68(
     KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC, BL_STEP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS, KC_HOME,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,  KC_PGUP,
     KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,    KC_PGDN,
     KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                       KC_RALT,MO(NUMPAD),MO(FUNCTIONS),  KC_LEFT,  KC_DOWN, KC_RGHT),

[COLEMAK] = LAYOUT_ansi_68(
     MO(MACROS),   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC, BL_STEP,
     LT(NUMPAD, KC_TAB),   KC_Q,     KC_W,     KC_F,     KC_P,     KC_B,     KC_J,     KC_L,     KC_U,     KC_Y,     KC_SCLN,    QK_LEAD,    KC_NO,    KC_BSLS, KC_PSCR,
     CTL_T(KC_ESC),  KC_A,     KC_R,     KC_S,     KC_T,     KC_G,     KC_M,     KC_N,     KC_E,     KC_I,     KC_O,     KC_QUOT,            KC_ENT,  KC_PGUP,
     KC_LSFT,  KC_X,     KC_C,     KC_D,     KC_V,     KC_Z,     KC_K,     KC_H,     KC_COMM,   KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,    KC_PGDN,
     KC_NO,  KC_LGUI,  ALT_T(KC_BSPC),                      KC_SPC,                       LT(SYMBOLS, KC_ENT),MO(FUNCTIONS),KC_NO,  KC_LEFT,  KC_DOWN, KC_RGHT),

[SYMBOLS] = LAYOUT_ansi_68(
     KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    BL_TOGG,
     KC_TRNS,  KC_TILD,  KC_DLR,   KC_LBRC,  KC_RBRC,  KC_CIRC,  KC_NO,    KC_PERC,  KC_PLUS,  KC_MINS,  KC_ASTR,  KC_NO,    KC_NO,    KC_NO,    KC_NO,
     KC_TRNS,  KC_GRV,   KC_UNDS,  KC_LCBR,  KC_RCBR,  KC_PIPE,  KC_NO,    KC_LPRN,  KC_RPRN,  KC_EQL,  KC_NO,    KC_NO,              KC_NO,    KC_NO,
     KC_TRNS,  KC_AT,    KC_AMPR,   KC_DOT,  KC_SLSH,  KC_BSLS,  KC_EXLM,  KC_LABK,  KC_RABK,  KC_HASH,  KC_NO,    KC_NO,    KC_NO,    KC_NO,
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
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  BAT_LVL,  KC_TRNS,  DB_TOGG,  QK_RBT,  QK_MAKE,  QK_BOOT,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                      KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),
[MACROS] = LAYOUT_ansi_68(
  KC_TRNS, MC_TEST, MC_RUT,  MC_PHONE, MC_EMAIL, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, AF_SKY_PMF, AF_SKY_VCHR, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
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

