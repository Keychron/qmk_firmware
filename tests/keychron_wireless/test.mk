# Copyright 2026 Vadym Stupakov
# SPDX-License-Identifier: GPL-2.0-or-later

NKRO_ENABLE = yes

KEYCHRON_TEST_ADAPTIVE_NKRO ?= yes
ifeq ($(KEYCHRON_TEST_ADAPTIVE_NKRO),yes)
OPT_DEFS += -DAPDAPTIVE_NKRO_ENABLE
endif

OPT_DEFS += -DQMK_KEYBOARD_H=\"quantum.h\" -DWIRELESS_NKRO_ENABLE -DFACTORY_TEST_ENABLE
VPATH += keyboards/keychron/common keyboards/keychron/common/wireless
SRC += keyboards/keychron/common/wireless/wireless.c \
       keyboards/keychron/common/wireless/report_buffer.c \
       keyboards/keychron/common/wireless/lpm.c

# The shared matcher supports boot reports only, even in an NKRO-enabled test.
$(TEST_OBJ)/$(TEST_OUTPUT)/tests/test_common/keyboard_report_util.o: FILE_SPECIFIC_CFLAGS += -UNKRO_ENABLE
