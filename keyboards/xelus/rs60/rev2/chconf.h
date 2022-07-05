/* Copyright 2022 QMK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This file was auto-generated by:
<<<<<<<< HEAD:keyboards/xelus/rs60/rev2/chconf.h
 *    `qmk chibios-confmigrate -i keyboards/acheron/austin/chconf.h -r platforms/chibios/common/configs/chconf.h`
========
 *    `qmk chibios-confmigrate -i keyboards/zvecr/zv48/f401/halconf.h -r platforms/chibios/common/configs/halconf.h`
>>>>>>>> master:keyboards/mlego/m60_split/rev2/halconf.h
 */

#pragma once

<<<<<<<< HEAD:keyboards/xelus/rs60/rev2/chconf.h
#define CH_CFG_ST_FREQUENCY 10000

#define CH_CFG_OPTIMIZE_SPEED FALSE
========
#define HAL_USE_PWM TRUE
>>>>>>>> master:keyboards/mlego/m60_split/rev2/halconf.h

#define HAL_USE_SERIAL TRUE

<<<<<<<< HEAD:keyboards/xelus/rs60/rev2/chconf.h
#include_next <chconf.h>
========
#define SERIAL_USB_BUFFERS_SIZE 256

#include_next <halconf.h>
>>>>>>>> master:keyboards/mlego/m60_split/rev2/halconf.h
