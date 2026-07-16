// SPDX-License-Identifier: GPL-2.0-or-later
//
// Recording state shared between the OpenRGB mock implementations
// (openrgb_test_mocks.cpp) and the gtest cases (openrgb_mapping.cpp).
#pragma once

#include <cstdint>
#include <vector>

// Singleton holding everything the mocks record and everything the tests preload.
struct OpenRgbMockState {
    // ---- values the test SETS, served back to openrgb.c ----
    uint8_t get_mode_value  = 0; // returned by rgb_matrix_get_mode()
    uint8_t get_speed_value = 0; // returned by rgb_matrix_get_speed()
    uint8_t get_hsv_h       = 0;
    uint8_t get_hsv_s       = 0;
    uint8_t get_hsv_v       = 0;

    // ---- values the mocks RECORD ----
    bool    mode_set                = false; // any rgb_matrix_mode*() called?
    uint8_t last_mode               = 0;     // arg to last rgb_matrix_mode*()
    bool    mode_set_via_noeeprom   = false; // true if last setter was the _noeeprom variant
    bool    mode_set_via_eeprom     = false; // true if last setter was the eeprom variant

    // kc_raw_hid_send (the FIXED USB transport)
    bool                 kc_send_called = false;
    uint8_t              kc_send_src    = 0xFF;
    std::vector<uint8_t> kc_send_buf;

    // core raw_hid_send (the OLD transport -- on wireless this drops the reply)
    bool                 core_send_called = false;
    std::vector<uint8_t> core_send_buf;

    void reset() { *this = OpenRgbMockState{}; }

    static OpenRgbMockState& Instance() {
        static OpenRgbMockState inst;
        return inst;
    }
};
