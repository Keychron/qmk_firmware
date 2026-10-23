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

#define POWER_ON_LED_DURATION 3000
static uint32_t power_on_indicator_timer;

#if defined(BT_INDICATION_LED_PIN_LIST)
pin_t bt_led_pins[] = BT_INDICATION_LED_PIN_LIST;
#endif

void eeconfig_init_kb(void) {
    default_layer_set(1U << WIN_BASE_LAYER);
    eeconfig_update_default_layer(default_layer_state);
    eeconfig_init_user();
}

void keyboard_post_init_kb(void) {
    keychron_common_init();
    power_on_indicator_timer = timer_read32();
    keyboard_post_init_user();
}

void keychron_task_kb(void) {
    if (power_on_indicator_timer) {
        if (timer_elapsed32(power_on_indicator_timer) > POWER_ON_LED_DURATION) {
            power_on_indicator_timer = 0;

            if (!host_keyboard_led_state().caps_lock) gpio_write_pin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);
            if (!host_keyboard_led_state().num_lock) gpio_write_pin(LED_NUM_LOCK_PIN, !LED_PIN_ON_STATE);
#if defined(LK_WIRELESS_ENABLE)
            gpio_write_pin(BAT_LOW_LED_PIN, !BAT_LOW_LED_PIN_ON_STATE);
#    if defined(BT_INDICATION_LED_PIN_LIST) && defined(P24G_LED_PIN)
            if (get_transport() != TRANSPORT_P2P4) {
                for (uint8_t i = 0; i < sizeof(bt_led_pins) / sizeof(pin_t); i++)
                    gpio_write_pin(bt_led_pins[i], 1);
                gpio_write_pin(P24G_LED_PIN, 1);
            }
            if (get_transport() == TRANSPORT_P2P4) {
                gpio_write_pin(P24G_LED_PIN, 1);
            }
#    endif
#endif
        } else {
            gpio_write_pin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);
            gpio_write_pin(LED_NUM_LOCK_PIN, LED_PIN_ON_STATE);
#if defined(LK_WIRELESS_ENABLE)
            gpio_write_pin(BAT_LOW_LED_PIN, BAT_LOW_LED_PIN_ON_STATE);
#    if defined(BT_INDICATION_LED_PIN_LIST) && defined(P24G_LED_PIN)
            if (get_transport() != TRANSPORT_P2P4) {
                for (uint8_t i = 0; i < sizeof(bt_led_pins) / sizeof(pin_t); i++)
                    gpio_write_pin(bt_led_pins[i], 0);
                gpio_write_pin(P24G_LED_PIN, 0);
            }
            if (get_transport() == TRANSPORT_P2P4) {
                gpio_write_pin(P24G_LED_PIN, 0);
            }
#    endif
#endif
        }
    }
}

#if defined(LK_WIRELESS_ENABLE)
bool lpm_is_kb_idle(void) {
    return power_on_indicator_timer == 0 && !backlight_indicator_is_active();
}
#endif
