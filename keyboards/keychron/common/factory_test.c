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

#include "quantum.h"
#include "keychron_common.h"
#include "usb_descriptor.h"
#include "raw_hid.h"
#include "keychron_raw_hid.h"
#include "keychron_task.h"
#include "backlit_indicator.h"
#if defined(LK_WIRELESS_ENABLE) || defined(KC_BLUETOOTH_ENABLE)
#    include "transport.h"
#    include "battery.h"
#    include "lpm.h"
#    include "wireless.h"
#    if defined(LK_WIRELESS_ENABLE)
#        include "lkbt51.h"
#    elif defined(KC_BLUETOOTH_ENABLE)
#        include "ckbt51.h"
#    endif
#endif
#ifdef DYNAMIC_DEBOUNCE_ENABLE
#    include "keychron_debounce.h"
#endif
#ifdef SNAP_CLICK_ENABLE
#    include "snap_click.h"
#endif
#ifdef ANANLOG_MATRIX
#    include "analog_matrix.h"
#endif
#include "config.h"
#include "version.h"
#ifdef STATE_NOTIFY_ENABLE
#    include "state_notify.h"
#endif

#ifndef BL_CYCLE_KEY
#    define BL_CYCLE_KEY KC_RIGHT
#endif

#ifndef BL_TRIG_KEY
#    define BL_TRIG_KEY KC_HOME
#endif

#ifndef P2P4G_CELAR_MASK
#    define P2P4G_CELAR_MASK P2P4G_CLEAR_PAIRING_TYPE_C
#endif

#ifdef ANANLOG_MATRIX
#    ifndef FACTORY_KEY_COL_OFFSET
#        define FACTORY_KEY_COL_OFFSET 0
#    endif

#    ifndef J_KEY_ROW
#        if MATRIX_ROWS == 5
#            define J_KEY_ROW 2
#        elif MATRIX_ROWS == 6
#            define J_KEY_ROW 3
#        else
#            error "J_KEY_ROW is not set"
#        endif
#    endif

#    ifndef J_KEY_COL
#        define J_KEY_COL (FACTORY_KEY_COL_OFFSET + 7)
#    endif

#    ifndef Z_KEY_ROW
#        if MATRIX_ROWS == 5
#            define Z_KEY_ROW 3
#        elif MATRIX_ROWS == 6
#            define Z_KEY_ROW 4
#        else
#            error "J_KEY_ROW is not set"
#        endif
#    endif

#    ifndef Z_KEY_COL
#        define Z_KEY_COL (FACTORY_KEY_COL_OFFSET + 2)
#    endif
#endif
#define KEY_MASK(r, c) (virtual_matrix[r] & (1 << c))

enum {
    BACKLIGHT_TEST_OFF = 0,
    BACKLIGHT_TEST_WHITE,
    BACKLIGHT_TEST_RED,
    BACKLIGHT_TEST_GREEN,
    BACKLIGHT_TEST_BLUE,
    BACKLIGHT_TEST_MAX,
};

enum {
    KEY_PRESS_J              = 0x01 << 1,
    KEY_PRESS_Z              = 0x01 << 2,
    KEY_PRESS_BL_KEY1        = 0x01 << 3,
    KEY_PRESS_BL_KEY2        = 0x01 << 4,
    KEY_PRESS_FACTORY_RESET  = KEY_PRESS_FN | KEY_PRESS_J | KEY_PRESS_Z,
    KEY_PRESS_BACKLIGTH_TEST = KEY_PRESS_FN | KEY_PRESS_BL_KEY1 | KEY_PRESS_BL_KEY2,
};

enum {
    FACTORY_TEST_CMD_BACKLIGHT = 0x01,
    FACTORY_TEST_CMD_OS_SWITCH,
    FACTORY_TEST_CMD_JUMP_TO_BL,
    FACTORY_TEST_CMD_INT_PIN,
    FACTORY_TEST_CMD_GET_TRANSPORT,
    FACTORY_TEST_CMD_CHARGING_ADC,
    FACTORY_TEST_CMD_RADIO_CARRIER,
    FACTORY_TEST_CMD_GET_BUILD_TIME,
    FACTORY_TEST_CMD_GET_DEVICE_ID,
};

enum {
    P2P4G_CLEAR_PAIRING_TYPE_A = 0x01 << 0,
    P2P4G_CLEAR_PAIRING_TYPE_C = 0x01 << 1,
};

enum {
    OS_SWITCH = 0x01,
};

static uint32_t factory_reset_timer = 0;
static uint8_t  factory_reset_state = 0;
static uint8_t  backlight_test_mode = BACKLIGHT_TEST_OFF;

static bool    report_os_sw_state = false;
static uint8_t keys_released      = 0;

extern void eeconfig_reset_custom_rgb(void);

void factory_timer_start(void) {
    factory_reset_timer = timer_read32();
}

void factory_reset(void) {
    clear_keyboard();                   // Avoid key being pressed after NKRO state changed
    clear_keyboard_but_mods_and_keys(); // Clear extra keys and mouse keys
#if !defined(KEYCOMBO_OS_SELECT_ENABLE) && !defined(KEYCOMBO_OS_TOGGLE_ENABLE)
    layer_state_t default_layer_tmp = default_layer_state;
#endif
    backlight_test_mode = BACKLIGHT_TEST_OFF;

    eeconfig_disable();
#ifdef ANANLOG_MATRIX
    analog_matrix_eeconfig_init();
    analog_matrix_clear_advance_keys();
#endif
    if (!eeconfig_is_enabled()) eeconfig_init();

    eeconfig_read_keymap(&keymap_config);
#if !defined(KEYCOMBO_OS_SELECT_ENABLE) && !defined(KEYCOMBO_OS_TOGGLE_ENABLE)
    default_layer_set(default_layer_tmp);
#endif
#ifdef DYNAMIC_DEBOUNCE_ENABLE
    debounce_config_reset();
#endif
#if defined(SNAP_CLICK_ENABLE) && !defined(ANANLOG_MATRIX)
    snap_click_config_reset();
#endif
#ifdef LED_MATRIX_ENABLE
    led_matrix_enable();
    led_matrix_init();
    eeconfig_update_led_matrix_default();
#endif
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_enable();
    rgb_matrix_init();
    eeconfig_update_rgb_matrix_default();
#    if defined(KEYCHRON_RGB_ENABLE) && defined(EECONFIG_SIZE_CUSTOM_RGB)
    eeconfig_reset_custom_rgb();
#    endif
#endif
#ifdef USB_REPORT_INTERVAL_ENABLE
    extern void report_rate_reset(void);
    report_rate_reset();
#endif
#if defined(LK_WIRELESS_ENABLE) || defined(KC_BLUETOOTH_ENABLE)
#    ifdef EECONFIG_SIZE_WIRELESS_CONFIG
    wireless_config_reset();
#    endif
    wait_ms(50);
#    if defined(LK_WIRELESS_ENABLE)
    lkbt51_factory_reset(P2P4G_CELAR_MASK);
#    elif defined(KC_BLUETOOTH_ENABLE)
    ckbt51_factory_reset();
#    endif
#    ifdef KEYCOMBO_CONN_SWITCH_ENABLE
    eeprom_update_transport(get_transport());
#    endif
#endif
#ifdef STATE_NOTIFY_ENABLE
    factory_reset_nofity();
#endif
#ifdef RGB_MATRIX_ENABLE
    RGB color = {.r = 255, .g = 0, .b = 0};
    backlight_indicator_start(250, 250, 3, color);
#endif
#ifdef LED_MATRIX_ENABLE
    backlight_indicator_start(250, 250, 3);
#endif
}

static inline void factory_timer_check(void) {
    if (timer_elapsed32(factory_reset_timer) > 3000) {
        factory_reset_timer = 0;

        if (factory_reset_state == KEY_PRESS_FACTORY_RESET) {
            keys_released = KEY_PRESS_J | KEY_PRESS_Z;
            factory_reset();
        } else if (factory_reset_state == KEY_PRESS_BACKLIGTH_TEST) {
#ifdef LED_MATRIX_ENABLE
            if (!led_matrix_is_enabled()) led_matrix_enable();
#endif
#ifdef RGB_MATRIX_ENABLE
            if (!rgb_matrix_is_enabled()) rgb_matrix_enable();
#endif
            backlight_test_mode = BACKLIGHT_TEST_WHITE;
        }

        factory_reset_state = 0;
    }
}

bool process_record_factory_test(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
#if defined(FN_KEY_1) || defined(FN_KEY_2) || defined(FN_KEY_3)
#    if defined(FN_KEY_1)
        case FN_KEY_1: /* fall through */
#    endif
#    if defined(FN_KEY_2)
        case FN_KEY_2:
#    endif
#    if defined(FN_KEY_3)
        case FN_KEY_3:
#    endif
            if (record->event.pressed) {
                factory_reset_state |= KEY_PRESS_FN;
            } else {
                factory_reset_state &= ~KEY_PRESS_FN;
                factory_reset_timer = 0;
            }
            break;
#endif
        case KC_J:
#if defined(FN_J_KEY)
        case FN_J_KEY:
#endif
            if (record->event.pressed) {
                factory_reset_state |= KEY_PRESS_J;
                if (factory_reset_state == 0x07) factory_timer_start();
                if ((factory_reset_state & KEY_PRESS_FN) && keycode == KC_J) return false;
            } else {
                factory_reset_state &= ~KEY_PRESS_J;
                factory_reset_timer = 0;
                /* Avoid changing backlight effect on key released if FN_Z_KEY is mode*/
                if (keys_released & KEY_PRESS_J) {
                    keys_released &= ~KEY_PRESS_J;
                    if (keycode >= QK_BACKLIGHT_ON && keycode <= RGB_MODE_TWINKLE) return false;
                }
            }
            break;
        case KC_Z:
#if defined(FN_Z_KEY)
        case FN_Z_KEY:
#endif
            if (record->event.pressed) {
                factory_reset_state |= KEY_PRESS_Z;
                if (factory_reset_state == 0x07) factory_timer_start();
                if ((factory_reset_state & KEY_PRESS_FN) && keycode == KC_Z) return false;
            } else {
                factory_reset_state &= ~KEY_PRESS_Z;
                factory_reset_timer = 0;
                /* Avoid changing backlight effect on key released if FN_Z_KEY is mode*/
                if (keys_released & KEY_PRESS_Z) {
                    keys_released &= ~KEY_PRESS_Z;
                    if (keycode >= QK_BACKLIGHT_ON && keycode <= RGB_MODE_TWINKLE) return false;
                }
            }
            break;
#if defined(BL_CYCLE_KEY) || defined(BL_CYCLE_KEY_2)
#    if defined(BL_CYCLE_KEY)
        case BL_CYCLE_KEY:
#    endif
#    if defined(FN_BL_CYCLE_KEY)
        case FN_BL_CYCLE_KEY:
#    endif
            if (record->event.pressed) {
                if (backlight_test_mode) {
                    if (++backlight_test_mode >= BACKLIGHT_TEST_MAX) {
                        backlight_test_mode = BACKLIGHT_TEST_WHITE;
                    }
                } else {
                    factory_reset_state |= KEY_PRESS_BL_KEY1;
                    if (factory_reset_state == 0x19) {
                        factory_timer_start();
                    }
                }
            } else {
                factory_reset_state &= ~KEY_PRESS_BL_KEY1;
                factory_reset_timer = 0;
            }
            break;
#endif
#if defined(BL_TRIG_KEY) || defined(FN_BL_TRIG_KEY)
#    if defined(BL_TRIG_KEY)
        case BL_TRIG_KEY:
#    endif
#    if defined(FN_BL_TRIG_KEY)
        case FN_BL_TRIG_KEY:
#    endif
            if (record->event.pressed) {
                if (backlight_test_mode) {
                    backlight_test_mode = BACKLIGHT_TEST_OFF;
                } else {
                    factory_reset_state |= KEY_PRESS_BL_KEY2;
                    if (factory_reset_state == 0x19) {
                        factory_timer_start();
                    }
                }
            } else {
                factory_reset_state &= ~KEY_PRESS_BL_KEY2;
                factory_reset_timer = 0;
            }
            break;
#endif
    }

    return true;
}

bool factory_test_indicator(void) {
#ifdef RGB_MATRIX_ENABLE
    if (backlight_test_mode) {
        switch (backlight_test_mode) {
            case BACKLIGHT_TEST_WHITE:
                rgb_matrix_set_color_all(255, 255, 255);
                break;
            case BACKLIGHT_TEST_RED:
                rgb_matrix_set_color_all(255, 0, 0);
                break;
            case BACKLIGHT_TEST_GREEN:
                rgb_matrix_set_color_all(0, 255, 0);
                break;
            case BACKLIGHT_TEST_BLUE:
                rgb_matrix_set_color_all(0, 0, 255);
                break;
        }
        return false;
    }
#endif

    return true;
}

#ifdef ANANLOG_MATRIX
void analog_matrix_factory_reset(void) {
    extern matrix_row_t virtual_matrix[MATRIX_ROWS];

    if (factory_reset_state & KEY_PRESS_FN) {
        if ((factory_reset_state & KEY_PRESS_J) == 0 && KEY_MASK(J_KEY_ROW, J_KEY_COL)) {
            factory_reset_state |= KEY_PRESS_J;
        } else if ((factory_reset_state & KEY_PRESS_J) && KEY_MASK(J_KEY_ROW, J_KEY_COL) == 0) {
            factory_reset_state &= ~KEY_PRESS_J;
            factory_reset_timer = 0;
        }

        if ((factory_reset_state & KEY_PRESS_Z) == 0 && KEY_MASK(Z_KEY_ROW, Z_KEY_COL)) {
            factory_reset_state |= KEY_PRESS_Z;
        } else if ((factory_reset_state & KEY_PRESS_Z) && KEY_MASK(Z_KEY_ROW, Z_KEY_COL) == 0) {
            factory_reset_state &= ~KEY_PRESS_Z;
            factory_reset_timer = 0;
        }

        if (factory_reset_state == KEY_PRESS_FACTORY_RESET && factory_reset_timer == 0) factory_timer_start();
    }
}
#endif

void factory_test_task(void) {
    if (factory_reset_timer) factory_timer_check();
#ifdef ANANLOG_MATRIX
    analog_matrix_factory_reset();
#endif
}

void factory_test_send(bool usb, uint8_t *payload, uint8_t length) {
#ifdef RAW_ENABLE
    uint16_t checksum         = 0;
    uint8_t  data[RAW_EPSIZE] = {0};

    uint8_t i = 0;
    data[i++] = 0xAB;

    memcpy(&data[i], payload, length);
    i += length;

    for (uint8_t i = 1; i < RAW_EPSIZE - 3; i++)
        checksum += data[i];
    data[RAW_EPSIZE - 2] = checksum & 0xFF;
    data[RAW_EPSIZE - 1] = (checksum >> 8) & 0xFF;

    kc_raw_hid_send(usb ? RAW_HID_SRC_USB : !RAW_HID_SRC_USB, data, RAW_EPSIZE);
#endif
}

void factory_test_rx(bool usb, uint8_t *data, uint8_t length) {
    if (data[0] == 0xAB) {
        uint16_t checksum = 0;

        for (uint8_t i = 1; i < RAW_EPSIZE - 3; i++) {
            checksum += data[i];
        }
        /* Verify checksum */
        if ((checksum & 0xFF) != data[RAW_EPSIZE - 2] || checksum >> 8 != data[RAW_EPSIZE - 1]) return;

        /* USB connection only command */
        if (!usb && data[1] <= FACTORY_TEST_CMD_RADIO_CARRIER) return;

        uint8_t payload[32];
        uint8_t len = 0;

        switch (data[1]) {
            case FACTORY_TEST_CMD_BACKLIGHT:
                backlight_test_mode = data[2];
                factory_reset_timer = 0;
                break;
            case FACTORY_TEST_CMD_OS_SWITCH:
                report_os_sw_state = data[2];
                if (report_os_sw_state) {
                    // dip_switch_read(true);
                }
                break;
            case FACTORY_TEST_CMD_JUMP_TO_BL:
                // if (memcmp(&data[2], "JumpToBootloader", strlen("JumpToBootloader")) == 0) bootloader_jump();
                break;
#if defined(LK_WIRELESS_ENABLE) || defined(KC_BLUETOOTH_ENABLE)
            case FACTORY_TEST_CMD_INT_PIN:
                switch (data[2]) {
                    /* Enalbe/disable test */
                    case 0xA1:
#    if defined(LK_WIRELESS_ENABLE)
                        lkbt51_int_pin_test(data[3]);
#    elif defined(KC_BLUETOOTH_ENABLE)
                        ckbt51_int_pin_test(data[3]);
#    endif
                        break;
                    /* Set INT state */
                    case 0xA2:
                        kc_printf("pin %d\n\r", data[3]);
#    if defined(LK_WIRELESS_ENABLE)
                        gpio_write_pin(MCU_TO_WIRELESS_INT_PIN, data[3]);
#    elif defined(KC_BLUETOOTH_ENABLE)
                        gpio_write_pin(MCU_TO_WIRELESS_INT_PIN, data[3]);
#    endif
                        break;
#    if defined(KC_BLUETOOTH_ENABLE)
                    /* Report INT state */
                    case 0xA3:
                        payload[len++] = FACTORY_TEST_CMD_INT_PIN;
                        payload[len++] = 0xA3;
                        payload[len++] = gpio_read_pin(WIRELESS_TO_MCU_INT_PIN);
                        factory_test_send(usb, payload, len);
                        break;
#    endif
                }
                break;
            case FACTORY_TEST_CMD_GET_TRANSPORT:
                payload[len++] = FACTORY_TEST_CMD_GET_TRANSPORT;
                payload[len++] = get_transport();
                payload[len++] = gpio_read_pin(USB_POWER_SENSE_PIN);
                factory_test_send(true, payload, len);
                break;
#endif
#ifdef BATTERY_CHARGE_DONE_DETECT_ADC
            case FACTORY_TEST_CMD_CHARGING_ADC:
            case 0xA1:
                battery_charging_monitor(data[3]);
                break;
            case 0xA2:
                payload[len++] = FACTORY_TEST_CMD_CHARGING_ADC;
                payload[len++] = battery_adc_read_charging_pin();
                factory_test_send(true, payload, len);
                break;
#endif
#if defined(LK_WIRELESS_ENABLE) || defined(KC_BLUETOOTH_ENABLE)
            case FACTORY_TEST_CMD_RADIO_CARRIER:
#    if defined(LK_WIRELESS_ENABLE)
                if (data[2] < 79) lkbt51_radio_test(data[2]);
#    elif defined(KC_BLUETOOTH_ENABLE)
                if (data[2] < 79) ckbt51_radio_test(data[2]);
#    endif
                break;

#    ifdef WERELESS_PRESSURE_TEST
            case 0x70:
                switch (data[2]) {
                    /* Enalbe/disable test */
                    case 0xB1:
                        SEND_STRING("abcdefghijklmnopqrstuvwxyz1234567890abcdefghijklmnopqrstuvwxyz1234567890abcdefghijklmnopqrstuvwxyz1234567890\n");
                        break;
                    case 0xB2:
                        payload[len++] = 0x70;
                        payload[len++] = 0xB2;
                        payload[len++] = wireless_get_state();
                        factory_test_send(true, payload, len);
                        break;
                }
                break;
#    endif
#endif
            case FACTORY_TEST_CMD_GET_BUILD_TIME: {
                payload[len++] = FACTORY_TEST_CMD_GET_BUILD_TIME;
                payload[len++] = 'v';
                if ((DEVICE_VER & 0xF000) != 0) itoa((DEVICE_VER >> 12), (char *)&payload[len++], 16);
                itoa((DEVICE_VER >> 8) & 0xF, (char *)&payload[len++], 16);
                payload[len++] = '.';
                itoa((DEVICE_VER >> 4) & 0xF, (char *)&payload[len++], 16);
                payload[len++] = '.';
                itoa(DEVICE_VER & 0xF, (char *)&payload[len++], 16);
                payload[len++] = ' ';
                memcpy(&payload[len], QMK_BUILDDATE, sizeof(QMK_BUILDDATE));
                len += sizeof(QMK_BUILDDATE);
                factory_test_send(usb, payload, len);
            } break;

            case FACTORY_TEST_CMD_GET_DEVICE_ID:
                payload[len++] = FACTORY_TEST_CMD_GET_DEVICE_ID;
                payload[len++] = 12; // UUID length
                memcpy(&payload[len], (const uint8_t *)UID_BASE, 12);

                len += 12;
                factory_test_send(usb, payload, len);
                break;
        }
    }
}

bool dip_switch_update_user(uint8_t index, bool active) {
    if (report_os_sw_state) {
#ifdef INVERT_OS_SWITCH_STATE
        active = !active;
#endif
        uint8_t payload[3] = {FACTORY_TEST_CMD_OS_SWITCH, OS_SWITCH, active};
        factory_test_send(true, payload, 3);
    }

    return true;
}
