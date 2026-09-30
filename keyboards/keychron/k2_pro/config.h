/* Copyright 2022 ~ 2025 @ Keychron (https://www.keychron.com)
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

#include "eeconfig_kb.h"

/* Shift Register Configuration for Matrix Scan */
#define HC595_STCP A0
#define HC595_SHCP A1
#define HC595_DS C15
#define HC595_START_INDEX 1
#define HC595_END_INDEX 15

/* Increase I2C speed to 1000 KHz */
#define I2C1_TIMINGR_PRESC 0U
#define I2C1_TIMINGR_SCLDEL 3U
#define I2C1_TIMINGR_SDADEL 0U
#define I2C1_TIMINGR_SCLH 15U
#define I2C1_TIMINGR_SCLL 51U

#if defined(RGB_MATRIX_ENABLE) || defined(LED_MATRIX_ENABLE)
#    define SNLED27351_SDB_PIN C14
#endif

/*
 * Preserve the original load compensation: shared battery code uses the LED
 * trim for RGB, while the monochrome driver normalizes duty as three channels.
 */
#if defined(RGB_MATRIX_ENABLE)
#    define VOLTAGE_TRIM_LED_MATRIX 60
#elif defined(LED_MATRIX_ENABLE)
#    define VOLTAGE_TRIM_LED_MATRIX 90
#endif

/* Bluetooth Configuration */
#ifdef KC_BLUETOOTH_ENABLE
#    define BT_MODE_SELECT_PIN A10

#    define CKBT51_RESET_PIN A9
#    define WIRELESS_TO_MCU_INT_PIN A6
#    define MCU_TO_WIRELESS_INT_PIN A5

#    define USB_POWER_SENSE_PIN B1
#    define USB_POWER_CONNECTED_LEVEL 0

#    define BAT_LOW_LED_PIN A4
#    define BAT_LOW_LED_PIN_ON_STATE 1

#    define HOST_DEVICES_COUNT 3

#    if defined(RGB_MATRIX_ENABLE) || defined(LED_MATRIX_ENABLE)

#        define BT_INDCATION_LED_MATRIX_LIST {17, 18, 19}

#        define BAT_LEVEL_LED_LIST {17, 18, 19, 20, 21, 22, 23, 24, 25, 26}

#    endif

#    define LED_DRIVER_REINIT_ON_TRANSPORT_CHANGE

#    define KEEP_USB_CONNECTION_IN_WIRELESS_MODE

#    define WIRELESS_NKRO_ENABLE
#endif

/* Factory test keys */
#define FN_KEY_1 MO(1)
#define FN_KEY_2 MO(3)
