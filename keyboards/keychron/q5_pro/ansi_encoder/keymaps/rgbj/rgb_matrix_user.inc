RGB_MATRIX_EFFECT(HOSTLIGHT)
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

/* Host frames call rgb_matrix_set_color() themselves. This effect must not
 * repaint, or the next flush replaces the frame that just arrived. */
bool HOSTLIGHT(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
