// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#ifdef ENABLE_RGB_MATRIX_PIXEL_RAIN
RGB_MATRIX_EFFECT(PIXEL_RAIN)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static fast_timer_t timer[2]        = { 0, 0 };

void PIXEL_RAIN_init(void) {
    timer[0] = timer[1] = 0;
}

bool PIXEL_RAIN(effect_params_t* params) {
    static uint16_t     index[2]     = {RGB_MATRIX_LED_COUNT + 1, RGB_MATRIX_LED_COUNT + 1};
    static bool         timer_update = false;

    if (params->iter == 0) {

        if (timer_elapsed_fast(timer[params->region]) > (320 - rgb_matrix_config.speed)) {
            index[params->region] = random8_max(RGB_MATRIX_LED_COUNT);
            timer_update          = true;
        }
    } else {
        if (timer_update) {
            timer[params->region]        = timer_read_fast();
            timer_update = false;
        }
    }

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    if (led_min <= index[params->region] && index[params->region] < led_max && HAS_ANY_FLAGS(g_led_config.flags[index[params->region]], params->flags)) {
        hsv_t hsv = (random8() & 2) ? (hsv_t){0, 0, 0} : (hsv_t){random8(), random8_min_max(127, 255), rgb_matrix_config.hsv.v};
        rgb_t rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_region_set_color(params->region, index[params->region], rgb.r, rgb.g, rgb.b);

        index[params->region] = RGB_MATRIX_LED_COUNT + 1;
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // ENABLE_RGB_MATRIX_PIXEL_RAIN
