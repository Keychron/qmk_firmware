/* Copyright 2022 ~ 2025 @ lokher (https://www.keychron.com)
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

/******************************************************************************
 *
 *  Filename:      lpm.c
 *
 *  Description:   Contains low power mode implementation
 *
 ******************************************************************************/

#include "quantum.h"
#if defined(PROTOCOL_CHIBIOS)
#    include <usb_main.h>
#endif
#include "debounce.h"
#include "wireless.h"
#include "indicator.h"
#include "lpm.h"
#include "transport.h"
#include "battery.h"
#include "bat_level_animation.h"
#include "report_buffer.h"
#include "keychron_common.h"

extern matrix_row_t matrix[MATRIX_ROWS];
extern wt_func_t    wireless_transport;

static uint32_t lpm_timer_buffer;
static bool     lpm_time_up    = false;
static uint32_t lpm_reset_time = DEFAULT_PROCESS_TIME;
#ifndef OPTICAL_SWITCH
static matrix_row_t empty_matrix[MATRIX_ROWS] = {0};
#endif

pin_t pins_row[MATRIX_ROWS] = MATRIX_ROW_PINS;
pin_t pins_col[MATRIX_COLS] = MATRIX_COL_PINS;

__attribute__((weak)) void select_all_cols(void) {
    for (uint8_t i = 0; i < MATRIX_COLS; i++) {
        if (pins_col[i] == NO_PIN) continue;

        gpio_set_pin_output_push_pull(pins_col[i]);
        gpio_write_pin_low(pins_col[i]);
    }
}

void lpm_init(void) {
#ifdef USB_POWER_SENSE_PIN
#    if (USB_POWER_CONNECTED_LEVEL == 0)
    gpio_set_pin_input_high(USB_POWER_SENSE_PIN);
#    else
    setPinInputLow(USB_POWER_SENSE_PIN);
#    endif
#endif
    lpm_timer_reset();
}

inline void lpm_timer_reset(void) {
    lpm_time_up      = false;
    lpm_timer_buffer = timer_read32();

#ifdef ANANLOG_MATRIX
    if (wireless_get_state() == WT_CONNECTED)
        lpm_reset_time = CONNECTED_PROCESS_TIME;
    else
#endif
        lpm_reset_time = DEFAULT_PROCESS_TIME;
}

void lpm_timer_stop(void) {
    lpm_time_up      = false;
    lpm_timer_buffer = 0;
}

static inline bool lpm_any_matrix_action(void) {
    return memcmp(matrix, empty_matrix, sizeof(empty_matrix));
}

__attribute__((weak)) void matrix_enter_low_power(void) {
    /* Enable key matrix wake up */
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        if (pins_row[x] != NO_PIN) {
            palEnableLineEvent(pins_row[x], PAL_EVENT_MODE_BOTH_EDGES);
        }
    }

    select_all_cols();
}

__attribute__((weak)) void matrix_exit_low_power(void) {
#ifdef ANANLOG_MATRIX
#    ifdef ANALOG_MATRIX_WAKEUP_PIN
    palDisableLineEvent(ANALOG_MATRIX_WAKEUP_PIN);
#    endif
#else
    /* Disable all wake up pins */
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        if (pins_row[x] != NO_PIN) {
            palDisableLineEvent(pins_row[x]);
        }
    }
#endif
}

static void lpm_turn_off_backlight_and_led(void) {
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_color_all(0, 0, 0);
    rgb_matrix_driver.flush();
    rgb_matrix_driver_shutdown();
#endif
#ifdef LED_MATRIX_ENABLE
    led_matrix_set_value_all(0);
    led_matrix_driver.flush();
    led_matrix_driver_shutdown();
#endif
#ifdef LED_NUM_LOCK_PIN
    gpio_write_pin(LED_NUM_LOCK_PIN, !LED_PIN_ON_STATE);
#endif
#ifdef LED_CAPS_LOCK_PIN
    gpio_write_pin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);
#endif
#ifdef BAT_LOW_LED_PIN
    gpio_write_pin(BAT_LOW_LED_PIN, !BAT_LOW_LED_PIN_ON_STATE);
#endif
#ifdef BT_INDICATION_LED_PIN_LIST
    pin_t bt_led_pins[] = BT_INDICATION_LED_PIN_LIST;
    for (uint8_t i = 0; i < sizeof(bt_led_pins) / sizeof(pin_t); i++)
        gpio_write_pin(bt_led_pins[i], !BT_INDICATION_LED_ON_STATE);
#endif
}

/* Implement of entering low power mode and wakeup varies per mcu or platform */

__attribute__((weak)) void lpm_pre_enter_low_power(void) {}

__attribute__((weak)) void lpm_enter_low_power_kb(void) {}

__attribute__((weak)) void lpm_enter_low_power(void) {
    if (get_transport() == TRANSPORT_USB && !usb_power_connected()) {
        lpm_turn_off_backlight_and_led();
    }

#if defined(KEEP_USB_CONNECTION_IN_WIRELESS_MODE)
    /* Usb unit is actived and running, stop and disconnect first */
    usbStop(&USBD1);
    usbDisconnectBus(&USBD1);

#    ifdef PWR_CR2_USV
    // TODO: Use usb_power_disconnect() ????
    /* Isolate USB to save power.*/
    // PWR->CR2 &= ~PWR_CR2_USV; /*PWR_CR2_USV is available on STM32L4x2xx and STM32L4x3xx devices only. */
#    endif
#endif

#if (HAL_USE_SPI == TRUE)
    spiStop(&SPI_DRIVER);
    palSetLineMode(SPI_SCK_PIN, PAL_MODE_INPUT_PULLDOWN);
#    ifdef SPI_MOSI_PIN
    palSetLineMode(SPI_MOSI_PIN, PAL_MODE_INPUT_PULLDOWN);
#    endif
#    ifdef SPI_MISO_PIN
    palSetLineMode(SPI_MISO_PIN, PAL_MODE_INPUT_PULLDOWN);
#    endif
#endif

#if defined(LK_WIRELESS_ENABLE)
    palEnableLineEvent(WIRELESS_TO_MCU_INT_PIN, PAL_EVENT_MODE_FALLING_EDGE);
#endif
#if defined(KC_BLUETOOTH_ENABLE)
    palEnableLineEvent(WIRELESS_TO_MCU_INT_PIN, PAL_EVENT_MODE_FALLING_EDGE);
#endif
#ifdef USB_POWER_SENSE_PIN
    palEnableLineEvent(USB_POWER_SENSE_PIN, PAL_EVENT_MODE_BOTH_EDGES);
#endif
#ifdef P24G_MODE_SELECT_PIN
    palEnableLineEvent(P24G_MODE_SELECT_PIN, PAL_EVENT_MODE_BOTH_EDGES);
#endif
#ifdef BT_MODE_SELECT_PIN
    palEnableLineEvent(BT_MODE_SELECT_PIN, PAL_EVENT_MODE_BOTH_EDGES);
#endif
    matrix_enter_low_power();

#if defined(DIP_SWITCH_PINS)
#    define NUMBER_OF_DIP_SWITCHES (sizeof(dip_switch_pad) / sizeof(pin_t))
    static pin_t dip_switch_pad[] = DIP_SWITCH_PINS;

    for (uint8_t i = 0; i < NUMBER_OF_DIP_SWITCHES; i++) {
        setPinInputLow(dip_switch_pad[i]);
    }
#endif
    battery_stop();
    lpm_enter_low_power_kb();
}

__attribute__((weak)) void lpm_post_enter_low_power(void) {}

__attribute__((weak)) void lpm_standby(pm_t mode) {}

__attribute__((weak)) void lpm_early_wakeup(void) {
    gpio_write_pin_low(MCU_TO_WIRELESS_INT_PIN);
}

__attribute__((weak)) void lpm_wakeup_init(void) {}

__attribute__((weak)) void lpm_pre_wakeup(void) {
    gpio_write_pin_high(MCU_TO_WIRELESS_INT_PIN);
}

__attribute__((weak)) void lpm_wakeup(void) {
#if defined(KC_BLUETOOTH_ENABLE)
    // Invoking halInit() takes more time, which can't be accept when using CKBT51
    chSysLock();
    /* TIMx is disable during stop/standby/sleep mode, init after wakeup */
    stInit();
    timer_init();
    chSysUnlock();
#else
    halInit();
#endif
    if (wireless_transport.init) wireless_transport.init(true);

#if defined(DIP_SWITCH_PINS)
    /* Init dip switch as early as possible, make sure first read later is correct. */
    dip_switch_init();
#endif
#if defined(KC_BLUETOOTH_ENABLE)
    palDisableLineEvent(WIRELESS_TO_MCU_INT_PIN);
#endif
#ifdef ENCODER_ENABLE
    encoder_cb_init();
#endif

    battery_init();

    matrix_exit_low_power();
#ifdef LK_WIRELESS_ENABLE
    palDisableLineEvent(WIRELESS_TO_MCU_INT_PIN);
#endif

#ifdef P24G_MODE_SELECT_PIN
    palDisableLineEvent(P24G_MODE_SELECT_PIN);
#endif
#ifdef BT_MODE_SELECT_PIN
    palDisableLineEvent(BT_MODE_SELECT_PIN);
#endif
#ifdef USB_POWER_SENSE_PIN
    palDisableLineEvent(USB_POWER_SENSE_PIN);

#    if defined(KEEP_USB_CONNECTION_IN_WIRELESS_MODE)
    if (usb_power_connected()
#        ifndef BT_MODE_SELECT_PIN
        && (get_transport() == TRANSPORT_USB)
#        endif
    ) {
        usb_event_queue_init();
        init_usb_driver(&USB_DRIVER);
    }
#    endif
#endif

    /* Call debounce_free() to avoiding memory leak of debounce_counters as debounce_init()
    invoked in matrix_init() alloc new memory to debounce_counters */
    debounce_free();
    matrix_init();

#ifdef ENABLE_RGB_MATRIX_PIXEL_RAIN
    extern void PIXEL_RAIN_init(void);
    PIXEL_RAIN_init();
#endif

#ifdef ENABLE_RGB_MATRIX_PIXEL_FLOW
    extern void PIXEL_FLOW_init(void);
    PIXEL_FLOW_init();
#endif

#ifdef ENABLE_RGB_MATRIX_PIXEL_FRACTAL
    extern void PIXEL_FRACTAL_init(void);
    PIXEL_FRACTAL_init();
#endif

#if defined(DIP_SWITCH_PINS)
    dip_switch_read(true);
#endif
}

__attribute__((weak)) void lpm_post_wakeup(void) {}

__attribute__((weak)) bool usb_power_connected(void) {
#ifdef USB_POWER_SENSE_PIN
    return gpio_read_pin(USB_POWER_SENSE_PIN) == USB_POWER_CONNECTED_LEVEL;
#else
    return true;
#endif
}

__attribute__((weak)) bool lpm_is_kb_idle(void) {
    return true;
}

__attribute__((weak)) bool lpm_set(pm_t mode) {
    return false;
}

__attribute__((weak)) void lpm_peripheral_enter_low_power(void) {}

__attribute__((weak)) void lpm_peripheral_exit_low_power(void) {}

bool allow_low_power_mode(pm_t mode) {
#if defined(KEEP_USB_CONNECTION_IN_WIRELESS_MODE)
    /* Don't enter low power mode if attached to the host */
    if (mode > PM_SLEEP && usb_power_connected()) return false;
#endif

    if (!lpm_set(mode)) return false;

    return true;
}

void lpm_task(void) {
    if (!lpm_time_up && timer_elapsed32(lpm_timer_buffer) > lpm_reset_time) {
        lpm_time_up      = true;
        lpm_timer_buffer = 0;
    }

    if (usb_power_connected()) {
        if (USBD1.state == USB_STOP) {
            /* TODO: Do we need this? */
            usb_event_queue_init();
            init_usb_driver(&USB_DRIVER);
        }
        /* No need to enter low power mode when USB power is connected */
        return;
    }

    switch (get_transport()) {
#ifdef KEYCOMBO_CONN_SWITCH_ENABLE
        case TRANSPORT_USB:
            /* Only allow sleep if no USB connection detected */
            /* TODO: Do we need this? */
            if (USBD1.state != USB_SUSPENDED && USBD1.state != USB_STOP) return;
            break;
#endif
        case TRANSPORT_BLUETOOTH:
        case TRANSPORT_P2P4:
            break;
        default:
            return;
    }

    if (!(lpm_time_up && !indicator_is_running() && lpm_is_kb_idle())) return;

#ifdef LED_MATRIX_ENABLE
    if (led_matrix_is_enabled() && !led_matrix_is_driver_shutdown()) return;
#elif defined(RGB_MATRIX_ENABLE)
    if (rgb_matrix_is_enabled() && !rgb_matrix_is_driver_shutdown()) return;
#else
    if (bat_level_animiation_actived()) return;
#endif

    if (lpm_any_matrix_action() || !allow_low_power_mode(LOW_POWER_MODE)) return;

    lpm_pre_enter_low_power();
    lpm_enter_low_power();
    lpm_post_enter_low_power();

    lpm_standby(LOW_POWER_MODE);
    lpm_early_wakeup();
    lpm_wakeup_init();

    lpm_pre_wakeup();
    lpm_wakeup();
    lpm_post_wakeup();
    lpm_timer_reset();
    report_buffer_init();
    lpm_set(PM_RUN);
}
