#include "action.h"
#include <ctype.h>
#include "concurrent_macros.h"
#include "deferred_exec.h"
#include "dynamic_keymap.h"
#include "eeprom.h"
#include "matrix.h"
#include "send_string.h"
#include "util.h"
#include "keymap_introspection.h"

#ifndef DYNAMIC_KEYMAP_MACRO_DELAY
#    define DYNAMIC_KEYMAP_MACRO_DELAY TAP_CODE_DELAY
#endif

typedef enum : uint8_t { CM_FREE, CM_IDLE, CM_DO_NEXT, CM_ERROR } concurrent_macros_state_t;

#define CM_FLAG_REPEAT_UNTIL_TOGGLE_OFF (1 << 0)
#define CM_FLAG_REPEAT_UNTIL_MACRO_KEY_UP (1 << 1)
#define CM_FLAG_START_REPEAT (1 << 2)
#define CM_FLAG_STOP (1 << 3)

char loop_init[] = "l_init";
char loop_go[] = "l_go";
char loop_toggle[] = "l_toggle";
char loop_stopall[] = "l_stopall";
char loop_stop[] = "l_stop";
char loop_check[20] = "";
uint8_t count = 0;

#define CM_FLAG_REPEAT_UNTIL_MASK (CM_FLAG_REPEAT_UNTIL_TOGGLE_OFF | CM_FLAG_REPEAT_UNTIL_MACRO_KEY_UP)

typedef struct {
    deferred_token            de_token;
    concurrent_macros_state_t state;
    uint8_t                   flags;
    keypos_t                  keypos;
    uint16_t                  start_offset;
    uint16_t                  offset;
	uint16_t                  loop;
} concurrent_macros_info_t;

#ifndef MAX_CONCURRENT_MACROS
#    define MAX_CONCURRENT_MACROS 10
#endif

static concurrent_macros_info_t macros[MAX_CONCURRENT_MACROS] = {[0 ... MAX_CONCURRENT_MACROS - 1] = {.de_token = INVALID_DEFERRED_TOKEN, .state = CM_FREE, .flags = 0, .start_offset = 0, .offset = 0, .keypos = {.col = 0xff, .row = 0xff}}};

#define RESERVATION_FAILED UINT8_MAX
static uint8_t reserve_concurrent_macro(void) {
    for (uint8_t idx = 0; idx < MAX_CONCURRENT_MACROS; idx++) {
        if (macros[idx].state == CM_FREE) {
            macros[idx].state = CM_IDLE;
            return idx;
        }
    }
    return RESERVATION_FAILED;
}

#define MACRO_NOT_FOUND UINT8_MAX
static uint8_t find_concurrent_macro(keypos_t *keypos) {
    for (uint8_t idx = 0; idx < MAX_CONCURRENT_MACROS; idx++) {
        if ((macros[idx].state != CM_FREE) && (macros[idx].keypos.row == keypos->row) && (macros[idx].keypos.col == keypos->col)) {
            return idx;
        }
    }
    return MACRO_NOT_FOUND;
}

deferred_executor_t deferred_executor_table[MAX_CONCURRENT_MACROS];

static void concurrent_macros_get_next(concurrent_macros_info_t *const macro, uint8_t *const data) {
    dynamic_keymap_macro_get_buffer(macro->offset++, 1, &data[0]);
    data[1] = 0;
    // Stop at the null terminator of this macro string
    if (data[0] == 0) {
        return;
    }
    if (data[0] == SS_QMK_PREFIX) {
        // Get the code
        dynamic_keymap_macro_get_buffer(macro->offset++, 1, &data[1]);
        // Unexpected null, abort.
        if (data[1] == 0) {
            data[0]      = 0;
            macro->state = CM_ERROR;
            return;
        }
        if (data[1] == SS_TAP_CODE || data[1] == SS_DOWN_CODE || data[1] == SS_UP_CODE) {
            // Get the keycode
            dynamic_keymap_macro_get_buffer(macro->offset++, 1, &data[2]);
            // Unexpected null, abort.
            if (data[2] == 0) {
                data[0]      = 0;
                macro->state = CM_ERROR;
                return;
            }
            // Null terminate
            data[3] = 0;
        } else if (data[1] == SS_DELAY_CODE) {
            // Get the number and '|'
            // At most this is 4 digits plus '|'
            uint8_t i = 2;
            while (1) {
                dynamic_keymap_macro_get_buffer(macro->offset++, 1, &data[i]);
                // Unexpected null, abort
                if (data[i] == 0) {
                    data[0]      = 0;
                    macro->state = CM_ERROR;
                    return;
                }
                // Found '|', duration value is complete
                if (data[i] == '|') {
                    data[i + 1] = 0;
                    break;
                }
                // If haven't found '|' by i==6 then
                // number too big, abort
                if (i == 6) {
                    data[0]      = 0;
                    macro->state = CM_ERROR;
                    return;
                }
                ++i;
            }
        }
    } else {
        data[1] = 0; // terminate the string
    }
}

#ifndef KC_HARD_STOP_OTHER_MACROS
#    define KC_HARD_STOP_OTHER_MACROS 2003
#endif
#ifndef KC_REPEAT_UNTIL_TOGGLE_OFF
#    define KC_REPEAT_UNTIL_TOGGLE_OFF 2004
#endif
#ifndef KC_REPEAT_UNTIL_MACRO_KEY_UP
#    define KC_REPEAT_UNTIL_MACRO_KEY_UP 2002
#endif
#ifndef KC_START_REPEAT
#    define KC_START_REPEAT 2001
#endif

static void concurrent_macros_stop_others(concurrent_macros_info_t *except_macro) {
    for (int idx = 0; idx < MAX_CONCURRENT_MACROS; idx++) {
        concurrent_macros_info_t *const macro = &macros[idx];

        if (macro == except_macro) {
            // skip the currently running macro
            continue;
        }
        if (macro->de_token != INVALID_DEFERRED_TOKEN) {
            cancel_deferred_exec_advanced(deferred_executor_table, MAX_CONCURRENT_MACROS, macro->de_token);
            macro->de_token = INVALID_DEFERRED_TOKEN;
        }
        macro->state        = CM_FREE;
        macro->flags        = 0;
        macro->start_offset = 0;
        macro->offset       = 0;
		macro->loop        	= 0;
        macro->keypos.row   = 0xff;
        macro->keypos.col   = 0xff;
    }
    clear_keyboard();
}

static void concurrent_macros_stop(concurrent_macros_info_t *except_macro) {
    for (int idx = 0; idx < MAX_CONCURRENT_MACROS; idx++) {
        concurrent_macros_info_t *const macro = &macros[idx];

        if (macro != except_macro) {
            // skip the currently running macro
            continue;
        }
        if (macro->de_token != INVALID_DEFERRED_TOKEN) {
            cancel_deferred_exec_advanced(deferred_executor_table, MAX_CONCURRENT_MACROS, macro->de_token);
            macro->de_token = INVALID_DEFERRED_TOKEN;
        }
        macro->state        = CM_FREE;
        macro->flags        = 0;
        macro->start_offset = 0;
        macro->offset       = 0;
		macro->loop        	= 0;
        macro->keypos.row   = 0xff;
        macro->keypos.col   = 0xff;
    }
    clear_keyboard();
}


static uint32_t concurrent_macros_execute_one_step(uint32_t trigger_time, void *macro_p) {
    uint8_t                   data[8];
    concurrent_macros_info_t *macro = (concurrent_macros_info_t *)macro_p;

    switch (macro->state) {
        case CM_IDLE:
            // nothing to do, why are we here?  is this an error?
            return 0;
        case CM_DO_NEXT:
            concurrent_macros_get_next(macro, data);
            if (macro->state == CM_ERROR) {
                return 0;
            }
            if (data[0] == 0) {
                // End of macro
                macro->state = CM_FREE;
                return 0;
            } else {
                if (data[0] == SS_QMK_PREFIX) {
                    switch (data[1]) {
                        case SS_TAP_CODE:
                        case SS_DOWN_CODE:
                        case SS_UP_CODE:
                            switch (data[2]) {
								//original code, no longer needed.
                                /*                    case KC_HARD_STOP_OTHER_MACROS:
                                    concurrent_macros_stop_others(macro);
                                    return 1;
                                case KC_START_REPEAT:
                                    if (macro->flags & CM_FLAG_START_REPEAT) {
                                        macro->state = CM_ERROR;
                                        return 0;
                                    } else {
                                        // don't allow any other START_REPEAT after this
                                        macro->flags |= CM_FLAG_START_REPEAT;
                                        macro->start_offset = macro->offset;
                                        return 1;
                                    }
                                case KC_REPEAT_UNTIL_MACRO_KEY_UP:
                                    if (macro->flags & CM_FLAG_REPEAT_UNTIL_MASK) {
                                        macro->state = CM_ERROR;
                                        return 0;
                                    } else {
                                        if (matrix_is_on(macro->keypos.row, macro->keypos.col)) {
                                            macro->offset = macro->start_offset;
                                        } else {
                                            // proceed to the postamble (if any), but don't allow any START_REPEAT
                                            // or REPEAT_UNTIL after this
                                            macro->flags |= CM_FLAG_START_REPEAT | CM_FLAG_REPEAT_UNTIL_MACRO_KEY_UP;
                                        }
                                        return 1;
                                    }
                                case KC_REPEAT_UNTIL_TOGGLE_OFF:
                                    if (macro->flags & CM_FLAG_REPEAT_UNTIL_MASK) {
                                        macro->state = CM_ERROR;
                                        return 0;
                                    } else {
                                        if (!(macro->flags & CM_FLAG_STOP)) {
                                            macro->offset = macro->start_offset;
                                        } else {
                                            // proceed to the postamble (if any), but don't allow any START_REPEAT
                                            // or REPEAT_UNTIL after this
                                            macro->flags |= CM_FLAG_START_REPEAT | CM_FLAG_REPEAT_UNTIL_TOGGLE_OFF;
                                        }
                                        return 1;
                                    }*/
								case KC_TRNS:
									data[2] = keycode_at_keymap_location(0, macro->keypos.row, macro->keypos.col);
									if (data[2] > 1 && (data[2] < MC_0 || data[2] > MC_31)){
										send_string_with_delay((const char *)data, 0);
										return MAX(DYNAMIC_KEYMAP_MACRO_DELAY, 1);
									}
									data[2] = KC_TRNS;
									
                                default:
                                    send_string_with_delay((const char *)data, 0);
                                    return MAX(DYNAMIC_KEYMAP_MACRO_DELAY, 1);
                            }
                            // stay in CM_DO_NEXT state
                        case SS_DELAY_CODE:
                            uint16_t ms      = 0;
                            uint8_t *ptr     = &data[2];
                            uint8_t  keycode = *ptr;
                            while (isdigit(keycode)) {
                                ms *= 10;
                                ms += keycode - '0';
                                keycode = *(++ptr);
                            }
							
							//ms loop system,no longer needed
							/*
							switch (ms){
                                case KC_HARD_STOP_OTHER_MACROS:
                                    concurrent_macros_stop_others(macro);
                                    return 1;
                                case KC_START_REPEAT:
                                    if (macro->flags & CM_FLAG_START_REPEAT) {
                                        macro->state = CM_ERROR;
                                        return 0;
                                    } else {
                                        // don't allow any other START_REPEAT after this
                                        macro->flags |= CM_FLAG_START_REPEAT;
                                        macro->start_offset = macro->offset;
                                        return 1;
                                    }
                                case KC_REPEAT_UNTIL_MACRO_KEY_UP:
                                    if (macro->flags & CM_FLAG_REPEAT_UNTIL_MASK) {
                                        macro->state = CM_ERROR;
                                        return 0;
                                    } else {
                                        if (matrix_is_on(macro->keypos.row, macro->keypos.col)) {
                                            macro->offset = macro->start_offset;
                                        } else {
                                            // proceed to the postamble (if any), but don't allow any START_REPEAT
                                            // or REPEAT_UNTIL after this
                                            macro->flags |= CM_FLAG_START_REPEAT | CM_FLAG_REPEAT_UNTIL_MACRO_KEY_UP;
                                        }
                                        return 1;
                                    }
                                case KC_REPEAT_UNTIL_TOGGLE_OFF:
                                    if (macro->flags & CM_FLAG_REPEAT_UNTIL_MASK) {
                                        macro->state = CM_ERROR;
                                        return 0;
                                    } else {
                                        if (!(macro->flags & CM_FLAG_STOP)) {
                                            macro->offset = macro->start_offset;
                                        } else {
                                            // proceed to the postamble (if any), but don't allow any START_REPEAT
                                            // or REPEAT_UNTIL after this
                                            macro->flags |= CM_FLAG_START_REPEAT | CM_FLAG_REPEAT_UNTIL_TOGGLE_OFF;
                                        }
                                        return 1;
                                    }
								
							}
							*/
                            //if ms ends in 1 and higher than 21, add a random delay of +-20
							if (ms % 10 == 1 && ms >=21){
								ms += -20+rand() % 40;
								
							}
							// stay in CM_DO_NEXT state
                            return ms;
                        default:
                            // unknown code
                            macro->state = CM_ERROR;
                            return 0;
                    }
                } else {
					//save offset
					macro->loop = macro->offset;
					//check at least if the first letter is l to prevent this from happening every single letter that gets typed
					if (data[0] == loop_go[0]){
						data[2]=data[0];
						//attempt to load 11 char max
						count = 0;
						while (data[2] >= 20 && count <=11){
							loop_check[count] = (char)data[2];
							dynamic_keymap_macro_get_buffer(macro->offset++, 1, &data[2]);
							count++;
						}
						loop_check[count] = '\0';
						macro->offset--;
						
						
						
						//check all posible loop state
						//do normal loop if the string is loop_go
						if (strcmp(loop_check,loop_go)==0){
							macro->loop = 0;
							if (macro->flags & CM_FLAG_REPEAT_UNTIL_MASK) {
								macro->state = CM_ERROR;
								return 0;
							} else {
								if (matrix_is_on(macro->keypos.row, macro->keypos.col)) {
									macro->offset = macro->start_offset;
								} else {
									// proceed to the postamble (if any), but don't allow any START_REPEAT
									// or REPEAT_UNTIL after this
									macro->flags |= CM_FLAG_START_REPEAT | CM_FLAG_REPEAT_UNTIL_MACRO_KEY_UP;
								}
								return 1;
							}

						}
						//trigger loop init if the string is loop_init
						if (strcmp(loop_check,loop_init)==0){
							if (macro->flags & CM_FLAG_START_REPEAT) {
								macro->state = CM_ERROR;
								return 0;
							} else {
								// don't allow any other START_REPEAT after this
								macro->flags |= CM_FLAG_START_REPEAT;
								macro->start_offset = macro->offset;
								return 1;
							}
						}
						
						//trigger toggle loop
						if (strcmp(loop_check,loop_toggle)==0){
							if (macro->flags & CM_FLAG_REPEAT_UNTIL_MASK) {
								macro->state = CM_ERROR;
								return 0;
							} else {
								if (!(macro->flags & CM_FLAG_STOP)) {
									macro->offset = macro->start_offset;
								} else {
									// proceed to the postamble (if any), but don't allow any START_REPEAT
									// or REPEAT_UNTIL after this
									macro->flags |= CM_FLAG_START_REPEAT | CM_FLAG_REPEAT_UNTIL_TOGGLE_OFF;
								}
								return 1;
							}
						
						}
					
						//stop this loop
						if (strcmp(loop_check,loop_stop)==0){
							if (!matrix_is_on(macro->keypos.row, macro->keypos.col)) {
								concurrent_macros_stop(macro);
							}
							return 1;

						}
						
						//stop all loops
						if (strcmp(loop_check,loop_stopall)==0){
							concurrent_macros_stop_others(macro);
							return 1;
						}
					}
					
					//if it makes it here, no loop was detected
					//reset buffer head
					macro->offset = macro->loop;
					send_string_with_delay((const char *)data, 0);
					return MAX(DYNAMIC_KEYMAP_MACRO_DELAY, 1);

                }
            }
            // shouldn't arrive here
            macro->state = CM_ERROR;
            return 0;

        case CM_ERROR:
            // this macro encountered an error.  This is a no-recovery state. Is it an error to reach this?
            return 0;

        default:
            // unknown state
            macro->state = CM_ERROR;
            return 0;
    }
}

static void concurrent_macros_send(uint8_t id, keypos_t *keypos) {
    if (id >= dynamic_keymap_macro_get_count()) {
        return;
    }

    // Check the last byte of the buffer.
    // If it's not zero, then we are in the middle
    // of buffer writing, possibly an aborted buffer
    // write. So do nothing.
    uint16_t offset = 0;
    uint16_t end    = dynamic_keymap_macro_get_buffer_size();
    uint8_t  byte;

    dynamic_keymap_macro_get_buffer(end - 1, 1, &byte);
    if (byte != 0) {
        return;
    }

    // Skip N null characters
    // offset will then point to the Nth macro
    while (id > 0) {
        // If we are past the end of the buffer, then there is
        // no Nth macro in the buffer.
        if (offset == end) {
            return;
        }
        dynamic_keymap_macro_get_buffer(offset, 1, &byte);
        if (byte == 0) {
            --id;
        }
        ++offset;
    }

    uint8_t cm_id = find_concurrent_macro(keypos);
    if (cm_id == MACRO_NOT_FOUND) {
        cm_id = reserve_concurrent_macro();
        if (cm_id == RESERVATION_FAILED) {
            // MAX_CONCURRENT_MACROS are in use
            return;
        }
    }

    concurrent_macros_info_t *const macro = &macros[cm_id];
    macro->start_offset                   = offset;

    switch (macro->state) {
        case CM_DO_NEXT:
            // macro is running already
            macro->flags |= CM_FLAG_STOP; // only used by KC_REPEAT_UNTIL_TOGGLE_OFF
            break;

        case CM_ERROR:
            // The user is trying to restart a macro that's in CM_ERROR state.  They might have fixed an error in the
            // macro, so let them retry it.  Fall through to the CM_IDLE state.
        case CM_IDLE:
            // Start the macro from start_offset
            macro->offset = macro->start_offset;
            macro->flags  = 0;
            macro->state  = CM_DO_NEXT;
            macro->keypos = *keypos;
            // Note that the minimum delay value allowed is 1.  Ideally I'd like to be able to use a value of zero.
            macro->de_token = defer_exec_advanced(deferred_executor_table, MAX_CONCURRENT_MACROS, 1, concurrent_macros_execute_one_step, macro);
            break;

        default:
            // Unknown state. Switch to error state
            macro->state = CM_ERROR;
            break;
    }
}

static uint32_t last_deferred_exec_check = 0;

void housekeeping_task_concurrent_macros(void) {
    deferred_exec_advanced_task(deferred_executor_table, MAX_CONCURRENT_MACROS, &last_deferred_exec_check);
}

// Called by QMK core to process VIA-specific keycodes.
bool process_record_concurrent_macros(uint16_t keycode, keyrecord_t *record) {
    // Handle macros
    if (record->event.pressed) {
        if (keycode >= QK_MACRO && keycode <= QK_MACRO_MAX) {
            uint8_t id = keycode - QK_MACRO;
            concurrent_macros_send(id, &record->event.key);
            return false;
        }
    }
    return true;
}

void concurrent_macros_macro_led_state_iterate(iterator_func func) {
    for (int idx = 0; idx < MAX_CONCURRENT_MACROS; idx++) {
        concurrent_macros_info_t *const macro = &macros[idx];
        if (macro->state != CM_FREE) {
            switch (macro->state) {
                case CM_IDLE:
                    func(&(macro->keypos), MACRO_LED_STATE_IDLE);
                    continue;
                case CM_DO_NEXT:
                    if (macro->flags & CM_FLAG_STOP) {
                        func(&(macro->keypos), MACRO_LED_STATE_STOPPING);
                    } else {
                        func(&(macro->keypos), MACRO_LED_STATE_RUNNING);
                    }
                    continue;
                case CM_ERROR:
                default:
                    func(&(macro->keypos), MACRO_LED_STATE_ERROR);
                    continue;
            }
        }
    }
}
