/* Copyright 2025 @ Keychron (https://www.keychron.com)
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

#include "keychron.h"

#ifndef POWER_ON_LED_DURATION
#    define POWER_ON_LED_DURATION 3000
#endif

static uint32_t power_on_indicator_timer;

// Override analog matrix init to reduce boot time for Mac boot selector
#ifdef ANANLOG_MATRIX
extern void analog_matrix_eeconfig_init(void);
extern void he_eeprom_driver_init(void);
extern uint16_t calibrate_values[MATRIX_ROWS][MATRIX_COLS][31];
extern uint8_t cur_calib;

#ifndef ANALOG_MATRIX_BOOT_SCANS
#    define ANALOG_MATRIX_BOOT_SCANS 5
#endif

void analog_matrix_init(void) {
    he_eeprom_driver_init();
    analog_matrix_eeconfig_init();
    cur_calib = 0;
    memset(calibrate_values, 0, MATRIX_ROWS * MATRIX_COLS * 31 * sizeof(calibrate_values[0][0][0]));
    
    // Reduced scans for faster boot - compromise between stability and boot speed
    for (uint8_t i = 0; i < ANALOG_MATRIX_BOOT_SCANS; i++)
        matrix_scan();
}
#endif

#ifdef DIP_SWITCH_ENABLE
bool dip_switch_update_kb(uint8_t index, bool active) {
    if (index == 0) {
        default_layer_set(1UL << (active ? 0 : 2));
    }
    dip_switch_update_user(index, active);

    return true;
}
#endif

void keyboard_post_init_kb(void) {
    power_on_indicator_timer = timer_read32();
    keychron_common_init();
    keyboard_post_init_user();
}

void keychron_task_kb(void) {
    if (power_on_indicator_timer) {
        if (timer_elapsed32(power_on_indicator_timer) > POWER_ON_LED_DURATION) {
            power_on_indicator_timer = 0;
#ifdef BAT_LOW_LED_PIN
            gpio_write_pin(BAT_LOW_LED_PIN, !BAT_LOW_LED_PIN_ON_STATE);
#endif
        } else {
#ifdef BAT_LOW_LED_PIN
            gpio_write_pin(BAT_LOW_LED_PIN, BAT_LOW_LED_PIN_ON_STATE);
#endif
        }
    }
}

#ifdef LK_WIRELESS_ENABLE
bool lpm_is_kb_idle(void) {
    return power_on_indicator_timer == 0 && !backlight_indicator_is_active();
}
#endif
