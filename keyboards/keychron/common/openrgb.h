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

#pragma once

#include <stdint.h>
#include "color.h"

#define OPENRGB_PROTOCOL_VERSION 0xD

enum openrgb_command_id {
    OPENRGB_GET_PROTOCOL_VERSION = 1,
    OPENRGB_GET_QMK_VERSION,
    OPENRGB_GET_DEVICE_INFO,
    OPENRGB_GET_MODE_INFO,
    OPENRGB_GET_LED_INFO,
    OPENRGB_GET_ENABLED_MODES,

    OPENRGB_SET_MODE,
    OPENRGB_DIRECT_MODE_SET_SINGLE_LED,
    OPENRGB_DIRECT_MODE_SET_LEDS,
};

enum openrgb_responses {
    OPENRGB_FAILURE        = 25,
    OPENRGB_SUCCESS        = 50,
    OPENRGB_END_OF_MESSAGE = 100,
};

/* OpenRGB mode identifiers. These mirror the `Modes` enum in OpenRGB's
 * QMKOpenRGBController.h: OpenRGB looks for these values in the
 * GET_ENABLED_MODES reply (via find()) and never assumes the keyboard's own
 * rgb_matrix effect order. openrgb.c maps each to a QMK rgb_matrix effect. */
enum openrgb_mode_id {
    OPENRGB_MODE_OPENRGB_DIRECT = 1,
    OPENRGB_MODE_SOLID_COLOR,
    OPENRGB_MODE_ALPHA_MOD,
    OPENRGB_MODE_GRADIENT_UP_DOWN,
    OPENRGB_MODE_GRADIENT_LEFT_RIGHT,
    OPENRGB_MODE_BREATHING,
    OPENRGB_MODE_BAND_SAT,
    OPENRGB_MODE_BAND_VAL,
    OPENRGB_MODE_BAND_PINWHEEL_SAT,
    OPENRGB_MODE_BAND_PINWHEEL_VAL,
    OPENRGB_MODE_BAND_SPIRAL_SAT,
    OPENRGB_MODE_BAND_SPIRAL_VAL,
    OPENRGB_MODE_CYCLE_ALL,
    OPENRGB_MODE_CYCLE_LEFT_RIGHT,
    OPENRGB_MODE_CYCLE_UP_DOWN,
    OPENRGB_MODE_CYCLE_OUT_IN,
    OPENRGB_MODE_CYCLE_OUT_IN_DUAL,
    OPENRGB_MODE_RAINBOW_MOVING_CHEVRON,
    OPENRGB_MODE_CYCLE_PINWHEEL,
    OPENRGB_MODE_CYCLE_SPIRAL,
    OPENRGB_MODE_DUAL_BEACON,
    OPENRGB_MODE_RAINBOW_BEACON,
    OPENRGB_MODE_RAINBOW_PINWHEELS,
    OPENRGB_MODE_RAINDROPS,
    OPENRGB_MODE_JELLYBEAN_RAINDROPS,
    OPENRGB_MODE_HUE_BREATHING,
    OPENRGB_MODE_HUE_PENDULUM,
    OPENRGB_MODE_HUE_WAVE,
    OPENRGB_MODE_TYPING_HEATMAP,
    OPENRGB_MODE_DIGITAL_RAIN,
    OPENRGB_MODE_SOLID_REACTIVE_SIMPLE,
    OPENRGB_MODE_SOLID_REACTIVE,
    OPENRGB_MODE_SOLID_REACTIVE_WIDE,
    OPENRGB_MODE_SOLID_REACTIVE_MULTIWIDE,
    OPENRGB_MODE_SOLID_REACTIVE_CROSS,
    OPENRGB_MODE_SOLID_REACTIVE_MULTICROSS,
    OPENRGB_MODE_SOLID_REACTIVE_NEXUS,
    OPENRGB_MODE_SOLID_REACTIVE_MULTINEXUS,
    OPENRGB_MODE_SPLASH,
    OPENRGB_MODE_MULTISPLASH,
    OPENRGB_MODE_SOLID_SPLASH,
    OPENRGB_MODE_SOLID_MULTISPLASH,
    OPENRGB_MODE_PIXEL_RAIN,
    OPENRGB_MODE_PIXEL_FLOW,
    OPENRGB_MODE_PIXEL_FRACTAL,
};

extern RGB g_openrgb_direct_mode_colors[];

bool openrgb_command_handler(uint8_t *data, uint8_t length);
