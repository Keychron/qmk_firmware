#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_DIGITAL_RAIN)
RGB_MATRIX_EFFECT(DIGITAL_RAIN)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#        ifndef RGB_DIGITAL_RAIN_DROPS
// lower the number for denser effect/wider keyboard
#            define RGB_DIGITAL_RAIN_DROPS 24
#        endif

uint8_t rain_rgb_frame_buffer[MATRIX_ROWS][MATRIX_COLS] = {{0}};

bool DIGITAL_RAIN(effect_params_t* params) {
    // algorithm ported from https://github.com/tremby/Kaleidoscope-LEDEffect-DigitalRain
    const uint8_t drop_ticks           = 28;
    const uint8_t pure_green_intensity = (((uint16_t)rgb_matrix_config.hsv.v) * 3) >> 2;
    const uint8_t max_brightness_boost = (((uint16_t)rgb_matrix_config.hsv.v) * 3) >> 2;
    const uint8_t max_intensity        = rgb_matrix_config.hsv.v;
    const uint8_t decay_ticks          = 0xff / max_intensity;

    static uint8_t drop  = 0;
    static uint8_t decay = 0;
    static bool render = true;
    // brightness the frame buffer was last rendered with
    static uint8_t last_max_intensity = 0;

    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (params->init) {
        rgb_matrix_region_set_color_all(params->region, 0, 0, 0);
        memset(rain_rgb_frame_buffer, 0, sizeof(rain_rgb_frame_buffer));
        drop = 0;
    }

    if (params->iter == 0) {

        // If brightness was decreased, scale the frame buffer down so no
        // pixel is left above the new maximum.
        if (last_max_intensity > max_intensity) {
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
                    rain_rgb_frame_buffer[row][col] = rain_rgb_frame_buffer[row][col] * (uint16_t)max_intensity / last_max_intensity;
                }
            }
        }
        last_max_intensity = max_intensity;

        if (render)
            decay++;

        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
                if (render) {
                    if (row == 0 && drop == 0 && rand() < RAND_MAX / RGB_DIGITAL_RAIN_DROPS) {
                        // top row, pixels have just fallen and we're
                        // making a new rain drop in this column
                        rain_rgb_frame_buffer[row][col] = max_intensity;
                    } else if (rain_rgb_frame_buffer[row][col] > 0 && rain_rgb_frame_buffer[row][col] < max_intensity) {
                        // neither fully bright nor dark, decay it
                        if (decay == decay_ticks) {
                            rain_rgb_frame_buffer[row][col]--;
                        }
                    }
                }
                // set the pixel colour
                uint8_t led[LED_HITS_TO_REMEMBER];
                uint8_t led_count = rgb_matrix_map_row_column_to_led(row, col, led);

                // TODO: multiple leds are supported mapped to the same row/column
                if (led_count > 0) {
                    if (rain_rgb_frame_buffer[row][col] > pure_green_intensity) {
                        const uint8_t boost = (uint8_t)((uint16_t)max_brightness_boost * (rain_rgb_frame_buffer[row][col] - pure_green_intensity) / (max_intensity - pure_green_intensity));
                        rgb_matrix_region_set_color(params->region, led[0], boost, max_intensity, boost);
                    } else {
                        const uint8_t green = (uint8_t)((uint16_t)max_intensity * rain_rgb_frame_buffer[row][col] / pure_green_intensity);
                        rgb_matrix_region_set_color(params->region, led[0], 0, green, 0);
                    }
                }
            }
        }

        if (render) {
            if (decay == decay_ticks) {
                decay = 0;
            }

            if (++drop > drop_ticks) {
                // reset drop timer
                drop = 0;
                for (uint8_t row = MATRIX_ROWS - 1; row > 0; row--) {
                    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                        // if ths is on the bottom row and bright allow decay
                        if (row == MATRIX_ROWS - 1 && rain_rgb_frame_buffer[row][col] == max_intensity) {
                            rain_rgb_frame_buffer[row][col]--;
                        }
                        // check if the pixel above is bright
                        if (rain_rgb_frame_buffer[row - 1][col] >= max_intensity) { // Note: can be larger than max_intensity if val was recently decreased
                            // allow old bright pixel to decay
                            rain_rgb_frame_buffer[row - 1][col] = max_intensity - 1;
                            // make this pixel bright
                            rain_rgb_frame_buffer[row][col] = max_intensity;
                        }
                    }
                }
            }
        }
        render = false;
    } else {
        render = true;
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && !defined(ENABLE_RGB_MATRIX_DIGITAL_RAIN)
