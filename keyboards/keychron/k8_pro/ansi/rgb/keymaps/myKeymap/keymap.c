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
#include "keymap_japanese.h"

// --- GUI(L/R) + Space で日英切替処理 ======================================================
bool process_gui_space_ime(uint16_t keycode, keyrecord_t *record) {
    // WIN_BASE レイヤー限定
    if (get_highest_layer(layer_state) != WIN_BASE) {
        return true;
    }

    // GUI(L/R) が押されている状態か
    uint8_t gui = keyboard_report->mods & (MOD_BIT(KC_LGUI) | MOD_BIT(KC_RGUI));

    // Spaceキーで押下時のみ処理
    if (gui && keycode == KC_SPC && record->event.pressed) {
        unregister_mods(gui);  // GUI を一時解除

        #ifdef MAC
            tap_code16(KC_LANG2);  // Mac: 英数キー
        #else
            tap_code16(JP_ZKHK);   // Windows: 半角/全角キー
        #endif

        register_mods(gui);    // GUI を元に戻す
        return false;          // Space自体は送らない
    }

    // GUI押してない場合やSpace以外は通常処理
    return true;
}
// ========================================================================

// JIS specific symbols processing ========================================
// reference : https://utb.sakura.ne.jp/blog/archives/39
static bool process_jp_symbols_impl(uint16_t keycode, bool pressed) {
    if (!pressed) {
        return true;
    }
    uint8_t shift = keyboard_report->mods & (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT));
    if (!shift) {
        return true;
    }
    uint16_t s;
    switch (keycode) {
        // Replace Shift-Symbols like ANSI for JIS.
        case KC_2:    s = JP_AT; break;   // Shift + 2 -> @
        case KC_6:    s = JP_CIRC; break; // Shift + 6 -> ^
        case KC_7:    s = JP_AMPR; break; // Shift + 7 -> &
        case KC_8:    s = JP_ASTR; break; // Shift + 8 -> *
        case KC_9:    s = JP_LPRN; break; // Shift + 9 -> (
        case KC_0:    s = JP_RPRN; break; // Shift + 0 -> )
        case KC_MINS: s = JP_UNDS; break; // Shift + - -> _
        case KC_EQL:  s = JP_PLUS; break; // Shift + = -> +
        case KC_GRV:  s = JP_TILD; break; // Shift + ` -> ~
        case KC_LBRC: s = JP_LCBR; break; // Shift + [ -> {
        case KC_RBRC: s = JP_RCBR; break; // Shift + ] -> }
        case KC_BSLS: s = JP_PIPE; break; // Shift + \ -> |
        case KC_SCLN: s = JP_COLN; break; // Shift + ; -> :
        case KC_QUOT: s = JP_DQUO; break; // Shift + ' -> "
        case KC_COMM: s = JP_LABK; break; // Shift + , -> <
        case KC_DOT:  s = JP_RABK; break; // Shift + . -> >
        case KC_SLSH: s = JP_QUES; break; // Shift + / -> ?
        default: return true;
    }
    unregister_mods(shift);
    tap_code16(s);
    register_mods(shift);
    return false;
}

bool process_jp_symbols(uint16_t keycode, keyrecord_t *record) {
    return process_jp_symbols_impl(keycode, record->event.pressed);
}

void tap_code16jp(uint16_t keycode) {
    if (process_jp_symbols_impl(keycode, true)) {
        tap_code16(keycode);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return process_jp_symbols(keycode, record);
}
// ==========================================================================

// Enter Password ===========================================================
enum custom_keycodes {
    WINPW = SAFE_RANGE
    MACPW
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case WINPW:
                SEND_STRING("!?dF4q3XrN!???!?dF4q3XrN!???");
                return false;
            case MACPW:
                SEND_STRING("gyaasinn");
                return false;
        }
    }
    return true;
}
// ========================================================================

// clang-format off
enum layers{
  MAC_BASE,
  MAC_FN,
  WIN_BASE,
  WIN_FN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[MAC_BASE] = LAYOUT_tkl_ansi(
     KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,           KC_SNAP,   KC_SIRI,  RGB_MOD,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC, KC_INS,    KC_HOME,  KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS, KC_DEL,    KC_END,   KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,            KC_UP,
     KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, KC_ROPTN, MO(MAC_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[MAC_FN] = LAYOUT_tkl_ansi(
     KC_TRNS,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_TRNS,  KC_TRNS,  RGB_TOG,
     KC_TRNS,  BT_HST1,  BT_HST2,  BT_HST3,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  BAT_LVL,  NK_TOGG,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

[WIN_BASE] = LAYOUT_tkl_ansi(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_SNAP,   KC_CTANA, RGB_MOD,
     JP_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  JP_EQL,   KC_BSPC,  KC_INS,    KC_HOME,  KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     JP_LBRC,  JP_RBRC,  JP_BSLS,  KC_DEL,    KC_END,   KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  JP_QUOT,            KC_ENT,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,            KC_UP,
     KC_LCTL,  KC_LALT,  KC_LGUI,                                KC_SPC,                                 KC_RGUI,  KC_RALT, MO(WIN_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[WIN_FN] = LAYOUT_tkl_ansi(
     KC_TRNS,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,            KC_TRNS,  KC_TRNS,  RGB_TOG,
     KC_TRNS,  BT_HST1,  BT_HST2,  BT_HST3,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     RGB_TOG,  RGB_MOD,  WINPW,  RGB_HUI,  RGB_SAI,  RGB_SPI,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  BAT_LVL,  NK_TOGG,  MACPW,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS)

};
