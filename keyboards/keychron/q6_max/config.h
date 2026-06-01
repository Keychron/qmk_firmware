/* Copyright 2024 ~ 2026 @ Keychron (https://www.keychron.com)
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

/* Encoder Configuration */
#define ENCODER_DEFAULT_POS 0x3
#define ENCODER_MAP_KEY_DELAY 2

/* SPI configuration */
#if defined(RGB_MATRIX_ENABLE) || defined(LK_WIRELESS_ENABLE)
#    define SPI_DRIVER SPID1
#    define SPI_SCK_PIN A5
#    define SPI_MISO_PIN A6
#    define SPI_MOSI_PIN A7
#endif

/* SNLED27351 Driver Configuration */
#if defined(RGB_MATRIX_ENABLE)
#    define SNLED27351_SDB_PIN B7
#    define SNLED27351_SELECT_PINS \
        { B8, B9 }
#    define SNLED27351_SPI_DIVISOR 16
#endif

#ifdef LK_WIRELESS_ENABLE
/* Hardware configuration */
#    define P24G_MODE_SELECT_PIN A10
#    define BT_MODE_SELECT_PIN A9

#    define LKBT51_RESET_PIN C4
#    define WIRELESS_TO_MCU_INT_PIN B1
#    define MCU_TO_WIRELESS_INT_PIN A4

#    define USB_POWER_SENSE_PIN B0
#    define USB_POWER_CONNECTED_LEVEL 0

#    define BAT_LOW_LED_PIN C9
#    define BAT_LOW_LED_PIN_ON_STATE 1

#    define BAT_CHARGING_PIN B13
#    define BAT_CHARGING_LEVEL 0

#    define BT_HOST_DEVICES_COUNT 3

#    if defined(RGB_MATRIX_ENABLE)

#        define BT_INDCATION_LED_MATRIX_LIST \
            { 1, 2, 3 }

#        define P24G_INDICATION_LED_INDEX 4

#        define BAT_LEVEL_LED_LIST \
            { 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 }

/* Reinit LED driver on tranport changed */
#        define LED_DRIVER_REINIT_ON_TRANSPORT_CHANGE
#    endif

/* Keep USB connection in wireless mode */
#    define KEEP_USB_CONNECTION_IN_WIRELESS_MODE

/* Enable wireless NKRO */
#    define WIRELESS_NKRO_ENABLE

#endif

/* Factory test keys */
#define FN_KEY_1 MO(1)
#define FN_KEY_2 MO(3)
