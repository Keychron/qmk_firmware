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

    // L (Sol Janus Dawn): Carga 1 agendada a los ~20s
    macro_conteo_l = 0;
    if (macro_rotacion == 1) {
        macro_proximo_l = ahora + 20000 + (rand() % 2001);
    }

    // G: 1.ª pulsación a los ~25s
    macro_conteo_g = 0;
    macro_proximo_g = ahora + 25000 + (rand() % 1001);

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

    // Liberaciones asíncronas de seguridad
    if (macro_y_activo && timer_expired32(ahora, macro_fin_y)) {
        unregister_code(KC_Y);
        macro_y_activo = false;
        macro_fin_y = 0;
    }

    if (macro_c_activo && timer_expired32(ahora, macro_fin_c)) {
        unregister_code(KC_C);
        macro_c_activo = false;
        macro_fin_c = 0;
    }

    if (macro_l_activo && timer_expired32(ahora, macro_fin_l)) {
        unregister_code(KC_L);
        macro_l_activo = false;
        macro_fin_l = 0;
    }

    if (macro_g_activo && timer_expired32(ahora, macro_fin_g)) {
        unregister_code(KC_G);
        macro_g_activo = false;
        macro_fin_g = 0;
    }

    if (macro_home_activo && timer_expired32(ahora, macro_fin_home)) {
        unregister_code(KC_HOME);
        macro_home_activo = false;
        macro_fin_home = 0;
    }

    if (!timer_expired32(ahora, macro_proximo_evento)) {
        return;
    }

    switch (macro_estado) {

        /* ----------------------------------------------------
         * SECUENCIA INICIAL: F1 -> Y -> SHIFT+W -> HOME
         * ---------------------------------------------------- */
        case 0:
            macro_liberar_teclas();
            macro_proximo_evento = ahora + 150;
            macro_estado = 1;
            break;

        case 1:
            register_code(KC_F1);
            macro_proximo_evento = ahora + 80 + (rand() % 31);
            macro_proximo_c = ahora + 60000 + (rand() % 2001);
            macro_estado = 2;
            break;

        case 2:
            unregister_code(KC_F1);
            macro_proximo_evento = ahora + 1000 + (rand() % 1001);
            macro_estado = 3;
            break;

        // Erda Shower (Y) Inicial
        case 3:
            register_code(KC_Y);
            macro_y_activo = true;
            macro_fin_y = ahora + 120 + (rand() % 31);
            macro_proximo_y = ahora + 60000 + (rand() % 3001); // Recast 60-63s
            macro_proximo_evento = macro_fin_y;
            macro_estado = 4;
            break;

        case 4:
            unregister_code(KC_Y);
            macro_y_activo = false;
            macro_fin_y = 0;
            macro_proximo_evento = ahora + 400 + (rand() % 101);
            macro_estado = 5;
            break;

        case 5:
            register_code(KC_LSFT);
            macro_proximo_evento = ahora + 40 + (rand() % 25);
            macro_estado = 6;
            break;

        case 6:
            unregister_code(KC_LSFT);
            macro_proximo_evento = ahora + 600 + (rand() % 151);
            macro_estado = 7;
            break;

        case 7:
            register_code(KC_W);
            macro_proximo_evento = ahora + 100 + (rand() % 25);
            macro_estado = 8;
            break;

        case 8:
            unregister_code(KC_W);
            macro_combos_realizados++;
            
            macro_proximo_evento = ahora + 200 + (rand() % 51);
            macro_estado = 50;
            break;

        /* ====================================================
         * HOME DEDICADO Y AISLADO
         * ==================================================== */
        case 50:
            macro_liberar_teclas();
            macro_proximo_evento = ahora + 150;
            macro_estado = 51;
            break;

        case 51:
            register_code(KC_HOME);
            macro_home_activo = true;
            macro_fin_home = ahora + 140 + (rand() % 41);

            macro_proximo_home = ahora + 100000 + (rand() % 3001);
            macro_proximo_sws = ahora + 7000 + (rand() % 501);

            macro_proximo_evento = macro_fin_home;
            macro_estado = 52;
            break;

        case 52:
            unregister_code(KC_HOME);
            macro_home_activo = false;
            macro_fin_home = 0;

            macro_proximo_evento = ahora + 200 + (rand() % 51);
            macro_estado = 10;
            break;

        /* ====================================================
         * BUCLE PRINCIPAL (JERARQUÍA Y PRIORIDAD)
         * ==================================================== */

        case 10:
            // 1. HOME (Prioridad Máxima)
            if (macro_proximo_home != 0 && timer_expired32(ahora, macro_proximo_home)) {
                macro_estado = 50;
                break;
            }

            // 2. SOL JANUS DAWN (L): Cargas aisladas
            if (macro_proximo_l != 0 && timer_expired32(ahora, macro_proximo_l)) {
                macro_liberar_teclas();
                macro_proximo_evento = ahora + 100;
                macro_estado = 60;
                break;
            }

            // 3. ERDA SHOWER (Y): Colocación cada 60-63s
            if (macro_proximo_y != 0 && timer_expired32(ahora, macro_proximo_y)) {
                macro_liberar_teclas();
                macro_proximo_evento = ahora + 100;
                macro_estado = 90;
                break;
            }

            // 4. G (2 veces por rotación)
            if (macro_proximo_g != 0 && timer_expired32(ahora, macro_proximo_g)) {
                macro_liberar_teclas();
                macro_proximo_evento = ahora + 100;
                macro_estado = 70;
                break;
            }

            // 5. C (Aislada)
            if (macro_proximo_c != 0 && timer_expired32(ahora, macro_proximo_c)) {
                macro_liberar_teclas();
                macro_proximo_evento = ahora + 100;
                macro_estado = 80;
                break;
            }

            // 6. Shift + W + S (Cada 7s)
            if (macro_proximo_sws != 0 && timer_expired32(ahora, macro_proximo_sws)) {
                macro_estado = 20;
                break;
            }

            // 7. Seleccionar combo de combate base
            if (macro_proximo_home != 0 && (macro_proximo_home - ahora <= 10000)) {
                macro_estado = 30; // Ruta Shift + Q (10s antes de Home)
            } else {
                macro_estado = 11; // Ruta Shift + W
            }
            break;

        /* --- RUTA SHIFT + W (COMBO BASE DIRECTO Y CONTINUO) --- */
        case 11:
            register_code(KC_LSFT);
            macro_proximo_evento = ahora + 40 + (rand() % 25);
            macro_estado = 12;
            break;

        case 12:
            register_code(KC_W);
            macro_proximo_evento = ahora + 100 + (rand() % 25);
            macro_estado = 13;
            break;

        case 13:
            unregister_code(KC_W);
            unregister_code(KC_LSFT);

            macro_combos_realizados++;

            if (timer_expired32(ahora, macro_fin_rotacion)) {
                macro_liberar_teclas();

                if (macro_rotacion >= 9) {
                    macro_cancelar();
                    return;
                }

                macro_proximo_evento = ahora + 1500 + (rand() % 1001);
                macro_estado = 100;
                return;
            }

            macro_proximo_evento = ahora + 650 + (rand() % 151);
            macro_estado = 10;
            break;

        /* --- RUTA SHIFT + Q (10 SECONDS ANTES DE HOME) --- */
        case 30:
            register_code(KC_LSFT);
            macro_proximo_evento = ahora + 40 + (rand() % 25);
            macro_estado = 31;
            break;

        case 31:
            register_code(KC_Q);
            macro_proximo_evento = ahora + 100 + (rand() % 25);
            macro_estado = 32;
            break;

        case 32:
            unregister_code(KC_Q);
            unregister_code(KC_LSFT);

            macro_combos_realizados++;

            if (timer_expired32(ahora, macro_fin_rotacion)) {
                macro_liberar_teclas();

                if (macro_rotacion >= 9) {
                    macro_cancelar();
                    return;
                }

                macro_proximo_evento = ahora + 1500 + (rand() % 1001);
                macro_estado = 100;
                return;
            }

            macro_proximo_evento = ahora + 650 + (rand() % 151);
            macro_estado = 10;
            break;

        /* --- RUTA SHIFT + W + S (CADA 7 SEG) --- */
        case 20:
            register_code(KC_LSFT);
            macro_proximo_evento = ahora + 40 + (rand() % 25);
            macro_estado = 21;
            break;

        case 21:
            register_code(KC_W);
            macro_proximo_evento = ahora + 100 + (rand() % 25);
            macro_estado = 22;
            break;

        case 22:
            unregister_code(KC_W);
            macro_proximo_evento = ahora + 150 + (rand() % 81);
            macro_estado = 23;
            break;

        case 23:
            register_code(KC_S);
            macro_proximo_evento = ahora + 100 + (rand() % 25);
            macro_estado = 24;
            break;

        case 24:
            unregister_code(KC_S);
            unregister_code(KC_LSFT);

            macro_proximo_sws = ahora + 7000 + (rand() % 501);

            if (timer_expired32(ahora, macro_fin_rotacion)) {
                macro_liberar_teclas();

                if (macro_rotacion >= 9) {
                    macro_cancelar();
                    return;
                }

                macro_proximo_evento = ahora + 1500 + (rand() % 1001);
                macro_estado = 100;
                return;
            }

            macro_proximo_evento = ahora + 650 + (rand() % 151);
            macro_estado = 10;
            break;

        /* --- SOL JANUS DAWN (L): CARGA INDIVIDUAL + RETORNO --- */
        case 60:
            register_code(KC_L);
            macro_l_activo = true;
            macro_fin_l = ahora + 130 + (rand() % 31);
            macro_proximo_evento = macro_fin_l;
            macro_estado = 61;
            break;

        case 61:
            unregister_code(KC_L);
            macro_l_activo = false;
            macro_fin_l = 0;
            macro_conteo_l++;

            if (macro_conteo_l < 3) {
                // Programa la siguiente carga tras 5 a 7s de combate
                macro_proximo_l = ahora + 5000 + (rand() % 2001);
            } else {
                // Cooldown de 60 a 63s de Sol Janus tras colocar las 3 cargas
                macro_proximo_l = ahora + 60000 + (rand() % 3001);
                macro_conteo_l = 0;
            }

            macro_proximo_evento = ahora + 150;
            macro_estado = 10;
            break;

        /* --- ERDA SHOWER (Y): SECUENCIA AISLADA --- */
        case 90:
            register_code(KC_Y);
            macro_y_activo = true;
            macro_fin_y = ahora + 130 + (rand() % 31);
            macro_proximo_y = ahora + 60000 + (rand() % 3001); // Recast 60-63s
            macro_proximo_evento = macro_fin_y;
            macro_estado = 91;
            break;

        case 91:
            unregister_code(KC_Y);
            macro_y_activo = false;
            macro_fin_y = 0;
            macro_proximo_evento = ahora + 150;
            macro_estado = 10;
            break;

        /* --- TECLA G (2 veces por rotación) --- */
        case 70:
            register_code(KC_G);
            macro_g_activo = true;
            macro_fin_g = ahora + 130 + (rand() % 31);

            macro_conteo_g++;

            if (macro_conteo_g < 2) {
                macro_proximo_g = ahora + 55000 + (rand() % 2001);
            } else {
                macro_proximo_g = 0;
            }

            macro_proximo_evento = macro_fin_g;
            macro_estado = 71;
            break;

        case 71:
            unregister_code(KC_G);
            macro_g_activo = false;
            macro_fin_g = 0;
            macro_proximo_evento = ahora + 150;
            macro_estado = 10;
            break;

        /* --- TECLA C --- */
        case 80:
            register_code(KC_C);
            macro_c_activo = true;
            macro_fin_c = ahora + 130 + (rand() % 31);
            macro_proximo_c = ahora + 60000 + (rand() % 2001);

            macro_proximo_evento = macro_fin_c;
            macro_estado = 81;
            break;

        case 81:
            unregister_code(KC_C);
            macro_c_activo = false;
            macro_fin_c = 0;
            macro_proximo_evento = ahora + 150;
            macro_estado = 10;
            break;

        case 100:
            macro_nueva_rotacion();
            break;
    }
}
