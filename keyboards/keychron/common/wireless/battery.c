/* Copyright 2022 ~ 2025 @ lokher (https://www.keychron.com)
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

#include "quantum.h"
#include "wireless.h"
#include "battery.h"
#include "transport.h"
#if defined(LK_WIRELESS_ENABLE)
#    include "lkbt51.h"
#elif defined(KC_BLUETOOTH_ENABLE)
#    include "ckbt51.h"
#endif
#include "lpm.h"
#include "indicator.h"
#include "rtc_timer.h"
#include "analog.h"
#include "backlit_indicator.h"
#include "config.h"

#define BATTERY_EMPTY_COUNT 10
#define CRITICAL_LOW_COUNT 20

/* Battery voltage resistive voltage divider setting of MCU */
#ifndef RVD_R1
#    define RVD_R1 10 // Upper side resitor value (uint: KΩ)
#endif
#ifndef RVD_R2
#    define RVD_R2 10 // Lower side resitor value (uint: KΩ)
#endif

/* Battery voltage resistive voltage divider setting of Bluetooth */
#ifndef LKBT51_RVD_R1
#    define LKBT51_RVD_R1 560
#endif
#ifndef LKBT51_RVD_R2
#    define LKBT51_RVD_R2 499
#endif

#ifndef VOLTAGE_TRIM_LED_MATRIX
#    define VOLTAGE_TRIM_LED_MATRIX 30
#endif

#ifndef VOLTAGE_TRIM_RGB_MATRIX
#    define VOLTAGE_TRIM_RGB_MATRIX 60
#endif

static uint32_t bat_monitor_timer_buffer = 0;
static uint16_t voltage                  = FULL_VOLTAGE_VALUE;
static uint8_t  bat_empty                = 0;
static uint8_t  critical_low             = 0;
static uint8_t  bat_state;
static uint8_t  power_on_sample = 0;

void battery_init(void) {
    bat_state = BAT_NOT_CHARGING;
#if defined(BAT_CHARGING_PIN)
#    if (BAT_CHARGING_LEVEL == 0)
    palSetLineMode(BAT_CHARGING_PIN, PAL_MODE_INPUT_PULLUP);
#    else
    palSetLineMode(BAT_CHARGING_PIN, PAL_MODE_INPUT_PULLDOWN);
#    endif
#endif

#ifdef BAT_ADC_ENABLE_PIN
    palSetLineMode(BAT_ADC_ENABLE_PIN, PAL_MODE_OUTPUT_PUSHPULL);
    gpio_write_pin(BAT_ADC_ENABLE_PIN, 1);
#endif
#ifdef BAT_ADC_PIN
    palSetLineMode(BAT_ADC_PIN, PAL_MODE_INPUT_ANALOG);
#endif
}

void battery_stop(void) {
#if (HAL_USE_ADC)
#    ifdef BAT_ADC_ENABLE_PIN
    gpio_write_pin(BAT_ADC_ENABLE_PIN, 0);
#    endif
#    ifdef BAT_ADC_PIN
    palSetLineMode(BAT_ADC_PIN, PAL_MODE_INPUT_ANALOG);
    analog_stop(BAT_ADC_PIN);
#    endif
#endif
}

__attribute__((weak)) void battery_measure(void) {
#if defined(LK_WIRELESS_ENABLE)
    lkbt51_read_state_reg(0x05, 0x02);
#elif defined(KC_BLUETOOTH_ENABLE)
    ckbt51_read_state_reg(0x05, 0x02);
#endif
}

/* Calculate the voltage */
__attribute__((weak)) void battery_calculate_voltage(bool src_bt, uint16_t value) {
    uint16_t voltage;

#if defined(LK_WIRELESS_ENABLE)
    if (src_bt)
        voltage = ((uint32_t)value) * (LKBT51_RVD_R1 + LKBT51_RVD_R2) / LKBT51_RVD_R2;
    else
        voltage = (uint32_t)value * 3300 / 1024 * (RVD_R1 + RVD_R2) / RVD_R2;
#elif defined(KC_BLUETOOTH_ENABLE)
    voltage = ((uint32_t)value) * 2246 / 1000;
#else
    return;
#endif

#ifdef LED_MATRIX_ENABLE
    if (led_matrix_is_enabled()) {
        /* We assumpt it is linear relationship*/
        uint32_t compensation = (VOLTAGE_TRIM_LED_MATRIX * led_matrix_driver.get_total_duty_ratio());
        voltage += compensation;
    }
#endif
#ifdef RGB_MATRIX_ENABLE
    if (rgb_matrix_is_enabled()) {

        /* We assumpt it is linear relationship*/
        uint32_t compensation = (VOLTAGE_TRIM_LED_MATRIX * rgb_matrix_driver.get_total_duty_ratio());
        voltage += compensation;
    }
#endif

    battery_set_voltage(voltage);
}

void battery_set_voltage(uint16_t value) {
    voltage = value;
}

uint16_t battery_get_voltage(void) {
    return voltage;
}

uint8_t battery_get_percentage(void) {
    if (voltage > FULL_VOLTAGE_VALUE) return 100;

    if (voltage > EMPTY_VOLTAGE_VALUE) {
        return ((uint32_t)voltage - EMPTY_VOLTAGE_VALUE) * 80 / (FULL_VOLTAGE_VALUE - EMPTY_VOLTAGE_VALUE) + 20;
    }

    if (voltage > SHUTDOWN_VOLTAGE_VALUE) {
        return ((uint32_t)voltage - SHUTDOWN_VOLTAGE_VALUE) * 20 / (EMPTY_VOLTAGE_VALUE - SHUTDOWN_VOLTAGE_VALUE);
    } else
        return 0;
}

uint8_t battery_get_charging_state(void) {
#if defined(BAT_CHARGING_PIN)
    if (usb_power_connected()) {
        if (gpio_read_pin(BAT_CHARGING_PIN) == BAT_CHARGING_LEVEL)
            return BAT_CHARGING;
        else
            return BAT_FULL_CHARGED;
    }
#endif
    return BAT_NOT_CHARGING;
}

bool battery_is_empty(void) {
    return bat_empty > BATTERY_EMPTY_COUNT;
}

bool battery_is_critical_low(void) {
    return critical_low > CRITICAL_LOW_COUNT;
}

void battery_check_empty(void) {
    if (voltage < EMPTY_VOLTAGE_VALUE) {
        if (bat_empty <= BATTERY_EMPTY_COUNT) {
            if (++bat_empty > BATTERY_EMPTY_COUNT) {
                indicator_battery_low_enable(true);
                power_on_sample = VOLTAGE_POWER_ON_MEASURE_COUNT;
            }
        }
    }
}

void battery_check_critical_low(void) {
    if (voltage < SHUTDOWN_VOLTAGE_VALUE) {
        if (critical_low <= CRITICAL_LOW_COUNT) {
            if (++critical_low > CRITICAL_LOW_COUNT) wireless_low_battery_shutdown();
        }
    } else if (critical_low <= CRITICAL_LOW_COUNT) {
        critical_low = 0;
    }
}

bool battery_power_on_sample(void) {
    return power_on_sample < VOLTAGE_POWER_ON_MEASURE_COUNT;
}

void battery_timer_reset(void) {
    bat_monitor_timer_buffer = rtc_timer_read_ms();
}

void battery_task(void) {
    uint32_t t = rtc_timer_elapsed_ms(bat_monitor_timer_buffer);
    bool     monitor_voltage = false;

    if ((get_transport() & TRANSPORT_WIRELESS) && (wireless_get_state() == WT_CONNECTED || battery_power_on_sample())) {
        monitor_voltage = true;
#if defined(BAT_CHARGING_PIN)
        if (usb_power_connected() && t > VOLTAGE_MEASURE_INTERVAL) {
            if (gpio_read_pin(BAT_CHARGING_PIN) == BAT_CHARGING_LEVEL)
                lkbt51_update_bat_state(BAT_CHARGING);
            else
                lkbt51_update_bat_state(BAT_FULL_CHARGED);
        }
#endif
    } else if (get_transport() == TRANSPORT_USB && usb_power_connected()) {
        /* Cable mode: still poll the wireless module for cell voltage (read-only).
         * Charging state already comes from usb_power + BAT_CHARGING_PIN; this
         * keeps percentage/mV in sync for host tools over USB raw HID. */
        monitor_voltage = true;
    }

    if (monitor_voltage) {
        if ((battery_power_on_sample()
#if defined(LED_MATRIX_ENABLE) || defined(RGB_MATRIX_ENABLE)
             && !indicator_is_enabled()
#endif
             && t > BACKLIGHT_OFF_VOLTAGE_MEASURE_INTERVAL) ||
            t > VOLTAGE_MEASURE_INTERVAL) {

            battery_check_empty();
            battery_check_critical_low();

            bat_monitor_timer_buffer = rtc_timer_read_ms();
            if (bat_monitor_timer_buffer > RTC_MAX_TIME) {
                bat_monitor_timer_buffer = 0;
                rtc_timer_clear();
            }

            battery_measure();
            if (power_on_sample < VOLTAGE_POWER_ON_MEASURE_COUNT) power_on_sample++;
        }
    }

    if ((bat_empty || critical_low) && usb_power_connected()) {
        bat_empty    = false;
        critical_low = false;
        indicator_battery_low_enable(false);
    }
}
