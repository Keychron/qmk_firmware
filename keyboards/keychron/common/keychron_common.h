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

#pragma once

#include <stdint.h>
#include <assert.h>
#include "keycodes.h"

#ifndef MAC_BASE_LAYER
#define MAC_BASE_LAYER 0
#endif

#ifndef WIN_BASE_LAYER
#define WIN_BASE_LAYER 2
#endif

#ifndef CUSTOM_KEYCODES_ENABLE
// clang-format off
enum {
    KC_LOPTN = QK_KB_0,
    KC_ROPTN,
    KC_LCMMD,
    KC_RCMMD,
    KC_MAC_MISSION_CONTROL,
    KC_MAC_LAUCHPAD,
    KC_WIN_TASK_VIEW,
    KC_WIN_FILE_EXPLORER,
    KC_MAC_SCREEN_SHOT,
    KC_WIN_CORTANA,
#ifdef WIN_LOCK_SCREEN_ENABLE
    KC_WIN_LOCK_SCREEN,
    __KC_WIN_LOCK_SCREEN_NEXT,
#else
    __KC_WIN_LOCK_SCREEN_NEXT = KC_WIN_CORTANA + 1,
#endif
#ifdef MAC_LOCK_SCREEN_ENABLE
    KC_MAC_LOCK_SCREEN = __KC_WIN_LOCK_SCREEN_NEXT,
    __KC_MAC_LOCK_SCREEN_NEXT,
#else
    __KC_MAC_LOCK_SCREEN_NEXT = __KC_WIN_LOCK_SCREEN_NEXT,
#endif
    KC_MAC_SIRI = __KC_MAC_LOCK_SCREEN_NEXT,
#if defined(LK_WIRELESS_ENABLE) || defined(KC_BLUETOOTH_ENABLE)
    BT_HST1,
    BT_HST2,
    BT_HST3,
#if (defined(P24G_MODE_SELECT_PIN) || defined(KEYCOMBO_CONN_SWITCH_ENABLE)) && !defined(KC_BLUETOOTH_ENABLE)
    P2P4G,
    __P2P4G_NEXT,
#else
    __P2P4G_NEXT = BT_HST3 + 1,
#endif
#if defined(KEYCOMBO_CONN_SWITCH_ENABLE)
    TP_USB = __P2P4G_NEXT,
    __TP_USB_NEXT,
#else
    __TP_USB_NEXT = __P2P4G_NEXT,
#endif
    BAT_LVL = __TP_USB_NEXT,
    __BAT_LVL_NEXT = BAT_LVL + 1,
#else
    BT_HST1 = KC_NO,
    BT_HST2 = KC_NO,
    BT_HST3 = KC_NO,
    P2P4G   = KC_NO,
#if defined(KEYCOMBO_CONN_SWITCH_ENABLE)
    TP_USB  = KC_NO,
#endif
    BAT_LVL = KC_NO,
    __BAT_LVL_NEXT = KC_MAC_SIRI + 1,
#endif
#ifdef KEYCOMBO_OS_TOGGLE_ENABLE
    OS_TOGGL = __BAT_LVL_NEXT,
    __OS_TOGGL_NEXT,
#else
    __OS_TOGGL_NEXT = __BAT_LVL_NEXT,
#endif
#ifdef KEYCOMBO_OS_SELECT_ENABLE
    OS_WIN = __OS_TOGGL_NEXT,
    OS_MAC,
    __OS_MAC_NEXT,
#else
    __OS_MAC_NEXT = __BAT_LVL_NEXT,
#endif
#ifdef ANANLOG_MATRIX
    PROF1 = __OS_MAC_NEXT,
    PROF2,
    PROF3,
    __PROF3_NEXT,
#else
    __PROF3_NEXT = __OS_MAC_NEXT,
#endif
#ifdef LED_MATRIX_ENABLE
    BL_SPI = __PROF3_NEXT,
    BL_SPD,
    __BL_SPD_NEXT,
#else
    __BL_SPD_NEXT = __PROF3_NEXT,
#endif
    NEW_SAFE_RANGE = __BL_SPD_NEXT,
};

#define KC_MCTRL KC_MAC_MISSION_CONTROL
#define KC_LNPAD KC_MAC_LAUCHPAD
#define KC_SIRI KC_MAC_SIRI
#define KC_SNAP KC_MAC_SCREEN_SHOT
#define KC_CTANA KC_WIN_CORTANA
#define KC_WLCK KC_WIN_LOCK_SCREEN
#define KC_MLCK KC_MAC_LOCK_SCREEN
#define KC_TASK KC_WIN_TASK_VIEW
#define KC_FILE KC_WIN_FILE_EXPLORER
#else

#include "keycodes_custom.h"
#endif

// clang-format on
static_assert(NEW_SAFE_RANGE <= 0x7E1F /* QK_KB_31 */, "Keycode overflow");

enum {
    KEY_PRESS_FN = 0x01 << 0,
#ifdef ANANLOG_MATRIX
    KEY_PRESS_P          = 0x01 << 1,
    KEY_PRESS_PROF_COMBO = KEY_PRESS_FN | KEY_PRESS_P,
#endif
};

typedef struct PACKED {
    uint8_t len;
    uint8_t keycode[3];
} key_combination_t;

void keychron_common_init(void);
#if defined(LED_MAC_OS_PIN) && defined(LED_WIN_OS_PIN)
void keychron_os_led_trigger(void);
void keychron_os_led_process(void);
#endif
bool process_record_keychron_common(uint16_t keycode, keyrecord_t *record);
void keychron_common_task(void);

#ifdef ENCODER_ENABLE
void encoder_cb_init(void);
#endif
