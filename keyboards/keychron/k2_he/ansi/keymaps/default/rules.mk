# OpenRGB for the K2 HE default keymap.
#
# keychron_common.mk (which turns OPENRGB_ENABLE into the openrgb.c source +
# -DOPENRGB_ENABLE) is `include`d from the *keyboard* rules.mk, i.e. evaluated
# before this keymap rules.mk is read. A plain `OPENRGB_ENABLE = yes` here is
# therefore seen too late and the sources are silently dropped (OpenRGB commands
# 1-9 then time out on hardware). Wire it directly instead: SRC/OPT_DEFS
# accumulate regardless of include order, and this keeps OpenRGB scoped to this
# keymap (the `keychron` keymap keeps VIA). See tests/openrgb_firmware_wiring.sh.
RAW_ENABLE = yes
OPT_DEFS += -DOPENRGB_ENABLE
SRC += $(TOP_DIR)/keyboards/keychron/common/openrgb.c
