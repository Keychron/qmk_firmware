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

#include "keychron.h"
#include "nvm_eeconfig.h"

void keyboard_post_init_kb(void) {
    /* Earlier firmware had no keyboard datablock, so on the first boot after
     * an update this region still holds stale VIA/keymap bytes. Wipe it and
     * load defaults before the common code reads its settings from it. */
    if (!eeconfig_is_kb_datablock_valid()) {
        nvm_eeconfig_init_kb_datablock(); // write version marker, zero the block
        eeconfig_init_kb_datablock();     // Keychron defaults
    }

    keychron_common_init();

    gpio_set_pin_output_push_pull(LED_MAC_OS_PIN);
    gpio_set_pin_output_push_pull(LED_WIN_OS_PIN);
    gpio_write_pin(LED_MAC_OS_PIN, !LED_OS_PIN_ON_STATE);
    gpio_write_pin(LED_WIN_OS_PIN, !LED_OS_PIN_ON_STATE);

    keyboard_post_init_user();
}

void keychron_task_kb(void) {
    if (get_highest_layer(default_layer_state) == MAC_BASE_LAYER) {
        gpio_write_pin(LED_MAC_OS_PIN, LED_OS_PIN_ON_STATE);
        gpio_write_pin(LED_WIN_OS_PIN, !LED_OS_PIN_ON_STATE);
    }
    if (get_highest_layer(default_layer_state) == WIN_BASE_LAYER) {
        gpio_write_pin(LED_MAC_OS_PIN, !LED_OS_PIN_ON_STATE);
        gpio_write_pin(LED_WIN_OS_PIN, LED_OS_PIN_ON_STATE);
    }
}

void suspend_power_down_keychron_kb(void) {
    gpio_write_pin(LED_WIN_OS_PIN, !LED_OS_PIN_ON_STATE);
    gpio_write_pin(LED_MAC_OS_PIN, !LED_OS_PIN_ON_STATE);
    // suspend_power_down_user() is called by suspend_power_down_kb() in
    // the Keychron common code right after this hook.
}
