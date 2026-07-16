// SPDX-License-Identifier: GPL-2.0-or-later
//
// Shared mock shim for the Keychron K2 HE OpenRGB unit tests.
//
// openrgb.c (the code under test, UNMODIFIED) pulls in a pile of QMK headers via
// "quantum.h" et al. To compile it in isolation against the host gtest harness we
// place a mocks/ directory FIRST on the include path so the shadow headers
// (quantum.h, color.h, raw_hid.h, usb_descriptor.h, version.h,
// keymap_introspection.h, keychron_raw_hid.h) shadow the real QMK ones. Every one
// of those shadow headers just #includes this single shim, which declares exactly
// the symbols openrgb.c references.
//
// This file is C/C++ shared: the recording state and the mock function bodies live
// in openrgb_test_mocks.cpp; the gtest cases in openrgb_mapping.cpp reach into the
// recording state through openrgb_test_recorder.h.

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
// Color types (mirror quantum/color.h rgb_t / hsv_t layout)
// ---------------------------------------------------------------------------
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB;

typedef struct {
    uint8_t h;
    uint8_t s;
    uint8_t v;
} HSV;

// ---------------------------------------------------------------------------
// LED config (mirror led_config_t / led_point_t)
// ---------------------------------------------------------------------------
typedef struct {
    uint8_t x;
    uint8_t y;
} led_point_t;

typedef struct {
    uint8_t     matrix_co[MATRIX_ROWS][MATRIX_COLS];
    led_point_t point[RGB_MATRIX_LED_COUNT];
    uint8_t     flags[RGB_MATRIX_LED_COUNT];
} led_config_t;

extern led_config_t g_led_config;

// ---------------------------------------------------------------------------
// Keycodes / keymap introspection
// ---------------------------------------------------------------------------
#define KC_NO 0

uint16_t keycode_at_keymap_location(uint8_t layer, uint8_t row, uint8_t col);

// ---------------------------------------------------------------------------
// RGB matrix effect enum -- mirrors the real 2025q3
// quantum/rgb_matrix/animations/rgb_matrix_effects.inc ORDER so that the OpenRGB
// mode order (in openrgb.h) genuinely differs from QMK's effect numbering.
//
// Crucially: RAINBOW_MOVING_CHEVRON sits BEFORE CYCLE_OUT_IN here (real QMK
// order), but AFTER it in OpenRGB's order -- this divergence is what makes the
// mode-mapping fix observable. Only the effects gated by the -DENABLE_RGB_MATRIX_*
// flags actually land in openrgb_mode_map[]; the rest just occupy enum slots
// (exactly as in real QMK, where every animation file contributes an enum value
// regardless of which are compiled in).
// ---------------------------------------------------------------------------
enum rgb_matrix_effects {
    RGB_MATRIX_NONE = 0,
    RGB_MATRIX_SOLID_COLOR,
    RGB_MATRIX_ALPHAS_MODS,
    RGB_MATRIX_GRADIENT_UP_DOWN,
    RGB_MATRIX_GRADIENT_LEFT_RIGHT,
    RGB_MATRIX_BREATHING,
    RGB_MATRIX_BAND_SAT,
    RGB_MATRIX_BAND_VAL,
    RGB_MATRIX_BAND_PINWHEEL_SAT,
    RGB_MATRIX_BAND_PINWHEEL_VAL,
    RGB_MATRIX_BAND_SPIRAL_SAT,
    RGB_MATRIX_BAND_SPIRAL_VAL,
    RGB_MATRIX_CYCLE_ALL,
    RGB_MATRIX_CYCLE_LEFT_RIGHT,
    RGB_MATRIX_CYCLE_UP_DOWN,
    RGB_MATRIX_RAINBOW_MOVING_CHEVRON, // index 15 (before CYCLE_OUT_IN)
    RGB_MATRIX_CYCLE_OUT_IN,           // index 16
    RGB_MATRIX_CYCLE_OUT_IN_DUAL,
    RGB_MATRIX_CYCLE_PINWHEEL,
    RGB_MATRIX_CYCLE_SPIRAL,
    RGB_MATRIX_DUAL_BEACON,
    RGB_MATRIX_RAINBOW_BEACON,
    RGB_MATRIX_RAINBOW_PINWHEELS,
    RGB_MATRIX_FLOWER_BLOOMING,
    RGB_MATRIX_RAINDROPS,
    RGB_MATRIX_JELLYBEAN_RAINDROPS,
    RGB_MATRIX_HUE_BREATHING,
    RGB_MATRIX_HUE_PENDULUM,
    RGB_MATRIX_HUE_WAVE,
    RGB_MATRIX_PIXEL_RAIN,
    RGB_MATRIX_PIXEL_FLOW,
    RGB_MATRIX_PIXEL_FRACTAL,
    RGB_MATRIX_TYPING_HEATMAP,
    RGB_MATRIX_DIGITAL_RAIN,
    RGB_MATRIX_SOLID_REACTIVE_SIMPLE,
    RGB_MATRIX_SOLID_REACTIVE,
    RGB_MATRIX_SOLID_REACTIVE_WIDE,
    RGB_MATRIX_SOLID_REACTIVE_CROSS,
    RGB_MATRIX_SOLID_REACTIVE_NEXUS,
    RGB_MATRIX_SPLASH,
    RGB_MATRIX_SOLID_SPLASH,
    RGB_MATRIX_STARLIGHT_SMOOTH,
    RGB_MATRIX_STARLIGHT,
    RGB_MATRIX_STARLIGHT_DUAL_SAT,
    RGB_MATRIX_STARLIGHT_DUAL_HUE,
    RGB_MATRIX_RIVERFLOW,
    RGB_MATRIX_OPENRGB_DIRECT,
    RGB_MATRIX_EFFECT_MAX
};

// ---------------------------------------------------------------------------
// RGB matrix API used by openrgb.c. Implementations live in the mocks cpp and
// record / serve values for the tests.
// ---------------------------------------------------------------------------
HSV     rgb_matrix_get_hsv(void);
uint8_t rgb_matrix_get_mode(void);
uint8_t rgb_matrix_get_speed(void);
void    rgb_matrix_mode(uint8_t mode);
void    rgb_matrix_mode_noeeprom(uint8_t mode);
void    rgb_matrix_set_speed(uint8_t speed);
void    rgb_matrix_set_speed_noeeprom(uint8_t speed);
void    rgb_matrix_sethsv(uint8_t h, uint8_t s, uint8_t v);
void    rgb_matrix_sethsv_noeeprom(uint8_t h, uint8_t s, uint8_t v);

// ---------------------------------------------------------------------------
// Raw HID transports
// ---------------------------------------------------------------------------
#define RAW_HID_SRC_USB 0

// Keychron USB-explicit transport (the FIXED reply path).
void kc_raw_hid_send(uint8_t src, uint8_t *data, uint8_t len);

// Core QMK raw_hid_send (the OLD reply path -- on wireless this is dropped).
void raw_hid_send(uint8_t *data, uint8_t length);

#ifdef __cplusplus
}
#endif
