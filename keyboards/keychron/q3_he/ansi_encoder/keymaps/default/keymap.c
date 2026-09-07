/* Copyright 2024 ~ 2025 @ Keychron (https://www.keychron.com)
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
        KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                  KC_RCMMD, KC_ROPTN, FN_MAC, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

       [MAC_FN] = LAYOUT_ansi_87(
    _______, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, UG_TOGG, _______, _______, UG_TOGG,
    _______, BT_HST1, BT_HST2, BT_HST3, P2P4G, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    UG_TOGG, UG_NEXT, UG_VALU, UG_HUEU, UG_SATU, UG_SPDU, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, UG_PREV, UG_VALD, UG_HUED, UG_SATD, UG_SPDD, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, BAT_LVL, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

[WIN_BASE] = LAYOUT_ansi_87(
    KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_MUTE, KC_PSCR, KC_CTANA, UG_NEXT,
    KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_INS, KC_HOME, KC_PGUP,
    KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL, KC_END, KC_PGDN,
    KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,
    KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP,
    KC_LCTL, KC_LWIN, KC_LALT, KC_SPC, KC_RALT, KC_RWIN, FN_WIN, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
),

[WIN_FN] = LAYOUT_ansi_87(
    MACRO_HUMANA, KC_BRID, KC_BRIU, KC_TASK, KC_FILE, UG_VALD, UG_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, UG_TOGG, _______, _______, UG_TOGG,
    _______, BT_HST1, BT_HST2, BT_HST3, P2P4G, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    UG_TOGG, UG_NEXT, UG_VALU, UG_HUEU, UG_SATU, UG_SPDU, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, UG_PREV, UG_VALD, UG_HUED, UG_SATD, UG_SPDD, _______, _______, _______, MACRO_CANCEL, _______, _______, _______,
    _______, _______, _______, _______, _______, BAT_LVL, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

// clang-format on


#if defined(ENCODER_MAP_ENABLE)

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
    [WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
};

#endif


/* ============================================================
 * MACRO
 * ============================================================
 *
 * 5 rotaciones.
 *
 * Cada rotación:
 *
 *   F1
 *   espera 1250-1470 ms
 *   Shift
 *   W
 *   pausa 900-1100 ms
 *   Shift
 *   pausa 500-1000 ms
 *   Y
 *   pausa 900-1100 ms
 *   Shift / W / Shift / W / ...
 *
 * Duración aproximada de cada rotación:
 *   120000-125000 ms
 *
 * IMPORTANTE:
 * El temporizador de rotación NO corta una acción.
 * La rotación solamente termina después de completar
 * un ciclo completo Shift + W.
 *
 * Entre rotaciones:
 *   1500-2500 ms
 *
 * Y:
 *   Primera Y: 500-1000 ms después del segundo Shift.
 *   Después: cada 60000-63000 ms.
 *
 * El temporizador de Y es GLOBAL y NO se reinicia
 * al comenzar una nueva rotación.
 *
 * Fn + L:
 *   cancelación inmediata.
 *
 * Fn + Esc:
 *   iniciar/reiniciar después de una cancelación.
 * ============================================================
 */


static bool macro_activo = false;
static bool macro_y_activo = false;

static uint8_t macro_rotacion = 0;
static uint8_t macro_estado = 0;

static uint32_t macro_proximo_evento = 0;
static uint32_t macro_fin_rotacion = 0;

static uint32_t macro_proximo_y = 0;
static uint32_t macro_fin_y = 0;


/* ------------------------------------------------------------
 * Liberar todas las teclas utilizadas por el macro
 * ------------------------------------------------------------ */

static void macro_liberar_teclas(void) {
    unregister_code(KC_F1);
    unregister_code(KC_LSFT);
    unregister_code(KC_W);
    unregister_code(KC_Y);

    macro_y_activo = false;
    macro_fin_y = 0;
}


/* ------------------------------------------------------------
 * Cancelación completa
 * ------------------------------------------------------------ */

static void macro_cancelar(void) {
    macro_liberar_teclas();

    macro_activo = false;

    macro_rotacion = 0;
    macro_estado = 0;

    macro_proximo_evento = 0;
    macro_fin_rotacion = 0;

    macro_proximo_y = 0;
}


/* ------------------------------------------------------------
 * Iniciar una nueva rotación
 *
 * La nueva rotación comienza exactamente aquí con F1.
 * El temporizador de 120-125 s comienza también aquí.
 * ------------------------------------------------------------ */

static void macro_nueva_rotacion(void) {
    uint32_t ahora = timer_read32();

    macro_rotacion++;

    macro_fin_rotacion =
        ahora + 120000 + (rand() % 5001);

    macro_proximo_evento = ahora;

    macro_estado = 1;
}


/* ------------------------------------------------------------
 * Manejo de teclas especiales del macro
 * ------------------------------------------------------------ */

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

                /*
                 * Primera rotación.
                 * Comienza con F1.
                 */
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


/* ============================================================
 * STATE MACHINE
 * ============================================================ */

void housekeeping_task_user(void) {

    if (!macro_activo) {
        return;
    }

    uint32_t ahora = timer_read32();


    /* --------------------------------------------------------
     * Liberar Y cuando termine su duración
     * -------------------------------------------------------- */

    if (macro_y_activo &&
        timer_expired32(ahora, macro_fin_y)) {

        unregister_code(KC_Y);

        macro_y_activo = false;
        macro_fin_y = 0;
    }


    /* --------------------------------------------------------
     * Esperar hasta que llegue el momento del siguiente evento
     * -------------------------------------------------------- */

    if (!timer_expired32(ahora, macro_proximo_evento)) {
        return;
    }


    /* ========================================================
     * ROTACIÓN: F1
     * ======================================================== */


    switch (macro_estado) {

        /* ----------------------------------------------------
         * 1. F1 DOWN
         * ---------------------------------------------------- */

        case 1:

            register_code(KC_F1);

            macro_proximo_evento =
                ahora + 80 + (rand() % 31);

            macro_estado = 2;

            break;


        /* ----------------------------------------------------
         * 2. F1 UP
         *
         * Espera 1250-1470 ms.
         * ---------------------------------------------------- */

        case 2:

            unregister_code(KC_F1);

            macro_proximo_evento =
                ahora + 1250 + (rand() % 221);

            macro_estado = 3;

            break;


        /* ----------------------------------------------------
         * 3. Primer Shift
         * ---------------------------------------------------- */

        case 3:

            register_code(KC_LSFT);

            macro_proximo_evento =
                ahora + 120 + (rand() % 35);

            macro_estado = 4;

            break;


        /* ----------------------------------------------------
         * 4. W
         * ---------------------------------------------------- */

        case 4:

            register_code(KC_W);

            macro_proximo_evento =
                ahora + 120 + (rand() % 35);

            macro_estado = 5;

            break;


        /* ----------------------------------------------------
         * 5. Liberar W + Shift
         *
         * Esta es la primera acción Shift + W.
         * ---------------------------------------------------- */

        case 5:

            unregister_code(KC_W);
            unregister_code(KC_LSFT);

            macro_proximo_evento =
                ahora + 900 + (rand() % 201);

            macro_estado = 6;

            break;


        /* ----------------------------------------------------
         * 6. Segundo Shift
         * ---------------------------------------------------- */

        case 6:

            register_code(KC_LSFT);

            macro_proximo_evento =
                ahora + 120 + (rand() % 35);

            macro_estado = 7;

            break;


        /* ----------------------------------------------------
         * 7. Liberar segundo Shift
         * ---------------------------------------------------- */

        case 7:

            unregister_code(KC_LSFT);

            macro_proximo_evento =
                ahora + 500 + (rand() % 501);

            macro_estado = 8;

            break;


        /* ----------------------------------------------------
         * 8. Primera Y
         *
         * Esta es la Y inicial.
         * Después se programa la siguiente Y para
         * 60-63 segundos.
         * ---------------------------------------------------- */

        case 8:

            register_code(KC_Y);

            macro_y_activo = true;

            macro_fin_y =
                ahora + 100 + (rand() % 41);

            /*
             * IMPORTANTE:
             * Este contador NO se reinicia al cambiar
             * de rotación.
             */
            macro_proximo_y =
                ahora + 60000 + (rand() % 3001);

            macro_proximo_evento =
                macro_fin_y;

            macro_estado = 9;

            break;


        /* ----------------------------------------------------
         * 9. Liberar primera Y
         * ---------------------------------------------------- */

        case 9:

            unregister_code(KC_Y);

            macro_y_activo = false;
            macro_fin_y = 0;

            macro_proximo_evento =
                ahora + 900 + (rand() % 201);

            macro_estado = 10;

            break;


        /* ====================================================
         * BUCLE PRINCIPAL SHIFT / W
         * ==================================================== */


        /* ----------------------------------------------------
         * 10. Antes de iniciar otro ciclo Shift + W
         *
         * Si Y ya está programada y llegó su momento,
         * hacemos Y primero.
         * ---------------------------------------------------- */

        case 10:

            if (macro_proximo_y != 0 &&
                timer_expired32(ahora, macro_proximo_y)) {

                register_code(KC_Y);

                macro_y_activo = true;

                macro_fin_y =
                    ahora + 100 + (rand() % 41);

                /*
                 * Próxima Y: nuevamente 60-63 s.
                 */
                macro_proximo_y =
                    ahora + 60000 + (rand() % 3001);

                macro_proximo_evento =
                    macro_fin_y;

                macro_estado = 14;

                break;
            }


            /*
             * No hay Y pendiente.
             * Comenzamos otro conjunto Shift + W.
             */

            register_code(KC_LSFT);

            macro_proximo_evento =
                ahora + 120 + (rand() % 35);

            macro_estado = 11;

            break;


        /* ----------------------------------------------------
         * 11. Liberar Shift
         * ---------------------------------------------------- */

        case 11:

            unregister_code(KC_LSFT);

            macro_proximo_evento =
                ahora + 800 + (rand() % 201);

            macro_estado = 12;

            break;


        /* ----------------------------------------------------
         * 12. W DOWN
         * ---------------------------------------------------- */

        case 12:

            register_code(KC_W);

            macro_proximo_evento =
                ahora + 120 + (rand() % 35);

            macro_estado = 13;

            break;


        /* ----------------------------------------------------
         * 13. W UP
         *
         * AQUÍ se comprueba el final de la rotación.
         *
         * Nunca se corta antes de completar este W.
         * Por eso, cuando termina una rotación, la última
         * acción completa siempre es Shift + W.
         * ---------------------------------------------------- */

        case 13:

            unregister_code(KC_W);


            /*
             * La acción Shift + W acaba de terminar.
             *
             * Ahora sí comprobamos si alcanzamos
             * los 120-125 segundos.
             */

            if (timer_expired32(ahora, macro_fin_rotacion)) {

                /*
                 * Garantizamos que no quede ninguna tecla
                 * presionada antes de la transición.
                 */
                unregister_code(KC_LSFT);
                unregister_code(KC_W);


                /*
                 * ¿Fue la quinta rotación?
                 */
                if (macro_rotacion >= 5) {

                    macro_cancelar();

                    return;
                }


                /*
                 * Todavía quedan rotaciones.
                 *
                 * Pausa aleatoria de 1500-2500 ms.
                 *
                 * Durante esta pausa NO reiniciamos
                 * el temporizador global de Y.
                 */

                macro_proximo_evento =
                    ahora + 1500 + (rand() % 1001);

                macro_estado = 100;

                return;
            }


            /*
             * Todavía no terminó la rotación.
             *
             * Pausa normal después de W.
             */

            macro_proximo_evento =
                ahora + 900 + (rand() % 201);

            macro_estado = 10;

            break;


        /* ====================================================
         * Y PERIÓDICA
         * ==================================================== */


        /* ----------------------------------------------------
         * 14. Liberar Y periódica
         * ---------------------------------------------------- */

        case 14:

            unregister_code(KC_Y);

            macro_y_activo = false;
            macro_fin_y = 0;

            /*
             * Después de Y continuamos el ritmo normal.
             */
            macro_proximo_evento =
                ahora + 900 + (rand() % 201);

            macro_estado = 10;

            break;


        /* ====================================================
         * TRANSICIÓN ENTRE ROTACIONES
         * ==================================================== */


        /* ----------------------------------------------------
         * 100. Comienza la siguiente rotación
         *
         * macro_nueva_rotacion():
         *   incrementa contador
         *   crea nuevo tiempo 120-125 s
         *   comienza con F1
         *
         * El temporizador de Y NO se modifica.
         * ---------------------------------------------------- */

        case 100:

            macro_nueva_rotacion();

            break;
    }
}
