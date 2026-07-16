// SPDX-License-Identifier: GPL-2.0-or-later
//
// C-linkage mock implementations for the symbols openrgb.c references, plus the
// global g_led_config it indexes. All recording goes through OpenRgbMockState.

#include <cstdint>
#include <cstring>

#include "openrgb_test_recorder.h"

// Pull in the same shim openrgb.c sees (for the RGB/HSV/led_config_t types and the
// C function prototypes), so our definitions match the declarations exactly.
extern "C" {
#include "openrgb_test_shim.h"
}

// The global LED config openrgb.c reads (g_led_config.point/.flags/.matrix_co).
extern "C" {
led_config_t g_led_config = {};
}

extern "C" uint16_t keycode_at_keymap_location(uint8_t layer, uint8_t row, uint8_t col) {
    (void)layer;
    (void)row;
    (void)col;
    return 0;
}

// ---- rgb_matrix getters: serve what the test preloaded ----
extern "C" HSV rgb_matrix_get_hsv(void) {
    auto& s = OpenRgbMockState::Instance();
    HSV   v;
    v.h = s.get_hsv_h;
    v.s = s.get_hsv_s;
    v.v = s.get_hsv_v;
    return v;
}

extern "C" uint8_t rgb_matrix_get_mode(void) {
    return OpenRgbMockState::Instance().get_mode_value;
}

extern "C" uint8_t rgb_matrix_get_speed(void) {
    return OpenRgbMockState::Instance().get_speed_value;
}

// ---- rgb_matrix setters: record what openrgb.c chose ----
extern "C" void rgb_matrix_mode(uint8_t mode) {
    auto& s               = OpenRgbMockState::Instance();
    s.mode_set            = true;
    s.last_mode           = mode;
    s.mode_set_via_eeprom = true;
}

extern "C" void rgb_matrix_mode_noeeprom(uint8_t mode) {
    auto& s                 = OpenRgbMockState::Instance();
    s.mode_set              = true;
    s.last_mode             = mode;
    s.mode_set_via_noeeprom = true;
}

extern "C" void rgb_matrix_set_speed(uint8_t speed) {
    (void)speed;
}

extern "C" void rgb_matrix_set_speed_noeeprom(uint8_t speed) {
    (void)speed;
}

extern "C" void rgb_matrix_sethsv(uint8_t h, uint8_t s, uint8_t v) {
    (void)h;
    (void)s;
    (void)v;
}

extern "C" void rgb_matrix_sethsv_noeeprom(uint8_t h, uint8_t s, uint8_t v) {
    (void)h;
    (void)s;
    (void)v;
}

// ---- transports ----

// The FIXED reply path: explicit USB send.
extern "C" void kc_raw_hid_send(uint8_t src, uint8_t *data, uint8_t len) {
    auto& s          = OpenRgbMockState::Instance();
    s.kc_send_called = true;
    s.kc_send_src    = src;
    s.kc_send_buf.assign(data, data + len);
}

// The OLD reply path: core raw_hid_send. On real hardware in wireless mode this is
// dropped (active host driver's send_raw_hid is NULL). We model the drop by simply
// recording that it was the carrier and NOT delivering to the USB capture -- so a
// reverted openrgb.c (which would call this) fails the "delivered over USB" test.
extern "C" void raw_hid_send(uint8_t *data, uint8_t length) {
    auto& s            = OpenRgbMockState::Instance();
    s.core_send_called = true;
    s.core_send_buf.assign(data, data + length);
    // DROPPED: nothing is forwarded to kc_send_buf.
}
