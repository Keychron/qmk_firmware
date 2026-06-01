/* Copyright 2024 ~ 2026 @ Keychron (https://www.keychron.com)
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
    keychron_common_init();
    keyboard_post_init_user();
    power_on_indicator_timer = timer_read32();
}

void keychron_task_kb(void) {
    if (power_on_indicator_timer) {
        if (timer_elapsed32(power_on_indicator_timer) > POWER_ON_LED_DURATION) {
            power_on_indicator_timer = 0;

#ifdef LK_WIRELESS_ENABLE
            gpio_write_pin(BAT_LOW_LED_PIN, !BAT_LOW_LED_PIN_ON_STATE);
#endif
        } else {
#ifdef LK_WIRELESS_ENABLE
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
