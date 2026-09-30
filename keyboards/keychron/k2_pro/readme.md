# Keychron K2 Pro

![Keychron K2 Pro](https://github.com/Keychron/ProductImage/blob/main/K_Pro/k2_pro.jpg?raw=true)

A customizable 84-key 75% keyboard.

* Keyboard Maintainer: [Keychron](https://github.com/keychron)
* Hardware Supported: Keychron K2 Pro
* Hardware Availability: [Keychron K2 Pro QMK/VIA Wireless Mechanical Keyboard](https://www.keychron.com/products/keychron-k2-pro-qmk-via-wireless-mechanical-keyboard)

## Option A: Build And Flash Firmware

Make example for this keyboard (after setting up your build environment):

    make keychron/k2_pro/ansi/rgb:keychron
    make keychron/k2_pro/ansi/white:keychron
    make keychron/k2_pro/iso/rgb:keychron
    make keychron/k2_pro/iso/white:keychron
    make keychron/k2_pro/jis/rgb:keychron
    make keychron/k2_pro/jis/white:keychron

Flashing example for this keyboard:

    make keychron/k2_pro/ansi/rgb:keychron:flash
    make keychron/k2_pro/ansi/white:keychron:flash
    make keychron/k2_pro/iso/rgb:keychron:flash
    make keychron/k2_pro/iso/white:keychron:flash
    make keychron/k2_pro/jis/rgb:keychron:flash
    make keychron/k2_pro/jis/white:keychron:flash

**Reset Key**: Connect the USB cable, set the mode switch to "Off", hold down the *Esc* key or the reset button underneath the space bar, then move the mode switch to "Cable".

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Option B: Use Keychron Launcher (no code required)

If you just want to remap keys, configure layers, or change lighting, you don't need to build firmware:

1. Open [Keychron Launcher](https://launcher.keychron.com/) in a Chromium-based browser such as Google Chrome, Microsoft Edge, Brave, Opera, or Vivaldi
2. Connect your Keychron keyboard via USB
3. Remap keys, configure layers, and adjust lighting - changes apply instantly

Keychron Launcher works out of the box with no JSON import required, providing a simpler browser-based setup flow for supported boards. Safari and other non-Chromium browsers will not work with Keychron Launcher.
