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

// // --------------------------------------------------------------------------------------
// // A custom macro definition example
// // --------------------------------------------------------------------------------------
// // Below is an example of code with custom macro definitions. Please use it as needed.
// // ※以下オリジナルマクロ定義例。必要に応じて有効化して使ってキーマップに割り当ててください。

// // ---------------------------------------------------------
// // ■ オリジナルのマクロ定義
// // ---------------------------------------------------------
// enum custom_keycodes {
//     // マクロ用のキーコードを定義する
//     WIN_SCREEN_SHOT = SAFE_RANGE, //標準のキーコード (QMKにすでにあるキー) と衝突しないように、１つ目のキーコードにSAFE_RANGEを指定。それ以降の定義マクロを安全に識別させることができる。
//     WIN_ONE_LINE_COPY ,
//     WIN_ONE_LINE_SELECT ,
//     WIN_SWAP_WINDOW_TO_ANOTHER_DISPLAY
    
// };

// // 関数の説明メモ：
// // ★ register_code関数：キーダウン
// // ★ unregister_code関数：キーアップ

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {

//         // Windows専用
//         // スクリーンショット
//         // Shift + Win + S
//         case WIN_SCREEN_SHOT:
//             if (record->event.pressed) {
//                 register_code(KC_RSFT);
//                 register_code(KC_LWIN);
//                 register_code(KC_S);
//                 unregister_code(KC_S);
//                 unregister_code(KC_LWIN);
//                 unregister_code(KC_RSFT);
//             }
//             return false; // 処理を続行しない

//         // 1行を選択後、コピーする
//         case WIN_ONE_LINE_COPY:
//             if (record->event.pressed) {
//                 // 
//                 register_code(KC_HOME);
//                 unregister_code(KC_HOME);

//                 register_code(KC_RSFT);
//                 register_code(KC_END);
//                 unregister_code(KC_END);
//                 unregister_code(KC_RSFT);

//                 register_code(KC_LCTL);
//                 register_code(KC_C);
//                 unregister_code(KC_C);
//                 unregister_code(KC_LCTL);

//                 register_code(KC_RGHT);
//                 unregister_code(KC_RGHT);
//             }
//             return false; // 処理を続行しない

//         // Windows専用
//         // 1行を選択する
//         // Home -> Shift + End
//         case WIN_ONE_LINE_SELECT:
//             if (record->event.pressed) {
//                 // 
//                 register_code(KC_HOME);
//                 unregister_code(KC_HOME);

//                 register_code(KC_RSFT);
//                 register_code(KC_END);
//                 unregister_code(KC_END);
//                 unregister_code(KC_RSFT);
//             }
//             return false; // 処理を続行しない

//         // Windows専用
//         // 別のセカンドディスプレイへアプリケーションウインドウを移動させる
//         // Win + Shift + 矢印
//         case WIN_SWAP_WINDOW_TO_ANOTHER_DISPLAY:
//             if (record->event.pressed) {
//                 register_code(KC_LWIN);
//                 register_code(KC_RSFT);
//                 register_code(KC_RGHT);

//                 unregister_code(KC_RGHT);
//                 unregister_code(KC_RSFT);
//                 unregister_code(KC_LWIN);
//             }
//             return false;   // 処理を続行しない
//     }
//     return true;            // 既存の処理を続行する
// }



// // ---------------------------------------------------------
// // ■ キーマップで使う独自のキー定義
// // ---------------------------------------------------------
// // Windows(Macも？)にて、かな入力を強制させる「KANA」を定義
// // 参考：https://www.okqubit.net/keyboards/ergodash.html
// #define KANA KC_LNG1                                // かな入力の強制
// #define LT_KANA_SHIFT LT(WIN_LONG_PRESS, KANA)      // 長押しで「WIN_LONG_PRESS」レイヤーにあるキーへ切り替え、単押しでKANAを呼び出す。LT( <レイヤー名>, <1回短押し時のキー>)



// ---------------------------------------------------------
// ■ キーマップレイヤーの定義
// ※レイヤー追加方法：
//   レイヤーを増やす場合には下記のlayers定数に任意の要素を追加し、
// 　キーのマップである、keymaps定数や、
// 　ロータリーエンコーダーのマップである、encoder_map定数にも同じ名称の配列を用意する。
// ---------------------------------------------------------
enum layers {
    MAC_BASE,
    WIN_BASE,
    MAC_FN1,
    WIN_FN1,
    _FN2,
};


// Keychron Q2 Maxの日本語 71キー配列キーマップを定義
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_jis_71(
        KC_ESC,  KC_1,     KC_2,     KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,        KC_MINS,     KC_EQL,   KC_INT3, KC_BSPC, KC_MUTE,
        KC_TAB,  KC_Q,     KC_W,     KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,        KC_LBRC,     KC_RBRC,  KC_ENT,           KC_DEL,
        KC_CAPS, KC_A,     KC_S,     KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,     KC_QUOT,     KC_BSLS,                    KC_HOME,
        KC_LSFT,           KC_Z,     KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,      KC_SLSH,     KC_INT1,  KC_RSFT, KC_UP,
        KC_LCTL, KC_LOPTN, KC_LCMMD, KC_LNG2,                   KC_SPC,           KC_LNG1, KC_RCMMD,MO(MAC_FN1), MO(_FN2),    KC_LEFT,  KC_DOWN, KC_RGHT),

    [WIN_BASE] = LAYOUT_jis_71(
        KC_ESC,  KC_1,     KC_2,     KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,        KC_MINS,     KC_EQL,   KC_INT3, KC_BSPC, KC_MUTE,
        KC_TAB,  KC_Q,     KC_W,     KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,        KC_LBRC,     KC_RBRC,  KC_ENT,           KC_DEL,
        KC_CAPS, KC_A,     KC_S,     KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,     KC_QUOT,     KC_BSLS,                    KC_HOME,
        KC_LSFT,           KC_Z,     KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,      KC_SLSH,     KC_INT1,  KC_RSFT, KC_UP,
        KC_LCTL, KC_LGUI,  KC_LALT,  KC_INT5,                   KC_SPC,           KC_INT4, KC_RALT, MO(WIN_FN1), MO(_FN2),    KC_LEFT,  KC_DOWN, KC_RGHT),

    [MAC_FN1] = LAYOUT_jis_71(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,     KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______,  RGB_TOG,
        _______, BT_HST1, BT_HST2, BT_HST3, P2P4G,     _______, _______, _______, _______, _______, _______, _______, _______, _______,           _______,
        RGB_TOG, RGB_MOD, RGB_VAI, RGB_HUI, RGB_SAI,   RGB_SPI, _______, _______, _______, _______, _______, _______, _______,                    KC_END ,
        _______,          RGB_RMOD,RGB_VAD, RGB_HUD,   RGB_SAD, RGB_SPD, NK_TOGG, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______,                     _______,          _______, _______, _______, _______, _______, _______, _______ ),

    [WIN_FN1] = LAYOUT_jis_71(
        _______, KC_BRID, KC_BRIU, KC_TASK, KC_FILE,   RGB_VAD, RGB_VAI, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______,  RGB_TOG,
        _______, BT_HST1, BT_HST2, BT_HST3, P2P4G,     _______, _______, _______, _______, _______, _______, _______, _______, _______,           _______,
        RGB_TOG, RGB_MOD, RGB_VAI, RGB_HUI, RGB_SAI,   RGB_SPI, _______, _______, _______, _______, _______, _______, _______,                    KC_END ,
        _______,          RGB_RMOD,RGB_VAD, RGB_HUD,   RGB_SAD, RGB_SPD, NK_TOGG, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______,                     _______,          _______, _______, _______, _______, _______, _______, _______ ),

    [_FN2] = LAYOUT_jis_71(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,     KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______,  _______,
        _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______, _______, _______,           _______,
        _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______, _______,                    _______,
        _______,          _______, _______, _______,   _______, BAT_LVL, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______,                     _______,          _______, _______, _______, _______, _______, _______, _______ )

};

// ---------------------------------------------------------
// ■ ロータリーエンコーダーの設定
// ---------------------------------------------------------

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_FN1]  = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [WIN_FN1]  = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [_FN2]     = { ENCODER_CCW_CW(_______, _______)},
};
#endif // ENCODER_MAP_ENABLE



