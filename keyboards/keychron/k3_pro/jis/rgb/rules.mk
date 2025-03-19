# MCU & USB settings
MCU = STM32L432KB
BOOTLOADER = stm32-dfu

# Enable features
VIA_ENABLE = yes
RGB_MATRIX_ENABLE = yes

# Optimization
LTO_ENABLE = yes

# Fix for build error
SRC += matrix.c
SRC += rgb.c


include ../../rules.mk
