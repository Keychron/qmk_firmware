/* Copyright 2025 ~ 2026 @ Keychron (https://www.keychron.com)
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

/* 74HC595 Configuration */
#define HC595_STCP C3
#define HC595_SHCP B10
#define HC595_DS C8
#define HC595_START_INDEX 13
#define HC595_END_INDEX 20
#define HC595_OFFSET_INDEX 0

/* Encoder Configuration */
#define ENCODER_PULL_UP_DISABLE
#define ENCODER_MAP_KEY_DELAY 2

/* USB DP control pin */
#define DP_PULLUP_CONTROL_PIN B7

#ifdef RGB_MATRIX_ENABLE
/* RGB Matrix driver configuration */
#    define RGB_MATRIX_LED_COUNT 107
#    define SNLED27351_SELECT_PINS \
        { A8, C9 }
#    define SNLED27351_SDB_PIN C5
#    define SNLED27351_SPI_DIVISOR 16

/* Set LED driver current */
#    define SNLED27351_CURRENT_TUNE \
        { 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 }

/* Indications */
#    define WINLOCK_LED_LIST \
        { 95 }
#endif

#ifdef LK_WIRELESS_ENABLE
#    define BAT_CHARGING_PIN B14
#    define BAT_CHARGING_LEVEL 0

#    define BAT_LOW_LED_PIN A3
#    define BAT_LOW_LED_PIN_ON_STATE 1

#    define BT_INDICATION_LED_PIN_LIST \
        { A0, A0, A0 }
#    define BT_INDICATION_LED_ON_STATE 0
#    define P24G_LED_PIN A2

#    if defined(RGB_MATRIX_ENABLE)
#        define SNLED27351_SDB_PIN C5
#        define BT_INDCATION_LED_MATRIX_LIST \
            { 20, 21, 22 }
#        define P24G_INDICATION_LED_INDEX 23
#        define BAT_LEVEL_LED_LIST \
            { 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 }
#    endif
#endif

/* Win key lock toggle */
#define WIN_LOCK_HOLD_TIME 3000
#define GUI_TOGGLE_INDICATION_ENABLE

/* OS toggle */
#define KEYCOMBO_OS_TOGGLE_INDICATION_ENABLE
#define KEYCOMBO_OS_TOGGLE_ENABLE
#define KEYCOMBO_OS_TOGGLE_HOLD_TIME 3000
#define WIN_BASE_LAYER 2
#define MAC_BASE_LAYER 0
