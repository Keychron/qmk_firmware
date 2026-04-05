OPT_DEFS += -DFACTORY_TEST_ENABLE

KEYCHRON_COMMON_DIR = common
SRC += \
	$(KEYCHRON_COMMON_DIR)/keychron_task.c \
    $(KEYCHRON_COMMON_DIR)/keychron_common.c \
	$(KEYCHRON_COMMON_DIR)/keychron_raw_hid.c \
    $(KEYCHRON_COMMON_DIR)/factory_test.c

ifeq ($(strip $(OPENRGB_ENABLE)), yes)
    OPT_DEFS += -DOPENRGB_ENABLE
    SRC += $(KEYCHRON_COMMON_DIR)/openrgb.c
    RAW_ENABLE = yes
endif

VPATH += $(TOP_DIR)/keyboards/keychron/$(KEYCHRON_COMMON_DIR)

ESC := $(shell printf '\033')
YELLOW := $(ESC)[1;33m
RESET  := $(ESC)[0m

$(info $(YELLOW)Note: This branch is deprecated. Please use the '2025q3' branch instead.$(RESET))
