#include <keyboard.h>

typedef enum : uint8_t { MACRO_LED_STATE_IDLE, MACRO_LED_STATE_RUNNING, MACRO_LED_STATE_STOPPING, MACRO_LED_STATE_ERROR } concurrent_macros_led_state_t;

typedef void (*iterator_func)(keypos_t *keypos, concurrent_macros_led_state_t state);

void concurrent_macros_macro_led_state_iterate(iterator_func func);
