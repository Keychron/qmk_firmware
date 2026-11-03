
OPT_DEFS += -DKC_BLUETOOTH_ENABLE
OPT_DEFS += -DBLUETOOTH_NKRO_ENABLE
# Give the host time to complete USB enumeration before RGB/BT init runs.
# This avoids the first-boot-after-DFU SET_CONFIGURATION race on CKBT51 boards.
OPT_DEFS += -DUSB_ENUMERATION_WAIT_MS=500
# The encoder pins are also read from a pin ISR (encoder_quadrature_handle_inerrupt_read()).
OPT_DEFS += -DENCODER_QUADRATURE_ATOMIC_READ

BLUETOOTH_DIR = bluetooth
SRC += \
     $(BLUETOOTH_DIR)/bluetooth.c \
     $(BLUETOOTH_DIR)/report_buffer.c \
     $(BLUETOOTH_DIR)/ckbt51.c \
     $(BLUETOOTH_DIR)/indicator.c \
     $(BLUETOOTH_DIR)/bluetooth_main.c \
     $(BLUETOOTH_DIR)/transport.c \
     $(BLUETOOTH_DIR)/lpm.c \
     $(BLUETOOTH_DIR)/lpm_stm32l432.c \
     $(BLUETOOTH_DIR)/battery.c \
     $(BLUETOOTH_DIR)/factory_test.c \
     $(BLUETOOTH_DIR)/bat_level_animation.c \
     $(BLUETOOTH_DIR)/rtc_timer.c

VPATH += $(TOP_DIR)/keyboards/keychron/$(BLUETOOTH_DIR)

# Work around RTC clock issue without touching chibios, refer to the link for this bug
# https://forum.chibios.org/viewtopic.php?f=35&t=6197
OPT_DEFS += -DRCC_APBENR1_RTCAPBEN
