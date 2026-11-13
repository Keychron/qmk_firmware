OPT_DEFS += -DLK_WIRELESS_ENABLE
OPT_DEFS += -DNO_USB_STARTUP_CHECK
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
OPT_DEFS += -DWIRELESS_NKRO_ENABLE
# Give the host time to complete USB enumeration before RGB/wireless init runs.
# Same post-DFU SET_CONFIGURATION guard as keyboards/keychron/bluetooth/bluetooth.mk.
OPT_DEFS += -DUSB_ENUMERATION_WAIT_MS=500
# The encoder pins are also read from a pin ISR (encoder_quadrature_handle_inerrupt_read()).
OPT_DEFS += -DENCODER_QUADRATURE_ATOMIC_READ

INFO_RULES_MK = $(shell $(QMK_BIN) generate-rules-mk --quiet --escape --keyboard $(KEYBOARD) --output $(INTERMEDIATE_OUTPUT)/src/info_rules.mk)
include $(INFO_RULES_MK)

WIRELESS_DIR = common/wireless
SRC += \
     $(WIRELESS_DIR)/wireless.c \
     $(WIRELESS_DIR)/report_buffer.c \
     $(WIRELESS_DIR)/lkbt51.c \
     $(WIRELESS_DIR)/indicator.c \
     $(WIRELESS_DIR)/wireless_main.c \
     $(WIRELESS_DIR)/transport.c \
     $(WIRELESS_DIR)/lpm.c \
     $(WIRELESS_DIR)/battery.c \
     $(WIRELESS_DIR)/bat_level_animation.c \
     $(WIRELESS_DIR)/rtc_timer.c \
     $(WIRELESS_DIR)/keychron_wireless_common.c

ifeq ($(strip $(MCU)), STM32F401)
SRC += $(WIRELESS_DIR)/lpm_stm32f401.c
endif

ifeq ($(strip $(MCU)), STM32L432)
SRC += $(WIRELESS_DIR)/lpm_stm32l432.c
endif

ifeq ($(strip $(MCU)), WB32F3G71)
SRC += $(WIRELESS_DIR)/lpm_wb32f3g71.c
endif

VPATH += $(TOP_DIR)/keyboards/keychron/$(WIRELESS_DIR)

