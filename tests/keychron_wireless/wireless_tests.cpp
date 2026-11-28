// Copyright 2026 Vadym Stupakov
// SPDX-License-Identifier: GPL-2.0-or-later

#include "test_common.hpp"
#include "test_fixture.hpp"
#include "test_driver.hpp"
#include <functional>

extern "C" {
#include "wireless.h"
#include "report_buffer.h"
#include "transport.h"
#include "lpm.h"
#include "usb_device_state.h"

extern host_driver_t wireless_driver;
void                 wireless_event_task(void);
void                 advance_time(uint32_t ms);
void                 set_time(uint32_t time);
}

namespace {
std::vector<report_keyboard_t> reports;
std::vector<report_nkro_t>     nkro_reports;
std::vector<uint16_t>          consumer_reports;
bool                           acknowledge_reports;
unsigned                       connect_requests;
unsigned                       sleep_count;
std::function<void()>          wake_action;
transport_t                    transport = TRANSPORT_BLUETOOTH;

void send_keyboard(uint8_t* data) {
    report_keyboard_t report = {};
    memcpy(&report.mods, data, 8);
    reports.push_back(report);
    if (acknowledge_reports) report_buffer_set_retry(0);
}

void send_nkro(uint8_t* data) {
    report_nkro_t report = {};
    memcpy(&report.mods, data, sizeof(report) - offsetof(report_nkro_t, mods));
    nkro_reports.push_back(report);
    if (acknowledge_reports) report_buffer_set_retry(0);
}

void send_consumer(uint16_t usage) {
    consumer_reports.push_back(usage);
    if (acknowledge_reports) report_buffer_set_retry(0);
}

void connect(uint8_t, uint16_t) {
    ++connect_requests;
}
void noop() {}
} // namespace

extern "C" {
wt_func_t         wireless_transport = {};
host_driver_t     chibios_driver     = {};
test_usb_driver_t USBD1              = {1};
/* lpm.c reads the hardware matrix directly; the native matrix stays private. */
matrix_row_t matrix[MATRIX_ROWS] = {};

void gpio_set_pin_input_high(pin_t) {}
void gpio_set_pin_input_low(pin_t) {}
void gpio_set_pin_output_push_pull(pin_t) {}
void gpio_write_pin_low(pin_t) {}
void gpio_write_pin_high(pin_t) {}
bool gpio_read_pin(pin_t) {
    return true;
}
void palEnableLineEvent(pin_t, unsigned) {}
void palDisableLineEvent(pin_t) {}
void palSetLineMode(pin_t, unsigned) {}
void halInit(void) {}
void usbStop(test_usb_driver_t*) {}
void usbDisconnectBus(test_usb_driver_t*) {}
void usb_event_queue_init(void) {}
void init_usb_driver(test_usb_driver_t*) {}
bool usb_power_connected(void) {
    return false;
}
bool lpm_set(pm_t) {
    return true;
}
void lpm_standby(pm_t) {
    ++sleep_count;
    if (wake_action) wake_action();
}
bool battery_is_critical_low(void) {
    return false;
}
bool bat_level_animiation_actived(void) {
    return false;
}
bool battery_is_empty(void) {
    return false;
}
uint8_t battery_get_percentage(void) {
    return 100;
}
void battery_init(void) {}
void battery_stop(void) {}
void battery_task(void) {}
void battery_timer_reset(void) {}
void indicator_init(void) {}
void indicator_set(wt_state_t, uint8_t) {}
void indicator_battery_low_enable(bool) {}
bool indicator_is_running(void) {
    return false;
}
void indicator_task(void) {}
void keychron_wireless_common_task(void) {}
bool process_record_keychron_wireless(uint16_t, keyrecord_t*) {
    return true;
}
transport_t get_transport(void) {
    return transport;
}
}

/* Run QMK key processing with a virtual matrix and explicit radio/power events. */
class KeychronWireless : public TestFixture {
   protected:
    TestDriver driver;
    KeymapKey  command{0, 0, 0, KC_LGUI};
    KeymapKey  paste{0, 1, 0, KC_V};

    void event(event_type_t type, uint8_t host = 1) {
        wireless_event_t event = {};
        event.evt_type         = type;
        event.params.hostIndex = host;
        ASSERT_TRUE(wireless_event_enqueue(event));
        wireless_event_task();
    }

    /* Advance pacing until both the queue and its in-flight report are settled. */
    void drain() {
        for (unsigned i = 0; i < 300 && (!report_buffer_is_empty() || report_buffer_get_retry()); ++i) {
            advance_time(4);
            report_buffer_task();
        }
        ASSERT_TRUE(report_buffer_is_empty());
        ASSERT_EQ(0, report_buffer_get_retry());
    }

    void fill_report_queue() {
        report_buffer_t report = {};
        report.type            = REPORT_TYPE_KB;
        for (unsigned i = 0; i < REPORT_BUFFER_QUEUE_SIZE; ++i) {
            if (!report_buffer_enqueue(&report)) return;
        }
        FAIL() << "Report queue did not become full";
    }

    void SetUp() override {
        set_keymap({command, paste});
        sleep_count         = 0;
        wake_action         = {};
        transport           = TRANSPORT_BLUETOOTH;
        acknowledge_reports = true;
        advance_time(10);
        wireless_transport               = {};
        wireless_transport.connect_ex    = connect;
        wireless_transport.send_keyboard = send_keyboard;
        wireless_transport.send_nkro     = send_nkro;
        wireless_transport.send_consumer = send_consumer;
        wireless_transport.task          = noop;
        wireless_init();
        /* Exercise the Max-family 1 ms queue pacing independently of the transport mock. */
        report_buffer_set_inverval(1);
        host_set_driver(&wireless_driver);
        event(EVT_CONNECTED);
        drain();
        reports.clear();
        nkro_reports.clear();
        consumer_reports.clear();
        connect_requests = 0;
    }

    /* Evaluate idle policy before radio service can drain pending reports. */
    void mcu_sleep() {
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row)
            matrix[row] = matrix_get_row(row);
        lpm_timer_reset();
        advance_time(DEFAULT_PROCESS_TIME + 1);
        lpm_task();
    }

    void TearDown() override {
        acknowledge_reports = true;
        host_consumer_send(0);
        usb_device_state_set_protocol(USB_PROTOCOL_REPORT);
        if (matrix_is_on(command.position.row, command.position.col)) command.release();
        if (matrix_is_on(paste.position.row, paste.position.col)) paste.release();
        run_one_scan_loop();
        event(EVT_DISCONNECTED);
        host_set_driver(nullptr);
    }

    void expect_paste(bool command_released_first = true) {
        drain();
        ASSERT_EQ(4u, reports.size());
        EXPECT_EQ(MOD_BIT(KC_LGUI), reports[0].mods);
        EXPECT_EQ(MOD_BIT(KC_LGUI), reports[1].mods);
        EXPECT_EQ(KC_V, reports[1].keys[0]);
        EXPECT_EQ(command_released_first ? 0 : MOD_BIT(KC_LGUI), reports[2].mods);
        EXPECT_EQ(command_released_first ? KC_V : 0, reports[2].keys[0]);
        EXPECT_EQ(0, reports[3].mods);
        EXPECT_EQ(0, reports[3].keys[0]);
    }
};

TEST_F(KeychronWireless, CommandVImmediatelyAfterWirelessIdle) {
    event(EVT_SLEEP);
    tap_combo({command, paste});
    event(EVT_CONNECTED);
    expect_paste();
}

TEST_F(KeychronWireless, ReconnectionPreservesHeldCommand) {
    event(EVT_SLEEP);
    command.press();
    run_one_scan_loop();
    event(EVT_CONNECTED);
    EXPECT_EQ(MOD_BIT(KC_LGUI), get_mods());
    tap_key(paste);
    command.release();
    run_one_scan_loop();
    expect_paste(false);
}

TEST_F(KeychronWireless, ExpiredReconnectDoesNotReplayTap) {
    event(EVT_SLEEP);
    tap_key(paste);
    advance_time(WIRELESS_RECONNECT_REPORT_TIMEOUT + 1);
    event(EVT_CONNECTED);
    drain();
    for (const auto& report : reports)
        EXPECT_EQ(0, report.keys[0]);
}

TEST_F(KeychronWireless, DifferentHostDoesNotReceiveQueuedTap) {
    event(EVT_SLEEP);
    tap_key(paste);
    wireless_connect_ex(2, 0);
    event(EVT_CONNECTED, 2);
    drain();
    for (const auto& report : reports)
        EXPECT_EQ(0, report.keys[0]);
}

TEST_F(KeychronWireless, PairingInvalidatesReconnectTap) {
    event(EVT_SLEEP);
    tap_key(paste);
    wireless_pairing_ex(2, nullptr);
    event(EVT_CONNECTED, 2);
    drain();
    for (const auto& report : reports)
        EXPECT_EQ(0, report.keys[0]);
}

TEST_F(KeychronWireless, OverflowDiscardsIncompleteHistoryAndSynchronizesHeldState) {
    event(EVT_SLEEP);
    for (unsigned i = 0; i < 130; ++i)
        tap_key(paste);
    command.press();
    run_one_scan_loop();
    event(EVT_CONNECTED);
    drain();
    ASSERT_EQ(1u, reports.size());
    EXPECT_EQ(0, reports[0].keys[0]);
    EXPECT_EQ(MOD_BIT(KC_LGUI), reports[0].mods);
    command.release();
    run_one_scan_loop();
    drain();
    EXPECT_EQ(0, reports.back().mods);
}

TEST_F(KeychronWireless, TransportChangeDoesNotReplayQueuedTap) {
    event(EVT_SLEEP);
    tap_key(paste);
    transport = TRANSPORT_P2P4;
    event(EVT_CONNECTED);
    drain();
    for (const auto& report : reports)
        EXPECT_EQ(0, report.keys[0]);
}

TEST_F(KeychronWireless, NkroReportsSurviveReconnect) {
    keymap_config.nkro = true;
    event(EVT_SLEEP);
    add_key_bit(nkro_report, KC_A);
    send_keyboard_report();
    del_key_bit(nkro_report, KC_A);
    send_keyboard_report();
    event(EVT_CONNECTED);
    drain();
    ASSERT_EQ(2u, nkro_reports.size());
    EXPECT_EQ(1 << (KC_A & 7), nkro_reports[0].bits[KC_A >> 3]);
    EXPECT_EQ(0, nkro_reports[1].bits[KC_A >> 3]);
    keymap_config.nkro = false;
}

TEST_F(KeychronWireless, EarlyBootDoesNotDequeueWithoutSending) {
    set_time(0);
    report_buffer_init();
    report_buffer_t report  = {};
    report.type             = REPORT_TYPE_KB;
    report.keyboard.keys[0] = KC_V;
    ASSERT_TRUE(report_buffer_enqueue(&report));
    set_time(2);
    report_buffer_task();
    EXPECT_FALSE(report_buffer_is_empty());
    EXPECT_TRUE(reports.empty());
    set_time(3);
    report_buffer_task();
    ASSERT_EQ(1u, reports.size());
    EXPECT_EQ(KC_V, reports[0].keys[0]);
}

TEST_F(KeychronWireless, PendingReportPreventsMcuSleep) {
    report_buffer_t report  = {};
    report.type             = REPORT_TYPE_KB;
    report.keyboard.keys[0] = KC_V;
    ASSERT_TRUE(report_buffer_enqueue(&report));
    mcu_sleep();
    EXPECT_EQ(0u, sleep_count);
    EXPECT_FALSE(report_buffer_is_empty());
    drain();
    ASSERT_EQ(1u, reports.size());
    EXPECT_EQ(KC_V, reports[0].keys[0]);
}

TEST_F(KeychronWireless, PendingRetryPreventsMcuSleep) {
    report_buffer_set_retry(1);
    mcu_sleep();
    EXPECT_EQ(0u, sleep_count);
    EXPECT_EQ(1, report_buffer_get_retry());
    report_buffer_set_retry(0);
}

TEST_F(KeychronWireless, WakeDoesNotResetReportQueue) {
    wake_action = [] {
        report_buffer_t report  = {};
        report.type             = REPORT_TYPE_KB;
        report.keyboard.keys[0] = KC_V;
        EXPECT_TRUE(report_buffer_enqueue(&report));
    };
    mcu_sleep();
    EXPECT_EQ(1u, sleep_count);
    EXPECT_FALSE(report_buffer_is_empty());
    drain();
    ASSERT_EQ(1u, reports.size());
    EXPECT_EQ(KC_V, reports[0].keys[0]);
}

TEST_F(KeychronWireless, KeyboardOverflowPreservesConsumerReleaseWithDelayedAck) {
    acknowledge_reports = false;
    report_buffer_update_timer();
    host_consumer_send(AUDIO_VOL_UP);
    advance_time(4);
    report_buffer_task();
    ASSERT_EQ(std::vector<uint16_t>{AUDIO_VOL_UP}, consumer_reports);
    ASSERT_GT(report_buffer_get_retry(), 0);
    host_consumer_send(0);
    fill_report_queue();

    tap_key(paste);
    acknowledge_reports = true;
    drain();
    EXPECT_EQ(0, consumer_reports.back());
}

TEST_F(KeychronWireless, ConsumerOverflowSynchronizesReleasedUsage) {
    advance_time(4);
    host_consumer_send(AUDIO_VOL_UP);
    ASSERT_EQ(std::vector<uint16_t>{AUDIO_VOL_UP}, consumer_reports);
    fill_report_queue();

    host_consumer_send(0);
    drain();
    EXPECT_EQ(0, consumer_reports.back());
}

TEST_F(KeychronWireless, ReplayedKeyboardTapDoesNotLoseDisconnectedConsumerRelease) {
    advance_time(4);
    host_consumer_send(AUDIO_VOL_UP);
    event(EVT_SLEEP);
    host_consumer_send(0);
    tap_key(paste);
    event(EVT_CONNECTED);
    drain();
    EXPECT_EQ(0, consumer_reports.back());
    ASSERT_EQ(2u, reports.size());
    EXPECT_EQ(KC_V, reports[0].keys[0]);
    EXPECT_EQ(0, reports[1].keys[0]);
}

TEST_F(KeychronWireless, MissingConnectionEventAllowsLaterRetry) {
    event(EVT_SLEEP);
    tap_key(paste);
    const unsigned first_attempt_requests = connect_requests;
    ASSERT_GT(first_attempt_requests, 0u);

    // The transport may lose a command or omit the expected terminal event.
    advance_time(10000);
    tap_key(paste);
    EXPECT_GT(connect_requests, first_attempt_requests);
}

TEST_F(KeychronWireless, ExplicitDisconnectPreservesPendingRelease) {
    command.press();
    run_one_scan_loop();
    drain();
    ASSERT_FALSE(reports.empty());
    ASSERT_EQ(MOD_BIT(KC_LGUI), reports.back().mods);

    command.release();
    run_one_scan_loop();
    ASSERT_FALSE(report_buffer_is_empty());
    wireless_disconnect();

    // The receiver may remain attached to its host after the radio disconnects.
    ASSERT_FALSE(report_buffer_is_empty());
    drain();
    EXPECT_EQ(0, reports.back().mods);
}

TEST_F(KeychronWireless, ExplicitDisconnectNotificationStartsFreshSession) {
    command.press();
    run_one_scan_loop();
    drain();
    wireless_disconnect();
    event(EVT_DISCONNECTED);
    event(EVT_RECONNECTING);
    event(EVT_CONNECTED);
    EXPECT_EQ(0, get_mods());
}

TEST_F(KeychronWireless, ConnectedAfterExplicitDisconnectStartsFreshSession) {
    command.press();
    run_one_scan_loop();
    drain();
    wireless_disconnect();
    event(EVT_CONNECTED);
    EXPECT_EQ(0, get_mods());
}

#ifndef APDAPTIVE_NKRO_ENABLE
TEST_F(KeychronWireless, ClassicNkroBootProtocolSyncUsesBootFormat) {
    usb_device_state_set_protocol(USB_PROTOCOL_BOOT);
    keymap_config.nkro = true;
    register_code(KC_V);
    event(EVT_SLEEP);
    advance_time(WIRELESS_RECONNECT_REPORT_TIMEOUT + 1);
    reports.clear();
    nkro_reports.clear();
    event(EVT_CONNECTED);
    drain();
    ASSERT_EQ(1u, reports.size());
    EXPECT_EQ(KC_V, reports[0].keys[0]);
    EXPECT_TRUE(nkro_reports.empty());
    unregister_code(KC_V);
    keymap_config.nkro = false;
}

TEST_F(KeychronWireless, ClassicNkroResyncDoesNotReplayStaleBootReport) {
    keymap_config.nkro = false;
    register_code(KC_V);
    keymap_config.nkro = true;
    register_code(KC_A);
    event(EVT_SLEEP);
    advance_time(WIRELESS_RECONNECT_REPORT_TIMEOUT + 1);
    reports.clear();
    nkro_reports.clear();
    event(EVT_CONNECTED);
    drain();
    EXPECT_TRUE(reports.empty());
    ASSERT_EQ(1u, nkro_reports.size());
    EXPECT_EQ(1 << (KC_A & 7), nkro_reports[0].bits[KC_A >> 3]);
    unregister_code(KC_A);
    keymap_config.nkro = false;
    unregister_code(KC_V);
}
#endif
