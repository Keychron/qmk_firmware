OPT_DEFS += -DKEYCHRON_RGB_ENABLE
OPT_DEFS += -DRETAIL_DEMO_ENABLE

RGB_MATRIX_CUSTOM_KB = yes
RGB_MATRIX_DIR = $(TOP_DIR)/keyboards/keychron/common/rgb

SRC += \
     $(RGB_MATRIX_DIR)/keychron_rgb.c \
     $(RGB_MATRIX_DIR)/per_key_rgb.c \
     $(RGB_MATRIX_DIR)/mixed_rgb.c \
     $(RGB_MATRIX_DIR)/retail_demo.c

VPATH += $(RGB_MATRIX_DIR)


