/* Copyright 2025 ~ 2026 @ Keychron(https://www.keychron.com)
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

/* Reserves the keyboard EEPROM datablock used by the Keychron common code
 * (language, snap click). Without it that data overlaps VIA's storage. */
#include "eeconfig_kb.h"

/* I2C driver Configuration */
#define I2C1_SCL_PIN B8
#define I2C1_SDA_PIN B9
#define I2C1_CLOCK_SPEED 400000
#define I2C1_DUTY_CYCLE FAST_DUTY_CYCLE_2

/* Custom keycodes */
#define CUSTOM_KEYCODES_ENABLE

/*  OS Toggle */
#define KEYCOMBO_OS_TOGGLE_ENABLE
#define KEYCOMBO_OS_TOGGLE_INDICATION_ENABLE
#define MAC_BASE_LAYER 0
#define WIN_BASE_LAYER 2

/* Factory test keys: the Fn keys tracked for the Fn+J+Z factory reset and
 * the Fn+Home+Right backlight test (MAC_FN and WIN_FN layers) */
#define FN_KEY_1 MO(1)
#define FN_KEY_2 MO(3)
