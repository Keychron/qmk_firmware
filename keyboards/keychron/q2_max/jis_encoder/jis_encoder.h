#pragma once

#include "quantum.h"

// キーマトリクスのサイズを定義（例：5行 × 16列）
#define MATRIX_ROWS 5


#ifndef MATRIX_COLS
#define MATRIX_COLS 16
#endif


// Keychron Q2 Max日本語キー配列(71キー)を定義
// ここで定義した配列の並びは default/keymap.c で影響を与える。

// ※K15(ノブ)について
// MATRIX_COLS 16を定義しているが、変更が反映されておらず15列になっている。(上手くいかない理由は不明)
// そのため、K15(ノブ)を上から3段目に移動させている。
#define LAYOUT_jis_71( \
    K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K10, K11, K12, K13, K14, K15, \
    K16, K17, K18, K19, K20, K21, K22, K23, K24, K25, K26, K27, K28, K29,      K30, \
    K31, K32, K33, K34, K35, K36, K37, K38, K39, K40, K41, K42, K43, K44          , \
    K45,      K46, K47, K48, K49, K50, K51, K52, K53, K54, K55, K56, K57, K58,      \
    K59, K60, K61, K62,           K63,      K64, K65, K66, K67, K68, K69, K70 \
) \
{ \
    { K00, K01,   K02, K03, K04,   K05,   K06, K07,   K08, K09, K10, K11, K12, K13,   K14  }, \
    { K16, K17,   K18, K19, K20,   K21,   K22, K23,   K24, K25, K26, K27, K28, K29,   K30  }, \
    { K31, K32,   K33, K34, K35,   K36,   K37, K38,   K39, K40, K41, K42, K43, K44,   K15  }, \
    { K45, KC_NO, K46, K47, K48,   K49,   K50, K51,   K52, K53, K54, K55, K56, K57,   K58  }, \
    { K59, K60,   K61, K62, KC_NO, KC_NO, K63, KC_NO, K64, K65, K66, K67, K68, K69,   K70  }  \
}

