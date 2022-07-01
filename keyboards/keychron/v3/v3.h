/* Copyright 2022 @ Keychron (https://www.keychron.com)
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

#pragma once

#include "quantum.h"

#if defined(KEYBOARD_keychron_v3_v3_ansi_stm32l432)
#    include "v3_ansi_stm32l432.h"
#elif defined(KEYBOARD_keychron_v3_v3_ansi_stm32l432_ec11)
#    include "v3_ansi_stm32l432_ec11.h"
#elif defined(KEYBOARD_keychron_v3_v3_iso_stm32l432)
#    include "v3_iso_stm32l432.h"
#elif defined(KEYBOARD_keychron_v3_v3_iso_stm32l432_ec11)
#    include "v3_iso_stm32l432_ec11.h"
#endif
