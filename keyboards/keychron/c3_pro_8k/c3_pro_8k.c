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

void eeconfig_init_kb(void) {
    default_layer_set(1U << WIN_BASE_LAYER);
    eeconfig_update_default_layer(default_layer_state);
    eeconfig_init_user();
}
void keyboard_post_init_kb(void) {
    keychron_common_init();

    gpio_set_pin_output_push_pull(LED_MAC_OS_PIN);
    gpio_set_pin_output_push_pull(LED_WIN_OS_PIN);
    gpio_write_pin(LED_MAC_OS_PIN, !LED_OS_PIN_ON_STATE);
    gpio_write_pin(LED_WIN_OS_PIN, !LED_OS_PIN_ON_STATE);

#ifdef LED_OS_TIMEOUT
    keychron_os_led_trigger();
#endif

    keyboard_post_init_user();
}

void keychron_task_kb(void) {
    keychron_os_led_process();
}

void suspend_power_down_keychron_kb(void) {
    gpio_write_pin(LED_MAC_OS_PIN, !LED_OS_PIN_ON_STATE);
    gpio_write_pin(LED_WIN_OS_PIN, !LED_OS_PIN_ON_STATE);
    suspend_power_down_user();
}


