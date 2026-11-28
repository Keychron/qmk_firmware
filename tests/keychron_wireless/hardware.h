// Copyright 2026 Vadym Stupakov
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "lpm_stm32f401.h"

typedef uint16_t pin_t;

#define TRUE true
#define FALSE false
#define A11 11
#define A12 12
#define PAL_EVENT_MODE_FALLING_EDGE 0
#define PAL_EVENT_MODE_BOTH_EDGES 1
#define PAL_MODE_INPUT_PULLDOWN 0
#define PAL_STM32_OTYPE_PUSHPULL 0
#define PAL_STM32_OSPEED_HIGHEST 0
#define PAL_STM32_PUPDR_FLOATING 0
#define PAL_MODE_ALTERNATE(n) (n)

typedef struct {
    unsigned state;
} test_usb_driver_t;

#define USB_STOP 0
#define USB_DRIVER USBD1

#ifdef __cplusplus
extern "C" {
#endif

void                     gpio_set_pin_input_high(pin_t pin);
void                     gpio_set_pin_input_low(pin_t pin);
void                     gpio_set_pin_output_push_pull(pin_t pin);
void                     gpio_write_pin_low(pin_t pin);
void                     gpio_write_pin_high(pin_t pin);
bool                     gpio_read_pin(pin_t pin);
void                     palEnableLineEvent(pin_t pin, unsigned mode);
void                     palDisableLineEvent(pin_t pin);
void                     palSetLineMode(pin_t pin, unsigned mode);
void                     halInit(void);
extern test_usb_driver_t USBD1;
void                     usbStop(test_usb_driver_t* driver);
void                     usbDisconnectBus(test_usb_driver_t* driver);
void                     usb_event_queue_init(void);
void                     init_usb_driver(test_usb_driver_t* driver);
bool                     bat_level_animiation_actived(void);

#ifdef __cplusplus
}
#endif
