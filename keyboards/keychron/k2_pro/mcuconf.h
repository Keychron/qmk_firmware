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

#include_next <mcuconf.h>

#undef STM32_LSI_ENABLED
#define STM32_LSI_ENABLED TRUE

/* Use the lowest USB-compatible HCLK to reduce Bluetooth power consumption
 * until ChibiOS supports dynamic clocks on STM32L4. */
#undef STM32_PLLM_VALUE
#define STM32_PLLM_VALUE 2

#undef STM32_PLLN_VALUE
#define STM32_PLLN_VALUE 12

#undef STM32_I2C_USE_I2C1
#define STM32_I2C_USE_I2C1 TRUE

#ifdef KC_BLUETOOTH_ENABLE
#    undef STM32_SERIAL_USE_USART2
#    define STM32_SERIAL_USE_USART2 TRUE
#endif
