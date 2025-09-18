# Keychron K10 pro iso RBG moded by lioran

## Changes made

```
-Modified TO(x) to require alt to be pressed if higher than layer amount, TO(5) = TO(0) if alt is held, else nothing
-Increased amount of layer to 5
-totally changed K10 pro/ISO/RGB/VIA/keymap.c, removed mac, layer 5 exclusively for FN
-FN+esc for 3 sec = flashing mode even when plugged with UBS when on bluetooth
-FN+end for 3 sec = soft reboot keyboard
-Made modification to LED 16-19 (default F13-16) to display which layer 0 to 3, if fn is on, it will be red instead.
-disabled the mac/windows switch, instead when on the keyboard will use the default keymap.c values instead of dynamic one, layer lights will be blue instead
-Installed https://github.com/yeroca/qmk_concurrent_macros.git for macro looping
-Made modification to the macro module, see below
```

Macro mods
All delay that end with 1 will have a random delay added to them of +- 20, so 101 delay can be 80 to 120
Switched F21-24 to some text check instead along with other added features. any capitalization will be accepted.
KC_TRNS normally just do nothing when inside a macro, it now acts the way you expect it to, but it only fetches from layer 0
KC_TRNS is a valid key for a macro but website like keychron launcher and via block it, so I included a modified version of the via website
this version is exactly the same as the september 2025 usevia.app website exept it allows all valid keys below 0x00FF inside a macro.

- \[init] or \[i] = same as having F21, start of the loop
- \[loop] or \[l] = same as F22, go back to [init] if key is held
- \[toggle] or \[t] = same as F23, turn the macro off
- \[stopall] or \[sa] = same as F24, turn off all macro, except the one that did \[stopall]
- \[stop] or \[s] = stop the macro if the key is not held.
- \[end] or \[e] = stop the current macro running [end]
- \[ctrl] or \[ct] = if button ctrl is not held, end the macro
- \[alt] or \[al] = if button alt is not held, end the macro 
- \[shift] or \[sh] = if button shift is not held, end the macro
- \[;0] = will jump the amount of steps after the ; , this can also be applied to ctrl,alt or shift as such \
	\[ct;10] so if ctrl is held, it will jump 10 steps in the data else it will continue the macro normally, usefull to make macro work in different ways based on button held,
	it's very annoying to count the amount of steps needed based on what you're doing but a keypress or release are 3 steps, delay is 4-7 based on the amount of numbers, text is 1 per character
	I suggest testing by adding something like text 123456789 to figure out the exact position you need to jump to.

Exemple: This macro here will act like the key it's associated to unless shift is held, if so it will spam the key+A until released.

- `[shift;28]{+KC_TRNS}[init]{10}[loop]{-KC_TRNS}[end][init]{KC_TRNS}{80}{KC_A}{20}[loop]`



### Basic initial install done
Before any code was modified.
Installed QMK_MSYS.exe
run drivers.exe and install the drivers Tools > install drivers
once done run the installed qmk msys and run these commands in order
	
- `qmk setup Keychron/qmk_firmware`
- `cd qmk_firmware`
- `git checkout wls_2025q1`
- `qmk git-submodule`
- `make clean keychron/k10_pro/iso/rgb:via`

That last command will compile a bare bone build same as the official one except holding ESC to flash won't work anymore, not a problem.
You should flash this before continuing in case this bricks your keyboard.
If it does not work, and even connecting it via usb gives no respons, your firmware is probably miss configured.
You can recover it still, There is a button hidden under the space bar, hold it(hard to press) and plug with usb, make sure it's not on bluetooth, you should see the same yellow message
popup in driver.exe, then you can reload the official firmware, you can find all keychron firmware here
https://www.keychron.com/pages/firmware-and-json-files-of-the-keychron-qmk-k-pro-and-k-max-series-keyboards
and figure out why yours didn't work then continue.

### Installing macro module

- `cd qmk_firmware`
- `cd modules`
- `git submodule add https://github.com/yeroca/qmk_concurrent_macros.git`
- `cd`
- `cd qmk_firmware`
- `git cherry-pick 386a5019a8147136c2732452cd87954378e0ec16`
- go in folder `qmk_firmware\keyboard\keychron\k10_pro\info.json` and add this to the json

	```
	"modules": [
        "qmk_concurrent_macros/concurrent_macros",
	],```
	
- `make clean keychron/k10_pro/iso/rgb:via`

Once this update is installed, the bluetooth macro bug where each buttons inside the macro is held till the macro ends will be fixed.