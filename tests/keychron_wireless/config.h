// Copyright 2026 Vadym Stupakov
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "test_common.h"
#include "hardware.h"

#define MATRIX_ROW_PINS {1, 2, 3, 4}
#define MATRIX_COL_PINS {10, 11, 12, 13, 14, 15, 16, 17, 18, 19}
#define LKBT51_INT_INPUT_PIN 20
#define MCU_TO_WIRELESS_INT_PIN 21
#define KEEP_USB_CONNECTION_IN_WIRELESS_MODE

#ifndef REPORT_BUFFER_QUEUE_SIZE
#    define REPORT_BUFFER_QUEUE_SIZE 256
#endif
