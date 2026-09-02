/* Copyright 2025 ~ 2026 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "keycodes.h"

enum custom_keycodes {
    KC_MAC_MISSION_CONTROL = QK_KB_0,
    KC_MAC_LAUCHPAD,
    KC_LOPTN,
    KC_ROPTN,
    KC_LCMMD,
    KC_RCMMD,
    KC_MAC_SIRI,
    KC_WIN_TASK_VIEW,
    KC_WIN_FILE_EXPLORER,
    KC_MAC_SCREEN_SHOT,
    KC_WIN_CORTANA,
    OS_TOGGL,
    NEW_SAFE_RANGE,
};

#define KC_MCTRL KC_MAC_MISSION_CONTROL
#define KC_LNPAD KC_MAC_LAUCHPAD
#define KC_SIRI KC_MAC_SIRI
#define KC_TASK KC_WIN_TASK_VIEW
#define KC_FILE KC_WIN_FILE_EXPLORER
#define KC_SNAP KC_MAC_SCREEN_SHOT
#define KC_CTANA KC_WIN_CORTANA
