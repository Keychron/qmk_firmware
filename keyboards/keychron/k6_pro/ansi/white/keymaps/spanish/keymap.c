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
#include <time.h>

// clang-format off
enum layers{
  QWERTY,
  COLEMAK,
  // COLEMAK_WITH_MIRRORING,
  // COLEMAK_MIRROR,
  SYMBOLS,
  NUMPAD,
  FUNCTIONS,
  MACROS,
  MACROS_2,
  MOUSE,
};

enum custom_keycodes {
  MC_TEST = SAFE_RANGE,
  MC_RUT,
  MC_PHONE,
  MC_EMAIL,
  MC_PASSP,
  MC_RETU,
  MC_SKY_EMAIL,
  MC_PWM,
  MC_PW1,
  MC_DATE,
  MC_MG_SORT,
  MC_HTTP,
  MC_HTTPS,
  MC_LOCH,
  MC_SKCOM,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[QWERTY] = LAYOUT_ansi_68(
    KC_ESC,  ES_1,    ES_2,   ES_3,  ES_4,  ES_5,  ES_6,  ES_7, ES_8,     ES_9,     ES_0,       MC_RETU,  ES_EQL,     KC_BSPC,    BL_STEP,
    KC_TAB,  ES_Q,    ES_W,   ES_E,  ES_R,  ES_T,  ES_Y,  ES_U, ES_I,     ES_O,     ES_P,       ES_LBRC,  ES_RBRC,    ES_BSLS,    KC_HOME,
    KC_CAPS, ES_A,    ES_S,   ES_D,  ES_F,  ES_G,  ES_H,  ES_J, ES_K,     ES_L,     ES_NTIL,    ES_ACUT,         KC_ENT,          KC_PGUP,
    KC_LSFT, ES_Z,    ES_X,   ES_C,  ES_V,  ES_B,  ES_N,  ES_M, ES_COMM,  ES_DOT,   ES_SLSH,           KC_RSFT,          KC_UP,   KC_PGDN,
    KC_LCTL, KC_LGUI, KC_LALT,                     KC_SPC,                KC_RALT,  MO(FUNCTIONS), MO(SYMBOLS), KC_LEFT,  KC_DOWN, KC_RGHT),

[COLEMAK] = LAYOUT_ansi_68(
    MO(MACROS),         ES_1,  ES_2,  ES_3,  ES_4,  ES_5,  ES_6,  ES_7,  ES_8,    ES_9,   ES_0,     MC_RETU,  ES_EQL,   KC_BSPC,   BL_STEP,
    LT(NUMPAD, KC_TAB), ES_Q,  ES_W,  ES_F,  ES_P,  ES_B,  ES_J,  ES_L,  ES_U,    ES_Y,   ES_COLN,  ES_ACUT,  XXXXXXX,  TG(MOUSE), KC_PSCR,
    CTL_T(KC_ESC),      ES_A,  ES_R,  ES_S,  ES_T,  ES_G,  ES_M,  ES_N,  ES_E,    ES_I,   ES_O,     ES_QUOT,       KC_ENT,         KC_PGUP,
    KC_LSFT,            ES_X,  ES_C,  ES_D,  ES_V,  ES_Z,  ES_K,  ES_H,  ES_COMM, ES_DOT, ES_SCLN,          KC_RSFT,        KC_UP, KC_PGDN,
    XXXXXXX,   KC_LGUI, ALT_T(KC_BSPC),    KC_SPC,   LT(SYMBOLS, KC_ENT), MO(FUNCTIONS), XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT),

// [COLEMAK_WITH_MIRRORING] = LAYOUT_ansi_68(
//     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______,     _______,
//     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______,     _______, _______,
//     _______, _______, _______,              LT(COLEMAK_MIRROR, KC_SPC),             _______, _______, _______, _______, _______, _______),

// [COLEMAK_MIRROR] = LAYOUT_ansi_68(
//     MO(MACROS),         ES_1,  ES_2,  ES_3,  ES_4,  ES_5,  ES_6,  ES_7,  ES_8,    ES_9,   ES_0,     MC_RETU,  ES_EQL,   KC_BSPC,   BL_STEP,
//     ES_ACUT, ES_COLN,  ES_Y,  ES_U,  ES_L,  ES_J,  ES_B,  ES_P,  ES_F,    ES_W,   ES_Q,  KC_TAB,  XXXXXXX,  TG(MOUSE), KC_PSCR,
//     ES_QUOT,  ES_O,  ES_I,  ES_E,  ES_N,  ES_M,  ES_G,  ES_T,  ES_S,    ES_R,   ES_A,     KC_LSFT,       KC_ENT,         KC_PGUP,
//     KC_SCLN,            ES_DOT,  ES_COMM,  ES_H,  ES_K,  ES_Z,  ES_V,  ES_D,  ES_C, ES_X, KC_LSFT,          KC_RSFT,        KC_UP, KC_PGDN,
//     XXXXXXX,   KC_LGUI, ALT_T(KC_BSPC),           KC_SPC,           LT(SYMBOLS, KC_ENT), MO(FUNCTIONS), XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT),

[SYMBOLS] = LAYOUT_ansi_68(
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  BL_TOGG,
    _______,  ES_TILD,  ES_DLR,   ES_LBRC,  ES_RBRC,  ES_CIRC,  ES_IEXL,  ES_PERC,  ES_PLUS,  ES_MINS,  ES_ASTR,  XXXXXXX,  XXXXXXX,   XXXXXXX, XXXXXXX,
    _______,  ES_GRV,   ES_UNDS,  ES_LCBR,  ES_RCBR,  ES_PIPE,  ES_IQUE,  ES_LPRN,  ES_RPRN,  ES_EQL,   ES_NTIL,  XXXXXXX,        _______,      XXXXXXX,
    _______,  ES_AT,    ES_AMPR,  ES_DOT,   ES_SLSH,  ES_BSLS,  ES_EXLM,  ES_LABK,  ES_RABK,  ES_HASH,  ES_QUES,       _______,       XXXXXXX,  XXXXXXX,
    _______,  _______,  _______,                              _______,                        _______,  _______,  _______,  _______,  _______,  _______),

[NUMPAD] = LAYOUT_ansi_68(
    XXXXXXX,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,  KC_F8,  KC_F9,  KC_F10,   KC_F11,   KC_F12,      XXXXXXX,     BL_TOGG,
    _______,  KC_HOME,  KC_UP,    KC_END,   KC_PGUP,  XXXXXXX,  ES_SLSH,  ES_7,   ES_8,   ES_9,   ES_ASTR,  ES_MINS,  XXXXXXX,     XXXXXXX,     XXXXXXX,
    _______,  KC_LEFT,  KC_DOWN,  KC_RIGHT, KC_BSPC,  XXXXXXX,  ES_EQL,   ES_4,   ES_5,   ES_6,   ES_PLUS,  ES_COMM,         _______,           _______,
    _______,  XXXXXXX,  KC_DEL,   KC_PGDN,  XXXXXXX,  XXXXXXX,  ES_0,     ES_1,   ES_2,   ES_3,   ES_DOT,         KC_RSFT,          XXXXXXX,    XXXXXXX,
    _______,  _______,  _______,                                _______,                      _______,  _______,  _______,  _______,  _______,  _______),

[FUNCTIONS] = LAYOUT_ansi_68(
    ES_TILD,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_CALC,  KC_F6,    KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE, KC_VOLD,   KC_VOLU,     _______,    BL_TOGG,
    _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  DT_PRNT,  DT_DOWN,  DT_UP,    _______,  _______,  _______,  _______,  _______,     _______,    _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,       _______,
    _______,  _______,  _______,  _______,  _______,  BAT_LVL,  _______,  DB_TOGG,  QK_RBT,  QK_MAKE,  QK_BOOT,          _______,          _______,  _______,
    _______,  _______,  _______,                                 _______,                          _______,  _______,  _______,  _______,  _______,  _______),
[MACROS] = LAYOUT_ansi_68(
    XXXXXXX,  MC_TEST,      MC_RUT,       MC_PHONE,  MC_SKY_EMAIL,  MC_SKCOM, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MC_PWM, XXXXXXX,      XXXXXXX,
    XXXXXXX,  MC_PASSP,     XXXXXXX,      XXXXXXX,   MC_EMAIL,      XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, MC_PW1,  XXXXXXX, XXXXXXX,   XXXXXXX,   XXXXXXX,
    XXXXXXX,  XXXXXXX,      XXXXXXX,      MC_LOCH,   MC_HTTP,  MC_MG_SORT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,         XXXXXXX,     XXXXXXX,
    XXXXXXX,  XXXXXXX,      XXXXXXX,      XXXXXXX,   XXXXXXX,       XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX,      XXXXXXX, XXXXXXX,
    XXXXXXX,  XXXXXXX,      MO(MACROS_2),                                XXXXXXX,                              XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX),
[MACROS_2] = LAYOUT_ansi_68(
    XXXXXXX,  XXXXXXX,      XXXXXXX,       XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MC_PWM, XXXXXXX,      XXXXXXX,
    XXXXXXX,  XXXXXXX,     XXXXXXX,      XXXXXXX,   XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,   XXXXXXX,   XXXXXXX,
    XXXXXXX,  XXXXXXX,      XXXXXXX,      XXXXXXX,   MC_HTTPS,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,         XXXXXXX,     XXXXXXX,
    XXXXXXX,  XXXXXXX,      XXXXXXX,      XXXXXXX,   XXXXXXX,       XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX,      XXXXXXX, XXXXXXX,
    XXXXXXX,  XXXXXXX,      XXXXXXX,                                XXXXXXX,                              XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX),
[MOUSE] = LAYOUT_ansi_68(
    MO(MACROS),  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    XXXXXXX, XXXXXXX,
    _______,     KC_BTN4, KC_MS_U, KC_BTN5, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_WH_U, XXXXXXX, XXXXXXX, XXXXXXX,    _______, XXXXXXX,
    _______,     KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX, XXXXXXX, XXXXXXX, KC_BTN1, KC_WH_D, KC_BTN2, XXXXXXX, XXXXXXX,        XXXXXXX,      XXXXXXX,
    KC_ACL2,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,         _______,     XXXXXXX, XXXXXXX,
    _______,     _______,      _______,                       KC_ACL0,                      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX),
};
// [EMPTY] = LAYOUT_ansi_68(
//     XXXXXXX,  XXXXXXX,      XXXXXXX,      XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MC_PWM, XXXXXXX,      XXXXXXX,
//     XXXXXXX,  XXXXXXX,      XXXXXXX,      XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,   XXXXXXX,   XXXXXXX,
//     XXXXXXX,  XXXXXXX,      XXXXXXX,      XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,         XXXXXXX,     XXXXXXX,
//     XXXXXXX,  XXXXXXX,      XXXXXXX,      XXXXXXX,   XXXXXXX,       XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX,      XXXXXXX, XXXXXXX,
//     XXXXXXX,  XXXXXXX,      XXXXXXX,                                XXXXXXX,                              XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX),


char first_name[8] = "Alberto";
char last_name[5] = "Test";
char rut[10] = "111111111";
char phone[10] = "321232123";
char passport[10] = "";
char email[24] = "pedron.albert@gmail.com";
char sky_email[27] = "user_acid49@skyairline.com";
char pwm[41] = "";
char pw1[11] = "";

// void send_current_date(void) {
//   // char s[100];
//   // time_t temp;
//   // struct tm *timeptr;

//   // temp = time(NULL);
//   // timeptr = localtime(&temp);

//   // strftime(s,sizeof(s),"Today is %A, %b %d.Time:  %r", timeptr);

//   // SEND_STRING(s);
// }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        if (keycode == ES_GRV || keycode == ES_CIRC) {
            unregister_code16(keycode);
            tap_code(KC_SPC);
            return false;
        }
        return true;
    }

    const char* sendStr = NULL;
    uint8_t delay = 10; // Default delay

    switch (keycode) {
        case MC_TEST: sendStr = last_name; break;
        case MC_RUT: sendStr = rut; break;
        case MC_PHONE: sendStr = phone; break;
        case MC_EMAIL: sendStr = email; break;
        case MC_PASSP: sendStr = passport; break;
        case MC_SKY_EMAIL: sendStr = sky_email; break; // Delay is the same for all, can be adjusted if needed
        case MC_PWM: sendStr = pwm; break;
        case MC_PW1: sendStr = pw1; break;
        case MC_RETU: sendStr = "../"; break;
        case MC_MG_SORT: sendStr = "{ _id: -1 }"; break;
        case MC_HTTP: sendStr = "http://"; break;
        case MC_HTTPS: sendStr = "https://"; break;
        case MC_LOCH: sendStr = "localhost:"; break;
        case MC_SKCOM: sendStr = "skyairline.com"; break;
        default: return true;
    }

    if (sendStr != NULL) {
        SEND_STRING_DELAY(sendStr, delay);
        return false;
    }

    return true;
}

const key_override_t quotes_key_override = ko_make_basic(MOD_MASK_SHIFT, ES_QUOT, ES_DQUO);
const key_override_t enie_key_override = ko_make_with_layers(MOD_MASK_SHIFT, ES_LPRN, ES_NTIL, SYMBOLS);
const key_override_t iexcl_key_override = ko_make_basic(MOD_MASK_SHIFT, ES_COMM, ES_IEXL);
const key_override_t excl_key_override = ko_make_basic(MOD_MASK_SHIFT, ES_DOT, ES_EXLM);
const key_override_t iqmark_key_override = ko_make_basic(MOD_MASK_SHIFT, ES_COLN, ES_IQUE);
const key_override_t qmark_key_override = ko_make_basic(MOD_MASK_SHIFT, ES_SCLN, ES_QUES);

const key_override_t **key_overrides = (const key_override_t *[]){
    &quotes_key_override,
    &enie_key_override,
    &iexcl_key_override,
    &excl_key_override,
    &iqmark_key_override,
    &qmark_key_override,
    NULL // Null terminate the array of overrides!
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
        case KC_RALT:
        case ES_DOT:
        case LT(SYMBOLS, KC_ENT):
        case KC_UNDS:
        case ES_UNDS:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(NUMPAD, KC_TAB):
            return TAPPING_TERM + 400;
        default:
            return TAPPING_TERM;
    }
}
