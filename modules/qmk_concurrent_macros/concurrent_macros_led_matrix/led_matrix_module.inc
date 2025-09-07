#ifndef COMMUNITY_MODULE_CONCURRENT_MACROS_ENABLE
#    error "To use the concurrent_macros animation, you must add 'concurrent_macros' to your keyboard's module list in its keyboard.json file"
#endif
LED_MATRIX_EFFECT(CONCURRENT_MACROS)
#ifdef LED_MATRIX_CUSTOM_EFFECT_IMPLS
#    include "concurrent_macros.h"

static inline uint8_t triangle_wave(uint32_t counter) {
    uint16_t masked = counter & 0x1ff;
    return ((masked >= 0x100) ? (uint8_t)(0x1ff - masked) : (uint8_t)masked);
}

#define NORMAL_RATE (time)
#define SLOW_RATE (time >> 1)
#define FAST_RATE (time << 2)

static void iter_func(keypos_t* keypos, concurrent_macros_led_state_t led_state) {
    uint8_t  led_index = g_led_config.matrix_co[keypos->row][keypos->col];
    uint8_t  led_value;
    uint32_t time = scale16by8((uint16_t)(g_led_timer), led_matrix_eeconfig.speed);

    switch (led_state) {
        case MACRO_LED_STATE_RUNNING:
            led_value = triangle_wave(NORMAL_RATE);
            break;
        case MACRO_LED_STATE_STOPPING:
            led_value = triangle_wave(SLOW_RATE);
            break;
        case MACRO_LED_STATE_ERROR:
            led_value = triangle_wave(FAST_RATE);
            break;
        case MACRO_LED_STATE_IDLE:
        default:
            led_value = led_matrix_eeconfig.val;
            break;
    }
    led_matrix_set_value(led_index, led_value);
}

bool CONCURRENT_MACROS(effect_params_t* params) {
    led_matrix_set_value_all(led_matrix_eeconfig.val);
    concurrent_macros_macro_led_state_iterate(iter_func);

    return false; // false means we are done rendering
}
#endif // LED_MATRIX_CUSTOM_EFFECT_IMPLS
