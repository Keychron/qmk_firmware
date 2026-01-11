#pragma once
#include QMK_KEYBOARD_H

// Add missing Mod-Tap combos
#ifndef LCS_T
#define LCS_T(kc) MT(MOD_LCTL | MOD_LSFT, kc)
#endif

#ifndef LCG_T
#define LCG_T(kc) MT(MOD_LCTL | MOD_LGUI, kc)
#endif

#ifndef LCSG_T
#define LCSG_T(kc) MT(MOD_LCTL | MOD_LSFT | MOD_LGUI, kc)
#endif


// Fallback definitions for mouse key aliases if not defined in this QMK version
#ifndef MS_BTN1
#define MS_BTN1 KC_BTN1
#endif
#ifndef MS_BTN2
#define MS_BTN2 KC_BTN2
#endif
#ifndef MS_BTN3
#define MS_BTN3 KC_BTN3
#endif
#ifndef MS_BTN4
#define MS_BTN4 KC_BTN4
#endif
#ifndef MS_BTN5
#define MS_BTN5 KC_BTN5
#endif

#ifndef MS_UP
#define MS_UP KC_MS_UP
#endif
#ifndef MS_DOWN
#define MS_DOWN KC_MS_DOWN
#endif
#ifndef MS_LEFT
#define MS_LEFT KC_MS_LEFT
#endif
#ifndef MS_RGHT
#define MS_RGHT KC_MS_RIGHT
#endif

#ifndef MS_WHLU
#define MS_WHLU KC_MS_WH_UP
#endif
#ifndef MS_WHLD
#define MS_WHLD KC_MS_WH_DOWN
#endif
// Add more if needed
