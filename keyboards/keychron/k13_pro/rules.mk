# Enter lower-power sleep mode when on the ChibiOS idle thread
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
OPT_DEFS += -DNO_USB_STARTUP_CHECK -DENABLE_FACTORY_TEST

# Opt-in experiment (keep default sym_defer_g until measured on hardware with medir_polling.py):
# DEBOUNCE_TYPE = asym_eager_defer_pk lowers press latency but can ghost on the shared HC595 bus

SRC += matrix.c

include keyboards/keychron/bluetooth/bluetooth.mk
