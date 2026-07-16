#!/usr/bin/env bash
# Build-wiring guard for the K2 HE OpenRGB feature.
#
# The gtest unit tests (openrgb_mapping.cpp) compile keyboards/keychron/common/
# openrgb.c *directly*, so they pass even when the keyboard build never actually
# compiles openrgb.c. This guard builds the real default-keymap firmware and
# asserts the OpenRGB sources are linked in.
#
# It catches the regression where `OPENRGB_ENABLE = yes` is set in the keymap
# rules.mk but keychron_common.mk (which adds openrgb.c when OPENRGB_ENABLE=yes)
# is `include`d from the *keyboard* rules.mk -- i.e. evaluated before the keymap's
# rules.mk is read -- so the flag is seen too late and openrgb.c is silently
# dropped, making OpenRGB commands 1-9 time out on real hardware.
#
# Run from the nix dev shell:  nix develop --command bash keyboards/keychron/common/tests/openrgb_firmware_wiring.sh
set -uo pipefail
cd "$(git -C "$(dirname "$0")" rev-parse --show-toplevel)"

qmk compile -kb keychron/k2_he/ansi -km default >/dev/null 2>&1 || true

obj=$(find .build -path '*k2_he_ansi*' -name 'openrgb.o' 2>/dev/null | head -1)
if [ -n "$obj" ]; then
    echo "PASS: openrgb.c is compiled into the K2 HE default firmware"
    exit 0
fi
echo "FAIL: openrgb.c is NOT compiled into the K2 HE default firmware --"
echo "      OpenRGB commands 1-9 will be dropped (cmd reaches the dispatch default"
echo "      case -> 'return false' -> no reply). Check OPENRGB_ENABLE wiring."
exit 1
