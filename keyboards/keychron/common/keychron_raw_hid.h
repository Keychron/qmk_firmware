/* Copyright 2024 ~ 2025 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define PROTOCOL_VERSION 0x02
#define MISC_PROTOCOL_VERSION   0x0002
#define QMK_COMMAND_SET 2

enum {
    KC_GET_PROTOCOL_VERSION = 0xA0,
    KC_GET_FIRMWARE_VERSION = 0xA1,
    KC_GET_SUPPORT_FEATURE  = 0xA2,
    KC_GET_DEFAULT_LAYER    = 0xA3,
    KC_GET_BATTERY_LEVEL    = 0xA4,
    KC_MISC_CMD_GROUP       = 0xA7,
    KC_KEYCHRON_RGB         = 0xA8,
    KC_ANALOG_MATRIX        = 0xA9,
    KC_WIRELESS_DFU         = 0xAA,
    KC_FACTORY_TEST         = 0xAB
};

enum {
    // Byte 0
    FEATURE_DEFAULT_LAYER       = 0x01U << 0,
    FEATURE_BLUETOOTH           = 0x01U << 1,
    FEATURE_P24G                = 0x01U << 2,
    FEATURE_ANALOG_MATRIX       = 0x01U << 3,
    FEATURE_STATE_NOTIFY        = 0x01U << 4,
    FEATURE_DYNAMIC_DEBOUNCE    = 0x01U << 5,
    FEATURE_SNAP_CLICK          = 0x01U << 6,
    FEATURE_KEYCHRON_RGB        = 0x01U << 7,
    // Byte 1
    FEATURE_QUICK_START         = 0x01U << 8,
    FEATURE_NKRO                = 0x01U << 9,
};

enum {
    MISC_DFU_INFO            = 0x01U << 0,
    MISC_LANGUAGE            = 0x01U << 1,
    MISC_DEBOUNCE            = 0x01U << 2,
    MISC_SNAP_CLICK          = 0x01U << 3,
    MISC_WIRELESS_LPM        = 0x01U << 4,
    MISC_REPORT_REATE        = 0x01U << 5,
    MISC_QUICK_START         = 0x01U << 6,
    MISC_NKRO                = 0x01U << 7,
};

enum {
    MISC_GET_PROTOCOL_VER = 0x01,
    DFU_INFO_GET,
    LANGUAGE_GET,
    LANGUAGE_SET,
    DEBOUNCE_GET,           // 5
    DEBOUNCE_SET,
    SNAP_CLICK_GET_INFO,
    SNAP_CLICK_GET,
    SNAP_CLICK_SET,
    SNAP_CLICK_SAVE,        // A
    WIRELESS_LPM_GET,
    WIRELESS_LPM_SET,
    REPORT_RATE_GET,
    REPORT_RATE_SET,
    DIP_SWITCH_GET,
    DIP_SWITCH_SET,         // 0x10
    FACTORY_RESET,
    NKRO_GET,
    NKRO_SET,
};

enum {
    KC_SUCCESS = 0,
    KC_FAIL,
};

void kc_raw_hid_send(uint8_t src, uint8_t *data, uint8_t len);
