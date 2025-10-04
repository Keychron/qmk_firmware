#pragma once
#include QMK_KEYBOARD_H

// キーコード定義（他のファイルから使用される）
typedef enum shingeta_keycodes {
    NG_Q = SAFE_RANGE,
    NG_W,
    NG_E,
    NG_R,
    NG_T,
    NG_Y,
    NG_U,
    NG_I,
    NG_O,
    NG_P,

    NG_A,
    NG_S,
    NG_D,
    NG_F,
    NG_G,
    NG_H,
    NG_J,
    NG_K,
    NG_L,
    NG_SCLN,

    NG_Z,
    NG_X,
    NG_C,
    NG_V,
    NG_B,
    NG_N,
    NG_M,
    NG_COMM,
    NG_DOT,
    NG_SLSH,

    NG_1,
    NG_2,
    NG_3,
    NG_4,
    NG_5,
    NG_6,
    NG_7,
    NG_8,
    NG_9,
    NG_0,
    NG_MINS,

    NG_X1,

    NG_SHFT,
} NGKEYS;

#define NG_SAFE_RANGE (SAFE_RANGE + 43)
#define KC_NGSHFT NG_SHFT

// カスタムキーコード用
enum custom_keycodes {
    NG_TOGGLE = NG_SAFE_RANGE, // 薙刀式トグルキー
};

// 公開関数（keymap.cから呼び出される）
void set_shingeta(uint8_t layer);
void shingeta_on(void);
void shingeta_off(void);
bool shingeta_state(void);
bool process_shingeta(uint16_t keycode, keyrecord_t *record);
void shingeta_mode(uint16_t keycode, keyrecord_t *record);
void shingeta_timer_task(void);
