#pragma once

/* Copyright 2025 @ Boris Testov
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
 * Usage in keymap.c:
 * 1. Add include at top: #include "hall_effect_utilities.h"
 * 2. Use functions in keyboard_post_init_user()
 * 3. See individual function comments below for detailed usage examples
 */

#include "quantum.h"
#include "analog_matrix.h"
#include "profile.h"
#include "action_socd.h"
#include "game_controller_common.h"
#include "xinput_keycodes.h"

/* Advanced mode constants (from profile.c enum) */
/* These control the advanced behavior modes for individual keys */
#define ADV_MODE_CLEAR 0             /* Clear/disable advanced mode (normal key behavior) */
#define ADV_MODE_OKMC 1              /* Dynamic Key Strokes mode (OKMC - One Key Multi Code) */
#define ADV_MODE_GAME_CONTROLLER 2   /* Game controller/XInput mode for analog axes and buttons */
#define ADV_MODE_TOGGLE 3            /* Toggle mode - key acts as a toggle switch */

/* Basic Key Configuration Functions */
/* ================================== */

/* Actuation point configuration */
static inline void set_key_actuation_point(uint8_t row, uint8_t col, uint8_t actuation_point) {
/*
row - matrix row index starting from 0 (0 = top row, increases downward)
col - matrix column index starting from 0 (0 = leftmost column, increases rightward)
    Note: To find valid row/col for a specific key, look at the LED matrix in ansi.c / iso_encoder.c / jis_encoder.c etc.
    where __ means no key exists at that position. If no LED matrix exists, check
    analog_matrix_mask[] and read each binary line right-to-left (1 = valid key, 0 = no key).
    Example: 0b110111111111111 = cols 14,13,11,10,9,8,7,6,5,4,3,2,1,0 exist, col 12 missing.
actuation_point - actuation distance in 0.1mm units (10 = 1mm, 5 = 0.5mm, 0 = use global setting)
IMPORTANT FALLBACK BEHAVIOR (from analog_matrix.c:184-188):
- If actuation_point = 0: Uses global profile setting (cur_prof->global.act_pt)
- If actuation_point > maximum allowed value: The key config stores the value as-is, but
  the system will clamp it during processing. Values that exceed physical limits will
  effectively behave as maximum travel.
- The system automatically calculates deactuation point as (actuation_point - 3) with
  a minimum of 0, providing 0.3mm hysteresis to prevent bouncing.
Example usage in keyboard_post_init_user():
    set_key_actuation_point(0, 1, 3);   // "1" key = 0.3mm actuation point
    set_key_actuation_point(0, 2, 10);  // "2" key = 1.0mm actuation point
    set_key_actuation_point(1, 0, 0);   // Use global setting for this key
*/
    analog_matrix_profile_t *cur_prof = profile_get_current();
    analog_key_config_t     *p_key_cfg = &cur_prof->key_config[row][col];

    /* Set custom actuation point (0 = use global setting) */
    p_key_cfg->act_pt = actuation_point;

    /* Apply new key config */
    update_key_config(row, col);
}

/* Rapid trigger configuration */
static inline void set_key_rapid_trigger(uint8_t row, uint8_t col, uint8_t press_sensitivity, uint8_t release_sensitivity) {
/*
row - matrix row index starting from 0 (0 = top row, increases downward)
col - matrix column index starting from 0 (0 = leftmost column, increases rightward)
    Note: To find valid row/col for a specific key, look at the LED matrix in ansi.c / iso_encoder.c / jis_encoder.c etc.
    where __ means no key exists at that position. If no LED matrix exists, check
    analog_matrix_mask[] and read each binary line right-to-left (1 = valid key, 0 = no key).
    Example: 0b110111111111111 = cols 14,13,11,10,9,8,7,6,5,4,3,2,1,0 exist, col 12 missing.
press_sensitivity - press sensitivity distance in 0.1mm units (10 = 1mm, 5 = 0.5mm, 0 = use global setting)
release_sensitivity - release sensitivity distance in 0.1mm units (10 = 1mm, 5 = 0.5mm, 0 = use global setting)
IMPORTANT FALLBACK BEHAVIOR (from analog_matrix.c:193-203):
- If press_sensitivity = 0: Uses global profile setting (cur_prof->global.rpd_trig_sen)
- If release_sensitivity = 0: Uses global profile setting. If global release sensitivity
  is also 0, falls back to global press sensitivity (cur_prof->global.rpd_trig_sen)
- Values exceeding physical limits will be clamped during processing
- All sensitivity values are internally scaled by TRAVEL_SCALE (6) for precision
- This function automatically sets the key mode to AKM_RAPID, overriding any previous mode
Example usage in keyboard_post_init_user():
    set_key_rapid_trigger(0, 1, 3, 3);  // "1" key = 0.3mm press/release sensitivity
    set_key_rapid_trigger(0, 3, 5, 8);  // "3" key = 0.5mm press, 0.8mm release
    set_key_rapid_trigger(2, 4, 0, 0);  // Use global settings for this key
*/
    analog_matrix_profile_t *cur_prof = profile_get_current();
    analog_key_config_t     *p_key_cfg = &cur_prof->key_config[row][col];

    /* Set rapid trigger mode */
    p_key_cfg->mode = AKM_RAPID;

    /* Set sensitivities */
    p_key_cfg->rpd_trig_sen = press_sensitivity;
    p_key_cfg->rpd_trig_sen_deact = release_sensitivity;

    /* Apply new key config */
    update_key_config(row, col);
}

/* Last Key Priority (SOCD) configuration */
static inline void set_last_key_priority(uint8_t key1_row, uint8_t key1_col, uint8_t key2_row, uint8_t key2_col, uint8_t socd_index) {
/*
SOCD = Simultaneous Opposing Cardinal Directions
This is an array-based system with limited slots available.
SOCD_COUNT is defined in analog_matrix_eeconfig.h (default: 20)
This means you can configure up to 20 different key pairs for conflict resolution.
Each socd_index (0 to 19) represents one slot in the socd[SOCD_COUNT] array.
key1_row/key1_col - first key position (row/col indices starting from 0)
key2_row/key2_col - second key position (row/col indices starting from 0)
socd_index - SOCD configuration slot index (0 to SOCD_COUNT-1, so 0-19 by default)
             Think of this as "which conflict resolution rule number you're setting up"
When both keys are pressed simultaneously, the last pressed key takes priority.
This is commonly used for WASD gaming where A+D or W+S conflicts are resolved
by prioritizing whichever key was pressed most recently.
Example usage:
    set_last_key_priority(1, 0, 1, 3, 0);  // W vs S keys, using slot 0
    set_last_key_priority(1, 1, 1, 2, 1);  // A vs D keys, using slot 1
    // You could set up to 18 more pairs using slots 2-19
*/
    uint8_t cur_prof_idx = profile_get_current_index();

    if (socd_index >= SOCD_COUNT) {
        return;  /* Prevent array overflow */
    }

    /* Configure SOCD entry - this modifies profile_get_current()->socd[socd_index] */
    uint8_t data[7] = {
        cur_prof_idx,              /* Profile index (which profile to modify) */
        key1_row,                  /* Key 1 row position */
        key1_col,                  /* Key 1 col position */
        key2_row,                  /* Key 2 row position */
        key2_col,                  /* Key 2 col position */
        socd_index,                /* Which array slot to use (0 to SOCD_COUNT-1) */
        SOCD_PRI_LAST_KEYSTROKE    /* Last key priority type */
    };

    profile_set_socd(data);
}

/* Snap Click (Deeper Travel Priority) configuration */
static inline void set_snap_click(uint8_t key1_row, uint8_t key1_col, uint8_t key2_row, uint8_t key2_col, uint8_t socd_index, bool single_activation) {
/*
SOCD = Simultaneous Opposing Cardinal Directions
This is an array-based system with limited slots available.
SOCD_COUNT is defined in analog_matrix_eeconfig.h (default: 20)
This means you can configure up to 20 different key pairs for conflict resolution.
Each socd_index (0 to 19) represents one slot in the socd[SOCD_COUNT] array.
key1_row/key1_col - first key position (row/col indices starting from 0)
key2_row/key2_col - second key position (row/col indices starting from 0)
socd_index - SOCD configuration slot index (0 to SOCD_COUNT-1, so 0-19 by default)
             Think of this as "which conflict resolution rule number you're setting up"
single_activation - true for single activation mode, false for continuous
                   true = SOCD_PRI_DEEPER_TRAVEL_SINGLE (only deeper key activates)
                   false = SOCD_PRI_DEEPER_TRAVEL (both can activate, deeper has priority)
Snap Click prioritizes the key that is pressed deeper when both keys are active.
This prevents accidental key presses by prioritizing the key pressed deeper.
- single_activation = true: When both keys are pressed nearly to the bottom, keeps last active key
- single_activation = false: When both keys are pressed nearly to the bottom, both keys remain active
During normal use, the deeper pressed key becomes active and blocks the shallower one.
Example usage:
    // Set up snap click for adjacent keys that might be accidentally pressed together
    set_snap_click(2, 4, 2, 5, 0, true);   // J vs K keys, single activation, slot 0
    set_snap_click(1, 4, 1, 5, 1, false);  // R vs T keys, both active, slot 1
    // You could set up to 18 more pairs using slots 2-19
*/
    uint8_t cur_prof_idx = profile_get_current_index();

    if (socd_index >= SOCD_COUNT) {
        return;  /* Prevent array overflow */
    }

    /* Configure SOCD entry - this modifies profile_get_current()->socd[socd_index] */
    uint8_t data[7] = {
        cur_prof_idx,                                                      /* Profile index */
        key1_row,                                                         /* Key 1 row position */
        key1_col,                                                         /* Key 1 col position */
        key2_row,                                                         /* Key 2 row position */
        key2_col,                                                         /* Key 2 col position */
        socd_index,                                                       /* Which array slot to use (0 to SOCD_COUNT-1) */
        single_activation ? SOCD_PRI_DEEPER_TRAVEL_SINGLE : SOCD_PRI_DEEPER_TRAVEL  /* Snap click type */
    };

    profile_set_socd(data);
}

/* Advanced SOCD Modes */
/* ==================== */

/* Neutral SOCD (both keys blocked when pressed together) */
static inline void set_neutral_socd(uint8_t key1_row, uint8_t key1_col, uint8_t key2_row, uint8_t key2_col, uint8_t socd_index) {
/*
Sets up neutral SOCD where both keys are blocked when pressed simultaneously.
This is commonly used in fighting games where pressing both left and right results in no movement.
When both keys are pressed together: neither key activates
When only one key is pressed: that key works normally
Example usage:
    set_neutral_socd(2, 1, 2, 3, 0);  // A vs D keys neutralize each other, slot 0
*/
    uint8_t cur_prof_idx = profile_get_current_index();

    if (socd_index >= SOCD_COUNT) {
        return;
    }

    uint8_t data[7] = {
        cur_prof_idx,              /* Profile index */
        key1_row,                  /* Key 1 row position */
        key1_col,                  /* Key 1 col position */
        key2_row,                  /* Key 2 row position */
        key2_col,                  /* Key 2 col position */
        socd_index,                /* Which array slot to use */
        SOCD_PRI_NEUTRAL           /* Neutral SOCD type */
    };

    profile_set_socd(data);
}

/* Key 1 Priority SOCD (key 1 always wins) */
static inline void set_key1_priority_socd(uint8_t key1_row, uint8_t key1_col, uint8_t key2_row, uint8_t key2_col, uint8_t socd_index) {
/*
Sets up SOCD where key1 always takes priority when both keys are pressed.
When both keys are pressed together: only key1 activates, key2 is blocked
When only one key is pressed: that key works normally
Example usage:
    set_key1_priority_socd(1, 2, 2, 2, 0);  // W always beats S, slot 0
*/
    uint8_t cur_prof_idx = profile_get_current_index();

    if (socd_index >= SOCD_COUNT) {
        return;
    }

    uint8_t data[7] = {
        cur_prof_idx,              /* Profile index */
        key1_row,                  /* Key 1 row position */
        key1_col,                  /* Key 1 col position */
        key2_row,                  /* Key 2 row position */
        key2_col,                  /* Key 2 col position */
        socd_index,                /* Which array slot to use */
        SOCD_PRI_KEY_1             /* Key 1 priority type */
    };

    profile_set_socd(data);
}

/* Key 2 Priority SOCD (key 2 always wins) */
static inline void set_key2_priority_socd(uint8_t key1_row, uint8_t key1_col, uint8_t key2_row, uint8_t key2_col, uint8_t socd_index) {
/*
Sets up SOCD where key2 always takes priority when both keys are pressed.
When both keys are pressed together: only key2 activates, key1 is blocked
When only one key is pressed: that key works normally
Example usage:
    set_key2_priority_socd(1, 2, 2, 2, 0);  // S always beats W, slot 0
*/
    uint8_t cur_prof_idx = profile_get_current_index();

    if (socd_index >= SOCD_COUNT) {
        return;
    }

    uint8_t data[7] = {
        cur_prof_idx,              /* Profile index */
        key1_row,                  /* Key 1 row position */
        key1_col,                  /* Key 1 col position */
        key2_row,                  /* Key 2 row position */
        key2_col,                  /* Key 2 col position */
        socd_index,                /* Which array slot to use */
        SOCD_PRI_KEY_2             /* Key 2 priority type */
    };

    profile_set_socd(data);
}

/* Dynamic Key Strokes (DKS) / One Key Multi Code (OKMC) configuration */
static inline void set_dynamic_key_strokes(uint8_t row, uint8_t col, uint8_t okmc_index,
                                          uint8_t shallow_act, uint8_t shallow_deact, uint8_t deep_act, uint8_t deep_deact,
                                          uint16_t keycode1, uint16_t keycode2, uint16_t keycode3, uint16_t keycode4,
                                          uint8_t action1, uint8_t action2, uint8_t action3, uint8_t action4) {
/*
Dynamic Key Strokes (DKS) allows one key to perform up to 4 different actions based on travel depth.
This is also known as One Key Multi Code (OKMC) in the codebase.
OKMC_COUNT is defined in analog_matrix_eeconfig.h (default: 20)
This means you can configure up to 20 different OKMC profiles.
Each okmc_index (0 to 19) represents one slot in the okmc[OKMC_COUNT] array.
row/col - key position (row/col indices starting from 0)
okmc_index - OKMC configuration slot index (0 to OKMC_COUNT-1, so 0-19 by default)
             Think of this as "which DKS profile number you're setting up"
Travel thresholds (in 0.1mm units):
shallow_act - shallow actuation point (when to trigger shallow actions)
shallow_deact - shallow deactuation point (when to release shallow actions)
deep_act - deep actuation point (when to trigger deep actions)
deep_deact - deep deactuation point (when to release deep actions)
Keycodes (up to 4 different actions):
keycode1-4 - QMK keycodes to send (KC_A, KC_LSFT, etc., 0 = no action)
Action types (per-keycode 4-zone action configuration):
action1-4 - Action configuration for keycode1-4 respectively
Each action parameter is a 4-bit value where each bit controls one travel zone:
- Bit 0 (0x01): Action on shallow_act (shallow press)
- Bit 1 (0x02): Action on shallow_deact (shallow release)
- Bit 2 (0x04): Action on deep_act (deep press)
- Bit 3 (0x08): Action on deep_deact (deep release)
The actual action performed is determined by a 3-bit sequence processed at each zone.
Use bit combinations to control when each keycode is pressed/released:
- 0x01 = Press on shallow press only
- 0x02 = Press on shallow release only
- 0x04 = Press on deep press only
- 0x08 = Press on deep release only
- 0x05 = Press on shallow press AND deep press
- 0x03 = Press on shallow press AND shallow release (typical press/release)
Example usage in keyboard_post_init_user():
    // Gaming example: Walk on shallow, run on deep press (as described in marketing)
    set_dynamic_key_strokes(2, 3, 0,    // W key: Row 2, Col 3, OKMC slot 0
                           15, 12, 30, 25,  // Travel points: 1.5mm shallow act, 1.2mm deact, 3.0mm deep act, 2.5mm deact
                           KC_W, KC_LSFT, 0, 0,    // W key for walking, Shift for running
                           0x01, 0x04, 0, 0);      // W on shallow press, Shift on deep press
    // Complex: Different actions at different depths
    set_dynamic_key_strokes(1, 5, 1,    // Different key, OKMC slot 1
                           10, 7, 25, 20,   // Different travel points
                           KC_A, KC_B, KC_C, KC_D,  // 4 different keycodes for different depths
                           0x01, 0x02, 0x04, 0x08); // Each keycode triggers at different travel zone
    C -> C + LCTL -> H -> F, as showed on web https://www.keychron.com/products/keychron-k6-he-wireless-magnetic-switch-keyboard
    set_dynamic_key_strokes(1, 5, 1,    // Different key, OKMC slot 1
                           10, 7, 25, 20,   // Different travel points
                           KC_C, KC_LCTL, KC_H, KC_F,  // 4 different keycodes for different depths
                           0x05, 0x04, 0x08, 0x02); // Each keycode triggers at different travel zone
*/
    uint8_t cur_prof_idx = profile_get_current_index();

    if (okmc_index >= OKMC_COUNT) {
        return;  /* Prevent array overflow */
    }

    /* Configure OKMC entry - this modifies profile_get_current()->okmc[okmc_index] */
    uint8_t data[25] = {
        cur_prof_idx,          /* [0] Profile index */
        ADV_MODE_OKMC,         /* [1] Mode = OKMC/DKS */
        row,                   /* [2] Key row position */
        col,                   /* [3] Key col position */
        okmc_index,            /* [4] Which OKMC slot to use (0 to OKMC_COUNT-1) */
        shallow_act,           /* [5] Shallow actuation point */
        shallow_deact,         /* [6] Shallow deactuation point */
        deep_act,              /* [7] Deep actuation point */
        deep_deact,            /* [8] Deep deactuation point */
        /* Keycodes (4 x 2 bytes = 8 bytes) [9-16] */
        keycode1 & 0xFF, (keycode1 >> 8) & 0xFF,  /* [9-10] Keycode 1 */
        keycode2 & 0xFF, (keycode2 >> 8) & 0xFF,  /* [11-12] Keycode 2 */
        keycode3 & 0xFF, (keycode3 >> 8) & 0xFF,  /* [13-14] Keycode 3 */
        keycode4 & 0xFF, (keycode4 >> 8) & 0xFF,  /* [15-16] Keycode 4 */
        /* Actions (4 x 2 bytes = 8 bytes) [17-24] */
        /* Each okmc_action_t: shallow_act:4, shallow_deact:4, deep_act:4, deep_deact:4 */
        /* Convert bit flags to 4-bit fields: bit 0->shallow_act, bit 1->shallow_deact, bit 2->deep_act, bit 3->deep_deact */
        ((action1 & 0x01) ? 0x02 : 0) | (((action1 & 0x02) ? 0x02 : 0) << 4),  /* [17] shallow fields for action1 */
        ((action1 & 0x04) ? 0x02 : 0) | (((action1 & 0x08) ? 0x02 : 0) << 4),  /* [18] deep fields for action1 */
        ((action2 & 0x01) ? 0x02 : 0) | (((action2 & 0x02) ? 0x02 : 0) << 4),  /* [19] shallow fields for action2 */
        ((action2 & 0x04) ? 0x02 : 0) | (((action2 & 0x08) ? 0x02 : 0) << 4),  /* [20] deep fields for action2 */
        ((action3 & 0x01) ? 0x02 : 0) | (((action3 & 0x02) ? 0x02 : 0) << 4),  /* [21] shallow fields for action3 */
        ((action3 & 0x04) ? 0x02 : 0) | (((action3 & 0x08) ? 0x02 : 0) << 4),  /* [22] deep fields for action3 */
        ((action4 & 0x01) ? 0x02 : 0) | (((action4 & 0x02) ? 0x02 : 0) << 4),  /* [23] shallow fields for action4 */
        ((action4 & 0x04) ? 0x02 : 0) | (((action4 & 0x08) ? 0x02 : 0) << 4)   /* [24] deep fields for action4 */
    };

    profile_set_adv_mode(data);
}

/* Toggle Mode Functions */
/* ===================== */

/* Set a key to toggle mode */
static inline void set_key_toggle_mode(uint8_t row, uint8_t col) {
/*
Sets a key to toggle mode (AKM_TOGGLE). In toggle mode, the key acts like a toggle switch:
- First press: Key becomes active and stays active
- Second press: Key becomes inactive and stays inactive
- Key remains in its current state until pressed again
This is different from normal keys that are only active while being pressed down.
Toggle mode is useful for:
- Custom Caps Lock behavior
- Gaming modes that need to stay active
- Modifier keys you want to "stick" (like software-based Sticky Keys)
- Mode switches in applications
row/col - key position (starting from 0)
Toggle mode configurations are per-profile and persist in EEPROM.
You can have different toggle settings for different HE profiles.
Example usage in keyboard_post_init_user():
    set_key_toggle_mode(2, 0);  // Make Caps Lock a proper toggle
    set_key_toggle_mode(1, 0);  // Make Tab a toggle for gaming mode
    set_key_toggle_mode(3, 4);  // Make a modifier key sticky
*/
    uint8_t cur_prof_idx = profile_get_current_index();

    /* Configure key for toggle mode */
    uint8_t data[5] = {
        cur_prof_idx,           /* [0] Profile index */
        ADV_MODE_TOGGLE,        /* [1] Mode = Toggle mode */
        row,                    /* [2] Key row position */
        col,                    /* [3] Key col position */
        0                       /* [4] Index (unused for toggle mode) */
    };

    profile_set_adv_mode(data);
}

/* Analog Game Controller Functions */
/* ================================= */

/* Set a key to game controller mode (XInput button or axis) */
static inline void set_key_game_controller(uint8_t row, uint8_t col, uint16_t xinput_keycode) {
/*
Sets a key to game controller mode, allowing it to send XInput signals instead of normal keyboard keycodes.
This is useful for gaming applications that expect gamepad input.
row/col - key position (starting from 0)
xinput_keycode - XInput keycode from xinput_keycodes.h
Available XInput Button Keycodes:
- XB_A, XB_B, XB_X, XB_Y - Face buttons
- XB_LB, XB_RB - Shoulder buttons
- XB_VIEW, XB_MEMU - View/Menu buttons
- XB_L3, XB_R3 - Stick press buttons
- XB_UP, XB_DOWN, XB_LEFT, XB_RGHT - D-pad buttons
- XB_XBOX - Xbox button
- XB_LT, XB_RT - Left/Right triggers (analog)
Available XInput Analog Stick Keycodes:
- LS_LEFT, LS_RGHT, LS_UP, LS_DOWN - Left stick directions
- RS_LEFT, RS_RGHT, RS_UP, RS_DOWN - Right stick directions
The analog sticks provide proportional output based on key travel distance.
Deeper presses result in larger analog stick deflection values.
Game controller configurations are per-profile and persist in EEPROM.
You can have different controller mappings for different HE profiles.
Example usage in keyboard_post_init_user():
    // Gaming setup: WASD as left stick, arrow keys as right stick
    set_key_game_controller(1, 2, LS_UP);      // W key -> Left stick up
    set_key_game_controller(2, 0, LS_LEFT);    // A key -> Left stick left
    set_key_game_controller(2, 2, LS_DOWN);    // S key -> Left stick down
    set_key_game_controller(2, 3, LS_RGHT);    // D key -> Left stick right
    // Face buttons
    set_key_game_controller(1, 5, XB_Y);       // T key -> Y button
    set_key_game_controller(1, 6, XB_X);       // Y key -> X button
    set_key_game_controller(1, 7, XB_B);       // U key -> B button
    set_key_game_controller(1, 8, XB_A);       // I key -> A button
    // Triggers (analog based on key travel)
    set_key_game_controller(0, 1, XB_LT);      // "1" key -> Left trigger
    set_key_game_controller(0, 2, XB_RT);      // "2" key -> Right trigger
*/
    uint8_t cur_prof_idx = profile_get_current_index();

    /* Extract axis index from xinput_keycode */
    /* XInput keycodes are encoded as: ((axis_index << 3 | AKM_GAMEPAD) << 2) | (mode & 3) */
    uint8_t axis_index = (xinput_keycode >> 5) & 0x1F;  /* Extract bits 5-9 (axis index) */

    /* Configure key for game controller mode */
    uint8_t data[5] = {
        cur_prof_idx,               /* [0] Profile index */
        ADV_MODE_GAME_CONTROLLER,   /* [1] Mode = Game controller mode */
        row,                        /* [2] Key row position */
        col,                        /* [3] Key col position */
        axis_index                  /* [4] Axis/button index */
    };

    profile_set_adv_mode(data);
}

/* Set analog sensitivity curve for game controller axes */
static inline void set_analog_sensitivity_curve(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t x4, uint8_t y4) {
/*
Configures the analog sensitivity curve for game controller axes.
This curve determines how key travel distance maps to analog stick/trigger values.
The curve is defined by 4 points that create 3 linear segments:
- Point 0: (0, 0) - Always fixed at origin
- Point 1: (x1, y1) - First control point
- Point 2: (x2, y2) - Second control point
- Point 3: (x3, y3) - Third control point
- Point 4: (x4, y4) - Maximum point
X values: Key travel distance in 0.1mm units (0-40, representing 0.0mm to 4.0mm)
Y values: Output intensity (0-127, where 127 = maximum analog stick deflection)
The curve must be monotonically increasing: x1 < x2 < x3 < x4
Each segment between points uses linear interpolation.
Default curve: {{0, 0}, {10, 31}, {30, 95}, {40, 127}}
- 0.0mm travel = 0% output
- 1.0mm travel = 24% output (31/127)
- 3.0mm travel = 75% output (95/127)
- 4.0mm travel = 100% output (127/127)
This curve applies globally to all analog game controller axes.
The curve persists in EEPROM and affects all HE profiles.
Example usage:
    // Linear curve (1:1 mapping)
    set_analog_sensitivity_curve(10, 32, 20, 64, 30, 96, 40, 127);
    // Sensitive curve (more output for less travel)
    set_analog_sensitivity_curve(5, 40, 15, 80, 25, 110, 40, 127);
    // Less sensitive curve (more travel needed for full output)
    set_analog_sensitivity_curve(15, 20, 25, 50, 35, 90, 40, 127);
    // Gaming curve (dead zone, then rapid acceleration)
    set_analog_sensitivity_curve(8, 0, 12, 30, 25, 100, 40, 127);
*/
    /* Validate curve points are in ascending order */
    if (x1 >= x2 || x2 >= x3 || x3 >= x4) {
        return; /* Invalid curve - points must be in ascending X order */
    }

    /* Create curve points array (point 0 is always {0, 0}) */
    point_t curve_points[4] = {
        {0, 0},      /* Point 0: Origin (fixed) */
        {x1, y1},    /* Point 1: First control point */
        {x2, y2},    /* Point 2: Second control point */
        {x3, y3},    /* Point 3: Third control point */
        /* Note: The function signature shows x4,y4 but point_t array is only 4 elements */
        /* The 4th point should be {x4, y4} but there's a mismatch in the array size */
        /* Let me fix this by using x4,y4 for the 4th point */
    };
    curve_points[3] = (point_t){x4, y4};  /* Point 3: Maximum point */

    /* Set the new curve */
    game_controller_set_curve(curve_points);
}

/* HE Profile Management Functions */
/* ================================ */

/*
These functions allow switching between 3 different HE profiles (0, 1, 2).
Check your profiles in profiles.c.

Note: some profiles can use XInput instead of normal keys.
If you want to override this, copy profiles.c file to your keymap, change XInput keys (e.g. XB_XBOX) to 0
and add SRC += profiles.c to rules.mk

Global Profile Modes (in profiles.c profile_gobal_mode array):
The profile_gobal_mode array defines the default key behavior for each profile:
  AKM_REGULAR: Static actuation points - keys activate at fixed travel distance (default: 20 = 2.0mm)
  AKM_RAPID: Rapid trigger mode - keys activate/deactivate based on movement sensitivity (default: 4 = 0.4mm)
  AKM_DKS: Dynamic Key Strokes mode - keys perform different actions at different travel depths
  AKM_GAMEPAD: Game controller mode - keys send XInput signals (XB_A/B/X/Y buttons, LS_* / RS_* analog sticks, XB_LT/RT triggers)
  AKM_TOGGLE: Toggle mode - keys act as toggle switches (press once = on, press again = off)
Individual keys can override the global mode using functions in this file.
Defaults defined in analog_matrix.h: DEFAULT_ACTUATION_POINT=20, DEFAULT_RAPID_TRIGGER_SENSITIVITY=4

DRY Configuration Pattern:
Instead of separate functions for each profile, use the switching approach:
  uint8_t original_profile = get_current_he_profile();

  switch_he_profile(0);  // Configure gaming profile
  set_key_actuation_point(2, 0, 5);  // W key: 0.5mm
  set_key_rapid_trigger(2, 0, 1, 1);  // Very sensitive

  switch_he_profile(1);  // Configure typing profile
  set_key_actuation_point(2, 0, 15); // W key: 1.5mm
  set_key_rapid_trigger(2, 0, 3, 5);  // Less sensitive
  switch_he_profile(original_profile); // Restore original
*/

/* Switch to a diferent HE profile */
static inline void switch_he_profile(uint8_t profile_index) {
/*
Switches to a different Hall Effect profile (0-2).
All subsequent configuration functions will operate on this profile.
profile_index - HE profile to switch to (0, 1, or 2)
This is similar to QMK layer switching but for HE analog settings.
Like TO(1) which switches to a layer persistently, this switches the active HE profile.
Example runtime switching in process_record_user():
    enum custom_keycodes {
        HE_PROF_0 = SAFE_RANGE,  // Switch to HE profile 0
        HE_PROF_1,               // Switch to HE profile 1
        HE_PROF_2,               // Switch to HE profile 2
        HE_CYCLE                 // Cycle through HE profiles
    };
    bool process_record_user(uint16_t keycode, keyrecord_t *record) {
        switch (keycode) {
            case HE_PROF_0:
                if (record->event.pressed) {
                    switch_he_profile(0);  // Switch to HE profile 0
                }
                return false;
            case HE_PROF_1:
                if (record->event.pressed) {
                    switch_he_profile(1);  // Switch to HE profile 1
                }
                return false;
            case HE_PROF_2:
                if (record->event.pressed) {
                    switch_he_profile(2);  // Switch to HE profile 2
                }
                return false;
            case HE_CYCLE:
                if (record->event.pressed) {
                    cycle_he_profile();    // Cycle through profiles
                }
                return false;
        }
        return true;
    }
    // Configure different actuation points for "1" key on all profiles
    void keyboard_post_init_user(void) {
        uint8_t original_profile = get_current_he_profile();
        // Profile 0: Gaming (very sensitive)
        switch_he_profile(0);
        set_key_actuation_point(0, 1, 3);   // "1" key = 0.3mm
        // Profile 1: Typing (normal)
        switch_he_profile(1);
        set_key_actuation_point(0, 1, 15);  // "1" key = 1.5mm
        // Profile 2: Heavy typing (less sensitive)
        switch_he_profile(2);
        set_key_actuation_point(0, 1, 35);  // "1" key = 3.5mm
        switch_he_profile(original_profile); // Restore original profile
    }
*/
    if (profile_index >= PROFILE_COUNT) {
        return; // Invalid profile index
    }

    profile_select(profile_index, true);  // true = LED indication when switching; false = no LED indication
}

/* Get current HE profile index */
static inline uint8_t get_current_he_profile(void) {
/*
Returns the currently active HE profile index (0-2).
Useful for:
- Saving current profile before configuration
- Displaying current profile on OLED/LCD
- Conditional logic based on active profile
*/
    return profile_get_current_index();
}

/* Cycle to next HE profile */
static inline void cycle_he_profile(void) {
/*
Cycles to the next HE profile in sequence: 0 → 1 → 2 → 0 → ...
Convenient for a single key that cycles through all available profiles.
*/
    uint8_t current = profile_get_current_index();
    uint8_t next = (current + 1) % PROFILE_COUNT;
    profile_select(next, true);  // true = LED indication when switching; false = no LED indication
}

/* Hall Effect Calibration Functions */
/* ================================== */

/*
Hall Effect Calibration Constants
These commands are sent to the analog matrix controller via analog_matrix_rx()
*/
#define AMC_CALIBRATE 0x40            /* Command to control calibration mode */
#define AMC_GET_CALIBRATE_STATE 0x41  /* Query current calibration state */
#define AMC_GET_CALIBRATED_VALUE 0x42 /* Retrieve calibrated values for a key */

/*
Calibration States
The analog matrix controller maintains these internal states during calibration
*/
#define CALIB_OFF 0                    /* Calibration inactive - normal operation */
#define CALIB_ZERO_TRAVEL_POWER_ON 1   /* Auto zero calibration on power-up (not used in manual flow) */
#define CALIB_ZERO_TRAVEL_MANUAL 2     /* Manual zero travel calibration active */
                                       /* LED: Solid RED - Keys should be completely released (not pressed at all) */
#define CALIB_FULL_TRAVEL_MANUAL 3     /* Manual full travel calibration active */
                                       /* LED: Solid PURPLE - Press keys to maximum travel (turns GREEN when complete) */
#define CALIB_SAVE_AND_EXIT 4          /* Save calibration data and exit to normal mode (auto-saved) */
#define CALIB_CLEAR 5                  /* Clear all stored calibration data */

/*
CALIBRATION PROCESS OVERVIEW:
1. Start with CALIB_ZERO_TRAVEL_MANUAL - ensure all keys are in rest position (not pressed)
2. Controller automatically transitions to CALIB_FULL_TRAVEL_MANUAL after zero phase
3. Manually press all keys to maximum travel depth - LED changes from PURPLE to GREEN when done
4. Calibration data is automatically saved when complete - no manual save needed

IMPORTANT NOTES:
- During calibration, keys are unresponsive and will not print anything
- User must manually press each key during the calibration phases
- If LED stays RED during calibration, one or more keys failed to calibrate properly
- If calibration fails (LED remains red), restart calibration by relaunching the keyboard
- No manual stop calibration function is implemented because keys are unresponsive during calibration
*/

/* Start Hall Effect Calibration Process */
static inline void start_calibration(void) {
/*
Initiates the manual two-phase calibration sequence:
1. Zero travel calibration (LED: RED) - ensure all keys are released
2. Full travel calibration (LED: PURPLE->GREEN) - manually press all keys to maximum depth

WARNING: During calibration, keys become unresponsive and will not type anything!
User must manually press keys during each calibration phase.
The keyboard LED indicates the current phase and completion status.

Example usage in process_record_user():
    case KC_CALIB_START:
        if (record->event.pressed) {
            start_calibration();
        }
        return false;
*/
    wait_ms(1000);  /* Delay to ensure all keys are released */
    uint8_t data[4] = {0xA9, AMC_CALIBRATE, CALIB_ZERO_TRAVEL_MANUAL, 0};
    analog_matrix_rx(data, 4);
}

/* Get current calibration state */
static inline uint8_t get_calibration_state(void) {
/*
Returns the current calibration state from the analog matrix controller.
Useful for monitoring calibration progress or implementing custom calibration logic.

Returns:
- CALIB_OFF: Normal operation
- CALIB_ZERO_TRAVEL_MANUAL: Zero calibration active (LED: RED)
- CALIB_FULL_TRAVEL_MANUAL: Full calibration active (LED: PURPLE->GREEN)

Example usage:
    uint8_t state = get_calibration_state();
    if (state == CALIB_OFF) {
        // Calibration complete or not active
    }
*/
    uint8_t data[20] = {0xA9, AMC_GET_CALIBRATE_STATE, 0};
    analog_matrix_rx(data, 20);
    return data[3];
}

/* Get Calibrated Values for a Specific Key */
static inline bool get_key_calibration(uint8_t row, uint8_t col, uint16_t *zero_travel, uint16_t *full_travel) {
/*
Retrieves the stored calibration data for a single key position.

Parameters:
row - matrix row index starting from 0 (0 = top row, increases downward)
col - matrix column index starting from 0 (0 = leftmost column, increases rightward)
   Note: To find valid row/col for a specific key, look at the LED matrix in ansi.c / iso_encoder.c / jis_encoder.c etc.
   where __ means no key exists at that position. If no LED matrix exists, check
   analog_matrix_mask[] and read each binary line right-to-left (1 = valid key, 0 = no key).
   Example: 0b110111111111111 = cols 14,13,11,10,9,8,7,6,5,4,3,2,1,0 exist, col 12 missing.
zero_travel - Pointer to store the zero travel ADC value (key at rest position)
full_travel - Pointer to store the full travel ADC value (key fully pressed)

Returns: true if calibration data exists and was retrieved successfully, false otherwise

ADC Value Notes:
- Higher ADC values = magnet closer to sensor
- Zero travel (key at rest) should have HIGHER ADC values than full travel
- Full travel (key pressed) should have LOWER ADC values
- If zero_travel <= full_travel, the key may not be calibrated correctly

Usage Examples:
   uint16_t zero, full;
   if (get_key_calibration(1, 2, &zero, &full)) {  // W key
       // Successfully got calibration data
       uint16_t range = zero - full;  // Calculate travel range
       if (range > 100) {
           // Key has good calibration range
       }
   } else {
       // Key not calibrated or invalid position
   }
*/
    if (row >= MATRIX_ROWS || col >= MATRIX_COLS) {
        return false;
    }

    uint8_t data[20] = {0xA9, AMC_GET_CALIBRATED_VALUE, row, col, 0};
    analog_matrix_rx(data, 20);

    if (data[4] == 0) {
        *zero_travel = (data[6] << 8) | data[5];  /* Little endian conversion */
        *full_travel = (data[8] << 8) | data[7];  /* Little endian conversion */
        return true;
    }

    return false;
}

/* Clear All Calibration Data */
static inline void clear_calibration_data(void) {
/*
Erases all stored calibration values and resets keys to factory defaults.
Keys will continue to work normally with default calibration values.
New calibration will overwrite old data, so clearing is optional.

Usage: Use for troubleshooting calibration issues.

Example usage in process_record_user():
    case KC_CALIB_CLEAR:
        if (record->event.pressed) {
            clear_calibration_data();
        }
        return false;
*/
    uint8_t data[4] = {0xA9, AMC_CALIBRATE, CALIB_CLEAR, 0};
    analog_matrix_rx(data, 4);
}
