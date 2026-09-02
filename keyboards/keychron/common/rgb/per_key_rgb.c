/* Copyright 2024 ~ 2025 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "quantum.h"
#include "rgb_matrix.h"
#include "keychron_rgb_type.h"
#include <math.h>
#include <lib/lib8tion/lib8tion.h>

#if defined(KEYCHRON_RGB_ENABLE)

// PER_KEY_RGB data
uint8_t per_key_rgb_type;
HSV     per_key_led[RGB_MATRIX_LED_COUNT] = {0};

bool per_key_rgb_solid(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    HSV hsv;

    for (uint8_t i = led_min; i < led_max; i++) {
        hsv     = per_key_led[i];
        hsv.v   = scale8(rgb_matrix_config.hsv.v, hsv.v);
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_region_set_color(params->region, i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

bool per_key_rgb_breahting(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    HSV      hsv;
    uint16_t time = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8);

    for (uint8_t i = led_min; i < led_max; i++) {
        hsv     = per_key_led[i];
        hsv.v   = scale8(rgb_matrix_config.hsv.v, hsv.v);
        hsv.v   = scale8(abs8(sin8(time) - 128) * 2, hsv.v);
        RGB rgb = hsv_to_rgb(hsv);
        RGB_MATRIX_TEST_LED_FLAGS();
        rgb_matrix_region_set_color(params->region, i, rgb.r, rgb.g, rgb.b);
    }

    return rgb_matrix_check_finished_leds(led_max);
}

bool per_key_rgb_reactive_simple(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint16_t max_tick = 65535 / qadd8(rgb_matrix_config.speed, 1);
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        uint16_t tick = max_tick;
        // Reverse search to find most recent key hit
        for (int8_t j = g_last_hit_tracker.count - 1; j >= 0; j--) {
            if (g_last_hit_tracker.index[j] == i && g_last_hit_tracker.tick[j] < tick) {
                tick = g_last_hit_tracker.tick[j];
                break;
            }
        }

        uint16_t offset = scale16by8(tick, qadd8(rgb_matrix_config.speed, 1));
        HSV      hsv    = per_key_led[i];

        if (offset > 255) offset = 255;
        hsv.v = scale8(rgb_matrix_config.hsv.v, hsv.v);
        hsv.v = scale8(255 - offset, hsv.v);

        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_region_set_color(params->region, i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

typedef HSV (*reactive_splash_f)(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick);

bool per_key_rgb_effect_runner_reactive_splash(uint8_t start, effect_params_t *params, reactive_splash_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        HSV hsv = rgb_matrix_config.hsv;
        hsv.v   = 0;
        for (uint8_t j = start; j < count; j++) {
            int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t  dist = sqrt16(dx * dx + dy * dy);
            uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], qadd8(rgb_matrix_config.speed, 1));
            hsv           = effect_func(hsv, dx, dy, dist, tick);
        }
        hsv.h = per_key_led[i].h;
        hsv.s = per_key_led[i].s;
        hsv.v = scale8(rgb_matrix_config.hsv.v, per_key_led[i].v);

        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_region_set_color(params->region, i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

static HSV solid_reactive_wide_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
    uint16_t effect = tick + dist * 5;
    if (effect > 255) effect = 255;
#    ifdef RGB_MATRIX_SOLID_REACTIVE_GRADIENT_MODE
    hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 8) >> 4);
#    endif
    hsv.v = qadd8(hsv.v, 255 - effect);
    return hsv;
}

bool per_key_rgb_reactive_multi_wide(effect_params_t *params) {
    return per_key_rgb_effect_runner_reactive_splash(0, params, &solid_reactive_wide_math);
}

static HSV SPLASH_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
    uint16_t effect = tick - dist;
    if (effect > 255) effect = 255;
    hsv.h += effect;
    hsv.v = qadd8(hsv.v, 255 - effect);
    return hsv;
}

bool per_key_rgb_reactive_splash(effect_params_t *params) {
    return per_key_rgb_effect_runner_reactive_splash(qsub8(g_last_hit_tracker.count, 1), params, &SPLASH_math);
}

bool per_key_rgb(effect_params_t *params) {
    switch (per_key_rgb_type) {
        case PER_KEY_RGB_BREATHING:
            return per_key_rgb_breahting(params);

        case PER_KEY_RGB_REATIVE_SIMPLE:
            return per_key_rgb_reactive_simple(params);

        case PER_KEY_RGB_REATIVE_MULTI_WIDE:
            return per_key_rgb_reactive_multi_wide(params);

        case PER_KEY_RGB_REATIVE_SPLASH:
            return per_key_rgb_reactive_splash(params);

        default:
            return per_key_rgb_solid(params);
    }
}

#endif
