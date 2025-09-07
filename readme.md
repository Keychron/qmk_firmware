Keychron K10 pro iso RBG moded by lioran

Changes made,
Installed https://github.com/yeroca/qmk_concurrent_macros.git and applied some modificitation to it
instead of key F21 to F24, it's simple text l_init, l_go, l_toggle, l_stopall, l_stop

Modified TO(x) to MOD 4 
and made it to TO(4) to (7) require alt to be held to work

made modification to LED 16-19 to represent which layer is on.


Basic initial install done.
	Install QMK_MSYS.exe

	Run the installed qmk msys and run these commands in order
	
	
qmk setup Keychron/qmk_firmware
cd qmk_firmware
git checkout wls_2025q1
qmk git-submodule
qmk compile -kb keychron/k10_pro/iso/rgb -km via

	That last command will compile a bare bone build same as the official one
	to flash run driver.exe
	unplug your keyboard, hold the ESC key then plug your keyboard while hold ESC
	you will see this yellow message popup in driver.exe window
	STM32 DFU device connected (WinUSB): STMicroelectronics STM32  BOOTLOADER (0483:DF11:2200)
	you can select the bin file you just compiled, should be in qmk_firmware/.build
	flashing it will reset everything, once the firmed is finished installing, unplug your keyboard and replug, it should now work as normal.
	If it does not, and even connecting it via usb gives no respons, your firmware is probably miss configured.
	
	You can recover your keyboard if it no longer works, there is a button hidden under the space bar, hold it and plug the keyboard, you should see the same yellow message
	popup in driver.exe, then you can reload the official firmware, you can find all keychron firmware here
	https://www.keychron.com/pages/firmware-and-json-files-of-the-keychron-qmk-k-pro-and-k-max-series-keyboards
	
	how to install macro module


cd qmk_firmware
cd modules
git submodule add https://github.com/yeroca/qmk_concurrent_macros.git
cd
cd qmk_firmware

go in folder qmk_firmware\keyboard\keychron\k10_pro\info.json
opeen it in a text editor and add this to the json
	"modules": [
        "qmk_concurrent_macros/concurrent_macros",
	],


git cherry-pick 386a5019a8147136c2732452cd87954378e0ec16
make clean keychron/k10_pro/iso/rgb:via