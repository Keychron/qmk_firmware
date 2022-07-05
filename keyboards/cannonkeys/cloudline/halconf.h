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
<<<<<<<< HEAD:keyboards/cannonkeys/cloudline/halconf.h
 *    `qmk chibios-confmigrate -i keyboards/cannonkeys/devastatingtkl/halconf.h -r platforms/chibios/common/configs/halconf.h`
========
 *    `qmk chibios-confmigrate -i keyboards/acheron/austin/chconf.h -r platforms/chibios/common/configs/chconf.h`
>>>>>>>> master:keyboards/xelus/rs60/rev2/chconf.h
 */

#pragma once

<<<<<<<< HEAD:keyboards/cannonkeys/cloudline/halconf.h
#define HAL_USE_PWM TRUE
========
#define CH_CFG_ST_FREQUENCY 10000

#define CH_CFG_OPTIMIZE_SPEED FALSE
>>>>>>>> master:keyboards/xelus/rs60/rev2/chconf.h

#define HAL_USE_SPI TRUE

<<<<<<<< HEAD:keyboards/cannonkeys/cloudline/halconf.h
#include_next <halconf.h>

========
#include_next <chconf.h>
>>>>>>>> master:keyboards/xelus/rs60/rev2/chconf.h
