/* Copyright 2021 @ Keychron (https://www.keychron.com)
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

/* USB Device descriptor parameter */
<<<<<<<< HEAD:keyboards/keychron/q10/q10_ansi_stm32l432_ec11/config.h
#define PRODUCT_ID 0x01a1
#define DEVICE_VER 0x0100

/* RGB Matrix Configuration */
#define DRIVER_1_LED_TOTAL 47
#define DRIVER_2_LED_TOTAL 41
========
#define PRODUCT_ID 0x0115
#define DEVICE_VER 0x0101

/* RGB Matrix Configuration */
#define DRIVER_1_LED_TOTAL 35
#define DRIVER_2_LED_TOTAL 36
>>>>>>>> playground:keyboards/keychron/q2/q2_jis_stm32l432_ec11/config.h
#define DRIVER_LED_TOTAL (DRIVER_1_LED_TOTAL + DRIVER_2_LED_TOTAL)

/* Encoder Configuration */
#define ENCODERS_PAD_A { A10 }
<<<<<<<< HEAD:keyboards/keychron/q10/q10_ansi_stm32l432_ec11/config.h
#define ENCODERS_PAD_B { A8 }
========
#define ENCODERS_PAD_B { B5 }
>>>>>>>> playground:keyboards/keychron/q2/q2_jis_stm32l432_ec11/config.h
#define ENCODER_RESOLUTION 4
#define ENCODER_DEFAULT_POS 0x3

/* Enable caps-lock LED */
<<<<<<<< HEAD:keyboards/keychron/q10/q10_ansi_stm32l432_ec11/config.h
#define CAPS_LOCK_LED_INDEX 49
========
#define CAPS_LOCK_LED_INDEX 30
>>>>>>>> playground:keyboards/keychron/q2/q2_jis_stm32l432_ec11/config.h
