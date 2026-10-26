/* Copyright 2023 @ Keychron (https://www.keychron.com)
 * Copyright 2026 José A Miranda Velez <admin@rgbjunkie.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */

#include QMK_KEYBOARD_H
#include "eeconfig_kb.h"
#include "keychron_debounce.h"
#include "raw_hid.h"

extern void debounce_set(uint8_t new_debounce_type, uint8_t time, bool force);

enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_102_ansi(
        KC_ESC,             KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,    KC_VOLD,  KC_VOLU,            KC_DEL,   KC_F13,   KC_F14,   KC_F15,     KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,    KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,  KC_P7,    KC_P8,    KC_P9,      KC_PPLS,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,  KC_P4,    KC_P5,    KC_P6,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,              KC_P1,    KC_P2,    KC_P3,      KC_PENT,
        KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT           ),
    [MAC_FN] = LAYOUT_102_ansi(
        _______,            KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,   KC_F12,             _______,  _______,  _______,  _______,    RGB_TOG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,    _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,    _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,  _______,  _______,  _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,            _______,  _______,  _______,    _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______,  _______,            _______           ),
    [WIN_BASE] = LAYOUT_102_ansi(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,   KC_F12,             KC_DEL,   KC_F13,   KC_F14,   KC_F15,     KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,    KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,  KC_P7,    KC_P8,    KC_P9,      KC_PPLS,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,  KC_P4,    KC_P5,    KC_P6,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,              KC_P1,    KC_P2,    KC_P3,      KC_PENT,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(WIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT           ),
    [WIN_FN] = LAYOUT_102_ansi(
        _______,            KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,    KC_VOLD,  KC_VOLU,            _______,  _______,  _______,  _______,    RGB_TOG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,    _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,    _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,  _______,  _______,  _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,            _______,  _______,  _______,    _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______,  _______,            _______           ),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_FN]   = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_FN]   = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
};
#endif // ENCODER_MAP_ENABLE
// clang-format on

/* Host lighting commands. The report id is stripped before we see the packet,
 * so byte 0 is the command. Replies put the command back in byte 0. The host
 * read includes the report id, so our byte 0 lands in the host's byte 1. */
enum {
    HOST_GET_QMK_VERSION       = 0x21,
    HOST_GET_PROTOCOL_VERSION  = 0x22,
    HOST_GET_UNIQUE_IDENTIFIER = 0x23,
    HOST_STREAM_RGB_DATA       = 0x24,
    HOST_MODE_ENABLE           = 0x25,
    HOST_MODE_DISABLE          = 0x26,
    HOST_GET_TOTAL_LEDS        = 0x27,
    HOST_GET_FIRMWARE_TYPE     = 0x28,
};

static uint8_t host_packet[32];
static bool     host_lighting_active = false;

static void host_reply(uint8_t command, uint8_t b1, uint8_t b2, uint8_t b3) {
    memset(host_packet, 0, sizeof(host_packet));
    host_packet[0] = command;
    host_packet[1] = b1;
    host_packet[2] = b2;
    host_packet[3] = b3;
    raw_hid_send(host_packet, sizeof(host_packet));
}

/* Num Lock is LED 32, Caps Lock is LED 55. While that lock is on, both this
 * packet and the indicator task must write the same white. If the packet
 * writes the effect colour instead, it lands after the indicator and the key
 * flickers. */
static bool host_lock_led_is_lit(uint8_t led) {
#if defined(NUM_LOCK_INDEX)
    if (led == NUM_LOCK_INDEX && host_keyboard_led_state().num_lock) {
        return true;
    }
#endif
#if defined(CAPS_LOCK_INDEX) && !defined(DIM_CAPS_LOCK)
    if (led == CAPS_LOCK_INDEX && host_keyboard_led_state().caps_lock) {
        return true;
    }
#endif
#if defined(SCROLL_LOCK_INDEX)
    if (led == SCROLL_LOCK_INDEX && host_keyboard_led_state().scroll_lock) {
        return true;
    }
#endif
    return false;
}

static void host_led_streaming(uint8_t *data) {
    uint8_t index        = data[1];
    uint8_t numberofleds = data[2];

    /* Stay silent on a bad frame. A reply uses the colour command, and the
     * next handshake can read that leftover report as its answer. */
    if ((uint16_t)index + numberofleds > RGB_MATRIX_LED_COUNT || numberofleds >= 10) {
        return;
    }

    for (uint8_t i = 0; i < numberofleds; i++) {
        uint8_t led    = index + i;
        uint8_t offset = (i * 3) + 3;
        if (host_lock_led_is_lit(led)) {
            rgb_matrix_set_color(led, 255, 255, 255);
        } else {
            rgb_matrix_set_color(led, data[offset], data[offset + 1], data[offset + 2]);
        }
    }
}

bool q5_pro_host_lighting_rx(uint8_t *data, uint8_t length) {
    (void)length;

    switch (data[0]) {
        case HOST_GET_QMK_VERSION:
            host_reply(HOST_GET_QMK_VERSION, 0, 26, 0);
            return true;
        case HOST_GET_PROTOCOL_VERSION:
            host_reply(HOST_GET_PROTOCOL_VERSION, 1, 0, 6);
            return true;
        case HOST_GET_UNIQUE_IDENTIFIER:
            host_reply(HOST_GET_UNIQUE_IDENTIFIER, 0, 0, 0);
            return true;
        case HOST_STREAM_RGB_DATA:
            /* No reply. A reply is just our own frame echoed back, and the host
             * would answer that with another frame. */
            host_led_streaming(data);
            return true;
        case HOST_MODE_ENABLE:
            host_lighting_active = true;
            rgb_matrix_enable_noeeprom();
            rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_HOSTLIGHT);
            return true;
        case HOST_MODE_DISABLE:
            host_lighting_active = false;
            rgb_matrix_reload_from_eeprom();
            return true;
        case HOST_GET_TOTAL_LEDS:
            host_reply(HOST_GET_TOTAL_LEDS, RGB_MATRIX_LED_COUNT, 0, 0);
            return true;
        case HOST_GET_FIRMWARE_TYPE:
            /* 2 is the VIA firmware type the host already accepts. */
            host_reply(HOST_GET_FIRMWARE_TYPE, 2, 0, 0);
            return true;
        default:
            return false;
    }
}

/* Paint the lock keys white on every frame, including while the host is
 * streaming. The packet writes the same white, so the key stays solid. */
void os_state_indicate(void) {
#if defined(NUM_LOCK_INDEX)
    if (host_keyboard_led_state().num_lock) {
        rgb_matrix_set_color(NUM_LOCK_INDEX, 255, 255, 255);
    }
#endif
#if defined(CAPS_LOCK_INDEX)
    if (host_keyboard_led_state().caps_lock) {
#    if defined(DIM_CAPS_LOCK)
        rgb_matrix_set_color(CAPS_LOCK_INDEX, 0, 0, 0);
#    else
        rgb_matrix_set_color(CAPS_LOCK_INDEX, 255, 255, 255);
#    endif
    }
#endif
#if defined(SCROLL_LOCK_INDEX)
    if (host_keyboard_led_state().scroll_lock) {
        rgb_matrix_set_color(SCROLL_LOCK_INDEX, 255, 255, 255);
    }
#endif
}

/* While a host is streaming, keep its effect selected. The lighting keys and
 * the Launcher would otherwise load an onboard effect and the two painters
 * flicker against each other. */
bool rgb_matrix_effect_locked(uint8_t mode) {
    return host_lighting_active && mode != RGB_MATRIX_CUSTOM_HOSTLIGHT;
}

/* The running debounce is stored on the keyboard. A flash does not replace
 * it, so the first boot after this firmware moves the old eager setting
 * over. A later change in the Launcher is left alone. */
void keyboard_post_init_user(void) {
    uint8_t *cfg  = (uint8_t *)EECONFIG_BASE_DYNAMIC_DEBOUNCE;
    uint8_t  type = eeprom_read_byte(cfg);

    if (type == DEBOUNCE_ASYM_EAGER_DEFER_PER_KEY) {
        return;
    }
    if (type != DEBOUNCE_SYM_EAGER_PER_KEY && type < DEBOUNCE_MAX) {
        return;
    }

    debounce_set(DEBOUNCE_ASYM_EAGER_DEFER_PER_KEY, DEBOUNCE, true);
    eeprom_update_byte(cfg, DEBOUNCE_ASYM_EAGER_DEFER_PER_KEY);
    eeprom_update_byte(cfg + 1, DEBOUNCE);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    (void)record;
    if (host_lighting_active && IS_RGB_KEYCODE(keycode)) {
        return false;
    }
    return true;
}

void housekeeping_task_user(void) {
    if (!host_lighting_active) {
        return;
    }
    if (!rgb_matrix_is_enabled() || rgb_matrix_get_mode() != RGB_MATRIX_CUSTOM_HOSTLIGHT) {
        rgb_matrix_enable_noeeprom();
        rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_HOSTLIGHT);
    }
}
