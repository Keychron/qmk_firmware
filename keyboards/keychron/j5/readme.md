# Keychron J5

![Keychron J5](https://cdn.shopify.com/s/files/1/0059/0630/1017/files/Keychron-J5-QMK-Wireless-Mechanical-Keyboard-page3.jpg)

A customizable full size wireless keyboard.

* Keyboard Maintainer: [Keychron](https://github.com/keychron)
* Hardware Supported: Keychron J5
* Hardware Availability: [Keychron J5 QMK/VIA Wireless Custom Mechanical Keyboard](https://www.keychron.com/products/keychron-j5-qmk-wireless-mechanical-keyboard)

## Option A: Build And Flash Firmware

Make example for this keyboard (after setting up your build environment):

    make keychron/j5/ansi:keychron
    make keychron/j5/ansi_encoder:keychron

Flashing example for this keyboard:

    make keychron/j5/ansi:keychron:flash
    make keychron/j5/ansi_encoder:keychron:flash

**Reset Key**: Toggle mode switch to "Cable", hold down the *Esc* key or reset button underneath space bar while connecting the USB cable,

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Option B: Use Keychron Launcher (no code required)

If you just want to remap keys, configure layers, or change lighting, you don't need to build firmware:

1. Open [Keychron Launcher](https://launcher.keychron.com/) in a Chromium-based browser such as Google Chrome, Microsoft Edge, Brave, Opera, or Vivaldi
2. Connect your Keychron keyboard via USB
3. Remap keys, configure layers, and adjust lighting - changes apply instantly

Keychron Launcher works out of the box with no JSON import required, providing a simpler browser-based setup flow for supported boards. Safari and other non-Chromium browsers will not work with Keychron Launcher.

