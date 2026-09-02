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
#ifdef LK_WIRELESS_ENABLE
#    include "keychron_raw_hid.h"
#endif

#ifndef POWER_ON_LED_DURATION
#    define POWER_ON_LED_DURATION 3000
#endif

static uint32_t power_on_indicator_timer;

#ifdef LK_WIRELESS_ENABLE
#    ifndef BATTERY_PUSH_INTERVAL
#        define BATTERY_PUSH_INTERVAL 2000
#    endif
static uint32_t battery_push_timer = 0;
#endif

#ifdef DIP_SWITCH_ENABLE
bool dip_switch_update_kb(uint8_t index, bool active) {
    if (index == 0) {
        default_layer_set(1UL << (active ? 0 : 2));
    }
    dip_switch_update_user(index, active);
#    if defined(LED_WIN_PIN) && defined(LED_MAC_PIN)
    if (active == 0) {
        gpio_write_pin(LED_WIN_PIN, !LED_OS_PIN_ON_STATE);
        gpio_write_pin(LED_MAC_PIN, LED_OS_PIN_ON_STATE);
    } else if (active == 1) {
        gpio_write_pin(LED_WIN_PIN, LED_OS_PIN_ON_STATE);
        gpio_write_pin(LED_MAC_PIN, !LED_OS_PIN_ON_STATE);
    }
#    endif
    return true;
}
#endif

void keyboard_post_init_kb(void) {
    setPinOutputPushPull(LED_WIN_PIN);
    setPinOutputPushPull(LED_MAC_PIN);
    gpio_write_pin(LED_WIN_PIN, !LED_OS_PIN_ON_STATE);
    gpio_write_pin(LED_MAC_PIN, !LED_OS_PIN_ON_STATE);

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
            if (!host_keyboard_led_state().caps_lock) {
                gpio_set_pin_output_push_pull(LED_CAPS_LOCK_PIN);
                gpio_write_pin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);
            }
            if (!host_keyboard_led_state().num_lock) {
                gpio_set_pin_output_push_pull(LED_NUM_LOCK_PIN);
                gpio_write_pin(LED_NUM_LOCK_PIN, !LED_PIN_ON_STATE);
            }
#if defined(LED_WIN_PIN) && defined(LED_MAC_PIN)
            if (gpio_read_pin(B14) == 0) {
                gpio_write_pin(LED_MAC_PIN, !LED_OS_PIN_ON_STATE);
            } else if (gpio_read_pin(B14) == 1) {
                gpio_write_pin(LED_WIN_PIN, !LED_OS_PIN_ON_STATE);
            }
#endif

        } else {
#ifdef BAT_LOW_LED_PIN
            gpio_write_pin(BAT_LOW_LED_PIN, BAT_LOW_LED_PIN_ON_STATE);
#endif
            gpio_set_pin_output_push_pull(LED_CAPS_LOCK_PIN);
            gpio_write_pin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);
            gpio_set_pin_output_push_pull(LED_NUM_LOCK_PIN);
            gpio_write_pin(LED_NUM_LOCK_PIN, LED_PIN_ON_STATE);
#if defined(LED_WIN_PIN) && defined(LED_MAC_PIN)
            gpio_write_pin(LED_WIN_PIN, LED_OS_PIN_ON_STATE);
            gpio_write_pin(LED_MAC_PIN, LED_OS_PIN_ON_STATE);
#endif
        }
    }

#ifdef LK_WIRELESS_ENABLE
    /* Push the battery state to the host over the wireless link (PUSH model). */
    if (timer_elapsed32(battery_push_timer) > BATTERY_PUSH_INTERVAL) {
        battery_push_timer = timer_read32();
        if ((get_transport() & TRANSPORT_WIRELESS) && wireless_get_state() == WT_CONNECTED) {
            kc_battery_push();
        }
    }
#endif
}

#ifdef LK_WIRELESS_ENABLE
bool lpm_is_kb_idle(void) {
    return power_on_indicator_timer == 0 && !backlight_indicator_is_active();
}
#endif

void enter_power_mode_kb(pm_t mode) {
    if (wireless_get_state() == WT_SUSPEND || wireless_get_state() == WT_DISCONNECTED) {
        gpio_write_pin(LED_WIN_PIN, !LED_OS_PIN_ON_STATE);
        gpio_write_pin(LED_MAC_PIN, !LED_OS_PIN_ON_STATE);
    }
}

void suspend_power_down_keychron_kb(void) {
    if (get_transport() == TRANSPORT_USB) {
        gpio_write_pin(LED_WIN_PIN, !LED_OS_PIN_ON_STATE);
        gpio_write_pin(LED_MAC_PIN, !LED_OS_PIN_ON_STATE);
    }

    suspend_power_down_user();
}

void suspend_wakeup_init_keychron_kb(void) {
    if (get_transport() == TRANSPORT_USB) {
        dip_switch_read(true);
    }
    suspend_wakeup_init_user();
}
