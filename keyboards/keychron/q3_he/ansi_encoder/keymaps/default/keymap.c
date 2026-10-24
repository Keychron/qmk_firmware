/* Copyright 2024 ~ 2026 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or later.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

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
        KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, UG_VALD, UG_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_MUTE, KC_SNAP, KC_SIRI, UG_NEXT,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS, KC_EQL,  KC_BSPC, KC_INS, KC_HOME, KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL, KC_END, KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN, KC_QUOT,             KC_ENT,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM, KC_DOT,  KC_SLSH,             KC_RSFT,              KC_UP,
        KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, KC_ROPTN, FN_MAC, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [MAC_FN] = LAYOUT_ansi_87(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  UG_TOGG, _______, _______, UG_TOGG,
        _______, BT_HST1, BT_HST2, BT_HST3, P2P4G,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        UG_TOGG, UG_NEXT, UG_VALU, UG_HUEU, UG_SATU, UG_SPDU, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, UG_PREV, UG_VALD, UG_HUED, UG_SATD, UG_SPDD, _______, _______, _______, _______, _______, _______,          _______,
        _______,          _______, _______, _______, _______, BAT_LVL, _______, _______, _______, _______, _______,          _______,          _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______, _______, _______
    ),

    [WIN_BASE] = LAYOUT_ansi_87(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_MUTE, KC_PSCR, KC_CTANA, UG_NEXT,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_INS,  KC_HOME,  KC_PGUP,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL,  KC_END,   KC_PGDN,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,           KC_UP,
        KC_LCTL, KC_LWIN, KC_LALT,                   KC_SPC,                             KC_RALT, KC_RWIN, FN_WIN,  KC_RCTL, KC_LEFT, KC_DOWN,  KC_RGHT
    ),

    [WIN_FN] = LAYOUT_ansi_87(
        MACRO_HUMANA, KC_BRID, KC_BRIU, KC_TASK, KC_FILE, UG_VALD, UG_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, UG_TOGG, _______, _______, UG_TOGG,
        _______,      BT_HST1, BT_HST2, BT_HST3, P2P4G,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        UG_TOGG,      UG_NEXT, UG_VALU, UG_HUEU, UG_SATU, UG_SPDU, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,      UG_PREV, UG_VALD, UG_HUED, UG_SATD, UG_SPDD, _______, _______, _______, MACRO_CANCEL, _______, _______,       _______,
        _______,               _______, _______, _______, _______, BAT_LVL, _______, _______, _______, _______, _______,       _______,          _______,
        _______,      _______, _______,                            _______,                            _______, _______, _______, _______, _______, _______, _______
    ),
};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
    [WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
};

#endif

static bool macro_activo = false;
static bool macro_y_activo = false;
static bool macro_c_activo = false;
static bool macro_l_activo = false;
static bool macro_g_activo = false;
static bool macro_home_activo = false;

static uint8_t macro_rotacion = 0;
static uint8_t macro_estado = 0;

static uint32_t macro_proximo_evento = 0;
static uint32_t macro_fin_rotacion = 0;

static uint32_t macro_proximo_y = 0;
static uint32_t macro_fin_y = 0;

static uint32_t macro_proximo_c = 0;
static uint32_t macro_fin_c = 0;

static uint32_t macro_proximo_l = 0;
static uint32_t macro_fin_l = 0;

static uint32_t macro_proximo_g = 0;
static uint32_t macro_fin_g = 0;

static uint32_t macro_proximo_home = 0;
static uint32_t macro_fin_home = 0;

static uint32_t macro_proximo_sws = 0;

static uint8_t macro_combos_realizados = 0;
static uint8_t macro_conteo_l = 0;
static uint8_t macro_conteo_g = 0;

static void macro_liberar_teclas(void) {
    unregister_code(KC_HOME);
    unregister_code(KC_F1);
    unregister_code(KC_LSFT);
    unregister_code(KC_W);
    unregister_code(KC_Q);
    unregister_code(KC_S);
    unregister_code(KC_Y);
    unregister_code(KC_C);
    unregister_code(KC_L);
    unregister_code(KC_G);

    macro_y_activo = false;
    macro_fin_y = 0;

    macro_c_activo = false;
    macro_fin_c = 0;

    macro_l_activo = false;
    macro_fin_l = 0;

    macro_g_activo = false;
    macro_fin_g = 0;

    macro_home_activo = false;
    macro_fin_home = 0;
}

static void macro_cancelar(void) {
    macro_liberar_teclas();

    macro_activo = false;

    macro_rotacion = 0;
    macro_estado = 0;

    macro_proximo_evento = 0;
    macro_fin_rotacion = 0;

    macro_proximo_y = 0;
    macro_proximo_c = 0;
    macro_proximo_l = 0;
    macro_proximo_g = 0;
    macro_proximo_home = 0;
    macro_proximo_sws = 0;

    macro_combos_realizados = 0;
    macro_conteo_l = 0;
    macro_conteo_g = 0;
}

static void macro_nueva_rotacion(void) {
    uint32_t ahora = timer_read32();

    macro_rotacion++;

    macro_fin_rotacion = ahora + 114000 + (rand() % 3001);

    macro_proximo_evento = ahora;

    macro_combos_realizados = 0;

    // L: Se programa para iniciar exactamente en el medio (~45 segundos tras iniciar la rotación)
    macro_conteo_l = 0;
    macro_proximo_l = ahora + 45000 + (rand() % 2001);

    // G: 1.ª pulsación agendada a los ~25 segundos del inicio de la rotación
    macro_conteo_g = 0;
    macro_proximo_g = ahora + 25000 + (rand() % 2001);

    macro_estado = 0;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    switch (keycode) {

        case MACRO_HUMANA:

            if (record->event.pressed && !macro_activo) {

                srand(timer_read32());

                macro_activo = true;

                macro_rotacion = 0;
                macro_estado = 0;

                macro_proximo_evento = 0;
                macro_fin_rotacion = 0;

                macro_proximo_y = 0;
                macro_fin_y = 0;
                macro_y_activo = false;

                macro_proximo_c = 0;
                macro_fin_c = 0;
                macro_c_activo = false;

                macro_proximo_l = 0;
                macro_fin_l = 0;
                macro_l_activo = false;

                macro_proximo_g = 0;
                macro_fin_g = 0;
                macro_g_activo = false;

                macro_proximo_home = 0;
                macro_fin_home = 0;
                macro_home_activo = false;

                macro_proximo_sws = 0;
                macro_combos_realizados = 0;
                macro_conteo_l = 0;
                macro_conteo_g = 0;

                macro_nueva_rotacion();
            }

            return false;

        case MACRO_CANCEL:

            if (record->event.pressed) {
                macro_cancelar();
            }

            return false;
    }

    return true;
}

void housekeeping_task_user(void) {

    if (!macro_activo) {
        return;
    }

    uint32_t ahora = timer_read32();

    // Liberación asíncrona de Y
    if (macro_y_activo && timer_expired32(ahora, macro_fin_y)) {
        unregister_code(KC_Y);
        macro_y_activo = false;
        macro_fin_y = 0;
    }

    // Liberación asíncrona de C
    if (macro_c_activo && timer_expired32(ahora, macro_fin_c)) {
        unregister_code(KC_C);
        macro_c_activo = false;
        macro_fin_c = 0;
    }

    // Liberación asíncrona de L
    if (macro_l_activo && timer_expired32(ahora, macro_fin_l)) {
        unregister_code(KC_L);
        macro_l_activo = false;
        macro_fin_l = 0;
    }

    // Liberación asíncrona de G
    if (macro_g_activo && timer_expired32(ahora, macro_fin_g)) {
        unregister_code(KC_G);
        macro_g_activo = false;
        macro_fin_g = 0;
    }

    // Liberación asíncrona de Home
    if (macro_home_activo && timer_expired32(ahora, macro_fin_home)) {
        unregister_code(KC_HOME);
        macro_home_activo = false;
        macro_fin_home = 0;
    }

    if (!timer_expired32(ahora, macro_proximo_evento)) {
        return;
    }

    switch (macro_estado) {
