/* Copyright 2021 @ Keychron (https://www.keychron.com)
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

#include "quantum.h"

// clang-format off
#ifdef LED_MATRIX_ENABLE
const ckled2001_led g_ckled2001_leds[LED_MATRIX_LED_COUNT] = {
/* Refer to CKLED2001 manual for these locations
 *   driver
 *   |  LED address
 *   |  | */
   {0, A_16},
   {0, A_15},
   {0, A_14},
   {0, A_13},
   {0, A_12},
   {0, A_11},
   {0, A_10},
   {0, A_9},
   {0, A_8},
   {0, A_7},
   {0, A_6},
   {0, A_5},
   {0, A_4},
   {0, A_3},
   {0, A_2},
   {0, A_1},
   {0, G_1},
   {0, G_2},
   {0, G_3},

   {0, B_16},
   {0, B_15},
   {0, B_14},
   {0, B_13},
   {0, B_12},
   {0, B_11},
   {0, B_10},
   {0, B_9},
   {0, B_8},
   {0, B_7},
   {0, B_6},
   {0, B_5},
   {0, B_4},
   {0, B_3},
   {0, B_2},
   {0, B_1},
   {0, G_4},
   {0, G_5},

   {0, C_16},
   {0, C_15},
   {0, C_14},
   {0, C_13},
   {0, C_12},
   {0, C_11},
   {0, C_10},
   {0, C_9},
   {0, C_8},
   {0, C_7},
   {0, C_6},
   {0, C_5},
   {0, C_4},
   {0, C_2},
   {0, C_1},
   {0, G_6},
   {0, G_7},

   {0, D_16},
   {0, D_15},
   {0, D_14},
   {0, D_13},
   {0, D_12},
   {0, D_11},
   {0, D_10},
   {0, D_9},
   {0, D_8},
   {0, D_7},
   {0, D_6},
   {0, D_5},
   {0, D_3},
   {0, C_3},
   {0, D_2},
   {0, D_1},
   {0, G_8},

   {0, E_16},
   {0, E_15},
   {0, E_14},
   {0, E_13},
   {0, E_12},
   {0, E_11},
   {0, E_10},
   {0, E_9},
   {0, E_8},
   {0, E_7},
   {0, E_6},
   {0, E_5},
   {0, E_4},
   {0, E_3},
   {0, E_2},
   {0, E_1},
   {0, G_9},
   {0, G_10},

   {0, F_16},
   {0, F_15},
   {0, F_14},
   {0, F_10},
   {0, F_6},
   {0, F_5},
   {0, F_4},
   {0, F_3},
   {0, F_2},
   {0, F_1},
   {0, G_11},
   {0, G_12},
};

#define __ NO_LED

led_config_t g_led_config = {
	{
        // Key Matrix to LED Index
		{ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17 },
        { 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36 },
        { 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 67, 50, 51, 52, 53 },
        { 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, __, 66, 68, 69, 70, 18 },
        { 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88 },
        { 89, 90, 91, __, __, __, 92, __, __, __, 93, 94, 95, 96, 97, 98, 99,100 }
    },
    {
        // LED Index to Physical Position
        {0,0},  {12,0},  {24,0},  {36,0},  {48,0},  {60,0},  {72,0},  {85,0},  {97,0},   {109,0},  {121,0},  {133,0},  {145,0},  {157,0},  {169,0},  {188,0},  {200,0},  {212,0},  {224,0},
        {0,15}, {12,15}, {24,15}, {36,15}, {48,15}, {60,15}, {72,15}, {85,15}, {97,15},  {109,15}, {121,15}, {133,15}, {145,15}, {163,15},           {188,15}, {200,15}, {212,15}, {224,15},
        {3,26}, {18,26}, {30,26}, {42,26}, {54,26}, {66,26}, {78,26}, {91,26}, {103,26}, {115,26}, {127,26}, {139,26}, {151,26},                     {188,26}, {200,26}, {212,26}, {224,32},
        {5,38}, {21,38}, {33,38}, {45,38}, {57,38}, {69,38}, {81,38}, {94,38}, {106,38}, {118,38}, {130,38}, {142,38}, {154,38}, {170,34},           {188,38}, {200,38}, {212,38},
        {2,49}, {15,49}, {27,49}, {39,49}, {51,49}, {63,49}, {75,49}, {87,49}, {100,49}, {112,49}, {124,49}, {136,49},           {152,49}, {172,52}, {188,49}, {200,49}, {212,49}, {224,55},
        {2,61}, {17,61}, {32,61},                            {77,61},                              {121,61}, {133,61}, {145,61}, {160,64}, {172,64}, {184,64}, {200,61}, {212,61},
    },
    {
        // RGB LED Index to Flag
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4,
        1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    4, 4, 4, 4,
        1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,       4, 4, 4, 4,
        8, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,    4, 4, 4,
        1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    1, 1, 4, 4, 4, 4,
        1, 1, 1,          4,          1, 1, 1, 1, 1, 1, 4, 4,
    }
};
#endif
