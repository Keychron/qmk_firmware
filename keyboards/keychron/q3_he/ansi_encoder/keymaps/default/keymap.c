/* Copyright 2024 ~ 2025 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"

enum custom_keycodes {
    MACRO_HUMANA = SAFE_RANGE,
    MACRO_CANCEL,
};

enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
};

#define FN_MAC MO(MAC_FN)
#define FN_WIN MO(WIN_FN)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_ansi_87(
        KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_MUTE,   KC_SNAP,  KC_SIRI,  UG_NEXT,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,   KC_INS,   KC_HOME,  KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,   KC_DEL,   KC_END,   KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,             KC_UP,
        KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, KC_ROPTN, FN_MAC,   KC_RCTL,   KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_ansi_87(
        _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   UG_TOGG,   _______,  _______,  UG_TOGG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,
        UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,
        _______,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,            _______,             _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,   _______,  _______,  _______),

    [WIN_BASE] = LAYOUT_ansi_87(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_MUTE,   KC_PSCR,  KC_CTANA, UG_NEXT,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,   KC_INS,   KC_HOME,  KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,   KC_DEL,   KC_END,   KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,             KC_UP,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RWIN,  FN_WIN,   KC_RCTL,   KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_ansi_87(
        MACRO_HUMANA, KC_BRID, KC_BRIU, KC_TASK, KC_FILE, UG_VALD, UG_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, UG_TOGG, _______, _______, UG_TOGG,
        _______, BT_HST1, BT_HST2, BT_HST3, P2P4G, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        UG_TOGG, UG_NEXT, UG_VALU, UG_HUEU, UG_SATU, UG_SPDU, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, UG_PREV, UG_VALD, UG_HUED, UG_SATD, UG_SPDD, _______, _______, _______, MACRO_CANCEL, _______, _______, _______,
        _______, _______, _______, _______, _______, BAT_LVL, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
    [WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
};
#endif

static bool macro_activo = false;
static uint8_t macro_rotacion = 0;
static uint8_t macro_estado = 0;

static uint32_t macro_proximo_evento = 0;
static uint32_t macro_fin_rotacion = 0;
static uint32_t macro_proximo_y = 0;

static void macro_liberar_teclas(void) {
    unregister_code(KC_F1);
    unregister_code(KC_LSFT);
    unregister_code(KC_W);
    unregister_code(KC_Y);
}

static void macro_cancelar(void) {
    macro_activo = false;
    macro_estado = 0;
    macro_rotacion = 0;
    macro_liberar_teclas();
}

static void macro_nueva_rotacion(void) {
    macro_rotacion++;

    macro_estado = 1;
    macro_fin_rotacion = timer_read32() + 120000 + (rand() % 5001);
    macro_proximo_y = timer_read32() + 60000 + (rand() % 3001);
    macro_proximo_evento = timer_read32();
}

void process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MACRO_HUMANA:
            if (record->event.pressed && !macro_activo) {
                srand(timer_read32());
                macro_activo = true;
                macro_rotacion = 0;
                macro_nueva_rotacion();
            }
            return;

        case MACRO_CANCEL:
            if (record->event.pressed && macro_activo) {
                macro_cancelar();
            }
            return;
    }

    return true;
}

void housekeeping_task_user(void) {
    if (!macro_activo) {
        return;
    }

    uint32_t ahora = timer_read32();

    if (timer_expired32(ahora, macro_fin_rotacion)) {
        macro_liberar_teclas();

        if (macro_rotacion >= 5) {
            macro_cancelar();
            return;
        }

        macro_estado = 10;
        macro_proximo_evento = ahora + 1500 + (rand() % 1001);
    }

    if (timer_expired32(ahora, macro_proximo_y)) {
        macro_liberar_teclas();

        register_code(KC_Y);
        macro_proximo_evento = ahora + 100 + (rand() % 41);
        macro_estado = 20;

        macro_proximo_y = ahora + 60000 + (rand() % 3001);
    }

    if (!timer_expired32(ahora, macro_proximo_evento)) {
        return;
    }

    switch (macro_estado) {
        case 1:
            register_code(KC_F1);
            macro_proximo_evento = ahora + 80 + (rand() % 31);
            macro_estado = 2;
            break;

        case 2:
            unregister_code(KC_F1);
            macro_proximo_evento = ahora + 1250 + (rand() % 221);
            macro_estado = 3;
            break;

        case 3:
            register_code(KC_LSFT);
            macro_proximo_evento = ahora + 120 + (rand() % 35);
            macro_estado = 4;
            break;

        case 4:
            unregister_code(KC_LSFT);
            macro_proximo_evento = ahora + 800 + (rand() % 201);
            macro_estado = 5;
            break;

        case 5:
            register_code(KC_W);
            macro_proximo_evento = ahora + 120 + (rand() % 35);
            macro_estado = 6;
            break;

        case 6:
            unregister_code(KC_W);
            macro_proximo_evento = ahora + 900 + (rand() % 201);
            macro_estado = 7;
            break;

        case 7:
            register_code(KC_LSFT);
            macro_proximo_evento = ahora + 120 + (rand() % 35);
            macro_estado = 8;
            break;

        case 8:
            unregister_code(KC_LSFT);
            macro_proximo_evento = ahora + 800 + (rand() % 201);
            macro_estado = 9;
            break;

        case 9:
            register_code(KC_W);
            macro_proximo_evento = ahora + 120 + (rand() % 35);
            macro_estado = 11;
            break;

        case 11:
            unregister_code(KC_W);
            macro_proximo_evento = ahora + 900 + (rand() % 201);
            macro_estado = 12;
            break;

        case 12:
            register_code(KC_LSFT);
            macro_proximo_evento = ahora + 120 + (rand() % 35);
            macro_estado = 13;
            break;

        case 13:
            unregister_code(KC_LSFT);
            macro_proximo_evento = ahora + 800 + (rand() % 201);
            macro_estado = 14;
            break;

        case 14:
            register_code(KC_W);
            macro_proximo_evento = ahora + 120 + (rand() % 35);
            macro_estado = 15;
            break;

        case 15:
            unregister_code(KC_W);
            macro_proximo_evento = ahora + 900 + (rand() % 201);
            macro_estado = 12;
            break;

        case 10:
            macro_nueva_rotacion();
            break;

        case 20:
            unregister_code(KC_Y);
            macro_estado = 15;
            macro_proximo_evento = ahora + 900 + (rand() % 201);
            break;
    }
}
