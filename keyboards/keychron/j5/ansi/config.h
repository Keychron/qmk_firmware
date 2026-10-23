/* Copyright 2025 ~ 2026 @ Keychron (https://www.keychron.com)
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

/* USB DP control pin */
#define DP_PULLUP_CONTROL_PIN B11

#ifdef RGB_MATRIX_ENABLE
/* RGB Matrix driver configuration */
#    define RGB_MATRIX_LED_COUNT 108
#    define SNLED27351_SELECT_PINS \
        { C9, A8 }
#    define SNLED27351_SDB_PIN B7
#    define SNLED27351_SPI_DIVISOR 16

/* Set LED driver current */
#    define SNLED27351_CURRENT_TUNE \
        { 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28 }

/* Indicators */
#    define WINLOCK_LED_LIST \
        { 96 }
#    define NKRO_TOGGLE_INDICATION_ENABLE
#endif

#ifdef LK_WIRELESS_ENABLE
#    define BAT_CHARGING_PIN C2
#    define BAT_CHARGING_LEVEL 0

#    define BAT_LOW_LED_PIN C3
#    define BAT_LOW_LED_PIN_ON_STATE 1

#    if defined(RGB_MATRIX_ENABLE)
#        define P24G_INDICATION_LED_INDEX 24
#        define BT_INDCATION_LED_MATRIX_LIST \
            { 21, 22, 23 }
#        define BAT_LEVEL_LED_LIST \
            { 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 }
#    endif
#endif

/* Win key lock toggle */
#define GU_TOGGGLE_INDICATION_ENABLE
#define WIN_LOCK_HOLD_TIME 3000
#define WIN_BASE_LAYER 0
