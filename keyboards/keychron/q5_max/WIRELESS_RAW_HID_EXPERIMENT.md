# Q5 Max 2.4G 配置实验

此补丁尝试启用 Q5 Max 已有的无线 Raw HID 通路，供 Launcher / VIA 读取与修改配置。
它是实验代码；构建和单元测试通过不代表键盘已验证成功。

## 适用目标与边界

- 构建目标：`keychron/q5_max/ansi_encoder:keychron`。
- USB 标识：`3434:0850`，Q5 Max Knob ANSI。
- 源码基线：Keychron `2025q3` 分支，提交 `57e682caa0a821e6de1b589e6f983f266151ab02`。
- 这是公开源码基线，不是已安装的 2025-04-23 原厂二进制的精确还原。
- 不修改接收器或键盘内部无线模块固件；仍依赖它们转发配置数据。
- USB PID 和设备版本保持原值，以便 Launcher 使用原产品定义。

## 改动

1. 在 Q5 Max 配置中启用上游 `WILRESS_RAW_ENABLE` 开关；拼写与上游保持一致。
2. 把不存在的 `via_raw_hid_receive` 调用接到现有的三参数 `raw_hid_receive`，保留无线来源标记，使回复经无线模块返回。
3. 启用功能时，每个队列事件持有自己的 32 字节配置包；拒绝其他长度，避免连续包覆盖或旧尾数据混入。
4. 未启用该开关的键盘保留原指针布局。

## 本地验证

在仓库根目录运行边界与队列所有权测试：

```sh
mkdir -p .build/q5-wireless-tests
cc -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined \
  -DRAW_ENABLE -DWILRESS_RAW_ENABLE \
  -Ikeyboards/keychron/common/wireless \
  keyboards/keychron/q5_max/tests/wireless_raw_hid_test.c \
  -o .build/q5-wireless-tests/enabled
.build/q5-wireless-tests/enabled
cc -std=c11 -Wall -Wextra -Werror -DRAW_ENABLE \
  -Ikeyboards/keychron/common/wireless \
  keyboards/keychron/q5_max/tests/wireless_raw_hid_test.c \
  -o .build/q5-wireless-tests/disabled
.build/q5-wireless-tests/disabled
```

准备 QMK CLI、完整 ARM GCC/newlib 工具链和固定版本子模块后构建：

```sh
git submodule update --init lib/chibios lib/chibios-contrib lib/printf
make keychron/q5_max/ansi_encoder:keychron
```

还需核对 ELF / BIN 的 VID、PID、内存范围和无线处理分支，保存制品 SHA256。

本次已使用 QMK 官方 macOS ARM64 工具链 GCC 15.2.0 完成 ANSI 目标构建；边界测试（含 ASan / UBSan）、未启用开关的布局检查和 DFU 后缀 CRC 检查通过。
ELF 确认无线事件处理以来源 `1`、长度 `32` 调用 VIA 入口；设备标识为 `3434:0850`。
本次链接结果的静态数据及预留栈共占 22384 字节 RAM，剩余堆区 43152 字节；该余量不是实机栈峰值或稳定性验证。
制品、原厂回退文件和构建记录保存在工作目录的 `output/q5-max-wireless/`；以其中的校验清单与源码提交记录为准。

## 刷写与设备验收

本次制作实验代码和固件不包含刷写授权。实际刷写前先完成以下准备：

- 导出 Launcher 的键位、宏和其他需要保留的配置；核对导出内容。
- 保存原厂回退固件。新构建日期可能触发 VIA 的键位和宏重置；整 BIN 刷写也可能覆盖模拟 EEPROM 扇区，不能承诺保留配置。
- 明确选择 Q5 Max Knob ANSI，确认可进入 STM32 ROM DFU；不要使用 mass-erase 或 unprotect。

原厂回退文件由 Keychron 固件页面的 Q5 Max / Knob ANSI 链接取得：

`https://cdn.shopify.com/s/files/1/0059/0630/1017/files/q5_max_ansi_encoder_v1.1.1_2504231120_1.bin?v=1745911646`

大小：104460 字节。

SHA256：`f943f39b07f15802aad3b3d5a4020fc5dd10cc45ddd1979fa4b77374c75c7244`。

验收顺序：先验证有线打字与 Launcher；再切纯 2.4G 查询 `01`（VIA 协议）、`02 04`（配置版本）、`A1`（主固件版本）；读取成功后才尝试可恢复的改键，并验证回读与实际按键行为。

若无线查询仍无回复，不能把本补丁称为已修复；需继续检查无线模块和接收器转发，或回退原厂固件。
