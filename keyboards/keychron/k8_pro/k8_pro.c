/* Copyright 2021 ~ 2025 @ Keychron (https://www.keychron.com)
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

#ifdef KC_BLUETOOTH_ENABLE
pin_t bt_led_pins[] = BT_INDICATION_LED_PIN_LIST;
#endif

bool dip_switch_update_kb(uint8_t index, bool active) {
    if (index == 0) {
        default_layer_set(1UL << (active ? 2 : 0));
    }
    dip_switch_update_user(index, active);

    return true;
}

void keyboard_post_init_kb(void) {
    keychron_common_init();
    keyboard_post_init_user();
    power_on_indicator_timer = timer_read32();
}

void keychron_task_kb(void) {
    if (power_on_indicator_timer) {
        if (timer_elapsed32(power_on_indicator_timer) > POWER_ON_LED_DURATION) {
            power_on_indicator_timer = 0;

            if (!host_keyboard_led_state().caps_lock) gpio_write_pin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);
#if defined(KC_BLUETOOTH_ENABLE)
            gpio_write_pin(BAT_LOW_LED_PIN, !BAT_LOW_LED_PIN_ON_STATE);
            for (uint8_t i = 0; i < sizeof(bt_led_pins) / sizeof(pin_t); i++)
                gpio_write_pin(bt_led_pins[i], !BT_INDICATION_LED_ON_STATE);
#endif
        } else {
            gpio_write_pin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);
#if defined(KC_BLUETOOTH_ENABLE)
            gpio_write_pin(BAT_LOW_LED_PIN, BAT_LOW_LED_PIN_ON_STATE);
            for (uint8_t i = 0; i < sizeof(bt_led_pins) / sizeof(pin_t); i++)
                gpio_write_pin(bt_led_pins[i], BT_INDICATION_LED_ON_STATE);
#endif
        }
    }
}

#ifdef KC_BLUETOOTH_ENABLE
bool lpm_is_kb_idle(void) {
#if defined(LED_MATRIX_ENABLE) || defined(RGB_MATRIX_ENABLE)
    return power_on_indicator_timer == 0 && !backlight_indicator_is_active();
#else
    return power_on_indicator_timer == 0;
#endif
}
#endif
