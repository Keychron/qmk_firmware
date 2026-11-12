// SPDX-License-Identifier: GPL-2.0-or-later
#include <assert.h>
#include <stdio.h>

#include "wireless_event_type.h"

int main(void) {
#if defined(RAW_ENABLE) && defined(WILRESS_RAW_ENABLE)
    uint8_t          packet[UINT8_MAX] = {0x01};
    wireless_event_t event             = {0};
    wireless_event_t queued[2]         = {0};
    const uint8_t    invalid_lengths[] = {0, 1, 31, 33, 64, UINT8_MAX};

    /* 无效包不能产生事件，也不能改动已有事件的数据。 */
    memset(&event, 0xA5, sizeof(event));
    uint8_t before[sizeof(event)];
    memcpy(before, &event, sizeof(event));
    for (size_t i = 0; i < sizeof(invalid_lengths); ++i) {
        assert(!wireless_event_set_raw_hid(&event, packet, invalid_lengths[i]));
        assert(memcmp(&event, before, sizeof(event)) == 0);
    }

    assert(wireless_event_set_raw_hid(&event, packet, 32));
    queued[0] = event;

    /* 与真实队列一样按值入队，再复用接收缓存和事件对象处理下一包。 */
    memset(packet, 0x5A, 32);
    packet[0] = 0x02;
    packet[1] = 0x04;
    assert(wireless_event_set_raw_hid(&event, packet, 32));
    queued[1] = event;
    memset(packet, 0xFF, sizeof(packet));
    memset(&event, 0, sizeof(event));

    assert(queued[0].evt_type == EVT_RAW_HID);
    assert(queued[0].params.raw_hid_data[0] == 0x01);
    for (size_t i = 1; i < 32; ++i) {
        assert(queued[0].params.raw_hid_data[i] == 0);
    }
    assert(queued[1].evt_type == EVT_RAW_HID);
    assert(queued[1].params.raw_hid_data[0] == 0x02);
    assert(queued[1].params.raw_hid_data[1] == 0x04);
    for (size_t i = 2; i < 32; ++i) {
        assert(queued[1].params.raw_hid_data[i] == 0x5A);
    }
    puts("wireless Raw HID: length checks and queued packet ownership passed");
#else
    /* 未启用此功能的键盘继续使用原指针成员。 */
    _Static_assert(sizeof(((wireless_event_t *)0)->params.raw_hid_data) == sizeof(uint8_t *), "legacy event layout changed");
    puts("wireless Raw HID: legacy event layout passed");
#endif
    return 0;
}
