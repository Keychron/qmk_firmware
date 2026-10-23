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

#define POWER_ON_LED_DURATION 3000

static uint32_t power_on_indicator_timer = 0;

#ifdef DIP_SWITCH_ENABLE
bool dip_switch_update_kb(uint8_t index, bool active) {
    if (!dip_switch_update_user(index, active)) {
        return false;
    }
    if (index == 0) {
        default_layer_set(1UL << (active ? MAC_BASE_LAYER : WIN_BASE_LAYER));
    }
    return true;
}
#endif

void keyboard_post_init_kb(void) {
    gpio_set_pin_output_push_pull(LED_MAC_OS_PIN);
    gpio_set_pin_output_push_pull(LED_WIN_OS_PIN);
    gpio_write_pin(LED_MAC_OS_PIN, !LED_OS_PIN_ON_STATE);
    gpio_write_pin(LED_WIN_OS_PIN, !LED_OS_PIN_ON_STATE);

    keychron_common_init();
    keyboard_post_init_user();
    power_on_indicator_timer = timer_read32();
}

void keychron_task_kb(void) {
    if (power_on_indicator_timer) {
        if (timer_elapsed32(power_on_indicator_timer) > POWER_ON_LED_DURATION) {
            power_on_indicator_timer = 0;

            if (!host_keyboard_led_state().caps_lock) gpio_write_pin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);
            if (!host_keyboard_led_state().num_lock) gpio_write_pin(LED_NUM_LOCK_PIN, !LED_PIN_ON_STATE);

#ifdef LED_OS_TIMEOUT
            keychron_os_led_trigger();
#endif
        } else {
            gpio_write_pin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);
            gpio_write_pin(LED_NUM_LOCK_PIN, LED_PIN_ON_STATE);
            gpio_write_pin(LED_MAC_OS_PIN, LED_OS_PIN_ON_STATE);
            gpio_write_pin(LED_WIN_OS_PIN, LED_OS_PIN_ON_STATE);
        }
    } else {
        keychron_os_led_process();
    }
}

void suspend_power_down_keychron_kb(void) {
    gpio_write_pin(LED_MAC_OS_PIN, !LED_OS_PIN_ON_STATE);
    gpio_write_pin(LED_WIN_OS_PIN, !LED_OS_PIN_ON_STATE);
    suspend_power_down_user();
}
