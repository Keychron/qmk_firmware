// SPDX-License-Identifier: GPL-2.0-or-later
//
// gtest cases proving two real bugs/fixes in the Keychron K2 HE OpenRGB code
// (keyboards/keychron/common/openrgb.c), compiled here against host mocks.
//
//   BUG 1 (mode mapping): GET_ENABLED_MODES returns OpenRGB mode ids (not a raw
//          1..N list); SET_MODE translates the OpenRGB counter through
//          openrgb_mode_map[] into a QMK effect; GET_MODE_INFO translates the
//          active QMK effect back to its 1-based OpenRGB counter.
//
//   BUG 2 (reply transport): replies go via kc_raw_hid_send(RAW_HID_SRC_USB,...)
//          (USB-explicit), NOT the core raw_hid_send() which is dropped on
//          wireless.
//
// Every case PASSES against the current (fixed) openrgb.c and would FAIL if the
// corresponding fix were reverted.

#include "gtest/gtest.h"

#include <cstdint>
#include <vector>

#include "openrgb_test_recorder.h"

// The shim (RGB/HSV types + rgb_matrix_effects enum) and the REAL openrgb.h
// (OPENRGB_MODE_* ids, openrgb_command_handler). mocks/ is first on the include
// path so openrgb.h's `#include "color.h"` resolves to the shadow.
extern "C" {
#include "openrgb_test_shim.h"
#include "openrgb.h"
}

// RAW_EPSIZE comes in via -DRAW_EPSIZE on the command line.
static constexpr int kEpSize = RAW_EPSIZE;

class OpenRgbMapping : public ::testing::Test {
   protected:
    void SetUp() override {
        OpenRgbMockState::Instance().reset();
    }

    // Run GET_ENABLED_MODES and return the null-terminated id list (terminator
    // excluded). Used to discover N (the Direct counter) at runtime.
    static std::vector<uint8_t> EnabledModeIds() {
        OpenRgbMockState::Instance().reset();
        uint8_t data[kEpSize] = {0};
        data[0]               = OPENRGB_GET_ENABLED_MODES;
        EXPECT_TRUE(openrgb_command_handler(data, kEpSize));
        const auto& buf = OpenRgbMockState::Instance().kc_send_buf;
        EXPECT_FALSE(buf.empty());
        std::vector<uint8_t> ids;
        for (size_t i = 1; i < buf.size(); ++i) {
            if (buf[i] == 0) break;
            ids.push_back(buf[i]);
        }
        return ids;
    }
};

// 1. GET_ENABLED_MODES returns OpenRGB mode IDs, null-terminated -- NOT a raw
//    1..N list. The first table entry is SOLID_COLOR whose OpenRGB id is 2, so
//    byte[1] must be 2 (not 1). The list must contain Direct's id (1) and end in 0.
TEST_F(OpenRgbMapping, EnabledModesAreOpenRgbIds) {
    uint8_t data[kEpSize] = {0};
    data[0]               = OPENRGB_GET_ENABLED_MODES;

    ASSERT_TRUE(openrgb_command_handler(data, kEpSize));

    const auto& buf = OpenRgbMockState::Instance().kc_send_buf;
    ASSERT_EQ((int)buf.size(), kEpSize);

    EXPECT_EQ(buf[0], OPENRGB_GET_ENABLED_MODES) << "reply command id";

    // First emitted id is SOLID_COLOR's OpenRGB id (2), proving these are ids and
    // not a raw 1-based counter list (old code would emit 1 here).
    EXPECT_EQ(buf[1], (uint8_t)OPENRGB_MODE_SOLID_COLOR) << "first id should be the OpenRGB SOLID_COLOR id (2), not 1";
    EXPECT_NE(buf[1], 1) << "a value of 1 would mean the old raw 1..N list";

    // The list must contain the Direct id (1) somewhere.
    auto ids       = EnabledModeIds();
    bool has_direct = false;
    for (uint8_t id : ids) {
        if (id == (uint8_t)OPENRGB_MODE_OPENRGB_DIRECT) has_direct = true;
    }
    EXPECT_TRUE(has_direct) << "enabled-modes list must include OPENRGB_MODE_OPENRGB_DIRECT (==1)";

    // Null-terminated: the byte right after the last id must be 0.
    ASSERT_LT(ids.size() + 1, (size_t)kEpSize);
    EXPECT_EQ(buf[1 + ids.size()], 0) << "enabled-modes list must be null-terminated";
}

// 2. SET_MODE with counter N (= Direct, the last table slot) maps to
//    RGB_MATRIX_OPENRGB_DIRECT -- NOT the raw counter N.
TEST_F(OpenRgbMapping, SetModeDirectMapsToDirectEffect) {
    const auto    ids = EnabledModeIds();
    const uint8_t N   = (uint8_t)ids.size();
    ASSERT_GT(N, 0);

    OpenRgbMockState::Instance().reset();

    // {cmd, h, s, v, mode-counter, speed, save}
    uint8_t data[kEpSize] = {0};
    data[0]               = OPENRGB_SET_MODE;
    data[1]               = 0;   // h
    data[2]               = 255; // s
    data[3]               = 255; // v
    data[4]               = N;   // counter N == Direct
    data[5]               = 127; // speed
    data[6]               = 0;   // save=0 -> _noeeprom path

    ASSERT_TRUE(openrgb_command_handler(data, kEpSize));

    auto& s = OpenRgbMockState::Instance();
    ASSERT_TRUE(s.mode_set) << "rgb_matrix_mode*() must have been called";
    EXPECT_TRUE(s.mode_set_via_noeeprom) << "save=0 must use the _noeeprom variant";
    EXPECT_EQ(s.last_mode, (uint8_t)RGB_MATRIX_OPENRGB_DIRECT) << "Direct counter must map to the Direct QMK effect, not the raw counter";
    EXPECT_NE(s.last_mode, N) << "must not be the raw counter N (old behavior)";
}

// 3. SET_MODE with a mid-list counter k whose mapped QMK effect != k goes through
//    the table. Counter 15 in the table is CYCLE_OUT_IN, whose QMK enum value
//    (16) differs from 15.
TEST_F(OpenRgbMapping, SetModeMidListMapsThroughTable) {
    const auto    ids = EnabledModeIds();
    const uint8_t k   = 15; // OpenRGB counter -> table slot 15 -> CYCLE_OUT_IN
    ASSERT_GE((int)ids.size(), (int)k);

    // Sanity: the chosen slot's QMK effect must actually differ from the counter,
    // otherwise this test can't distinguish the fix from identity behavior.
    ASSERT_NE((uint8_t)RGB_MATRIX_CYCLE_OUT_IN, k) << "test precondition: mapped effect must differ from counter";

    OpenRgbMockState::Instance().reset();

    uint8_t data[kEpSize] = {0};
    data[0]               = OPENRGB_SET_MODE;
    data[1]               = 0;
    data[2]               = 255;
    data[3]               = 255;
    data[4]               = k;
    data[5]               = 127;
    data[6]               = 0;

    ASSERT_TRUE(openrgb_command_handler(data, kEpSize));

    auto& s = OpenRgbMockState::Instance();
    ASSERT_TRUE(s.mode_set);
    EXPECT_EQ(s.last_mode, (uint8_t)RGB_MATRIX_CYCLE_OUT_IN) << "counter 15 must map through the table to CYCLE_OUT_IN";
    EXPECT_NE(s.last_mode, k) << "must not be the raw counter k (old behavior)";
}

// 4. GET_MODE_INFO translates the active QMK effect back to its 1-based OpenRGB
//    counter. With Direct active, byte[1] must be N (Direct's counter), not the
//    raw enum value.
TEST_F(OpenRgbMapping, GetModeInfoReportsOpenRgbCounter) {
    const auto    ids = EnabledModeIds();
    const uint8_t N   = (uint8_t)ids.size();
    ASSERT_GT(N, 0);

    OpenRgbMockState::Instance().reset();
    OpenRgbMockState::Instance().get_mode_value = (uint8_t)RGB_MATRIX_OPENRGB_DIRECT;

    uint8_t data[kEpSize] = {0};
    data[0]               = OPENRGB_GET_MODE_INFO;

    ASSERT_TRUE(openrgb_command_handler(data, kEpSize));

    const auto& buf = OpenRgbMockState::Instance().kc_send_buf;
    ASSERT_EQ((int)buf.size(), kEpSize);
    EXPECT_EQ(buf[0], OPENRGB_GET_MODE_INFO);
    EXPECT_EQ(buf[1], N) << "GET_MODE_INFO must report Direct's 1-based OpenRGB counter (N)";
    EXPECT_NE(buf[1], (uint8_t)RGB_MATRIX_OPENRGB_DIRECT) << "must not report the raw QMK enum value";
}

// 5. Replies are delivered via kc_raw_hid_send(RAW_HID_SRC_USB,...), NOT the core
//    raw_hid_send() (which our mock drops, modeling the wireless NULL driver).
TEST_F(OpenRgbMapping, ReplyGoesOverUsbNotCoreRawHid) {
    uint8_t data[kEpSize] = {0};
    data[0]               = OPENRGB_GET_PROTOCOL_VERSION;

    ASSERT_TRUE(openrgb_command_handler(data, kEpSize));

    auto& s = OpenRgbMockState::Instance();
    EXPECT_TRUE(s.kc_send_called) << "reply must be delivered via kc_raw_hid_send";
    EXPECT_EQ(s.kc_send_src, (uint8_t)RAW_HID_SRC_USB) << "reply must be sent with src == RAW_HID_SRC_USB";
    ASSERT_EQ((int)s.kc_send_buf.size(), kEpSize);
    EXPECT_EQ(s.kc_send_buf[0], OPENRGB_GET_PROTOCOL_VERSION) << "USB-delivered reply must carry the protocol-version reply";

    // The reply must NOT have been carried (and dropped) by the core transport.
    EXPECT_FALSE(s.core_send_called) << "reply must not be routed through core raw_hid_send (dropped on wireless)";
}

// 6. Full OpenRGB discovery handshake -- the exact command sequence OpenRGB
//    issues on detection. Each command must reply over USB (kc_raw_hid_send with
//    src == RAW_HID_SRC_USB) with sane data: i.e. OpenRGB can see + enumerate the
//    keyboard at the protocol level. This is the unit-level equivalent of
//    "OpenRGB sees the keyboard" -- it exercises the compiled openrgb.c against
//    mocks and never touches real hardware.
TEST_F(OpenRgbMapping, ProtocolDiscoveryHandshake) {
    auto& s = OpenRgbMockState::Instance();

    auto run = [&](uint8_t cmd) -> std::vector<uint8_t> {
        s.reset();
        uint8_t data[kEpSize] = {0};
        data[0]               = cmd;
        if (cmd == OPENRGB_GET_LED_INFO) { // GET_LED_INFO needs (first_led, count)
            data[1] = 0;
            data[2] = 8;
        }
        EXPECT_TRUE(openrgb_command_handler(data, kEpSize)) << "handler rejected cmd " << (int)cmd;
        EXPECT_TRUE(s.kc_send_called) << "cmd " << (int)cmd << " did not reply over USB";
        EXPECT_EQ(s.kc_send_src, (uint8_t)RAW_HID_SRC_USB) << "cmd " << (int)cmd << " reply not USB-src";
        EXPECT_FALSE(s.core_send_called) << "cmd " << (int)cmd << " routed via core raw_hid_send (would drop on wireless)";
        EXPECT_EQ((int)s.kc_send_buf.size(), kEpSize);
        return s.kc_send_buf;
    };

    // 1) GET_PROTOCOL_VERSION -> protocol version (RevD = 0x0D) in byte[1].
    auto pv = run(OPENRGB_GET_PROTOCOL_VERSION);
    EXPECT_EQ(pv[0], OPENRGB_GET_PROTOCOL_VERSION);
    EXPECT_EQ(pv[1], (uint8_t)OPENRGB_PROTOCOL_VERSION) << "discovery: protocol version must be RevD (0x0D)";

    // 2) GET_QMK_VERSION -> non-empty version string from byte[1].
    auto qv = run(OPENRGB_GET_QMK_VERSION);
    EXPECT_EQ(qv[0], OPENRGB_GET_QMK_VERSION);
    EXPECT_NE(qv[1], 0) << "discovery: QMK version string must be present";

    // 3) GET_DEVICE_INFO -> LED count, matrix cells, device name.
    auto di = run(OPENRGB_GET_DEVICE_INFO);
    EXPECT_EQ(di[0], OPENRGB_GET_DEVICE_INFO);
    EXPECT_EQ(di[1], (uint8_t)RGB_MATRIX_LED_COUNT) << "discovery: device-info LED count";
    EXPECT_GT(di[2], 0) << "discovery: device-info matrix-cell count";
    EXPECT_NE(di[3], 0) << "discovery: device name must be present";

    // 4) GET_ENABLED_MODES -> at least one enabled mode.
    auto em = run(OPENRGB_GET_ENABLED_MODES);
    EXPECT_EQ(em[0], OPENRGB_GET_ENABLED_MODES);
    EXPECT_NE(em[1], 0) << "discovery: at least one enabled mode";

    // 5) GET_MODE_INFO -> replies with the active-mode info block.
    auto mi = run(OPENRGB_GET_MODE_INFO);
    EXPECT_EQ(mi[0], OPENRGB_GET_MODE_INFO);

    // 6) GET_LED_INFO -> replies with the per-LED layout block.
    auto li = run(OPENRGB_GET_LED_INFO);
    EXPECT_EQ(li[0], OPENRGB_GET_LED_INFO);
}
