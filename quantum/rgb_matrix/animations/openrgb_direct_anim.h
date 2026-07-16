#ifdef OPENRGB_ENABLE
RGB_MATRIX_EFFECT(OPENRGB_DIRECT)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

extern RGB g_openrgb_direct_mode_colors[];

bool OPENRGB_DIRECT(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
#        ifdef OPENRGB_DIRECT_MODE_USE_UNIVERSAL_BRIGHTNESS
        const uint8_t v = rgb_matrix_config.hsv.v;
        rgb_matrix_set_color(i, (uint8_t)((uint16_t)g_openrgb_direct_mode_colors[i].r * v / UINT8_MAX), (uint8_t)((uint16_t)g_openrgb_direct_mode_colors[i].g * v / UINT8_MAX), (uint8_t)((uint16_t)g_openrgb_direct_mode_colors[i].b * v / UINT8_MAX));
#        else
        rgb_matrix_set_color(i, g_openrgb_direct_mode_colors[i].r, g_openrgb_direct_mode_colors[i].g, g_openrgb_direct_mode_colors[i].b);
#        endif
    }
    return rgb_matrix_check_finished_leds(led_max);
}
#    endif
#endif
