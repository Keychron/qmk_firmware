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

#ifdef LED_MATRIX_ENABLE
/* SNLED27351 Driver Configuration */
#    define SNLED27351_I2C_ADDRESS_1 SNLED27351_I2C_ADDRESS_GND
#    define SNLED27351_PHASE_CHANNEL SNLED27351_SCAN_PHASE_6_CHANNEL

/* LED Current Configuration */
#    define SNLED27351_CURRENT_TUNE {0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60}

/* LED Matrix Configuration */
#    define LED_MATRIX_LED_COUNT 84

/* Indications */
#    define DIM_CAPS_LOCK
#    define CAPS_LOCK_INDEX 46
#    define LOW_BAT_IND_INDEX {77}

#endif
