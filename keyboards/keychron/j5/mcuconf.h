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

#include_next <mcuconf.h>

#undef WB32_I2C_USE_I2C1
#define WB32_I2C_USE_I2C1 TRUE

#undef WB32_SPI_USE_QSPI
#define WB32_SPI_USE_QSPI TRUE

/* Enable LSI */
#undef WB32_LSI_ENABLED
#define WB32_LSI_ENABLED TRUE

/* RTC driver system settings. */
#define WB32_RTCSEL WB32_RTCSEL_HSEDIV
#define WB32_RTCLP_SEL WB32_RTCSEL_LSI
#define WB32_RTCAlarm_IRQ_PRIORITY         14
#define WB32_RTC_IRQ_PRIORITY              15

/* Enable EXTI */
#define WB32_EXTI_REQUIRED
