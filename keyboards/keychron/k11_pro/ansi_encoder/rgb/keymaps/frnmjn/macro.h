enum custom_keycodes {
    CUT = SAFE_RANGE,
    COPY,
    PASTE,
    UNDO,
    REDO,
    SAVE,
    SW_TAB,
    SW_WIN,
    DELROW,
    FIND,
    FILE,
    TERM,
    RUN,
};

bool is_ctl_tab_active = false;
uint16_t ctl_tab_timer = 0;

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    switch (keycode) {
      case CUT:
        SEND_STRING(SS_LCTL("x"));
        return false;
        break;
      case COPY:
        SEND_STRING(SS_LCTL("c"));
        return false;
        break;
      case PASTE:
        SEND_STRING(SS_LCTL("v"));
        return false;
        break;
      case UNDO:
        SEND_STRING(SS_LCTL("z"));
        return false;
        break;
      case REDO:
        SEND_STRING(SS_LCTL("y"));
        return false;
        break;
      case SAVE:
        SEND_STRING(SS_LCTL("s"));
        return false;
        break;
      case SW_TAB:
        if (!is_ctl_tab_active) {
          is_ctl_tab_active = true;
          register_code(KC_LCTL);
        }
        ctl_tab_timer = timer_read();
        register_code(KC_TAB);
        return false;
        break;
      case SW_WIN:
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LALT);
        }
        alt_tab_timer = timer_read();
        register_code(KC_TAB);
        return false;
        break;
      case DELROW:
        SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_D)SS_UP(X_LCTL));
        return false;
        break;
      case FIND:
        SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_F)SS_UP(X_LCTL));
        return false;
        break;
      case FILE:
        SEND_STRING(SS_DOWN(X_LCTL)SS_DOWN(X_LSFT)SS_TAP(X_R)SS_UP(X_LCTL)SS_UP(X_LSFT));
        return false;
        break;
      case TERM:
        SEND_STRING(SS_DOWN(X_LCTL)SS_DOWN(X_LALT)SS_TAP(X_T)SS_UP(X_LCTL)SS_UP(X_LALT));
        return false;
        break;
      case RUN:
        SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_ENT)SS_UP(X_LCTL));
        return false;
        break;
    }
  } else {
    switch (keycode) {
      case SW_TAB:
        unregister_code(KC_TAB);
        return false;
        break;
      case SW_WIN:
        unregister_code(KC_TAB);
        return false;
        break;
    }
  }
  return true;
};

void matrix_scan_user(void) {
  if (is_ctl_tab_active) {
    if (timer_elapsed(ctl_tab_timer) > 1000) {
      unregister_code(KC_LCTL);
      is_ctl_tab_active = false;
    }
  }
  else if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}
