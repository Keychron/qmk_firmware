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
#include <stdint.h>

// =============================================================================
// CONSTANTS AND DEFINES
// =============================================================================

// Advanced Mode Constants
// ========================
// These control the advanced behavior modes for individual keys (from profile.c enum)
#define ADV_MODE_CLEAR 0             // Clear/disable advanced mode (normal key behavior)
#define ADV_MODE_OKMC 1              // Dynamic Key Strokes mode (OKMC - One Key Multi Code)
#define ADV_MODE_GAME_CONTROLLER 2   // Game controller/XInput mode for analog axes and buttons
#define ADV_MODE_TOGGLE 3            // Toggle mode - key acts as a toggle switch

// Hall Effect Calibration Constants
// ==================================
// These commands are sent to the analog matrix controller via analog_matrix_rx()
#define AMC_CALIBRATE 0x40            // Command to control calibration mode
#define AMC_GET_CALIBRATE_STATE 0x41  // Query current calibration state
#define AMC_GET_CALIBRATED_VALUE 0x42 // Retrieve calibrated values for a key

// Calibration States
// ==================
// The analog matrix controller maintains these internal states during calibration
#define CALIB_OFF 0                    // Calibration inactive - normal operation
#define CALIB_ZERO_TRAVEL_POWER_ON 1   // Auto zero calibration on power-up (not used in manual flow)
#define CALIB_ZERO_TRAVEL_MANUAL 2     // Manual zero travel calibration active
                                       // LED: Solid RED - Keys should be completely released (not pressed at all)
#define CALIB_FULL_TRAVEL_MANUAL 3     // Manual full travel calibration active
                                       // LED: Solid PURPLE - Press keys to maximum travel (turns GREEN when complete)
#define CALIB_SAVE_AND_EXIT 4          // Save calibration data and exit to normal mode (auto-saved)
#define CALIB_CLEAR 5                  // Clear all stored calibration data

// =============================================================================
// FUNCTION DEFINITIONS
// =============================================================================

/*
 * ============================================================================
 * BASIC KEY CONFIGURATION FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Set custom actuation point for a specific key
 *
 * @details
 * Configures the actuation distance for an individual key, overriding the global profile setting.
 * The system automatically calculates deactuation point as (actuation_point - 3) with a minimum of 0,
 * providing 0.3mm hysteresis to prevent bouncing.
 *
 * @param row Matrix row index starting from 0 (0 = top row, increases downward)
 * @param col Matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param actuation_point Actuation distance in 0.1mm units (10 = 1mm, 5 = 0.5mm, 0 = use global setting)
 *
 * @note To find valid row/col for a specific key, look at the LED matrix in ansi.c / iso_encoder.c / jis_encoder.c etc.
 *       where __ means no key exists at that position. If no LED matrix exists, check
 *       analog_matrix_mask[] and read each binary line right-to-left (1 = valid key, 0 = no key).
 *       Example: 0b110111111111111 = cols 14,13,11,10,9,8,7,6,5,4,3,2,1,0 exist, col 12 missing.
 * @note FALLBACK BEHAVIOR: If actuation_point = 0: Uses global profile setting (cur_prof->global.act_pt)
 * @note If actuation_point > maximum allowed value: The key config stores the value as-is, but
 *       the system will clamp it during processing. Values that exceed physical limits will
 *       effectively behave as maximum travel.
 *
 * Example usage:
 * ```c
 * set_key_actuation_point(0, 1, 3);   // "1" key = 0.3mm actuation point
 * set_key_actuation_point(0, 2, 10);  // "2" key = 1.0mm actuation point
 * set_key_actuation_point(1, 0, 0);   // Use global setting for this key
 * ```
 */
static inline void set_key_actuation_point(uint8_t row, uint8_t col, uint8_t actuation_point) {
    analog_matrix_profile_t *cur_prof = profile_get_current();
    analog_key_config_t     *p_key_cfg = &cur_prof->key_config[row][col];

    // Set custom actuation point (0 = use global setting)
    p_key_cfg->act_pt = actuation_point;

    // Apply new key config
    update_key_config(row, col);
}

/**
 * @brief Configure rapid trigger sensitivity for a specific key
 *
 * @details
 * Sets up rapid trigger mode for an individual key with custom press and release sensitivities.
 * Rapid trigger activates/deactivates keys based on movement sensitivity rather than fixed positions.
 * This function automatically sets the key mode to AKM_RAPID, overriding any previous mode.
 *
 * @param row Matrix row index starting from 0 (0 = top row, increases downward)
 * @param col Matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param press_sensitivity Press sensitivity distance in 0.1mm units (10 = 1mm, 5 = 0.5mm, 0 = use global setting)
 * @param release_sensitivity Release sensitivity distance in 0.1mm units (10 = 1mm, 5 = 0.5mm, 0 = use global setting)
 *
 * @note To find valid row/col for a specific key, look at the LED matrix in ansi.c / iso_encoder.c / jis_encoder.c etc.
 *       where __ means no key exists at that position. If no LED matrix exists, check
 *       analog_matrix_mask[] and read each binary line right-to-left (1 = valid key, 0 = no key).
 *       Example: 0b110111111111111 = cols 14,13,11,10,9,8,7,6,5,4,3,2,1,0 exist, col 12 missing.
 * @note FALLBACK BEHAVIOR: If press_sensitivity = 0: Uses global profile setting (cur_prof->global.rpd_trig_sen)
 * @note If release_sensitivity = 0: Uses global profile setting. If global release sensitivity
 *       is also 0, falls back to global press sensitivity (cur_prof->global.rpd_trig_sen)
 * @note Values exceeding physical limits will be clamped during processing
 * @note All sensitivity values are internally scaled by TRAVEL_SCALE (6) for precision
 * @note MODE OVERRIDE: This function automatically sets the key mode to AKM_RAPID, overriding any previous mode
 *
 * Example usage:
 * ```c
 * set_key_rapid_trigger(0, 1, 3, 3);  // "1" key = 0.3mm press/release sensitivity
 * set_key_rapid_trigger(0, 3, 5, 8);  // "3" key = 0.5mm press, 0.8mm release
 * set_key_rapid_trigger(2, 4, 0, 0);  // Use global settings for this key
 * ```
 */
static inline void set_key_rapid_trigger(uint8_t row, uint8_t col, uint8_t press_sensitivity, uint8_t release_sensitivity) {
    analog_matrix_profile_t *cur_prof = profile_get_current();
    analog_key_config_t     *p_key_cfg = &cur_prof->key_config[row][col];

    // Set rapid trigger mode
    p_key_cfg->mode = AKM_RAPID;

    // Set sensitivities
    p_key_cfg->rpd_trig_sen = press_sensitivity;
    p_key_cfg->rpd_trig_sen_deact = release_sensitivity;

    // Apply new key config
    update_key_config(row, col);
}

/**
 * @brief Configure Last Key Priority SOCD (Simultaneous Opposing Cardinal Directions)
 *
 * @details
 * Sets up conflict resolution where the last pressed key takes priority when both keys are pressed.
 * SOCD is an array-based system with limited slots available (SOCD_COUNT = 20 by default).
 * This is commonly used for WASD gaming where A+D or W+S conflicts are resolved
 * by prioritizing whichever key was pressed most recently.
 *
 * @param key1_row First key matrix row index starting from 0 (0 = top row, increases downward)
 * @param key1_col First key matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param key2_row Second key matrix row index starting from 0 (0 = top row, increases downward)
 * @param key2_col Second key matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param socd_index SOCD configuration slot index (0 to SOCD_COUNT-1, so 0-19 by default)
 *
 * @note To find valid row/col for a specific key, look at the LED matrix in ansi.c / iso_encoder.c / jis_encoder.c etc.
 *       where __ means no key exists at that position. If no LED matrix exists, check
 *       analog_matrix_mask[] and read each binary line right-to-left (1 = valid key, 0 = no key).
 *       Example: 0b110111111111111 = cols 14,13,11,10,9,8,7,6,5,4,3,2,1,0 exist, col 12 missing.
 * @note SOCD_COUNT is defined in analog_matrix_eeconfig.h (default: 20)
 * @note Each socd_index represents one slot in the socd[SOCD_COUNT] array
 * @note Think of socd_index as "which conflict resolution rule number you're setting up"
 * @note When both keys are pressed simultaneously, the last pressed key takes priority
 *
 * Example usage:
 * ```c
 * set_last_key_priority(1, 0, 1, 3, 0);  // W vs S keys, using slot 0
 * set_last_key_priority(1, 1, 1, 2, 1);  // A vs D keys, using slot 1
 * // You could set up to 18 more pairs using slots 2-19
 * ```
 */
static inline void set_last_key_priority(uint8_t key1_row, uint8_t key1_col, uint8_t key2_row, uint8_t key2_col, uint8_t socd_index) {
    uint8_t cur_prof_idx = profile_get_current_index();

    if (socd_index >= SOCD_COUNT) return;  // Prevent array overflow

    // Configure SOCD entry - this modifies profile_get_current()->socd[socd_index]
    uint8_t data[7] = {
        cur_prof_idx,              // Profile index (which profile to modify)
        key1_row,                  // Key 1 row position
        key1_col,                  // Key 1 col position
        key2_row,                  // Key 2 row position
        key2_col,                  // Key 2 col position
        socd_index,                // Which array slot to use (0 to SOCD_COUNT-1)
        SOCD_PRI_LAST_KEYSTROKE    // Last key priority type
    };

    profile_set_socd(data);
}

/**
 * @brief Configure Snap Click (Deeper Travel Priority) SOCD
 *
 * @details
 * Sets up conflict resolution where the key pressed deeper takes priority when both keys are active.
 * Snap Click prevents accidental key presses by prioritizing the key pressed deeper.
 * During normal use, the deeper pressed key becomes active and blocks the shallower one.
 *
 * @param key1_row First key matrix row index starting from 0 (0 = top row, increases downward)
 * @param key1_col First key matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param key2_row Second key matrix row index starting from 0 (0 = top row, increases downward)
 * @param key2_col Second key matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param socd_index SOCD configuration slot index (0 to SOCD_COUNT-1, so 0-19 by default)
 * @param single_activation true for single activation mode, false for continuous
 *
 * @note To find valid row/col for a specific key, look at the LED matrix in ansi.c / iso_encoder.c / jis_encoder.c etc.
 *       where __ means no key exists at that position. If no LED matrix exists, check
 *       analog_matrix_mask[] and read each binary line right-to-left (1 = valid key, 0 = no key).
 *       Example: 0b110111111111111 = cols 14,13,11,10,9,8,7,6,5,4,3,2,1,0 exist, col 12 missing.
 * @note SOCD_COUNT is defined in analog_matrix_eeconfig.h (default: 20)
 * @note Each socd_index represents one slot in the socd[SOCD_COUNT] array
 * @note Think of socd_index as "which conflict resolution rule number you're setting up"
 * @note single_activation = true: SOCD_PRI_DEEPER_TRAVEL_SINGLE (only deeper key activates)
 * @note single_activation = false: SOCD_PRI_DEEPER_TRAVEL (both can activate, deeper has priority)
 * @note single_activation = true: When both keys are pressed nearly to the bottom, keeps last active key
 * @note single_activation = false: When both keys are pressed nearly to the bottom, both keys remain active
 *
 * Example usage:
 * ```c
 * // Set up snap click for adjacent keys that might be accidentally pressed together
 * set_snap_click(2, 4, 2, 5, 0, true);   // J vs K keys, single activation, slot 0
 * set_snap_click(1, 4, 1, 5, 1, false);  // R vs T keys, both active, slot 1
 * // You could set up to 18 more pairs using slots 2-19
 * ```
 */
static inline void set_snap_click(uint8_t key1_row, uint8_t key1_col, uint8_t key2_row, uint8_t key2_col, uint8_t socd_index, bool single_activation) {
    uint8_t cur_prof_idx = profile_get_current_index();

    if (socd_index >= SOCD_COUNT) return;  // Prevent array overflow

    // Configure SOCD entry - this modifies profile_get_current()->socd[socd_index]
    uint8_t data[7] = {
        cur_prof_idx,                                                      // Profile index
        key1_row,                                                         // Key 1 row position
        key1_col,                                                         // Key 1 col position
        key2_row,                                                         // Key 2 row position
        key2_col,                                                         // Key 2 col position
        socd_index,                                                       // Which array slot to use (0 to SOCD_COUNT-1)
        single_activation ? SOCD_PRI_DEEPER_TRAVEL_SINGLE : SOCD_PRI_DEEPER_TRAVEL  // Snap click type
    };

    profile_set_socd(data);
}

/*
 * ============================================================================
 * ADVANCED SOCD MODES
 * ============================================================================
 */

/**
 * @brief Configure Neutral SOCD (Simultaneous Opposing Cardinal Directions)
 *
 * @details
 * Sets up neutral SOCD where both keys are blocked when pressed simultaneously.
 * SOCD is an array-based system with limited slots available (SOCD_COUNT = 20 by default).
 * This is commonly used in fighting games where pressing both left and right results in no movement
 * to prevent directional conflicts.
 *
 * @param key1_row First key matrix row index starting from 0 (0 = top row, increases downward)
 * @param key1_col First key matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param key2_row Second key matrix row index starting from 0 (0 = top row, increases downward)
 * @param key2_col Second key matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param socd_index SOCD configuration slot index (0 to SOCD_COUNT-1, so 0-19 by default)
 *
 * @note To find valid row/col for a specific key, look at the LED matrix in ansi.c / iso_encoder.c / jis_encoder.c etc.
 *       where __ means no key exists at that position. If no LED matrix exists, check
 *       analog_matrix_mask[] and read each binary line right-to-left (1 = valid key, 0 = no key).
 *       Example: 0b110111111111111 = cols 14,13,11,10,9,8,7,6,5,4,3,2,1,0 exist, col 12 missing.
 * @note SOCD_COUNT is defined in analog_matrix_eeconfig.h (default: 20)
 * @note Each socd_index represents one slot in the socd[SOCD_COUNT] array
 * @note Think of socd_index as "which conflict resolution rule number you're setting up"
 * @note When both keys are pressed simultaneously, neither key activates (neutral state)
 *
 * Example usage:
 * ```c
 * set_neutral_socd(2, 1, 2, 3, 0);  // A vs D keys neutralize each other, slot 0
 * set_neutral_socd(1, 4, 1, 18, 1); // W vs S keys neutralize each other, slot 1
 * ```
 */
static inline void set_neutral_socd(uint8_t key1_row, uint8_t key1_col, uint8_t key2_row, uint8_t key2_col, uint8_t socd_index) {
    uint8_t cur_prof_idx = profile_get_current_index();

    if (socd_index >= SOCD_COUNT) return;

    uint8_t data[7] = {
        cur_prof_idx,              // Profile index
        key1_row,                  // Key 1 row position
        key1_col,                  // Key 1 col position
        key2_row,                  // Key 2 row position
        key2_col,                  // Key 2 col position
        socd_index,                // Which array slot to use
        SOCD_PRI_NEUTRAL           // Neutral SOCD type
    };

    profile_set_socd(data);
}

/**
 * @brief Configure Key 1 Priority SOCD (Simultaneous Opposing Cardinal Directions)
 *
 * @details
 * Sets up SOCD where key1 always takes priority when both keys are pressed.
 * SOCD is an array-based system with limited slots available (SOCD_COUNT = 20 by default).
 * This creates a hierarchy where one key dominates another in conflict situations.
 *
 * @param key1_row First key matrix row index starting from 0 (0 = top row, increases downward)
 * @param key1_col First key matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param key2_row Second key matrix row index starting from 0 (0 = top row, increases downward)
 * @param key2_col Second key matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param socd_index SOCD configuration slot index (0 to SOCD_COUNT-1, so 0-19 by default)
 *
 * @note To find valid row/col for a specific key, look at the LED matrix in ansi.c / iso_encoder.c / jis_encoder.c etc.
 *       where __ means no key exists at that position. If no LED matrix exists, check
 *       analog_matrix_mask[] and read each binary line right-to-left (1 = valid key, 0 = no key).
 *       Example: 0b110111111111111 = cols 14,13,11,10,9,8,7,6,5,4,3,2,1,0 exist, col 12 missing.
 * @note SOCD_COUNT is defined in analog_matrix_eeconfig.h (default: 20)
 * @note Each socd_index represents one slot in the socd[SOCD_COUNT] array
 * @note Think of socd_index as "which conflict resolution rule number you're setting up"
 * @note When both keys are pressed together: only key1 activates, key2 is blocked
 *
 * Example usage:
 * ```c
 * set_key1_priority_socd(1, 2, 2, 2, 0);  // W always beats S, slot 0
 * set_key1_priority_socd(2, 1, 2, 3, 1);  // A always beats D, slot 1
 * ```
 */
static inline void set_key1_priority_socd(uint8_t key1_row, uint8_t key1_col, uint8_t key2_row, uint8_t key2_col, uint8_t socd_index) {
    uint8_t cur_prof_idx = profile_get_current_index();

    if (socd_index >= SOCD_COUNT) return;

    uint8_t data[7] = {
        cur_prof_idx,              // Profile index
        key1_row,                  // Key 1 row position
        key1_col,                  // Key 1 col position
        key2_row,                  // Key 2 row position
        key2_col,                  // Key 2 col position
        socd_index,                // Which array slot to use
        SOCD_PRI_KEY_1             // Key 1 priority type
    };

    profile_set_socd(data);
}

/**
 * @brief Configure Key 2 Priority SOCD (Simultaneous Opposing Cardinal Directions)
 *
 * @details
 * Sets up SOCD where key2 always takes priority when both keys are pressed.
 * SOCD is an array-based system with limited slots available (SOCD_COUNT = 20 by default).
 * This creates a hierarchy where the second key dominates the first in conflict situations.
 *
 * @param key1_row First key matrix row index starting from 0 (0 = top row, increases downward)
 * @param key1_col First key matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param key2_row Second key matrix row index starting from 0 (0 = top row, increases downward)
 * @param key2_col Second key matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param socd_index SOCD configuration slot index (0 to SOCD_COUNT-1, so 0-19 by default)
 *
 * @note To find valid row/col for a specific key, look at the LED matrix in ansi.c / iso_encoder.c / jis_encoder.c etc.
 *       where __ means no key exists at that position. If no LED matrix exists, check
 *       analog_matrix_mask[] and read each binary line right-to-left (1 = valid key, 0 = no key).
 *       Example: 0b110111111111111 = cols 14,13,11,10,9,8,7,6,5,4,3,2,1,0 exist, col 12 missing.
 * @note SOCD_COUNT is defined in analog_matrix_eeconfig.h (default: 20)
 * @note Each socd_index represents one slot in the socd[SOCD_COUNT] array
 * @note Think of socd_index as "which conflict resolution rule number you're setting up"
 * @note When both keys are pressed together: only key2 activates, key1 is blocked
 *
 * Example usage:
 * ```c
 * set_key2_priority_socd(1, 2, 2, 2, 0);  // S always beats W, slot 0
 * set_key2_priority_socd(2, 1, 2, 3, 1);  // D always beats A, slot 1
 * ```
 */
static inline void set_key2_priority_socd(uint8_t key1_row, uint8_t key1_col, uint8_t key2_row, uint8_t key2_col, uint8_t socd_index) {
    uint8_t cur_prof_idx = profile_get_current_index();

    if (socd_index >= SOCD_COUNT) return;

    uint8_t data[7] = {
        cur_prof_idx,              // Profile index
        key1_row,                  // Key 1 row position
        key1_col,                  // Key 1 col position
        key2_row,                  // Key 2 row position
        key2_col,                  // Key 2 col position
        socd_index,                // Which array slot to use
        SOCD_PRI_KEY_2             // Key 2 priority type
    };

    profile_set_socd(data);
}

/**
 * @brief Configure Dynamic Key Strokes (DKS) / One Key Multi Code (OKMC)
 *
 * @details
 * Dynamic Key Strokes (DKS) allows one key to perform up to 4 different actions based on travel depth.
 * This is also known as One Key Multi Code (OKMC) in the codebase.
 * OKMC is an array-based system with limited slots available (OKMC_COUNT = 20 by default).
 *
 * Each action parameter is a 4-bit value where each bit controls one travel zone:
 * - Bit 0 (0x01): Add key on shallow_act (shallow press)
 * - Bit 1 (0x02): Add key on shallow_deact (shallow release)
 * - Bit 2 (0x04): Add key on deep_act (deep press)
 * - Bit 3 (0x08): Add key on deep_deact (deep release)
 *
 * IMPORTANT: OKMC uses ADDITIVE ACCUMULATION behavior:
 * - Keys get ADDED at each travel zone (never individually removed)
 * - Adding the same key multiple times has no effect (it's already active)
 * - ALL keys are released together when the physical key fully releases
 *
 * @param row Key matrix row index starting from 0 (0 = top row, increases downward)
 * @param col Key matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param okmc_index OKMC configuration slot index (0 to OKMC_COUNT-1, so 0-19 by default)
 * @param shallow_act Shallow actuation point in 0.1mm units (when to trigger shallow actions)
 * @param shallow_deact Shallow deactuation point in 0.1mm units (when to release shallow actions)
 * @param deep_act Deep actuation point in 0.1mm units (when to trigger deep actions)
 * @param deep_deact Deep deactuation point in 0.1mm units (when to release deep actions)
 * @param keycode1 First QMK keycode to send (KC_A, KC_LSFT, etc., 0 = no action)
 * @param keycode2 Second QMK keycode to send (KC_A, KC_LSFT, etc., 0 = no action)
 * @param keycode3 Third QMK keycode to send (KC_A, KC_LSFT, etc., 0 = no action)
 * @param keycode4 Fourth QMK keycode to send (KC_A, KC_LSFT, etc., 0 = no action)
 * @param action1 Action configuration for keycode1 (4-bit value controlling travel zones, 0 = skip)
 * @param action2 Action configuration for keycode2 (4-bit value controlling travel zones, 0 = skip)
 * @param action3 Action configuration for keycode3 (4-bit value controlling travel zones, 0 = skip)
 * @param action4 Action configuration for keycode4 (4-bit value controlling travel zones, 0 = skip)
 *
 * @note To find valid row/col for a specific key, look at the LED matrix in ansi.c / iso_encoder.c / jis_encoder.c etc.
 *       where __ means no key exists at that position. If no LED matrix exists, check
 *       analog_matrix_mask[] and read each binary line right-to-left (1 = valid key, 0 = no key).
 *       Example: 0b110111111111111 = cols 14,13,11,10,9,8,7,6,5,4,3,2,1,0 exist, col 12 missing.
 * @note OKMC_COUNT is defined in analog_matrix_eeconfig.h (default: 20)
 * @note Each okmc_index represents one slot in the okmc[OKMC_COUNT] array
 * @note Think of okmc_index as "which DKS profile number you're setting up"
 *
 * Common action patterns:
 * - 0x01 = Add key on shallow press
 * - 0x02 = Add key on shallow release
 * - 0x04 = Add key on deep press
 * - 0x08 = Add key on deep release
 * - 0 = Skip this keycode (no action)
 *
 * Example usage:
 * ```c
 * // Gaming example: Walk on shallow, run on deep press
 * set_dynamic_key_strokes(2, 3, 0,    // W key: Row 2, Col 3, OKMC slot 0
 *                        15, 12, 30, 25,  // Travel: 1.5mm shallow act, 1.2mm shallow deact, 3.0mm deep act, 2.5mm deep deact
 *                        KC_W, KC_LSFT, 0, 0,    // W key for walking, Shift for running
 *                        0x01, 0x04, 0, 0);      // W on shallow press, Shift on deep press, skip unused slots
 *
 * // Sequential accumulation: Each keycode gets added at different travel zones
 * // SHALLOW PRESS → SHALLOW RELEASE → DEEP PRESS → DEEP RELEASE
 * //       A       →      A+B        →   A+B+C    →   A+B+C+D     → all released
 * set_dynamic_key_strokes(1, 5, 1,    // Different key, OKMC slot 1
 *                        10, 7, 25, 20,   // Different travel points
 *                        KC_A, KC_B, KC_C, KC_D,  // 4 different keycodes
 *                        0x01, 0x02, 0x04, 0x08); // Add at: shallow press, shallow release, deep press, deep release
 *
 * // Keychron marketing example - shows actual behavior vs accumulation
 * // SHALLOW PRESS → DEEP PRESS → DEEP RELEASE → SHALLOW RELEASE
 * //       C       →   C+LCTL   →  C+LCTL+H    →   C+LCTL+H+F    → all released
 * // Note: C+LCTL gets replaced by H, then H gets replaced by F (not true accumulation)
 * set_dynamic_key_strokes(1, 5, 1,    // Different key, OKMC slot 1
 *                        10, 7, 25, 20,   // Different travel points
 *                        KC_C, KC_LCTL, KC_H, KC_F,  // 4 different keycodes for different depths
 *                        0x01, 0x04, 0x08, 0x02); // Add at: shallow press, deep press, deep release, shallow release
 * ```
 */
static inline void set_dynamic_key_strokes(uint8_t row, uint8_t col, uint8_t okmc_index,
                                          uint8_t shallow_act, uint8_t shallow_deact, uint8_t deep_act, uint8_t deep_deact,
                                          uint16_t keycode1, uint16_t keycode2, uint16_t keycode3, uint16_t keycode4,
                                          uint8_t action1, uint8_t action2, uint8_t action3, uint8_t action4) {
    uint8_t cur_prof_idx = profile_get_current_index();

    if (okmc_index >= OKMC_COUNT) return;  // Prevent array overflow

    // Configure OKMC entry - this modifies profile_get_current()->okmc[okmc_index]
    uint8_t data[25] = {
        cur_prof_idx,           // [0] Profile index
        ADV_MODE_OKMC,         // [1] Mode = OKMC/DKS
        row,                   // [2] Key row position
        col,                   // [3] Key col position
        okmc_index,            // [4] Which OKMC slot to use (0 to OKMC_COUNT-1)
        shallow_act,           // [5] Shallow actuation point
        shallow_deact,         // [6] Shallow deactuation point
        deep_act,              // [7] Deep actuation point
        deep_deact,            // [8] Deep deactuation point
        // Keycodes (4 x 2 bytes = 8 bytes) [9-16]
        keycode1 & 0xFF, (keycode1 >> 8) & 0xFF,  // [9-10] Keycode 1
        keycode2 & 0xFF, (keycode2 >> 8) & 0xFF,  // [11-12] Keycode 2
        keycode3 & 0xFF, (keycode3 >> 8) & 0xFF,  // [13-14] Keycode 3
        keycode4 & 0xFF, (keycode4 >> 8) & 0xFF,  // [15-16] Keycode 4
        // Actions (4 x 2 bytes = 8 bytes) [17-24]
        // Each okmc_action_t: shallow_act:4, shallow_deact:4, deep_act:4, deep_deact:4
        // Convert bit flags to 4-bit fields: bit 0->shallow_act, bit 1->shallow_deact, bit 2->deep_act, bit 3->deep_deact
        ((action1 & 0x01) ? 0x02 : 0) | (((action1 & 0x02) ? 0x02 : 0) << 4),  // [17] shallow fields for action1
        ((action1 & 0x04) ? 0x02 : 0) | (((action1 & 0x08) ? 0x02 : 0) << 4),  // [18] deep fields for action1
        ((action2 & 0x01) ? 0x02 : 0) | (((action2 & 0x02) ? 0x02 : 0) << 4),  // [19] shallow fields for action2
        ((action2 & 0x04) ? 0x02 : 0) | (((action2 & 0x08) ? 0x02 : 0) << 4),  // [20] deep fields for action2
        ((action3 & 0x01) ? 0x02 : 0) | (((action3 & 0x02) ? 0x02 : 0) << 4),  // [21] shallow fields for action3
        ((action3 & 0x04) ? 0x02 : 0) | (((action3 & 0x08) ? 0x02 : 0) << 4),  // [22] deep fields for action3
        ((action4 & 0x01) ? 0x02 : 0) | (((action4 & 0x02) ? 0x02 : 0) << 4),  // [23] shallow fields for action4
        ((action4 & 0x04) ? 0x02 : 0) | (((action4 & 0x08) ? 0x02 : 0) << 4)   // [24] deep fields for action4
    };

    profile_set_adv_mode(data);
}

/*
 * ============================================================================
 * TOGGLE MODE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Set a key to toggle mode (AKM_TOGGLE)
 *
 * @details
 * Sets a key to toggle mode where the key acts like a toggle switch.
 * This is different from normal keys that are only active while being pressed down.
 *
 * Toggle behavior:
 * - First press: Key becomes active and stays active
 * - Second press: Key becomes inactive and stays inactive
 * - Key remains in its current state until pressed again
 *
 * Toggle mode is useful for:
 * - Custom Caps Lock behavior
 * - Gaming modes that need to stay active
 * - Modifier keys you want to "stick" (like software-based Sticky Keys)
 * - Mode switches in applications
 *
 * @param row Key matrix row index starting from 0 (0 = top row, increases downward)
 * @param col Key matrix column index starting from 0 (0 = leftmost column, increases rightward)
 *
 * @note To find valid row/col for a specific key, look at the LED matrix in ansi.c / iso_encoder.c / jis_encoder.c etc.
 *       where __ means no key exists at that position. If no LED matrix exists, check
 *       analog_matrix_mask[] and read each binary line right-to-left (1 = valid key, 0 = no key).
 *       Example: 0b110111111111111 = cols 14,13,11,10,9,8,7,6,5,4,3,2,1,0 exist, col 12 missing.
 * @note Toggle mode configurations are per-profile and persist in EEPROM
 * @note You can have different toggle settings for different HE profiles
 *
 * Example usage:
 * ```c
 * set_key_toggle_mode(2, 0);  // Make Caps Lock a proper toggle
 * set_key_toggle_mode(1, 0);  // Make Tab a toggle for gaming mode
 * set_key_toggle_mode(3, 4);  // Make a modifier key sticky
 * ```
 */
static inline void set_key_toggle_mode(uint8_t row, uint8_t col) {
    uint8_t cur_prof_idx = profile_get_current_index();

    // Configure key for toggle mode
    uint8_t data[5] = {
        cur_prof_idx,           // [0] Profile index
        ADV_MODE_TOGGLE,        // [1] Mode = Toggle mode
        row,                    // [2] Key row position
        col,                    // [3] Key col position
        0                       // [4] Index (unused for toggle mode)
    };

    profile_set_adv_mode(data);
}

/*
 * ============================================================================
 * ANALOG GAME CONTROLLER FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Set a key to game controller mode (XInput button or axis)
 *
 * @details
 * Sets a key to game controller mode, allowing it to send XInput signals instead of normal keyboard keycodes.
 * This is useful for gaming applications that expect gamepad input.
 *
 * The analog sticks provide proportional output based on key travel distance.
 * Deeper presses result in larger analog stick deflection values.
 *
 * Available XInput Button Keycodes:
 * - XB_A, XB_B, XB_X, XB_Y - Face buttons
 * - XB_LB, XB_RB - Shoulder buttons
 * - XB_VIEW, XB_MEMU - View/Menu buttons
 * - XB_L3, XB_R3 - Stick press buttons
 * - XB_UP, XB_DOWN, XB_LEFT, XB_RGHT - D-pad buttons
 * - XB_XBOX - Xbox button
 * - XB_LT, XB_RT - Left/Right triggers (analog)
 *
 * Available XInput Analog Stick Keycodes:
 * - LS_LEFT, LS_RGHT, LS_UP, LS_DOWN - Left stick directions
 * - RS_LEFT, RS_RGHT, RS_UP, RS_DOWN - Right stick directions
 *
 * @param row Key matrix row index starting from 0 (0 = top row, increases downward)
 * @param col Key matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param xinput_keycode XInput keycode from xinput_keycodes.h
 *
 * @note To find valid row/col for a specific key, look at the LED matrix in ansi.c / iso_encoder.c / jis_encoder.c etc.
 *       where __ means no key exists at that position. If no LED matrix exists, check
 *       analog_matrix_mask[] and read each binary line right-to-left (1 = valid key, 0 = no key).
 *       Example: 0b110111111111111 = cols 14,13,11,10,9,8,7,6,5,4,3,2,1,0 exist, col 12 missing.
 * @note Game controller configurations are per-profile and persist in EEPROM
 * @note You can have different controller mappings for different HE profiles
 *
 * Example usage:
 * ```c
 * // Gaming setup: WASD as left stick, arrow keys as right stick
 * set_key_game_controller(1, 2, LS_UP);      // W key -> Left stick up
 * set_key_game_controller(2, 0, LS_LEFT);    // A key -> Left stick left
 * set_key_game_controller(2, 2, LS_DOWN);    // S key -> Left stick down
 * set_key_game_controller(2, 3, LS_RGHT);    // D key -> Left stick right
 *
 * // Face buttons
 * set_key_game_controller(1, 5, XB_Y);       // T key -> Y button
 * set_key_game_controller(1, 6, XB_X);       // Y key -> X button
 * set_key_game_controller(1, 7, XB_B);       // U key -> B button
 * set_key_game_controller(1, 8, XB_A);       // I key -> A button
 *
 * // Triggers (analog based on key travel)
 * set_key_game_controller(0, 1, XB_LT);      // "1" key -> Left trigger
 * set_key_game_controller(0, 2, XB_RT);      // "2" key -> Right trigger
 * ```
 */
static inline void set_key_game_controller(uint8_t row, uint8_t col, uint16_t xinput_keycode) {
    uint8_t cur_prof_idx = profile_get_current_index();

    // Extract axis index from xinput_keycode
    // XInput keycodes are encoded as: ((axis_index << 3 | AKM_GAMEPAD) << 2) | (mode & 3)
    uint8_t axis_index = (xinput_keycode >> 5) & 0x1F;  // Extract bits 5-9 (axis index)

    // Configure key for game controller mode
    uint8_t data[5] = {
        cur_prof_idx,               // [0] Profile index
        ADV_MODE_GAME_CONTROLLER,   // [1] Mode = Game controller mode
        row,                        // [2] Key row position
        col,                        // [3] Key col position
        axis_index                  // [4] Axis/button index
    };

    profile_set_adv_mode(data);
}

/**
 * @brief Set analog sensitivity curve for game controller axes
 *
 * @details
 * Configures the analog sensitivity curve that determines how key travel distance
 * maps to analog stick/trigger values for game controller mode. The curve is defined
 * by 4 control points that create 3 linear segments, with (0,0) as the fixed origin.
 *
 * @param x1 First control point X coordinate (key travel in 0.1mm units, 0-40)
 * @param y1 First control point Y coordinate (output intensity 0-127)
 * @param x2 Second control point X coordinate (key travel in 0.1mm units, 0-40)
 * @param y2 Second control point Y coordinate (output intensity 0-127)
 * @param x3 Third control point X coordinate (key travel in 0.1mm units, 0-40)
 * @param y3 Third control point Y coordinate (output intensity 0-127)
 * @param x4 Fourth control point X coordinate (key travel in 0.1mm units, 0-40)
 * @param y4 Fourth control point Y coordinate (output intensity 0-127)
 *
 * @note - Points must be in ascending X order: x1 < x2 < x3 < x4
 * @note - X values: 0-40 representing 0.0mm to 4.0mm travel distance
 * @note - Y values: 0-127 where 127 = maximum analog output
 * @note - Curve applies globally to all analog game controller axes and persists in EEPROM
 * @note - By default curve settings are (0, 0), (10,31), (30,95), (40,127)
 *
 * Example usage:
 * ```c
 * //Linear curve (1:1 mapping) - smooth and predictable response
 * set_analog_sensitivity_curve(10, 32, 20, 64, 30, 96, 40, 127);
 *
 * // Sensitive curve (more output for less travel) - great for racing games
 * set_analog_sensitivity_curve(5, 40, 15, 80, 25, 110, 40, 127);
 *
 * // Gaming curve with dead zone - prevents accidental inputs
 * set_analog_sensitivity_curve(8, 0, 12, 30, 25, 100, 40, 127);
 *
 * // Typing-friendly curve - minimal sensitivity until deeper travel
 * set_analog_sensitivity_curve(12, 10, 20, 30, 30, 70, 40, 127);
 * ```
 */
static inline void set_analog_sensitivity_curve(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t x4, uint8_t y4) {
    // Validate curve points are in ascending order
    if (x1 >= x2 || x2 >= x3 || x3 >= x4) {
        return; // Invalid curve - points must be in ascending X order
    }

    // Create curve points array (point 0 is always {0, 0})
    point_t curve_points[4] = {
        {x1, y1},    // Point 1: First control point
        {x2, y2},    // Point 2: Second control point
        {x3, y3},    // Point 3: Third control point
        {x4, y4}
    };

    // Set the new curve
    game_controller_set_curve(curve_points);
}

/*
 * ============================================================================
 * HE PROFILE MANAGEMENT FUNCTIONS
 * ============================================================================
 *
 * These functions allow switching between 3 different HE profiles (0, 1, 2).
 * Check your profiles in profiles.c.
 *
 * Note: some profiles can use XInput instead of normal keys.
 * If you want to override this, copy profiles.c file to your keymap, change XInput keys (e.g. XB_XBOX) to 0
 * and add SRC += profiles.c to rules.mk
 *
 * Global Profile Modes (in profiles.c profile_gobal_mode array):
 * The profile_gobal_mode array defines the default key behavior for each profile:
 *   AKM_REGULAR: Static actuation points - keys activate at fixed travel distance (default: 20 = 2.0mm)
 *   AKM_RAPID: Rapid trigger mode - keys activate/deactivate based on movement sensitivity (default: 4 = 0.4mm)
 *   AKM_DKS: Dynamic Key Strokes mode - keys perform different actions at different travel depths
 *   AKM_GAMEPAD: Game controller mode - keys send XInput signals (XB_A/B/X/Y buttons, LS_x/RS_x analog sticks, XB_LT/RT triggers)
 *   AKM_TOGGLE: Toggle mode - keys act as toggle switches (press once = on, press again = off)
 * Individual keys can override the global mode using functions in this file.
 * Defaults defined in analog_matrix.h: DEFAULT_ACTUATION_POINT=20, DEFAULT_RAPID_TRIGGER_SENSITIVITY=4
 *
 * DRY Configuration Pattern:
 * Instead of separate functions for each profile, use the switching approach:
 *   uint8_t original_profile = get_current_he_profile();
 *
 *   switch_he_profile(0);  // Configure gaming profile
 *   set_key_actuation_point(2, 0, 5);  // W key: 0.5mm
 *   set_key_rapid_trigger(2, 0, 1, 1);  // Very sensitive
 *
 *   switch_he_profile(1);  // Configure typing profile
 *   set_key_actuation_point(2, 0, 15); // W key: 1.5mm
 *   set_key_rapid_trigger(2, 0, 3, 5);  // Less sensitive
 *
 *   switch_he_profile(original_profile); // Restore original
 */

/**
 * @brief Switch to a different Hall Effect profile
 *
 * @details
 * Switches to a different Hall Effect profile (0-2).
 * All subsequent configuration functions will operate on this profile.
 * This is similar to QMK layer switching but for HE analog settings.
 * Like TO(1) which switches to a layer persistently, this switches the active HE profile.
 *
 * @param profile_index HE profile to switch to (0, 1, or 2)
 *
 * @note Profile index must be between 0 and PROFILE_COUNT-1
 * @note LED indication is enabled when switching profiles
 *
 * Example runtime switching:
 * ```c
 * enum custom_keycodes {
 *     HE_PROF_0 = SAFE_RANGE,  // Switch to HE profile 0
 *     HE_PROF_1,               // Switch to HE profile 1
 *     HE_PROF_2,               // Switch to HE profile 2
 *     HE_CYCLE                 // Cycle through HE profiles
 * };
 *
 * bool process_record_user(uint16_t keycode, keyrecord_t *record) {
 *     switch (keycode) {
 *         case HE_PROF_0:
 *             if (record->event.pressed) {
 *                 switch_he_profile(0);  // Switch to HE profile 0
 *             }
 *             return false;
 *         case HE_PROF_1:
 *             if (record->event.pressed) {
 *                 switch_he_profile(1);  // Switch to HE profile 1
 *             }
 *             return false;
 *         case HE_PROF_2:
 *             if (record->event.pressed) {
 *                 switch_he_profile(2);  // Switch to HE profile 2
 *             }
 *             return false;
 *         case HE_CYCLE:
 *             if (record->event.pressed) {
 *                 cycle_he_profile();    // Cycle through profiles
 *             }
 *             return false;
 *     }
 *     return true;
 * }
 *
 * // Configure different actuation points for "1" key on all profiles
 * void keyboard_post_init_user(void) {
 *     uint8_t original_profile = get_current_he_profile();
 *
 *     // Profile 0: Gaming (very sensitive)
 *     switch_he_profile(0);
 *     set_key_actuation_point(0, 1, 3);   // "1" key = 0.3mm
 *
 *     // Profile 1: Typing (normal)
 *     switch_he_profile(1);
 *     set_key_actuation_point(0, 1, 15);  // "1" key = 1.5mm
 *
 *     // Profile 2: Heavy typing (less sensitive)
 *     switch_he_profile(2);
 *     set_key_actuation_point(0, 1, 35);  // "1" key = 3.5mm
 *
 *     switch_he_profile(original_profile); // Restore original profile
 * }
 * ```
 */
static inline void switch_he_profile(uint8_t profile_index) {
    if (profile_index >= PROFILE_COUNT) {
        return; // Invalid profile index
    }

    profile_select(profile_index, true);  // true = LED indication when switching; false = no LED indication
}

/**
 * @brief Get current Hall Effect profile index
 *
 * @details
 * Returns the currently active HE profile index (0-2).
 *
 * Useful for:
 * - Saving current profile before configuration
 * - Displaying current profile on OLED/LCD
 * - Conditional logic based on active profile
 *
 * @return Current HE profile index (0 to PROFILE_COUNT-1)
 */
static inline uint8_t get_current_he_profile(void) {
    return profile_get_current_index();
}

/**
 * @brief Cycle to next Hall Effect profile
 *
 * @details
 * Cycles to the next HE profile in sequence: 0 → 1 → 2 → 0 → ...
 * Convenient for a single key that cycles through all available profiles.
 *
 * @note LED indication is enabled when switching profiles
 */
static inline void cycle_he_profile(void) {
    uint8_t current = profile_get_current_index();
    uint8_t next = (current + 1) % PROFILE_COUNT;
    profile_select(next, true);  // true = LED indication when switching; false = no LED indication
}

/*
 * ============================================================================
 * HALL EFFECT CALIBRATION FUNCTIONS
 * ============================================================================
 *
 * CALIBRATION PROCESS OVERVIEW:
 * 1. Start with CALIB_ZERO_TRAVEL_MANUAL - ensure all keys are in rest position (not pressed)
 * 2. Controller automatically transitions to CALIB_FULL_TRAVEL_MANUAL after zero phase
 * 3. Manually press all keys to maximum travel depth - LED changes from PURPLE to GREEN when done
 * 4. Calibration data is automatically saved when complete - no manual save needed
 *
 * IMPORTANT NOTES:
 * - During calibration, keys are unresponsive and will not print anything
 * - User must manually press each key during the calibration phases
 * - If LED stays RED during calibration, one or more keys failed to calibrate properly
 * - If calibration fails (LED remains red), restart calibration by relaunching the keyboard
 * - No manual stop calibration function is implemented because keys are unresponsive during calibration
 */

/**
 * @brief Start Hall Effect calibration process
 *
 * @details
 * Initiates the manual two-phase calibration sequence:
 * 1. Zero travel calibration (LED: RED) - ensure all keys are released
 * 2. Full travel calibration (LED: PURPLE->GREEN) - manually press all keys to maximum depth
 *
 * WARNING: During calibration, keys become unresponsive and will not type anything!
 * User must manually press keys during each calibration phase.
 * The keyboard LED indicates the current phase and completion status.
 *
 * @note Keys become unresponsive during calibration
 * @note LED color indicates calibration phase: RED = zero phase, PURPLE = full phase, GREEN = complete
 * @note Includes 1 second delay to ensure all keys are released before starting
 *
 * Example usage:
 * ```c
 * case KC_CALIB_START:
 *     if (record->event.pressed) {
 *         start_calibration();
 *     }
 *     return false;
 * ```
 */
static inline void start_calibration(void) {
    wait_ms(1000);  // 1 second delay to ensure all keys are released before calibration
    uint8_t data[4] = {0xA9, AMC_CALIBRATE, CALIB_ZERO_TRAVEL_MANUAL, 0};
    analog_matrix_rx(data, 4);
}

/**
 * @brief Get current calibration state
 *
 * @details
 * Returns the current calibration state from the analog matrix controller.
 * Useful for monitoring calibration progress or implementing custom calibration logic.
 *
 * @return Current calibration state:
 *         - CALIB_OFF: Normal operation
 *         - CALIB_ZERO_TRAVEL_MANUAL: Zero calibration active (LED: RED)
 *         - CALIB_FULL_TRAVEL_MANUAL: Full calibration active (LED: PURPLE->GREEN)
 *
 * Example usage:
 * ```c
 * uint8_t state = get_calibration_state();
 * if (state == CALIB_OFF) {
 *     // Calibration complete or not active
 * }
 * ```
 */
static inline uint8_t get_calibration_state(void) {
    uint8_t data[20] = {0xA9, AMC_GET_CALIBRATE_STATE, 0};
    analog_matrix_rx(data, 20);
    return data[3];
}

/**
 * @brief Check if calibration is currently active
 *
 * @details
 * Returns true if calibration is currently in progress, false otherwise.
 * Convenient wrapper around get_calibration_state() for simple checks.
 *
 * @return true if calibration is active, false if normal operation
 *
 * Example usage:
 * ```c
 * if (is_calibrating()) {
 *     // Add your logic during calibration process here
 *     return;
 * }
 * ```
 */
static inline bool is_calibrating(void) {
    return (get_calibration_state() != CALIB_OFF);
}

/**
 * @brief Get calibrated values for a specific key
 *
 * @details
 * Retrieves the stored calibration data for a single key position.
 *
 * ADC Value Notes:
 * - Higher ADC values = magnet closer to sensor
 * - Zero travel (key at rest) should have HIGHER ADC values than full travel
 * - Full travel (key pressed) should have LOWER ADC values
 * - If zero_travel <= full_travel, the key may not be calibrated correctly
 *
 * @param row Key matrix row index starting from 0 (0 = top row, increases downward)
 * @param col Key matrix column index starting from 0 (0 = leftmost column, increases rightward)
 * @param zero_travel Pointer to store the zero travel ADC value (key at rest position)
 * @param full_travel Pointer to store the full travel ADC value (key fully pressed)
 *
 * @return true if calibration data exists and was retrieved successfully, false otherwise
 *
 * @note To find valid row/col for a specific key, look at the LED matrix in ansi.c / iso_encoder.c / jis_encoder.c etc.
 *       where __ means no key exists at that position. If no LED matrix exists, check
 *       analog_matrix_mask[] and read each binary line right-to-left (1 = valid key, 0 = no key).
 *       Example: 0b110111111111111 = cols 14,13,11,10,9,8,7,6,5,4,3,2,1,0 exist, col 12 missing.
 *
 * Example usage:
 * ```c
 * uint16_t zero, full;
 * if (get_key_calibration(1, 2, &zero, &full)) {  // W key
 *     // Successfully got calibration data
 *     uint16_t range = zero - full;  // Calculate travel range
 *     if (range > 100) {
 *         // Do something here
 *     }
 * } else {
 *     // Key not calibrated or invalid position or default values used
 * }
 * ```
 */
static inline bool get_key_calibration(uint8_t row, uint8_t col, uint16_t *zero_travel, uint16_t *full_travel) {
    if (row >= MATRIX_ROWS || col >= MATRIX_COLS) {
        return false;
    }

    uint8_t data[20] = {0xA9, AMC_GET_CALIBRATED_VALUE, row, col, 0};
    analog_matrix_rx(data, 20);

    if (data[4] == 0) {
        *zero_travel = (data[6] << 8) | data[5];  // Little endian conversion
        *full_travel = (data[8] << 8) | data[7];  // Little endian conversion
        return true;
    }

    return false;
}

/**
 * @brief Clear all calibration data
 *
 * @details
 * Erases all stored calibration values and resets keys to factory defaults.
 * Keys will continue to work normally with default calibration values.
 * New calibration will overwrite old data, so clearing is optional.
 *
 * Usage: Use only for troubleshooting calibration issues.
 *
 * @note Keys continue to work with factory default calibration values after clearing
 * @note New calibration will overwrite old data automatically
 *
 * Example usage:
 * ```c
 * case KC_CALIB_CLEAR:
 *     if (record->event.pressed) {
 *         clear_calibration_data();
 *     }
 *     return false;
 * ```
 */
static inline void clear_calibration_data(void) {
    uint8_t data[4] = {0xA9, AMC_CALIBRATE, CALIB_CLEAR, 0};
    analog_matrix_rx(data, 4);
}
