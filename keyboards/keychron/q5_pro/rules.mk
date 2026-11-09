# common.mk compiles Keychron's debounce. Tell QMK not to also link its own.
DEBOUNCE_TYPE = custom

# Enter lower-power sleep mode when on the ChibiOS idle thread
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
OPT_DEFS += -DNO_USB_STARTUP_CHECK -DENABLE_FACTORY_TEST

include keyboards/keychron/bluetooth/bluetooth.mk
include keyboards/keychron/common/common.mk

# eeconfig_kb.h includes this header. The language module is optional, but the
# header has to be on the include path or the keyboard does not compile.
VPATH += $(TOP_DIR)/keyboards/keychron/common/language
