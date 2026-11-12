/* Copyright 2023~2024 @ lokher (https://www.keychron.com)
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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Type of an enumeration of the possible wireless events.*/
typedef enum {
    EVT_NONE = 0,
    EVT_RESET,
    EVT_DISCOVERABLE,
    EVT_RECONNECTING,
    EVT_CONNECTED,
    EVT_DISCONNECTED,
    EVT_BT_PINCODE_ENTRY,
    EVT_EXIT_BT_PINCODE_ENTRY,
    EVT_SLEEP,
    EVT_HID_SET_PROTOCOL,
    EVT_HID_INDICATOR,
    EVT_CONECTION_INTERVAL,
    EVT_RAW_HID,
} event_type_t;

typedef struct {
    event_type_t evt_type; /*The type of the event. */
    union {
        uint8_t reason;    /* Parameters to WT_RESET event */
        uint8_t hostIndex; /* Parameters to connection event from EVT_DISCOVERABLE to EVT_DISCONECTED */
        uint8_t led;       /* Parameters to EVT_HID_INDICATOR event */
        uint8_t protocol;  /* Parameters to EVT_HID_SET_PROTOCOL event */
        uint8_t interval;  /* Parameters to EVT_CONECTION_INTERVAL event */
#if defined(RAW_ENABLE) && defined(WILRESS_RAW_ENABLE)
        /* 事件拥有数据副本，避免排队期间被下一条无线配置包覆盖。 */
        uint8_t raw_hid_data[32];
#else
        uint8_t *raw_hid_data;
#endif
    } params;
    uint8_t data;
} wireless_event_t;

#if defined(RAW_ENABLE) && defined(WILRESS_RAW_ENABLE)
static inline bool wireless_event_set_raw_hid(wireless_event_t *event, const uint8_t *data, uint8_t length) {
    /* VIA 固定使用 32 字节，拒绝短包和超长包，防止旧数据混入或越界。 */
    if (length != sizeof(event->params.raw_hid_data)) {
        return false;
    }
    memcpy(event->params.raw_hid_data, data, sizeof(event->params.raw_hid_data));
    event->evt_type = EVT_RAW_HID;
    return true;
}
#endif
