# Keychron K10 pro iso RBG moded by lioran

## Changes made

Installed https://github.com/yeroca/qmk_concurrent_macros.git and made some modificitation to it
such as all delay that end with 1 will have a random delay added to them +- 20, so 101 delay cna be 80 to 120, dont ask why
made it so transparent keys,(KC_TRNS) will fetch the key of layer 0 of the key the macro is associated to instead.
switched the F21-24 to some text check instead

- \[init] = same as having F21, start of the loop
- \[loop] = same as F22, go back to init if key is held
- \[toggle] = same as F23
- \[stopall] = same as F24
- \[stop] = stop the macro if the key is not held.
- \[end] = custom, stop the current macro running this
- \[ctrl] = if button ctrl is not held, end the macro
- \[alt] = if button alt is not held, end the macro 
- \[shift] = if button shift is not held, end the macro
- \[;0] = will jump the amount of steps after the ; , this can also be applied to ctrl,alt or shift as such \[ctrl;10] so if ctrl is on, it will jump 10 steps in the data else it will continue the macro usefull to make macro work in different ways based on button held, it's very annoying to count the amount of steps needed based on what you're doing but a keypress or release is 3 steps, delay is 2-5 based on the amount of numbers, text is 1 per character

Exemple: This macro here will act like the key it's associated to unless shift is held, if so it will spam key+A until released.

- `[shift;28]{+KC_TRNS}[init]{10}[loop]{-KC_TRNS}[end][init]{KC_TRNS}{80}{KC_A}{20}[loop]`

Modified TO(x) to act differently if the number is higher than the amount of layers available for exemple if you have 4 layers
TO(0 to 3) will go to those layers but if TO(4 to 7) , it will go to layer 0 - 3 if alt is held, else it will do nothing.

Increased layers to 5, 5th one to be used exclusively for FN(lost 0.25kb of macro space for it). Increassed amount of macro to 32, no reason not to.
made modification to LED 16-19 (default F13-F16) to represent which layer is on 0 to 3 and if fn(layer 4) is on, the led will be red instead.



### Basic initial install done
Before any code was modified.
Installed QMK_MSYS.exe
once done run the installed qmk msys and run these commands in order
	
- `qmk setup Keychron/qmk_firmware`
- `cd qmk_firmware`
- `git checkout wls_2025q1`
- `qmk git-submodule`
- `qmk compile -kb keychron/k10_pro/iso/rgb -km via`

That last command will compile a bare bone build same as the official one
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
- `go in folder qmk_firmware\keyboard\keychron\k10_pro\info.json and add this to the json


	```"modules": [
        "qmk_concurrent_macros/concurrent_macros",
	],```
	
- `make clean keychron/k10_pro/iso/rgb:via`

Once this update is installed, flashing will no longer work without the button under the space bar and the bluetooth macro bug where each button is held till the end will be fixed.