/* Copyright 2023 ~ 2025 @ lokher (https://www.keychron.com)
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
#include "wireless.h"
#include "report_buffer.h"
#include "lpm.h"
#include "battery.h"
#include "indicator.h"
#include "transport.h"
#include "rtc_timer.h"
#include "keychron_wireless_common.h"
#include "keychron_task.h"
#include "wireless_config.h"
#include "keychron_raw_hid.h"
#include "raw_hid.h"
#include "eeprom.h"

extern uint8_t         pairing_indication;
extern host_driver_t   chibios_driver;
extern report_buffer_t kb_rpt;
extern uint32_t        retry_time_buffer;
extern uint8_t         retry;

static uint8_t host_index = 0;
static uint8_t led_state  = 0;

extern wt_func_t  wireless_transport;
static wt_state_t wireless_state           = WT_RESET;
static bool       pincodeEntry             = false;
uint8_t           wireless_report_protocol = true;

uint16_t backlit_disable_time = CONNECTED_BACKLIGHT_DISABLE_TIMEOUT;
uint16_t connected_idle_time  = CONNECTED_IDLE_TIME;

/* declarations */
uint8_t wreless_keyboard_leds(void);
void    wireless_send_keyboard(report_keyboard_t *report);
void    wireless_send_nkro(report_nkro_t *report);
void    wireless_send_mouse(report_mouse_t *report);
void    wireless_send_extra(report_extra_t *report);
void    wireless_send_joystick(report_joystick_t *report);
void    wireless_send_xinput(report_xinput_t *report);
void    wireless_send_raw_hid(uint8_t *data, uint8_t len);
bool    process_record_wireless(uint16_t keycode, keyrecord_t *record);

/* host struct */
host_driver_t wireless_driver = {
    wreless_keyboard_leds,  wireless_send_keyboard, wireless_send_nkro, wireless_send_mouse, wireless_send_extra,
#ifdef JOYSTICK_ENABLE
    wireless_send_joystick,
#endif
#ifdef XINPUT_ENABLE
    wireless_send_xinput,
#endif
#ifdef RAW_ENABLE
    wireless_send_raw_hid,
#endif
};

#define WT_EVENT_QUEUE_SIZE 16
wireless_event_t wireless_event_queue[WT_EVENT_QUEUE_SIZE];
uint8_t          wireless_event_queue_head;
uint8_t          wireless_event_queue_tail;

bool wireless_lpm_set(uint8_t *data);

void wireless_event_queue_init(void) {
    // Initialise the event queue
    memset(&wireless_event_queue, 0, sizeof(wireless_event_queue));
    wireless_event_queue_head = 0;
    wireless_event_queue_tail = 0;
}

bool wireless_event_enqueue(wireless_event_t event) {
    uint8_t next = (wireless_event_queue_head + 1) % WT_EVENT_QUEUE_SIZE;
    if (next == wireless_event_queue_tail) {
        /* Override the first report */
        wireless_event_queue_tail = (wireless_event_queue_tail + 1) % WT_EVENT_QUEUE_SIZE;
    }
    wireless_event_queue[wireless_event_queue_head] = event;
    wireless_event_queue_head                       = next;
    return true;
}

static inline bool wireless_event_dequeue(wireless_event_t *event) {
    if (wireless_event_queue_head == wireless_event_queue_tail) {
        return false;
    }
    *event                    = wireless_event_queue[wireless_event_queue_tail];
    wireless_event_queue_tail = (wireless_event_queue_tail + 1) % WT_EVENT_QUEUE_SIZE;
    return true;
}

#if defined(EECONFIG_BASE_WIRELESS_CONFIG)
void wireless_config_reset(void) {
    uint8_t data[4] = {0};

    uint16_t backlit_disable_time = CONNECTED_BACKLIGHT_DISABLE_TIMEOUT;
    uint16_t connected_idle_time  = CONNECTED_IDLE_TIME;

    memcpy(&data[0], &backlit_disable_time, sizeof(backlit_disable_time));
    memcpy(&data[2], &connected_idle_time, sizeof(connected_idle_time));
    wireless_lpm_set(data);
}

void wireless_config_load(void) {
    uint8_t offset = 0;
    eeprom_read_block(&backlit_disable_time, (uint8_t *)(EECONFIG_BASE_WIRELESS_CONFIG + offset), sizeof(backlit_disable_time));
    offset += sizeof(backlit_disable_time);
    eeprom_read_block(&connected_idle_time, (uint8_t *)(EECONFIG_BASE_WIRELESS_CONFIG + offset), sizeof(connected_idle_time));

    if (backlit_disable_time == 0)
        backlit_disable_time = CONNECTED_BACKLIGHT_DISABLE_TIMEOUT;
    else if (backlit_disable_time < 5)
        backlit_disable_time = 5;

    if (connected_idle_time == 0)
        connected_idle_time = CONNECTED_IDLE_TIME;
    else if (connected_idle_time < 30)
        connected_idle_time = 30;
}

void wireless_config_save(void) {
    uint8_t offset = 0;
    eeprom_update_block(&backlit_disable_time, (uint8_t *)(EECONFIG_BASE_WIRELESS_CONFIG + offset), sizeof(backlit_disable_time));
    offset += sizeof(backlit_disable_time);
    eeprom_update_block(&connected_idle_time, (uint8_t *)(EECONFIG_BASE_WIRELESS_CONFIG + offset), sizeof(connected_idle_time));
}
#endif

/*
 * Bluetooth init.
 */
void wireless_init(void) {
    wireless_state = WT_INITIALIZED;

    wireless_event_queue_init();
#ifndef DISABLE_REPORT_BUFFER
    report_buffer_init();
#endif
    indicator_init();
#ifdef BLUETOOTH_INT_INPUT_PIN
    gpio_set_pin_input_high(BLUETOOTH_INT_INPUT_PIN);
#endif

    battery_init();
    lpm_init();
#if HAL_USE_RTC
    rtc_timer_init();
#endif

#if defined(EECONFIG_BASE_WIRELESS_CONFIG)
    wireless_config_load();
#endif
}

/*
 * Bluetooth trasponrt init. Bluetooth module driver shall use this function to register a callback
 * to its implementation.
 */
void wireless_set_transport(wt_func_t *transport) {
    if (transport) memcpy(&wireless_transport, transport, sizeof(wt_func_t));
}

/*
 * Enter pairing with current host index
 */
void wireless_pairing(void) {
    if (battery_is_critical_low()) return;

    wireless_pairing_ex(0, NULL);
    wireless_state = WT_PARING;
}

/*
 * Enter pairing with specified host index and param
 */
void wireless_pairing_ex(uint8_t host_idx, void *param) {
    kc_printf("wireless_pairing_ex %d\n\r", host_idx);
    if (battery_is_critical_low()) return;
#if defined(KC_BLUETOOTH_ENABLE)
    ckbt51_param_init();
#endif
    if (wireless_transport.pairing_ex) wireless_transport.pairing_ex(host_idx, param);
    wireless_state = WT_PARING;

    host_index = host_idx;
}

/*
 * Initiate connection request to paired host
 */
void wireless_connect(void) {
    /*  Work around empty report after wakeup, which leads to reconneect/disconnected loop */
    if (battery_is_critical_low() || timer_read32() == 0) return;

    if (wireless_state == WT_RECONNECTING && !indicator_is_running()) {
        indicator_set(wireless_state, host_index);
    }
    wireless_transport.connect_ex(0, 0);
    wireless_state = WT_RECONNECTING;
}

/*
 * Initiate connection request to paired host with argument
 */
void wireless_connect_ex(uint8_t host_idx, uint16_t timeout) {
    kc_printf("wireless_connect_ex %d\n\r", host_idx);
    if (battery_is_critical_low()) return;

    if (host_idx != 0) {
        /* Do nothing when trying to connect to current connected host*/
        if (host_index == host_idx && wireless_state == WT_CONNECTED) return;

        host_index = host_idx;
        led_state  = 0;
    }
    wireless_transport.connect_ex(host_idx, timeout);
    wireless_state = WT_RECONNECTING;
}

/* Initiate a disconnection */
void wireless_disconnect(void) {
    kc_printf("wireless_disconnect\n\r");
    if (wireless_transport.disconnect) wireless_transport.disconnect();
}

/* Called when the BT device is reset. */
static void wireless_enter_reset(uint8_t reason) {
    kc_printf("wireless_enter_reset\n\r");
    wireless_state = WT_RESET;
    wireless_enter_reset_kb(reason);
}

/* Enters discoverable state. Upon entering this state we perform the following actions:
 *   - change state to WT_PARING
 *   - set pairing indication
 */
static void wireless_enter_discoverable(uint8_t host_idx) {
    kc_printf("wireless_enter_discoverable: %d\n\r", host_idx);
    host_index = host_idx;

    wireless_state = WT_PARING;
    indicator_set(wireless_state, host_idx);
    wireless_enter_discoverable_kb(host_idx);
}

/*
 * Enters reconnecting state. Upon entering this state we perform the following actions:
 *   - change state to RECONNECTING
 *   - set reconnect indication
 */
static void wireless_enter_reconnecting(uint8_t host_idx) {
    host_index = host_idx;

    kc_printf("wireless_reconnecting %d\n\r", host_idx);
    wireless_state = WT_RECONNECTING;
    indicator_set(wireless_state, host_idx);
    wireless_enter_reconnecting_kb(host_idx);
}

/* Enters connected state. Upon entering this state we perform the following actions:
 *   - change state to CONNECTED
 *   - set connected indication
 *   - enable NKRO if it is support
 */
static void wireless_enter_connected(uint8_t host_idx) {
    kc_printf("wireless_connected %d\n\r", host_idx);

    wireless_state = WT_CONNECTED;
    indicator_set(wireless_state, host_idx);
    host_index = host_idx;

    clear_keyboard();

    /* Enable NKRO since it may be disabled in pin code entry */
#if defined(NKRO_ENABLE) && !defined(WIRELESS_NKRO_ENABLE)
    keymap_config.nkro = false;
#endif

    wireless_enter_connected_kb(host_idx);
    if (battery_is_empty()) {
        indicator_battery_low_enable(true);
    }
    if (wireless_transport.update_bat_level) wireless_transport.update_bat_level(battery_get_percentage());
    lpm_timer_reset();
}

/* Enters disconnected state. Upon entering this state we perform the following actions:
 *   - change state to DISCONNECTED
 *   - set disconnected indication
 */
static void wireless_enter_disconnected(uint8_t host_idx, uint8_t reason) {
    kc_printf("wireless_disconnected %d, %d\n\r", host_idx, reason);

    uint8_t previous_state = wireless_state;
    led_state              = 0;
    if (get_transport() & TRANSPORT_WIRELESS) led_update_kb((led_t)led_state);

    wireless_state = WT_DISCONNECTED;

    if (previous_state == WT_CONNECTED) {
        lpm_timer_reset();
        indicator_set(WT_SUSPEND, host_idx);
    } else {
        indicator_set(wireless_state, host_idx);
    }

#if defined(RGB_MATRIX_ENABLE) || defined(LED_MATRIX_ENABLE)
    if (reason && (get_transport() & TRANSPORT_WIRELESS)) {
        indicator_set_backlit_timeout(DISCONNECTED_BACKLIGHT_DISABLE_TIMEOUT * 1000);
    }
#endif
#ifndef DISABLE_REPORT_BUFFER
    report_buffer_init();
#endif
    retry = 0;
    wireless_enter_disconnected_kb(host_idx, reason);

    indicator_battery_low_enable(false);
    battery_timer_reset();
}

/* Enter pin code entry state. */
static void wireless_enter_bluetooth_pin_code_entry(void) {
#if defined(NKRO_ENABLE)
    keymap_config.nkro = FALSE;
#endif
    pincodeEntry = true;
    wireless_enter_bluetooth_pin_code_entry_kb();
}

/* Exit pin code entry state. */
static void wireless_exit_bluetooth_pin_code_entry(void) {
#if defined(NKRO_ENABLE) || defined(WIRELESS_NKRO_ENABLE)
    eeconfig_read_keymap(&keymap_config);
#endif
    pincodeEntry = false;
    wireless_exit_bluetooth_pin_code_entry_kb();
}

bool is_wireless_pin_code_entry(void) {
    return pincodeEntry;
}

/* Enters disconnected state. Upon entering this state we perform the following actions:
 *   - change state to DISCONNECTED
 *   - set disconnected indication
 */
static void wireless_enter_sleep(void) {
    kc_printf("wireless_enter_sleep %d\n\r", wireless_state);
    uint8_t prev_state = wireless_state;
    led_state          = 0;
    if (get_transport() & TRANSPORT_WIRELESS) led_update_kb((led_t)led_state);

    wireless_state = WT_SUSPEND;
    if (prev_state == WT_CONNECTED || prev_state == WT_PARING) {
        kc_printf("WT_SUSPEND\n\r");
        lpm_timer_reset();

        wireless_enter_sleep_kb();
        indicator_set(wireless_state, 0);

        indicator_battery_low_enable(false);
    }
}

__attribute__((weak)) void wireless_enter_reset_kb(uint8_t reason) {}
__attribute__((weak)) void wireless_enter_discoverable_kb(uint8_t host_idx) {}
__attribute__((weak)) void wireless_enter_reconnecting_kb(uint8_t host_idx) {}
__attribute__((weak)) void wireless_enter_connected_kb(uint8_t host_idx) {}
__attribute__((weak)) void wireless_enter_disconnected_kb(uint8_t host_idx, uint8_t reason) {}
__attribute__((weak)) void wireless_enter_bluetooth_pin_code_entry_kb(void) {}
__attribute__((weak)) void wireless_exit_bluetooth_pin_code_entry_kb(void) {}
__attribute__((weak)) void wireless_enter_sleep_kb(void) {}

/*  */
static void wireless_hid_set_protocol(bool report_protocol) {
    wireless_report_protocol = false;
}

uint8_t wreless_keyboard_leds(void) {
    if (wireless_state == WT_CONNECTED) {
        return led_state;
    }

    return 0;
}

extern keymap_config_t keymap_config;

void wireless_send_keyboard(report_keyboard_t *report) {
    if (battery_is_critical_low()) return;
    if (wireless_state == WT_PARING && !pincodeEntry) return;
    if (wireless_state == WT_CONNECTED || (wireless_state == WT_PARING && pincodeEntry)) {
        if (wireless_transport.send_keyboard) {
#ifndef DISABLE_REPORT_BUFFER
            bool empty = report_buffer_is_empty();

            report_buffer_t report_buffer;
            report_buffer.type = REPORT_TYPE_KB;
            memcpy(&report_buffer.keyboard, report, sizeof(report_keyboard_t));
            report_buffer_enqueue(&report_buffer);

            if (empty) report_buffer_task();
#else
            wireless_transport.send_keyboard(&report->mods);
#endif
        }
    } else if (wireless_state != WT_RESET) {
        wireless_connect();
    }
}

void wireless_send_nkro(report_nkro_t *report) {
    if (battery_is_critical_low()) return;

    if (wireless_state == WT_PARING && !pincodeEntry) return;

    if (wireless_state == WT_CONNECTED || (wireless_state == WT_PARING && pincodeEntry)) {
        if (wireless_transport.send_nkro) {
#ifndef DISABLE_REPORT_BUFFER
            bool empty = report_buffer_is_empty();

            report_buffer_t report_buffer;
            report_buffer.type = REPORT_TYPE_NKRO;
            memcpy(&report_buffer.nkro, report, sizeof(report_nkro_t));
            report_buffer_enqueue(&report_buffer);

            if (empty) report_buffer_task();
#else
            wireless_transport.send_nkro(&report->mods);
#endif
        }
    } else if (wireless_state != WT_RESET) {
        wireless_connect();
    }
}

void wireless_send_mouse(report_mouse_t *report) {
    if (battery_is_critical_low()) return;

    if (wireless_state == WT_CONNECTED) {
        if (wireless_transport.send_mouse) wireless_transport.send_mouse((uint8_t *)report);
    } else if (wireless_state != WT_RESET) {
        wireless_connect();
    }
}

void wireless_send_system(uint16_t data) {
    if (wireless_state == WT_CONNECTED) {
        if (wireless_transport.send_system) wireless_transport.send_system(data);
    } else if (wireless_state != WT_RESET) {
        wireless_connect();
    }
}

void wireless_send_consumer(uint16_t data) {
    if (wireless_state == WT_CONNECTED) {
#ifndef DISABLE_REPORT_BUFFER
        if (report_buffer_is_empty() && report_buffer_next_inverval()) {
            if (wireless_transport.send_consumer) wireless_transport.send_consumer(data);
            report_buffer_update_timer();
        } else {
            report_buffer_t report_buffer;
            report_buffer.type     = REPORT_TYPE_CONSUMER;
            report_buffer.consumer = data;
            report_buffer_enqueue(&report_buffer);
        }
#else
        if (wireless_transport.send_consumer) wireless_transport.send_consumer(data);
#endif
    } else if (wireless_state != WT_RESET) {
        wireless_connect();
    }
}

void wireless_send_extra(report_extra_t *report) {
    if (battery_is_critical_low()) return;

    if (report->report_id == REPORT_ID_SYSTEM) {
        wireless_send_system(report->usage);
    } else if (report->report_id == REPORT_ID_CONSUMER) {
        wireless_send_consumer(report->usage);
    }
}

void wireless_send_joystick(report_joystick_t *report) {
#ifdef JOYSTICK_ENABLE
    if (battery_is_critical_low()) return;

    if (wireless_state == WT_CONNECTED) {
        if (wireless_transport.send_joystick) {
            wireless_transport.send_joystick((uint8_t *)(report->axes));
        }
    } else if (wireless_state != WT_RESET) {
        wireless_connect();
    }
#endif
}

void wireless_send_xinput(report_xinput_t *report) {
#ifdef XINPUT_ENABLE
    if (battery_is_critical_low()) return;

    if (wireless_state == WT_CONNECTED) {
        if (wireless_transport.send_xinput) {
            wireless_transport.send_xinput((uint8_t *)report);
        }
    } else if (wireless_state != WT_RESET) {
        wireless_connect();
    }
#endif
}

void wireless_send_raw_hid(uint8_t *data, uint8_t len) {
#ifdef RAW_ENABLE
    if (battery_is_critical_low()) return;

    if (wireless_state == WT_CONNECTED) {
        if (wireless_transport.send_raw_hid) {
            wireless_transport.send_raw_hid(data, len);
        }
    } else if (wireless_state != WT_RESET) {
        wireless_connect();
    }
#endif
}

void wireless_low_battery_shutdown(void) {
    indicator_battery_low_enable(false);

    report_buffer_init();
    clear_keyboard(); //
    wait_ms(50);      // wait a while for bt module to free buffer by sending report

    // Release all keys by sending empty reports
    if (keymap_config.nkro) {
        report_nkro_t empty_nkro_report;
        memset(&empty_nkro_report, 0, sizeof(empty_nkro_report));
        wireless_transport.send_nkro(&empty_nkro_report.mods);
    } else {
        report_keyboard_t empty_report;
        memset(&empty_report, 0, sizeof(empty_report));
        wireless_transport.send_keyboard(&empty_report.mods);
    }
    wait_ms(10);
    wireless_transport.send_consumer(0);
    wait_ms(10);
    report_mouse_t empty_mouse_report;
    memset(&empty_mouse_report, 0, sizeof(empty_mouse_report));
    wireless_transport.send_mouse((uint8_t *)&empty_mouse_report);
    wait_ms(300); // Wait for bt module to send all buffered report

    wireless_disconnect();
}

void wireless_event_task(void) {
    wireless_event_t event;
    while (wireless_event_dequeue(&event)) {
        switch (event.evt_type) {
            case EVT_RESET:
                wireless_enter_reset(event.params.reason);
                break;
            case EVT_CONNECTED:
                wireless_enter_connected(event.params.hostIndex);
                break;
            case EVT_DISCOVERABLE:
                wireless_enter_discoverable(event.params.hostIndex);
                break;
            case EVT_RECONNECTING:
                wireless_enter_reconnecting(event.params.hostIndex);
                break;
            case EVT_DISCONNECTED:
                wireless_enter_disconnected(event.params.hostIndex, event.data);
                break;
            case EVT_BT_PINCODE_ENTRY:
                wireless_enter_bluetooth_pin_code_entry();
                break;
            case EVT_EXIT_BT_PINCODE_ENTRY:
                wireless_exit_bluetooth_pin_code_entry();
                break;
            case EVT_SLEEP:
                wireless_enter_sleep();
                break;
            case EVT_HID_INDICATOR:
                led_state = event.params.led;
                break;
            case EVT_HID_SET_PROTOCOL:
                wireless_hid_set_protocol(event.params.protocol);
                break;
            case EVT_CONECTION_INTERVAL:
                report_buffer_set_inverval(event.params.interval);
                break;
#if defined(RAW_ENABLE) && defined(WIRELESS_RAW_ENABLE)
            case EVT_RAW_HID:
                raw_hid_receive(RAW_HID_SRC_WIRELESS, event.params.raw_hid_data, 32);
                lpm_timer_reset();
                break;
#endif
            default:
                break;
        }
    }
}

void wireless_task(void) {
    wireless_transport.task();
    wireless_event_task();
#ifndef DISABLE_REPORT_BUFFER
    report_buffer_task();
#endif
    indicator_task();
    keychron_wireless_common_task();
    battery_task();
    lpm_task();
}

void send_string_task(void) {
    if ((get_transport() & TRANSPORT_WIRELESS) && wireless_get_state() == WT_CONNECTED) {
        wireless_transport.task();
#ifndef DISABLE_REPORT_BUFFER
        report_buffer_task();
#endif
    }
}
wt_state_t wireless_get_state(void) {
    return wireless_state;
};

bool process_record_wireless(uint16_t keycode, keyrecord_t *record) {
    if (get_transport() & TRANSPORT_WIRELESS) {
        lpm_timer_reset();

        if (battery_is_empty() && wireless_get_state() == WT_CONNECTED && record->event.pressed) {
            indicator_battery_low_enable(true);
        }
    }

    if (!process_record_keychron_wireless(keycode, record)) return false;

    return true;
}

#if defined(EECONFIG_BASE_WIRELESS_CONFIG)
bool wireless_lpm_get(uint8_t *data) {
    uint8_t index = 1;
    memcpy(&data[index], &backlit_disable_time, sizeof(backlit_disable_time));
    index += sizeof(backlit_disable_time);
    memcpy(&data[index], &connected_idle_time, sizeof(connected_idle_time));

    return true;
}

bool wireless_lpm_set(uint8_t *data) {
    uint8_t index = 0;

    memcpy(&backlit_disable_time, &data[index], sizeof(backlit_disable_time));
    index += sizeof(backlit_disable_time);
    memcpy(&connected_idle_time, &data[index], sizeof(connected_idle_time));

    if (backlit_disable_time < 5 || connected_idle_time < 60) {
        wireless_config_load();
        return false;
    }

    wireless_config_save();

    // Reset backlight timeout
    if ((get_transport() & TRANSPORT_WIRELESS) && wireless_state == WT_CONNECTED) {
        indicator_set_backlit_timeout(backlit_disable_time * 1000);
        indicator_reset_backlit_time();

#    ifdef MOUSEKEY_ENABLE
        // Wiggle mouse to reset bluetooth module timer
        mousekey_on(MS_LEFT);
        mousekey_send();
        wait_ms(10);
        mousekey_on(MS_RGHT);
        mousekey_send();
        wait_ms(10);
        mousekey_off(MS_RGHT);
        mousekey_send();
        wait_ms(10);
#    else
        set_mods(0x02);
        send_keyboard_report();
        wait_ms(10);
        del_mods(0x02);
        send_keyboard_report();
#    endif
    }

    // Update bluetooth module param
#    if defined(LK_WIRELESS_ENABLE)
    lkbt51_param_init();
#    elif defined(KC_BLUETOOTH_ENABLE)
    ckbt51_param_init();
#    endif
    return true;
}

void wireless_raw_hid_rx(uint8_t *data, uint8_t length) {
    uint8_t cmd     = data[1];
    bool    success = true;

    switch (cmd) {
        case WIRELESS_LPM_GET:
            success = wireless_lpm_get(&data[2]);
            break;

        case WIRELESS_LPM_SET:
            success = wireless_lpm_set(&data[2]);
            break;

        default:
            data[0] = 0xFF;
            break;
    }

    data[2] = success ? 0 : 1;
}
#endif
