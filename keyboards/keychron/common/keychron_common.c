/* Copyright 2023 ~ 2025 @ Keychron (https://www.keychron.com)
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

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "keychron_raw_hid.h"
#include "backlit_indicator.h"
#ifdef FACTORY_TEST_ENABLE
#    include "factory_test.h"
#endif
#ifdef RETAIL_DEMO_ENABLE
#    include "retail_demo.h"
#endif
#if defined(LK_WIRELESS_ENABLE) || defined(KC_BLUETOOTH_ENABLE)
#    include "keychron_wireless_common.h"
#endif
#ifdef LED_MATRIX_ENABLE
#    include "led_matrix.h"
#endif
#include "config.h"
#include "usb_descriptor.h"
#include "raw_hid.h"
#ifdef ANANLOG_MATRIX
#    include "profile.h"
#endif

#ifdef KEYCOMBO_OS_SELECT_ENABLE
#    ifndef MAC_BASE_LAYER
#        error "MAC_BASE_LAYER is not defined"
#    endif
#    ifndef WIN_BASE_LAYER
#        error "WIN_BASE_LAYER is not defined"
#    endif
#endif

#ifndef OS_INDICATION_COLOR_MAC
#    define OS_INDICATION_COLOR_MAC \
        { .r = 255, .g = 0, .b = 0 }
#endif

#ifndef OS_INDICATION_COLOR_WIN
#    define OS_INDICATION_COLOR_WIN \
        { .r = 255, .g = 0, .b = 0 }
#endif

bool     is_siri_active = false;
uint32_t siri_timer     = 0;
#if defined(WIN_LOCK_HOLD_TIME)
static uint32_t winlock_timer = 0;
#endif
#if defined(KEYCOMBO_OS_SELECT_ENABLE)
static uint32_t os_keycombo_timer   = 0;
static uint16_t os_selected_keycode = 0;
#endif
#if defined(KEYCOMBO_OS_TOGGLE_ENABLE) && defined(KEYCOMBO_OS_TOGGLE_HOLD_TIME)
static uint32_t os_toggle_timer = 0;
#endif

static uint8_t mac_keycode[4] = {
    KC_LOPT,
    KC_ROPT,
    KC_LCMD,
    KC_RCMD,
};

// clang-format off
static key_combination_t key_comb_list[] = {
    {2, {KC_LWIN, KC_TAB}},
    {2, {KC_LWIN, KC_E}},
    {3, {KC_LSFT, KC_LCMD, KC_4}},
    {2, {KC_LWIN, KC_C}},
#ifdef WIN_LOCK_SCREEN_ENABLE
    {2, {KC_LWIN, KC_L}},
#endif
#ifdef MAC_LOCK_SCREEN_ENABLE
    {3, {KC_LCTL, KC_LCMD, KC_Q}},
#endif
};
// clang-format on

void keychron_common_init(void) {
#ifdef USB_REPORT_INTERVAL_ENABLE
    extern void report_rate_init(void);
    report_rate_init();
#endif
#if defined(SNAP_CLICK_ENABLE) && !defined(ANANLOG_MATRIX)
    extern void snap_click_init(void);
    snap_click_init();
#endif
#if defined(RGB_MATRIX_ENABLE) && defined(KEYCHRON_RGB_ENABLE)
    extern void eeconfig_init_custom_rgb(void);
    eeconfig_init_custom_rgb();
#endif
#if defined(ENCODER_ENABLE) && (PAL_USE_CALLBACKS == TRUE)
    encoder_cb_init();
#endif
#if defined(LK_WIRELESS_ENABLE) || defined(KC_BLUETOOTH_ENABLE)
    wireless_common_init();
#endif
}

void gui_toggle(void) {
#ifdef WIN_BASE_LAYER
    if (get_highest_layer(default_layer_state) != WIN_BASE_LAYER && !keymap_config.no_gui) {
        return;
    }
#endif
    keymap_config.no_gui = !keymap_config.no_gui;
    eeconfig_update_keymap(&keymap_config);
    led_update_kb(host_keyboard_led_state());
#ifdef GUI_TOGGLE_INDICATION_ENABLE
    RGB color = {.r = 255, .g = 0, .b = 0};
    backlight_indicator_start(250, 250, 3, color);
#endif
}

#ifdef KEYCOMBO_OS_TOGGLE_ENABLE
void os_toggle(void) {
    default_layer_xor(1U << MAC_BASE_LAYER);
    default_layer_xor(1U << WIN_BASE_LAYER);
    eeconfig_update_default_layer(default_layer_state);
#    ifdef KEYCOMBO_OS_TOGGLE_INDICATION_ENABLE
    if (get_highest_layer(default_layer_state) == MAC_BASE_LAYER) {
#        ifdef RGB_MATRIX_ENABLE
        RGB color = OS_INDICATION_COLOR_MAC;
        backlight_indicator_start(250, 250, 3, color);
#        endif
#        ifdef LED_MATRIX_ENABLE
        backlight_indicator_start(250, 250, 3);
#        endif
    } else {
#        ifdef RGB_MATRIX_ENABLE
        RGB color = OS_INDICATION_COLOR_WIN;
        backlight_indicator_start(250, 250, 3, color);
#        endif
#        ifdef LED_MATRIX_ENABLE
        backlight_indicator_start(250, 250, 3);
#        endif
    }
#    endif
}
#endif

bool process_record_keychron_common(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
#ifdef KC_MCTRL
        case KC_MCTRL:
            if (record->event.pressed) {
                register_code(KC_MISSION_CONTROL);
            } else {
                unregister_code(KC_MISSION_CONTROL);
            }
            return false; // Skip all further processing of this key
#endif
#ifdef KC_LNPAD
        case KC_LNPAD:
            if (record->event.pressed) {
                register_code(KC_LAUNCHPAD);
            } else {
                unregister_code(KC_LAUNCHPAD);
            }
            return false; // Skip all further processing of this key
#endif
        case KC_LOPTN:
        case KC_ROPTN:
        case KC_LCMMD:
        case KC_RCMMD:
            if (record->event.pressed) {
                register_code(mac_keycode[keycode - KC_LOPTN]);
            } else {
                unregister_code(mac_keycode[keycode - KC_LOPTN]);
            }
            return false; // Skip all further processing of this key
#ifdef KC_SIRI
        case KC_SIRI:
            if (record->event.pressed) {
                if (!is_siri_active) {
                    is_siri_active = true;
                    register_code(KC_LCMD);
                    register_code(KC_SPACE);
                }
                siri_timer = timer_read32();
            } else {
                // Do something else when release
            }
            return false; // Skip all further processing of this key
#endif
        case KC_TASK:
        case KC_FILE:
#ifdef KC_SNAP
        case KC_SNAP:
#endif
#ifdef KC_CTANA
        case KC_CTANA:
#endif
#ifdef WIN_LOCK_SCREEN_ENABLE
        case KC_WLCK:
#endif
#ifdef MAC_LOCK_SCREEN_ENABLE
        case KC_MLCK:
#endif
            if (record->event.pressed) {
                for (uint8_t i = 0; i < key_comb_list[keycode - KC_TASK].len; i++) {
                    register_code(key_comb_list[keycode - KC_TASK].keycode[i]);
                }
            } else {
                for (uint8_t i = 0; i < key_comb_list[keycode - KC_TASK].len; i++) {
                    unregister_code(key_comb_list[keycode - KC_TASK].keycode[i]);
                }
            }
            return false; // Skip all further processing of this key
#ifdef LED_MATRIX_ENABLE
        case BL_SPI:
            if (record->event.pressed) {
                led_matrix_increase_speed();
            }
            break;
        case BL_SPD:
            if (record->event.pressed) {
                led_matrix_decrease_speed();
            }
            break;
#endif

#if defined(WIN_LOCK_LED_PIN) || defined(WINLOCK_LED_LIST)
        case GU_TOGG:
#    if defined(WIN_LOCK_HOLD_TIME)
            if (record->event.pressed) {
                winlock_timer = timer_read32();
            } else {
                winlock_timer = 0;
            }
#    else
            if (record->event.pressed) gui_toggle();
#    endif
            return false;
#endif

#ifdef KEYCOMBO_OS_TOGGLE_ENABLE
        case OS_TOGGL:
#    if defined(KEYCOMBO_OS_TOGGLE_HOLD_TIME)
            if (record->event.pressed) {
                os_toggle_timer = timer_read32();
            } else {
                os_toggle_timer = 0;
            }
#    else
            if (record->event.pressed) {
                os_toggle();
            }
#    endif
            return false;
#endif

#ifdef KEYCOMBO_OS_SELECT_ENABLE
        case OS_WIN:
        case OS_MAC:
            if (record->event.pressed) {
                if (os_selected_keycode == 0) {
                    os_selected_keycode = keycode;
                    os_keycombo_timer   = timer_read32();
                }
            } else {
                if (os_selected_keycode == keycode) {
                    os_selected_keycode = 0;
                    os_keycombo_timer   = 0;
                }
            }
            break;
#endif
        default:
            return true; // Process all other keycodes normally
    }
    return true;
}

void keychron_common_task(void) {
    if (is_siri_active && timer_elapsed32(siri_timer) > 500) {
        unregister_code(KC_LCMD);
        unregister_code(KC_SPACE);
        is_siri_active = false;
        siri_timer     = 0;
    }

#ifdef ANANLOG_MATRIX
    process_profile_select_combo();
#endif

#if defined(WIN_LOCK_HOLD_TIME)
    if (winlock_timer) {
        if (keymap_config.no_gui) {
            winlock_timer = 0;
            gui_toggle();
        } else if (timer_elapsed32(winlock_timer) > WIN_LOCK_HOLD_TIME) {
            winlock_timer = 0;
            gui_toggle();
        }
    }
#endif
#if defined(KEYCOMBO_OS_SELECT_ENABLE) && defined(MAC_BASE_LAYER) && defined(WIN_BASE_LAYER)
    if (os_keycombo_timer && timer_elapsed32(os_keycombo_timer) > 3000) {
        if (os_selected_keycode == OS_WIN) {
            if (get_highest_layer(default_layer_state) == MAC_BASE_LAYER) {
                default_layer_set(1UL << WIN_BASE_LAYER);
                eeprom_update_byte(EECONFIG_DEFAULT_LAYER, 1U << WIN_BASE_LAYER);
            }
        } else {
            if (get_highest_layer(default_layer_state) == WIN_BASE_LAYER) {
                default_layer_set(1UL << MAC_BASE_LAYER);
                eeprom_update_byte(EECONFIG_DEFAULT_LAYER, 1U << MAC_BASE_LAYER);
            }
        }

        RGB color = {.r = 255, .g = 0, .b = 0};
        backlight_indicator_start(250, 250, 3, color);
        os_keycombo_timer = 0;
    }
#endif
#if defined(KEYCOMBO_OS_TOGGLE_ENABLE) && defined(KEYCOMBO_OS_TOGGLE_HOLD_TIME)
    if (os_toggle_timer && timer_elapsed32(os_toggle_timer) > KEYCOMBO_OS_TOGGLE_HOLD_TIME) {
        os_toggle();
        os_toggle_timer = 0;
    }
#endif
}


#if defined(ENCODER_ENABLE) && (PAL_USE_CALLBACKS == TRUE)
static void encoder_pad_pins(void *param) {
    uint8_t     index = (uint32_t)param;
    extern void encoder_quadrature_handle_inerrupt_read(uint8_t index);
    encoder_quadrature_handle_inerrupt_read(index);
}

void encoder_cb_init(void) {
    pin_t encoders_a_pins[] = ENCODER_A_PINS;
    pin_t encoders_b_pins[] = ENCODER_B_PINS;
    for (uint32_t i = 0; i < NUM_ENCODERS; i++) {
#    ifdef ENCODER_PULL_UP_DISABLE
        palSetLineMode(encoders_a_pins[i], PAL_MODE_INPUT);
        palSetLineMode(encoders_b_pins[i], PAL_MODE_INPUT);
#    endif
        palEnableLineEvent(encoders_a_pins[i], PAL_EVENT_MODE_BOTH_EDGES);
        palEnableLineEvent(encoders_b_pins[i], PAL_EVENT_MODE_BOTH_EDGES);
        palSetLineCallback(encoders_a_pins[i], encoder_pad_pins, (void *)i);
        palSetLineCallback(encoders_b_pins[i], encoder_pad_pins, (void *)i);
    }
}
#endif

#if defined(RGB_MATRIX_ENABLE) || defined(LED_MATRIX_ENABLE)
bool led_update_kb(led_t led_state) {
    bool res = led_update_user(led_state);
    if (res) {
        led_update_ports(led_state);

        if (!LED_DRIVER_IS_ENABLED()) {
#    if defined(LED_MATRIX_ENABLE) && defined(LED_MATRIX_DRIVER_SHUTDOWN_ENABLE)
            led_matrix_driver_exit_shutdown();
#    elif defined(RGB_MATRIX_ENABLE) && defined(RGB_MATRIX_DRIVER_SHUTDOWN_ENABLE)
            rgb_matrix_driver_exit_shutdown();
#    endif
            SET_ALL_LED_OFF();
            os_state_indicate();
            LED_DRIVER.flush();
#    if defined(LED_MATRIX_ENABLE) && defined(LED_MATRIX_DRIVER_SHUTDOWN_ENABLE)
            if (led_matrix_driver_allow_shutdown()) led_matrix_driver_shutdown();
#    elif defined(RGB_MATRIX_ENABLE) && defined(RGB_MATRIX_DRIVER_SHUTDOWN_ENABLE)
            if (rgb_matrix_driver_allow_shutdown()) rgb_matrix_driver_shutdown();
#    endif
        }
    }

    return res;
}
#endif

#if defined(LED_MAC_OS_PIN) && defined(LED_WIN_OS_PIN)

#ifdef LED_OS_TIMEOUT
static layer_state_t prev_default_layer_state = 0;
static uint32_t layer_switch_timer = 0;
static bool layer_switch_timer_active = false;

void keychron_os_led_trigger(void) {
    layer_switch_timer = timer_read32();
    layer_switch_timer_active = true;
}
#endif

void keychron_os_led_process(void) {
    bool is_mac = (get_highest_layer(default_layer_state) == MAC_BASE_LAYER);
    bool led_active = true;

#ifdef LED_OS_TIMEOUT
    if (prev_default_layer_state != default_layer_state) {
        prev_default_layer_state = default_layer_state;
        keychron_os_led_trigger();
    }

    if (layer_switch_timer_active && timer_elapsed32(layer_switch_timer) >= LED_OS_TIMEOUT) {
        layer_switch_timer_active = false;
    }
    led_active = layer_switch_timer_active;
#endif

    gpio_write_pin(LED_MAC_OS_PIN, (led_active && is_mac) ? LED_OS_PIN_ON_STATE : !LED_OS_PIN_ON_STATE);
    gpio_write_pin(LED_WIN_OS_PIN, (led_active && !is_mac) ? LED_OS_PIN_ON_STATE : !LED_OS_PIN_ON_STATE);
}
#endif
