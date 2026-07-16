/* Copyright 2020 Kasper
 * Copyright 2025 Keychron K2 HE OpenRGB patch
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

#include "quantum.h"
#include "openrgb.h"
#include "raw_hid.h"
#include "usb_descriptor.h"
#include "version.h"
#include "keymap_introspection.h"
#include "keychron_raw_hid.h"
#include "string.h"

#if !defined(OPENRGB_DIRECT_MODE_STARTUP_RED)
#    define OPENRGB_DIRECT_MODE_STARTUP_RED 0
#endif

#if !defined(OPENRGB_DIRECT_MODE_STARTUP_GREEN)
#    define OPENRGB_DIRECT_MODE_STARTUP_GREEN 0
#endif

#if !defined(OPENRGB_DIRECT_MODE_STARTUP_BLUE)
#    define OPENRGB_DIRECT_MODE_STARTUP_BLUE 0
#endif

RGB g_openrgb_direct_mode_colors[RGB_MATRIX_LED_COUNT] = {[0 ... RGB_MATRIX_LED_COUNT - 1] = {OPENRGB_DIRECT_MODE_STARTUP_RED, OPENRGB_DIRECT_MODE_STARTUP_GREEN, OPENRGB_DIRECT_MODE_STARTUP_BLUE}};

typedef struct {
    uint8_t openrgb_id;
    uint8_t qmk_mode;
} openrgb_mode_map_t;

/* Maps OpenRGB modes to QMK rgb_matrix effects, in OpenRGB's own mode order
 * (the if-order of OpenRGB's RGBController_QMKOpenRGBRevD.cpp), with OpenRGB
 * Direct last. OpenRGB assigns each enabled mode a sequential value in this
 * order and sends that value back in SET_MODE, so the index into this table is
 * exactly the value OpenRGB uses -- decoupling the protocol from QMK's own
 * effect numbering (which differs and contains effects OpenRGB doesn't know).
 * Each entry is guarded by the same ENABLE_RGB_MATRIX_* flag that gates the
 * effect, so the table reduces to precisely the effects compiled in. */
static const openrgb_mode_map_t openrgb_mode_map[] = {
    {OPENRGB_MODE_SOLID_COLOR, RGB_MATRIX_SOLID_COLOR},
#ifdef ENABLE_RGB_MATRIX_ALPHAS_MODS
    {OPENRGB_MODE_ALPHA_MOD, RGB_MATRIX_ALPHAS_MODS},
#endif
#ifdef ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
    {OPENRGB_MODE_GRADIENT_UP_DOWN, RGB_MATRIX_GRADIENT_UP_DOWN},
#endif
#ifdef ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
    {OPENRGB_MODE_GRADIENT_LEFT_RIGHT, RGB_MATRIX_GRADIENT_LEFT_RIGHT},
#endif
#ifdef ENABLE_RGB_MATRIX_BREATHING
    {OPENRGB_MODE_BREATHING, RGB_MATRIX_BREATHING},
#endif
#ifdef ENABLE_RGB_MATRIX_BAND_SAT
    {OPENRGB_MODE_BAND_SAT, RGB_MATRIX_BAND_SAT},
#endif
#ifdef ENABLE_RGB_MATRIX_BAND_VAL
    {OPENRGB_MODE_BAND_VAL, RGB_MATRIX_BAND_VAL},
#endif
#ifdef ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
    {OPENRGB_MODE_BAND_PINWHEEL_SAT, RGB_MATRIX_BAND_PINWHEEL_SAT},
#endif
#ifdef ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
    {OPENRGB_MODE_BAND_PINWHEEL_VAL, RGB_MATRIX_BAND_PINWHEEL_VAL},
#endif
#ifdef ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
    {OPENRGB_MODE_BAND_SPIRAL_SAT, RGB_MATRIX_BAND_SPIRAL_SAT},
#endif
#ifdef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
    {OPENRGB_MODE_BAND_SPIRAL_VAL, RGB_MATRIX_BAND_SPIRAL_VAL},
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_ALL
    {OPENRGB_MODE_CYCLE_ALL, RGB_MATRIX_CYCLE_ALL},
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
    {OPENRGB_MODE_CYCLE_LEFT_RIGHT, RGB_MATRIX_CYCLE_LEFT_RIGHT},
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
    {OPENRGB_MODE_CYCLE_UP_DOWN, RGB_MATRIX_CYCLE_UP_DOWN},
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
    {OPENRGB_MODE_CYCLE_OUT_IN, RGB_MATRIX_CYCLE_OUT_IN},
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
    {OPENRGB_MODE_CYCLE_OUT_IN_DUAL, RGB_MATRIX_CYCLE_OUT_IN_DUAL},
#endif
#ifdef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
    {OPENRGB_MODE_RAINBOW_MOVING_CHEVRON, RGB_MATRIX_RAINBOW_MOVING_CHEVRON},
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
    {OPENRGB_MODE_CYCLE_PINWHEEL, RGB_MATRIX_CYCLE_PINWHEEL},
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_SPIRAL
    {OPENRGB_MODE_CYCLE_SPIRAL, RGB_MATRIX_CYCLE_SPIRAL},
#endif
#ifdef ENABLE_RGB_MATRIX_DUAL_BEACON
    {OPENRGB_MODE_DUAL_BEACON, RGB_MATRIX_DUAL_BEACON},
#endif
#ifdef ENABLE_RGB_MATRIX_RAINBOW_BEACON
    {OPENRGB_MODE_RAINBOW_BEACON, RGB_MATRIX_RAINBOW_BEACON},
#endif
#ifdef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
    {OPENRGB_MODE_RAINBOW_PINWHEELS, RGB_MATRIX_RAINBOW_PINWHEELS},
#endif
#ifdef ENABLE_RGB_MATRIX_RAINDROPS
    {OPENRGB_MODE_RAINDROPS, RGB_MATRIX_RAINDROPS},
#endif
#ifdef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
    {OPENRGB_MODE_JELLYBEAN_RAINDROPS, RGB_MATRIX_JELLYBEAN_RAINDROPS},
#endif
#ifdef ENABLE_RGB_MATRIX_HUE_BREATHING
    {OPENRGB_MODE_HUE_BREATHING, RGB_MATRIX_HUE_BREATHING},
#endif
#ifdef ENABLE_RGB_MATRIX_HUE_PENDULUM
    {OPENRGB_MODE_HUE_PENDULUM, RGB_MATRIX_HUE_PENDULUM},
#endif
#ifdef ENABLE_RGB_MATRIX_HUE_WAVE
    {OPENRGB_MODE_HUE_WAVE, RGB_MATRIX_HUE_WAVE},
#endif
#ifdef ENABLE_RGB_MATRIX_TYPING_HEATMAP
    {OPENRGB_MODE_TYPING_HEATMAP, RGB_MATRIX_TYPING_HEATMAP},
#endif
#ifdef ENABLE_RGB_MATRIX_DIGITAL_RAIN
    {OPENRGB_MODE_DIGITAL_RAIN, RGB_MATRIX_DIGITAL_RAIN},
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
    {OPENRGB_MODE_SOLID_REACTIVE_SIMPLE, RGB_MATRIX_SOLID_REACTIVE_SIMPLE},
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE
    {OPENRGB_MODE_SOLID_REACTIVE, RGB_MATRIX_SOLID_REACTIVE},
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
    {OPENRGB_MODE_SOLID_REACTIVE_WIDE, RGB_MATRIX_SOLID_REACTIVE_WIDE},
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
    {OPENRGB_MODE_SOLID_REACTIVE_MULTIWIDE, RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE},
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
    {OPENRGB_MODE_SOLID_REACTIVE_CROSS, RGB_MATRIX_SOLID_REACTIVE_CROSS},
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
    {OPENRGB_MODE_SOLID_REACTIVE_MULTICROSS, RGB_MATRIX_SOLID_REACTIVE_MULTICROSS},
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
    {OPENRGB_MODE_SOLID_REACTIVE_NEXUS, RGB_MATRIX_SOLID_REACTIVE_NEXUS},
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
    {OPENRGB_MODE_SOLID_REACTIVE_MULTINEXUS, RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS},
#endif
#ifdef ENABLE_RGB_MATRIX_SPLASH
    {OPENRGB_MODE_SPLASH, RGB_MATRIX_SPLASH},
#endif
#ifdef ENABLE_RGB_MATRIX_MULTISPLASH
    {OPENRGB_MODE_MULTISPLASH, RGB_MATRIX_MULTISPLASH},
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_SPLASH
    {OPENRGB_MODE_SOLID_SPLASH, RGB_MATRIX_SOLID_SPLASH},
#endif
#ifdef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
    {OPENRGB_MODE_SOLID_MULTISPLASH, RGB_MATRIX_SOLID_MULTISPLASH},
#endif
    {OPENRGB_MODE_OPENRGB_DIRECT, RGB_MATRIX_OPENRGB_DIRECT},
};

#define OPENRGB_MODE_COUNT (sizeof(openrgb_mode_map) / sizeof(openrgb_mode_map[0]))

static uint8_t raw_hid_buffer[RAW_EPSIZE];

static void openrgb_get_protocol_version(void) {
    raw_hid_buffer[0] = OPENRGB_GET_PROTOCOL_VERSION;
    raw_hid_buffer[1] = OPENRGB_PROTOCOL_VERSION;
}

static void openrgb_get_qmk_version(void) {
    raw_hid_buffer[0]    = OPENRGB_GET_QMK_VERSION;
    uint8_t current_byte = 1;
    for (uint8_t i = 0; (current_byte < (RAW_EPSIZE - 2)) && (QMK_VERSION[i] != 0); i++) {
        raw_hid_buffer[current_byte] = QMK_VERSION[i];
        current_byte++;
    }
}

static void openrgb_get_device_info(void) {
    raw_hid_buffer[0] = OPENRGB_GET_DEVICE_INFO;
    raw_hid_buffer[1] = RGB_MATRIX_LED_COUNT;
    raw_hid_buffer[2] = MATRIX_COLS * MATRIX_ROWS;

#define MASSDROP_VID 0x04D8
#if VENDOR_ID == MASSDROP_VID
#    define PRODUCT_STRING PRODUCT
#    define MANUFACTURER_STRING MANUFACTURER
#else
#    define PRODUCT_STRING STR(PRODUCT)
#    define MANUFACTURER_STRING STR(MANUFACTURER)
#endif

    // Reserve the last 2 bytes for the status / end-of-message markers. The
    // product string takes the first half of the remaining space, the
    // manufacturer string the rest; both are explicitly null-terminated.
    uint8_t current_byte = 3;
    for (uint8_t i = 0; (current_byte < ((RAW_EPSIZE - 2) / 2)) && (PRODUCT_STRING[i] != 0); i++) {
        raw_hid_buffer[current_byte++] = PRODUCT_STRING[i];
    }
    raw_hid_buffer[current_byte++] = 0;

    for (uint8_t i = 0; (current_byte < (RAW_EPSIZE - 2)) && (MANUFACTURER_STRING[i] != 0); i++) {
        raw_hid_buffer[current_byte++] = MANUFACTURER_STRING[i];
    }
    raw_hid_buffer[current_byte++] = 0;

#undef MASSDROP_VID
#undef PRODUCT_STRING
#undef MANUFACTURER_STRING
}

static void openrgb_get_mode_info(void) {
    const HSV     hsv_color = rgb_matrix_get_hsv();
    const uint8_t qmk_mode  = rgb_matrix_get_mode();

    // Translate the active QMK effect back to OpenRGB's 1-based mode value. If
    // the current effect has no OpenRGB equivalent (e.g. one selected on the
    // keyboard itself), report 0 so OpenRGB just doesn't highlight a mode.
    uint8_t openrgb_mode = 0;
    for (uint8_t i = 0; i < OPENRGB_MODE_COUNT; i++) {
        if (openrgb_mode_map[i].qmk_mode == qmk_mode) {
            openrgb_mode = i + 1;
            break;
        }
    }

    raw_hid_buffer[0] = OPENRGB_GET_MODE_INFO;
    raw_hid_buffer[1] = openrgb_mode;
    raw_hid_buffer[2] = rgb_matrix_get_speed();
    raw_hid_buffer[3] = hsv_color.h;
    raw_hid_buffer[4] = hsv_color.s;
    raw_hid_buffer[5] = hsv_color.v;
}

static void openrgb_get_led_info(uint8_t *data) {
    const uint8_t first_led   = data[1];
    const uint8_t number_leds = data[2];

    raw_hid_buffer[0] = OPENRGB_GET_LED_INFO;

    const uint8_t max_leds = (RAW_EPSIZE - 2) / 7; // 7 bytes per LED entry
    for (uint8_t i = 0; i < number_leds && i < max_leds; i++) {
        const uint8_t led_idx  = first_led + i;
        const uint8_t data_idx = i * 7;

        if (led_idx >= RGB_MATRIX_LED_COUNT) {
            raw_hid_buffer[data_idx + 3] = OPENRGB_FAILURE;
        } else {
            raw_hid_buffer[data_idx + 1] = g_led_config.point[led_idx].x;
            raw_hid_buffer[data_idx + 2] = g_led_config.point[led_idx].y;
            raw_hid_buffer[data_idx + 3] = g_led_config.flags[led_idx];
            raw_hid_buffer[data_idx + 4] = g_openrgb_direct_mode_colors[led_idx].r;
            raw_hid_buffer[data_idx + 5] = g_openrgb_direct_mode_colors[led_idx].g;
            raw_hid_buffer[data_idx + 6] = g_openrgb_direct_mode_colors[led_idx].b;
        }

        uint8_t row   = 0;
        uint8_t col   = 0;
        uint8_t found = 0;

        for (row = 0; row < MATRIX_ROWS; row++) {
            for (col = 0; col < MATRIX_COLS; col++) {
                if (g_led_config.matrix_co[row][col] == led_idx) {
                    found = 1;
                    break;
                }
            }
            if (found == 1) {
                break;
            }
        }

        if (col >= MATRIX_COLS || row >= MATRIX_ROWS) {
            raw_hid_buffer[data_idx + 7] = KC_NO;
        } else {
            raw_hid_buffer[data_idx + 7] = (uint8_t)keycode_at_keymap_location(0, row, col);
        }
    }
}

static void openrgb_get_enabled_modes(void) {
    raw_hid_buffer[0] = OPENRGB_GET_ENABLED_MODES;

    // Report the OpenRGB mode ids we support, as a null-terminated list (the
    // host reads bytes until it hits a 0). Order is irrelevant here -- OpenRGB
    // matches by value -- but keep one byte free for the terminator.
    const uint8_t max_entries = RAW_EPSIZE - 2;
    uint8_t       idx         = 0;
    for (uint8_t i = 0; i < OPENRGB_MODE_COUNT && idx < max_entries - 1; i++) {
        raw_hid_buffer[1 + idx++] = openrgb_mode_map[i].openrgb_id;
    }
    raw_hid_buffer[1 + idx] = 0;
}

static void openrgb_set_mode(uint8_t *data) {
    const uint8_t h     = data[1];
    const uint8_t s     = data[2];
    const uint8_t v     = data[3];
    const uint8_t mode  = data[4]; // OpenRGB's 1-based mode value == index into openrgb_mode_map
    const uint8_t speed = data[5];
    const uint8_t save  = data[6];

    raw_hid_buffer[0] = OPENRGB_SET_MODE;

    if (mode < 1 || mode > OPENRGB_MODE_COUNT) {
        raw_hid_buffer[RAW_EPSIZE - 2] = OPENRGB_FAILURE;
        return;
    }

    const uint8_t qmk_mode = openrgb_mode_map[mode - 1].qmk_mode;

    if (save == 1) {
        rgb_matrix_mode(qmk_mode);
        rgb_matrix_set_speed(speed);
        rgb_matrix_sethsv(h, s, v);
    } else {
        rgb_matrix_mode_noeeprom(qmk_mode);
        rgb_matrix_set_speed_noeeprom(speed);
        rgb_matrix_sethsv_noeeprom(h, s, v);
    }

    raw_hid_buffer[RAW_EPSIZE - 2] = OPENRGB_SUCCESS;
}

static void openrgb_direct_mode_set_single_led(uint8_t *data) {
    const uint8_t led = data[1];
    const uint8_t r   = data[2];
    const uint8_t g   = data[3];
    const uint8_t b   = data[4];

    raw_hid_buffer[0] = OPENRGB_DIRECT_MODE_SET_SINGLE_LED;

    if (led >= RGB_MATRIX_LED_COUNT) {
        raw_hid_buffer[RAW_EPSIZE - 2] = OPENRGB_FAILURE;
        return;
    }

    g_openrgb_direct_mode_colors[led].r = r;
    g_openrgb_direct_mode_colors[led].g = g;
    g_openrgb_direct_mode_colors[led].b = b;

    raw_hid_buffer[RAW_EPSIZE - 2] = OPENRGB_SUCCESS;
}

static void openrgb_direct_mode_set_leds(uint8_t *data, uint8_t length) {
    const uint8_t number_leds = data[1];
    // Each LED entry is 4 bytes ([index, r, g, b]) following the 2-byte header
    // (command id + count). Clamp to what actually fits in the received report so
    // a malformed packet with an inflated count cannot drive reads past the buffer.
    const uint8_t max_leds = length > 2 ? (uint8_t)((length - 2) / 4) : 0;

    for (uint8_t i = 0; i < number_leds && i < max_leds; i++) {
        const uint8_t data_idx  = i * 4;
        const uint8_t color_idx = data[data_idx + 2];

        if (color_idx < RGB_MATRIX_LED_COUNT) {
            g_openrgb_direct_mode_colors[color_idx].r = data[data_idx + 3];
            g_openrgb_direct_mode_colors[color_idx].g = data[data_idx + 4];
            g_openrgb_direct_mode_colors[color_idx].b = data[data_idx + 5];
        }
    }
}

bool openrgb_command_handler(uint8_t *data, uint8_t length) {
    if (length < 1) {
        return false;
    }

    switch (data[0]) {
        case OPENRGB_GET_PROTOCOL_VERSION:
            openrgb_get_protocol_version();
            break;
        case OPENRGB_GET_QMK_VERSION:
            openrgb_get_qmk_version();
            break;
        case OPENRGB_GET_DEVICE_INFO:
            openrgb_get_device_info();
            break;
        case OPENRGB_GET_MODE_INFO:
            openrgb_get_mode_info();
            break;
        case OPENRGB_GET_LED_INFO:
            openrgb_get_led_info(data);
            break;
        case OPENRGB_GET_ENABLED_MODES:
            openrgb_get_enabled_modes();
            break;
        case OPENRGB_SET_MODE:
            openrgb_set_mode(data);
            break;
        case OPENRGB_DIRECT_MODE_SET_SINGLE_LED:
            openrgb_direct_mode_set_single_led(data);
            break;
        case OPENRGB_DIRECT_MODE_SET_LEDS:
            openrgb_direct_mode_set_leds(data, length);
            break;
        default:
            return false;
    }

    if (data[0] != OPENRGB_DIRECT_MODE_SET_LEDS) {
        raw_hid_buffer[RAW_EPSIZE - 1] = OPENRGB_END_OF_MESSAGE;
        // Reply over USB explicitly (like the Keychron Launcher's KC_* handlers do).
        // The core raw_hid_send() routes through the active host driver, whose
        // send_raw_hid is NULL while the wireless transport is installed (side
        // switch on BT/2.4G) -- silently dropping the reply. OpenRGB is USB-only
        // and the dispatch is already gated on src == RAW_HID_SRC_USB.
        kc_raw_hid_send(RAW_HID_SRC_USB, raw_hid_buffer, RAW_EPSIZE);
        memset(raw_hid_buffer, 0x00, RAW_EPSIZE);
    }

    return true;
}
