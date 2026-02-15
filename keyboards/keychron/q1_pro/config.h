/* Copyright 2023 ~ 2026 @ Keychron (https://www.keychron.com)
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

#include "eeconfig_kb.h"

/* HC595 Shift Register for Matrix Scan */
#define HC595_STCP B0
#define HC595_SHCP A1
#define HC595_DS A7
#define HC595_START_INDEX 0
#define HC595_END_INDEX 15

/* DIP switch for Mac/Win OS switch */
#define DIP_SWITCH_PINS { A8 }

#if defined(RGB_MATRIX_ENABLE) || defined(LED_MATRIX_ENABLE)
#    define SNLED27351_SDB_PIN C14
#endif

/* Bluetooth Configuration */
#ifdef KC_BLUETOOTH_ENABLE
#    define BT_MODE_SELECT_PIN C15

#    define CKBT51_RESET_PIN A9
#    define MCU_TO_WIRELESS_INT_PIN A5
#    define WIRELESS_TO_MCU_INT_PIN A6

#    define USB_POWER_SENSE_PIN B1
#    define USB_POWER_CONNECTED_LEVEL 0

#    define HOST_DEVICES_COUNT 3

#    if defined(RGB_MATRIX_ENABLE)
#        define BT_INDCATION_LED_MATRIX_LIST \
            { 16, 17, 18 }

#        define BAT_LEVEL_LED_LIST \
            { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 }
#    endif

#    define LED_DRIVER_REINIT_ON_TRANSPORT_CHANGE

#    define KEEP_USB_CONNECTION_IN_WIRELESS_MODE

#    define WIRELESS_NKRO_ENABLE
#endif

/* EEPROM */
#define WEAR_LEVELING_LOGICAL_SIZE 2048
#define WEAR_LEVELING_BACKING_SIZE (WEAR_LEVELING_LOGICAL_SIZE * 2)
#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR 2047

/* Factory test keys */
#define FN_KEY_1 MO(1)
#define FN_KEY_2 MO(3)

#define INVERT_OS_SWITCH_STATE
