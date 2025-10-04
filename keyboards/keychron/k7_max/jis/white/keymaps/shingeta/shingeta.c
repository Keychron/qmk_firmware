#include "shingeta.h"
#include <string.h>

#define NGBUFFER 5
#define NG_KEY_COUNT 43
#define NGMAP_SIZE (sizeof(ngmap) / sizeof(shingeta_keymap))

#define FIRST_OUTPUT_DELAY 50
#define REPEAT_DELAY 500
#define REPEAT_INTERVAL 50

// 内部関数の宣言
static void shingeta_type(void);
static void shingeta_type_repeat(void);
static void shingeta_clear(void);
static void save_combo_state(void);
static bool find_and_send_kana(uint64_t combo);

// 静的変数
static uint8_t  ng_chrcount    = 0;
static bool     is_shingeta    = false;
static uint8_t  shingeta_layer = 0;
static uint64_t keycomb        = (uint64_t)0;

// タイマーとリピート用の変数
static uint16_t shingeta_timer     = 0;
static bool     timer_active       = false;
static uint16_t repeat_timer       = 0;
static bool     combo_confirmed    = false;
static uint64_t confirmed_keycomb  = 0;
static uint8_t  confirmed_chrcount = 0;
static uint16_t confirmed_inputs[NGBUFFER];

// 文字入力バッファ
static uint16_t ninputs[NGBUFFER];

// 43キーを64bitの各ビットに割り当てる
#define B_Q ((uint64_t)1 << 0)
#define B_W ((uint64_t)1 << 1)
#define B_E ((uint64_t)1 << 2)
#define B_R ((uint64_t)1 << 3)
#define B_T ((uint64_t)1 << 4)

#define B_Y ((uint64_t)1 << 5)
#define B_U ((uint64_t)1 << 6)
#define B_I ((uint64_t)1 << 7)
#define B_O ((uint64_t)1 << 8)
#define B_P ((uint64_t)1 << 9)

#define B_A ((uint64_t)1 << 10)
#define B_S ((uint64_t)1 << 11)
#define B_D ((uint64_t)1 << 12)
#define B_F ((uint64_t)1 << 13)
#define B_G ((uint64_t)1 << 14)

#define B_H ((uint64_t)1 << 15)
#define B_J ((uint64_t)1 << 16)
#define B_K ((uint64_t)1 << 17)
#define B_L ((uint64_t)1 << 18)
#define B_SCLN ((uint64_t)1 << 19)

#define B_Z ((uint64_t)1 << 20)
#define B_X ((uint64_t)1 << 21)
#define B_C ((uint64_t)1 << 22)
#define B_V ((uint64_t)1 << 23)
#define B_B ((uint64_t)1 << 24)

#define B_N ((uint64_t)1 << 25)
#define B_M ((uint64_t)1 << 26)
#define B_COMM ((uint64_t)1 << 27)
#define B_DOT ((uint64_t)1 << 28)
#define B_SLSH ((uint64_t)1 << 29)

#define BB_1 ((uint64_t)1 << 30)
#define BB_2 ((uint64_t)1 << 31)
#define BB_3 ((uint64_t)1 << 32)
#define BB_4 ((uint64_t)1 << 33)
#define BB_5 ((uint64_t)1 << 34)
#define BB_6 ((uint64_t)1 << 35)
#define BB_7 ((uint64_t)1 << 36)
#define BB_8 ((uint64_t)1 << 37)
#define BB_9 ((uint64_t)1 << 38)
#define BB_0 ((uint64_t)1 << 39)
#define B_MINS ((uint64_t)1 << 40)

#define B_X1 ((uint64_t)1 << 41)

#define B_SHFT ((uint64_t)1 << 42)

// キーコードとキービットの対応
const uint64_t ng_key[] = {
    [NG_Q - NG_Q] = B_Q, [NG_W - NG_Q] = B_W, [NG_E - NG_Q] = B_E, [NG_R - NG_Q] = B_R, [NG_T - NG_Q] = B_T, [NG_Y - NG_Q] = B_Y, [NG_U - NG_Q] = B_U, [NG_I - NG_Q] = B_I, [NG_O - NG_Q] = B_O, [NG_P - NG_Q] = B_P, [NG_A - NG_Q] = B_A, [NG_S - NG_Q] = B_S, [NG_D - NG_Q] = B_D, [NG_F - NG_Q] = B_F, [NG_G - NG_Q] = B_G, [NG_H - NG_Q] = B_H, [NG_J - NG_Q] = B_J, [NG_K - NG_Q] = B_K, [NG_L - NG_Q] = B_L, [NG_SCLN - NG_Q] = B_SCLN, [NG_Z - NG_Q] = B_Z, [NG_X - NG_Q] = B_X, [NG_C - NG_Q] = B_C, [NG_V - NG_Q] = B_V, [NG_B - NG_Q] = B_B, [NG_N - NG_Q] = B_N, [NG_M - NG_Q] = B_M, [NG_COMM - NG_Q] = B_COMM, [NG_DOT - NG_Q] = B_DOT, [NG_SLSH - NG_Q] = B_SLSH, [NG_1 - NG_Q] = BB_1, [NG_2 - NG_Q] = BB_2, [NG_3 - NG_Q] = BB_3, [NG_4 - NG_Q] = BB_4, [NG_5 - NG_Q] = BB_5, [NG_6 - NG_Q] = BB_6, [NG_7 - NG_Q] = BB_7, [NG_8 - NG_Q] = BB_8, [NG_9 - NG_Q] = BB_9, [NG_0 - NG_Q] = BB_0, [NG_MINS - NG_Q] = B_MINS, [NG_X1 - NG_Q] = B_X1, [NG_SHFT - NG_Q] = B_SHFT,
};

typedef struct {
    uint64_t key;
    char     kana[5];
} shingeta_keymap;

const PROGMEM shingeta_keymap ngmap[] = {
    // 単独
    {.key = BB_1, .kana = "1"},
    {.key = BB_2, .kana = "2"},
    {.key = BB_3, .kana = "3"},
    {.key = BB_4, .kana = "4"},
    {.key = BB_5, .kana = "5"},
    {.key = BB_6, .kana = "6"},
    {.key = BB_7, .kana = "7"},
    {.key = BB_8, .kana = "8"},
    {.key = BB_9, .kana = "9"},
    {.key = BB_0, .kana = "0"},
    {.key = B_MINS, .kana = "-"},
    {.key = B_SHFT, .kana = " "},

    {.key = B_Q, .kana = "-"},
    {.key = B_W, .kana = "ni"},
    {.key = B_E, .kana = "ha"},
    {.key = B_R, .kana = ","},
    {.key = B_T, .kana = "ti"},
    {.key = B_Y, .kana = "gu"},
    {.key = B_U, .kana = "ba"},
    {.key = B_I, .kana = "ko"},
    {.key = B_O, .kana = "ga"},
    {.key = B_P, .kana = "hi"},
    {.key = B_X1, .kana = "ge"},

    {.key = B_A, .kana = "no"},
    {.key = B_S, .kana = "to"},
    {.key = B_D, .kana = "ka"},
    {.key = B_F, .kana = "nn"},
    {.key = B_G, .kana = "ltu"},
    {.key = B_H, .kana = "ku"},
    {.key = B_J, .kana = "u"},
    {.key = B_K, .kana = "i"},
    {.key = B_L, .kana = "si"},
    {.key = B_SCLN, .kana = "na"},

    {.key = B_Z, .kana = "su"},
    {.key = B_X, .kana = "ma"},
    {.key = B_C, .kana = "ki"},
    {.key = B_V, .kana = "ru"},
    {.key = B_B, .kana = "tu"},
    {.key = B_N, .kana = "te"},
    {.key = B_M, .kana = "ta"},
    {.key = B_COMM, .kana = "de"},
    {.key = B_DOT, .kana = "."},
    {.key = B_SLSH, .kana = "bu"},

    // 中指シフト
    {.key = B_K | B_Q, .kana = "fa"},
    {.key = B_K | B_W, .kana = "go"},
    {.key = B_K | B_E, .kana = "hu"},
    {.key = B_K | B_R, .kana = "fi"},
    {.key = B_K | B_T, .kana = "fe"},
    {.key = B_D | B_Y, .kana = "wi"},
    {.key = B_D | B_U, .kana = "pa"},
    {.key = B_D | B_I, .kana = "yo"},
    {.key = B_D | B_O, .kana = "mi"},
    {.key = B_D | B_P, .kana = "we"},
    {.key = B_D | B_X1, .kana = "ulo"},

    {.key = B_K | B_A, .kana = "ho"},
    {.key = B_K | B_S, .kana = "ji"},
    {.key = B_K | B_D, .kana = "re"},
    {.key = B_K | B_F, .kana = "mo"},
    {.key = B_K | B_G, .kana = "yu"},
    {.key = B_D | B_H, .kana = "he"},
    {.key = B_D | B_J, .kana = "a"},
    {.key = B_D | B_K, .kana = ""},
    {.key = B_D | B_L, .kana = "o"},
    {.key = B_D | B_SCLN, .kana = "e"},

    {.key = B_K | B_Z, .kana = "du"},
    {.key = B_K | B_X, .kana = "zo"},
    {.key = B_K | B_C, .kana = "bo"},
    {.key = B_K | B_V, .kana = "mu"},
    {.key = B_K | B_B, .kana = "fo"},
    {.key = B_D | B_N, .kana = "se"},
    {.key = B_D | B_M, .kana = "ne"},
    {.key = B_D | B_COMM, .kana = "be"},
    {.key = B_D | B_DOT, .kana = "pu"},
    {.key = B_D | B_SLSH, .kana = "vu"},

    {.key = B_K | BB_1, .kana = "la"},
    {.key = B_K | BB_2, .kana = "li"},
    {.key = B_K | BB_3, .kana = "lu"},
    {.key = B_K | BB_4, .kana = "le"},
    {.key = B_K | BB_5, .kana = "lo"},

    // 薬指シフト
    {.key = B_L | B_Q, .kana = "di"},
    {.key = B_L | B_W, .kana = "me"},
    {.key = B_L | B_E, .kana = "ke"},
    {.key = B_L | B_R, .kana = "teli"},
    {.key = B_L | B_T, .kana = "deli"},
    {.key = B_S | B_Y, .kana = "sye"},
    {.key = B_S | B_U, .kana = "pe"},
    {.key = B_S | B_I, .kana = "do"},
    {.key = B_S | B_O, .kana = "ya"},
    {.key = B_S | B_P, .kana = "je"},

    {.key = B_L | B_A, .kana = "wo"},
    {.key = B_L | B_S, .kana = "sa"},
    {.key = B_L | B_D, .kana = "o"},
    {.key = B_L | B_F, .kana = "ri"},
    {.key = B_L | B_G, .kana = "zu"},
    {.key = B_S | B_H, .kana = "bi"},
    {.key = B_S | B_J, .kana = "ra"},
    {.key = B_S | B_K, .kana = ""},
    {.key = B_S | B_L, .kana = ""},
    {.key = B_S | B_SCLN, .kana = "so"},

    {.key = B_L | B_Z, .kana = "ze"},
    {.key = B_L | B_X, .kana = "za"},
    {.key = B_L | B_C, .kana = "gi"},
    {.key = B_L | B_V, .kana = "ro"},
    {.key = B_L | B_B, .kana = "nu"},
    {.key = B_S | B_N, .kana = "wa"},
    {.key = B_S | B_M, .kana = "da"},
    {.key = B_S | B_COMM, .kana = "pi"},
    {.key = B_S | B_DOT, .kana = "po"},
    {.key = B_S | B_SLSH, .kana = "tile"},

    {.key = B_L | BB_1, .kana = "xya"},
    {.key = B_L | BB_2, .kana = "mya"},
    {.key = B_L | BB_3, .kana = "myu"},
    {.key = B_L | BB_4, .kana = "myo"},
    {.key = B_L | BB_5, .kana = "xwa"},

    {.key = B_I | B_E, .kana = "sho"},
    {.key = B_I | B_W, .kana = "shu"},
    {.key = B_I | B_R, .kana = "kyu"},
    {.key = B_I | B_F, .kana = "kyo"},
    {.key = B_I | B_V, .kana = "kya"},
    {.key = B_I | B_C, .kana = "sha"},
    {.key = B_I | B_Q, .kana = "hyu"},
    {.key = B_I | B_A, .kana = "hyo"},
    {.key = B_I | B_Z, .kana = "hya"},
    {.key = B_I | B_T, .kana = "chu"},
    {.key = B_I | B_G, .kana = "cho"},
    {.key = B_I | B_B, .kana = "cha"},

    {.key = B_I | BB_1, .kana = "xyu"},
    {.key = B_I | BB_2, .kana = "bya"},
    {.key = B_I | BB_3, .kana = "byu"},
    {.key = B_I | BB_4, .kana = "byo"},

    {.key = B_O | B_E, .kana = "jo"},
    {.key = B_O | B_W, .kana = "ju"},
    {.key = B_O | B_R, .kana = "gyu"},
    {.key = B_O | B_F, .kana = "gyo"},
    {.key = B_O | B_V, .kana = "gya"},
    {.key = B_O | B_C, .kana = "ja"},
    {.key = B_O | B_Q, .kana = "ryu"},
    {.key = B_O | B_A, .kana = "ryo"},
    {.key = B_O | B_Z, .kana = "rya"},
    {.key = B_O | B_T, .kana = "nyu"},
    {.key = B_O | B_G, .kana = "nyo"},
    {.key = B_O | B_B, .kana = "nya"},

    {.key = B_O | BB_1, .kana = "xyo"},
    {.key = B_O | BB_2, .kana = "pya"},
    {.key = B_O | BB_3, .kana = "pyu"},
    {.key = B_O | BB_4, .kana = "pyo"},

    // others
    {.key = B_R | B_F, .kana = "/"},
    {.key = B_R | B_G, .kana = "/"},
    {.key = B_F | B_G, .kana = "()" SS_TAP(X_LEFT)},
    {.key = B_F | B_V, .kana = "!"},
    {.key = B_F | B_B, .kana = "!"},
    {.key = B_N | B_J, .kana = "?"},
    {.key = B_H | B_J, .kana = "[]" SS_TAP(X_LEFT)},
    {.key = B_H | B_U, .kana = "/"},
};

// 内部関数の実装

static void shingeta_clear(void) {
    for (int i = 0; i < NGBUFFER; i++) {
        ninputs[i] = 0;
    }
    ng_chrcount = 0;
}

static bool find_and_send_kana(uint64_t combo) {
    shingeta_keymap bngmap;
    for (int i = 0; i < NGMAP_SIZE; i++) {
        memcpy_P(&bngmap, &ngmap[i], sizeof(bngmap));
        if (combo == bngmap.key) {
            send_string(bngmap.kana);
            return true;
        }
    }
    return false;
}

static void shingeta_type_repeat(void) {
    if (find_and_send_kana(confirmed_keycomb)) {
        return;
    }

    for (int j = 0; j < confirmed_chrcount; j++) {
        if (confirmed_inputs[j] >= NG_Q && confirmed_inputs[j] <= NG_SHFT) {
            find_and_send_kana(ng_key[confirmed_inputs[j] - NG_Q]);
        }
    }
}

static void shingeta_type(void) {
    if (find_and_send_kana(keycomb)) {
        shingeta_clear();
        return;
    }

    for (int j = 0; j < ng_chrcount; j++) {
        if (ninputs[j] >= NG_Q && ninputs[j] <= NG_SHFT) {
            find_and_send_kana(ng_key[ninputs[j] - NG_Q]);
        }
    }
    shingeta_clear();
}

static void save_combo_state(void) {
    combo_confirmed    = true;
    confirmed_keycomb  = keycomb;
    confirmed_chrcount = ng_chrcount;
    for (int i = 0; i < ng_chrcount && i < NGBUFFER; i++) {
        confirmed_inputs[i] = ninputs[i];
    }
    repeat_timer = timer_read();
}

// 公開関数の実装

void set_shingeta(uint8_t layer) {
    shingeta_layer = layer;
}

void shingeta_on(void) {
    is_shingeta = true;
    keycomb     = (uint64_t)0;
    shingeta_clear();
    layer_on(shingeta_layer);

    tap_code(KC_LNG1);
    tap_code(KC_INT4);
}

void shingeta_off(void) {
    is_shingeta = false;
    keycomb     = (uint64_t)0;
    shingeta_clear();
    layer_off(shingeta_layer);

    tap_code(KC_LNG2);
    tap_code(KC_INT5);
}

bool shingeta_state(void) {
    return is_shingeta;
}

void shingeta_mode(uint16_t keycode, keyrecord_t *record) {
    if (!is_shingeta) return;

    static uint8_t n_modifier = 0;

    switch (keycode) {
        case KC_LCTL:
        case KC_LSFT:
        case KC_LALT:
        case KC_LGUI:
        case KC_RCTL:
        case KC_RSFT:
        case KC_RALT:
        case KC_RGUI:
            if (record->event.pressed) {
                n_modifier++;
                layer_off(shingeta_layer);
            } else {
                n_modifier--;
                if (n_modifier == 0) {
                    layer_on(shingeta_layer);
                }
            }
            break;
    }
}

bool process_shingeta(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case NG_Q ... NG_SHFT:
                if (ng_chrcount >= NGBUFFER) {
                    shingeta_clear();
                    combo_confirmed = false;
                    timer_active    = false;
                }

                ninputs[ng_chrcount] = keycode;
                ng_chrcount++;
                keycomb |= ng_key[keycode - NG_Q];

                if (ng_chrcount >= 2) {
                    timer_active = false;
                    shingeta_type();
                    save_combo_state();
                } else if (ng_chrcount == 1) {
                    shingeta_timer  = timer_read();
                    timer_active    = true;
                    combo_confirmed = false;
                }
                return false;
                break;
        }
    } else {
        switch (keycode) {
            case NG_Q ... NG_SHFT:
                if (ng_chrcount > 0) {
                    timer_active = false;
                    shingeta_type();
                }

                keycomb &= ~ng_key[keycode - NG_Q];

                if (keycomb == 0) {
                    combo_confirmed = false;
                }

                return false;
                break;
        }
    }
    return true;
}

void shingeta_timer_task(void) {
    if (timer_active && ng_chrcount > 0) {
        if (timer_elapsed(shingeta_timer) > FIRST_OUTPUT_DELAY) {
            timer_active = false;
            shingeta_type();
            save_combo_state();
        }
    }

    if (combo_confirmed && keycomb == confirmed_keycomb && keycomb != 0) {
        uint16_t elapsed = timer_elapsed(repeat_timer);

        if (elapsed > REPEAT_DELAY) {
            shingeta_type_repeat();
            repeat_timer = timer_read() - (REPEAT_DELAY - REPEAT_INTERVAL);
        }
    }
}
