/* Copyright 2023 @ Keychron (https://www.keychron.com)
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
#ifdef RGB_MATRIX_ENABLE
const snled27351_led_t g_snled27351_leds[RGB_MATRIX_LED_COUNT] = {
/* Refer to SNLED27351 manual for these locations
 *   driver
 *   |  R location
 *   |  |          G location
 *   |  |          |          B location
 *   |  |          |          | */
   {0, CB9_CA1,   CB7_CA1,   CB8_CA1},
   {0, CB9_CA2,   CB7_CA2,   CB8_CA2},
   {0, CB9_CA3,   CB7_CA3,   CB8_CA3},
   {0, CB9_CA4,   CB7_CA4,   CB8_CA4},
   {0, CB9_CA5,   CB7_CA5,   CB8_CA5},
   {0, CB9_CA6,   CB7_CA6,   CB8_CA6},
   {0, CB9_CA7,   CB7_CA7,   CB8_CA7},
   {0, CB9_CA8,   CB7_CA8,   CB8_CA8},
   {0, CB9_CA9,   CB7_CA9,   CB8_CA9},
   {0, CB9_CA10,  CB7_CA10,  CB8_CA10},
   {0, CB9_CA11,  CB7_CA11,  CB8_CA11},
   {0, CB9_CA12,  CB7_CA12,  CB8_CA12},
   {0, CB9_CA13,  CB7_CA13,  CB8_CA13},
   {0, CB9_CA14,  CB7_CA14,  CB8_CA14},
   {0, CB9_CA16,  CB7_CA16,  CB8_CA16},

   {0, CB3_CA1,   CB1_CA1,   CB2_CA1},
   {0, CB3_CA2,   CB1_CA2,   CB2_CA2},
   {0, CB3_CA3,   CB1_CA3,   CB2_CA3},
   {0, CB3_CA4,   CB1_CA4,   CB2_CA4},
   {0, CB3_CA5,   CB1_CA5,   CB2_CA5},
   {0, CB3_CA6,   CB1_CA6,   CB2_CA6},
   {0, CB3_CA7,   CB1_CA7,   CB2_CA7},
   {0, CB3_CA8,   CB1_CA8,   CB2_CA8},
   {0, CB3_CA9,   CB1_CA9,   CB2_CA9},
   {0, CB3_CA10,  CB1_CA10,  CB2_CA10},
   {0, CB3_CA11,  CB1_CA11,  CB2_CA11},
   {0, CB3_CA12,  CB1_CA12,  CB2_CA12},
   {0, CB3_CA13,  CB1_CA13,  CB2_CA13},
   {0, CB3_CA14,  CB1_CA14,  CB2_CA14},
   {0, CB3_CA16,  CB1_CA16,  CB2_CA16},

   {0, CB6_CA1,   CB4_CA1,   CB5_CA1},
   {0, CB6_CA2,   CB4_CA2,   CB5_CA2},
   {0, CB6_CA3,   CB4_CA3,   CB5_CA3},
   {0, CB6_CA4,   CB4_CA4,   CB5_CA4},
   {0, CB6_CA5,   CB4_CA5,   CB5_CA5},
   {0, CB6_CA6,   CB4_CA6,   CB5_CA6},
   {0, CB6_CA7,   CB4_CA7,   CB5_CA7},
   {0, CB6_CA8,   CB4_CA8,   CB5_CA8},
   {0, CB6_CA9,   CB4_CA9,   CB5_CA9},
   {0, CB6_CA10,  CB4_CA10,  CB5_CA10},
   {0, CB6_CA11,  CB4_CA11,  CB5_CA11},
   {0, CB6_CA12,  CB4_CA12,  CB5_CA12},
   {0, CB6_CA13,  CB4_CA13,  CB5_CA13},
   {0, CB6_CA14,  CB4_CA14,  CB5_CA14},
   {0, CB6_CA16,  CB4_CA16,  CB5_CA16},

   {1, CB3_CA16,  CB1_CA16,  CB2_CA16},
   {1, CB3_CA15,  CB1_CA15,  CB2_CA15},
   {1, CB3_CA14,  CB1_CA14,  CB2_CA14},
   {1, CB3_CA13,  CB1_CA13,  CB2_CA13},
   {1, CB3_CA12,  CB1_CA12,  CB2_CA12},
   {1, CB3_CA11,  CB1_CA11,  CB2_CA11},
   {1, CB3_CA10,  CB1_CA10,  CB2_CA10},
   {1, CB3_CA9,   CB1_CA9,   CB2_CA9},
   {1, CB3_CA8,   CB1_CA8,   CB2_CA8},
   {1, CB3_CA7,   CB1_CA7,   CB2_CA7},
   {1, CB3_CA6,   CB1_CA6,   CB2_CA6},
   {1, CB3_CA5,   CB1_CA5,   CB2_CA5},
   {1, CB3_CA3,   CB1_CA3,   CB2_CA3},
   {1, CB3_CA1,   CB1_CA1,   CB2_CA1},

   {1, CB9_CA16,  CB7_CA16,  CB8_CA16},
   {1, CB9_CA14,  CB7_CA14,  CB8_CA14},
   {1, CB9_CA13,  CB7_CA13,  CB8_CA13},
   {1, CB9_CA12,  CB7_CA12,  CB8_CA12},
   {1, CB9_CA11,  CB7_CA11,  CB8_CA11},
   {1, CB9_CA10,  CB7_CA10,  CB8_CA10},
   {1, CB9_CA9,   CB7_CA9,   CB8_CA9},
   {1, CB9_CA8,   CB7_CA8,   CB8_CA8},
   {1, CB9_CA7,   CB7_CA7,   CB8_CA7},
   {1, CB9_CA6,   CB7_CA6,   CB8_CA6},
   {1, CB9_CA5,   CB7_CA5,   CB8_CA5},
   {1, CB9_CA3,   CB7_CA3,   CB8_CA3},
   {1, CB9_CA2,   CB7_CA2,   CB8_CA2},

   {1, CB6_CA16,  CB4_CA16,  CB5_CA16},
   {1, CB6_CA15,  CB4_CA15,  CB5_CA15},
   {1, CB6_CA14,  CB4_CA14,  CB5_CA14},
   {1, CB6_CA10,  CB4_CA10,  CB5_CA10},
   {1, CB6_CA6,   CB4_CA6,   CB5_CA6},
   {1, CB6_CA5,   CB4_CA5,   CB5_CA5},
   {1, CB6_CA4,   CB4_CA4,   CB5_CA4},
   {1, CB6_CA3,   CB4_CA3,   CB5_CA3},
   {1, CB6_CA2,   CB4_CA2,   CB5_CA2},
   {1, CB6_CA1,   CB4_CA1,   CB5_CA1}
};

led_config_t g_led_config = {
    {
        { 0,      1,      2,      3,      4,      5,      6,      7,      8,      9,      10,     11,     12,     13,     NO_LED, 14 },
        { 15,     16,     17,     18,     19,     20,     21,     22,     23,     24,     25,     26,     27,     28,     NO_LED, 29 },
        { 30,     31,     32,     33,     34,     35,     36,     37,     38,     39,     40,     41,     42,     43,     NO_LED, 44 },
        { 45,     46,     47,     48,     49,     50,     51,     52,     53,     54,     55,     56,     NO_LED, 57,     NO_LED, 58 },
        { 59,     NO_LED, 60,     61,     62,     63,     64,     65,     66,     67,     68,     69,     NO_LED, 70,     71,     NO_LED },
        { 72,     73,     74,     NO_LED, NO_LED, NO_LED, 75,     NO_LED, NO_LED, NO_LED, 76,     77,     78,     79,     80,     81 }
    },
    {
        {0, 0}, {18, 0}, {32, 0}, {47, 0}, {62, 0}, {80, 0}, {95, 0}, {109,  0}, {124,  0}, {142,  0}, {157,  0}, {172,  0}, {186,  0}, {205,  0},            {223,  0},
        {0,14}, {14,14}, {29,14}, {43,14}, {58,14}, {73,14}, {87,14}, {102, 14}, {117, 14}, {131, 14}, {146, 14}, {161, 14}, {175, 14}, {197, 14},            {224, 14},
        {3,26}, {21,26}, {36,26}, {51,26}, {65,26}, {80,26}, {95,26}, {109, 26}, {124, 26}, {139, 26}, {153, 26}, {168, 26}, {183, 26}, {201, 26},            {224, 26},
        {5,37}, {25,37}, {40,37}, {54,37}, {69,37}, {84,37}, {98,37}, {113, 37}, {128, 37}, {142, 37}, {157, 37}, {172, 37},            {195, 37},            {224, 37},
        {9,50},          {32,50}, {47,50}, {62,50}, {76,50}, {91,50}, {106, 50}, {120, 50}, {135, 50}, {150, 50}, {164, 50},            {185, 50}, {209, 50},
        {1,62}, {20,62}, {38,62},                            {93,62},                                  {146, 62}, {161, 62}, {176, 62}, {194, 62}, {209, 62}, {224, 62}

    },
    {
        4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,        4,
        4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,        4,
        4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,        4,
        4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,        4,        4,
        4,        4,   4,   4,   4,   4,   4,   4,   4,   4,   4,        4,   4,
        4,   4,   4,                  4,                  4,   4,   4,   4,   4,   4
    }
};

// Default Color of Per Key RGB
#define DC_RED {HSV_RED}
#define DC_BLU {HSV_BLUE}
#define DC_YLW {HSV_YELLOW}

HSV default_per_key_led[RGB_MATRIX_LED_COUNT] = {
    DC_RED, DC_YLW, DC_YLW, DC_YLW, DC_YLW, DC_YLW, DC_YLW, DC_YLW, DC_YLW, DC_YLW, DC_YLW, DC_YLW, DC_YLW, DC_YLW,         DC_YLW,
    DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_YLW,         DC_YLW,
    DC_YLW, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU,         DC_YLW,
    DC_YLW, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU,         DC_RED,         DC_YLW,
    DC_YLW,         DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU, DC_BLU,         DC_YLW, DC_YLW,
    DC_YLW, DC_YLW, DC_YLW,                         DC_BLU,                         DC_YLW, DC_YLW, DC_YLW, DC_YLW, DC_YLW, DC_YLW
};

// Default mixed RGB region
uint8_t default_region[RGB_MATRIX_LED_COUNT] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    0,    0,
    0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    0, 0,
    0, 0, 0,          0,          0, 0, 0, 0, 0, 0
};
#endif
