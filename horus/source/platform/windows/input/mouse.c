/* horus core layer */
#include <horus/core/assert.h>

/* horus input layer [ windows ] */
#include <horus/platform/windows/input/mouse.h>

static const mouse_scroll_state_t platform_input_mouse_scroll_direction_to_state_mapping[MOUSE_SCROLL_DIRECTION_COUNT] =
    {
        [MOUSE_SCROLL_DIRECTION_NONE] = MOUSE_SCROLL_STATE_NONE, /* */
        [MOUSE_SCROLL_DIRECTION_UP] = MOUSE_SCROLL_STATE_UP,     /* */
        [MOUSE_SCROLL_DIRECTION_DOWN] = MOUSE_SCROLL_STATE_DOWN, /* */
};

mouse_position_t __platform_input_mouse_current_position_state = {0};
mouse_position_t __platform_input_mouse_previous_position_state = {0};

mouse_button_state_t __platform_input_mouse_button_states[MOUSE_BUTTON_COUNT] = {
    MOUSE_BUTTON_STATE_NONE,
};

mouse_scroll_state_t __platform_input_mouse_scroll_current_state = MOUSE_SCROLL_STATE_NONE;

mouse_button_t __platform_input_mouse_button(UINT button, WPARAM wparam) {
  if (button == WM_LBUTTONUP || button == WM_LBUTTONDOWN) {
    return MOUSE_BUTTON_LEFT;
  }

  if (button == WM_RBUTTONUP || button == WM_RBUTTONDOWN) {
    return MOUSE_BUTTON_RIGHT;
  }

  if (button == WM_MBUTTONUP || button == WM_MBUTTONDOWN) {
    return MOUSE_BUTTON_MIDDLE;
  }

  if (button == WM_XBUTTONUP || button == WM_XBUTTONDOWN) {
    WORD xbutton = GET_XBUTTON_WPARAM(wparam);

    if (xbutton == XBUTTON1) {
      return MOUSE_BUTTON_SIDE_BACKWARD;
    }

    if (xbutton == XBUTTON2) {
      return MOUSE_BUTTON_SIDE_FORWARD;
    }
  }

  return MOUSE_BUTTON_NONE;
}

b8 __platform_input_mouse_button_pressed(UINT button) {
  if (button == WM_LBUTTONDOWN) {
    return true;
  }

  if (button == WM_RBUTTONDOWN) {
    return true;
  }

  if (button == WM_MBUTTONDOWN) {
    return true;
  }

  if (button == WM_XBUTTONDOWN) {
    return true;
  }

  return false;
}

b8 __platform_input_mouse_button_released(UINT button) {
  if (button == WM_LBUTTONUP) {
    return true;
  }

  if (button == WM_RBUTTONUP) {
    return true;
  }

  if (button == WM_MBUTTONUP) {
    return true;
  }

  if (button == WM_XBUTTONUP) {
    return true;
  }

  return false;
}

mouse_scroll_direction_t __platform_input_mouse_scroll_direction(WPARAM wparam) {
  short delta = GET_WHEEL_DELTA_WPARAM(wparam);

  if (delta > 0) {
    return MOUSE_SCROLL_DIRECTION_UP;
  }

  if (delta < 0) {
    return MOUSE_SCROLL_DIRECTION_DOWN;
  }

  return MOUSE_SCROLL_DIRECTION_NONE;
}

mouse_position_t __platform_input_mouse_current_position(void) {
  return __platform_input_mouse_current_position_state;
}

mouse_position_t __platform_input_mouse_previous_position(void) {
  return __platform_input_mouse_previous_position_state;
}

b8 __platform_input_mouse_set_current_position(mouse_position_t position) {
  __platform_input_mouse_current_position_state = position;

  return true;
}

b8 __platform_input_mouse_set_previous_position(mouse_position_t position) {
  __platform_input_mouse_previous_position_state = position;

  return true;
}

b8 __platform_input_mouse_scroll_set_state(mouse_scroll_state_t state) {
  if (state < MOUSE_SCROLL_STATE_COUNT && state >= MOUSE_SCROLL_STATE_NONE) {
    __platform_input_mouse_scroll_current_state = state;

    return true;
  }

  return false;
}

b8 __platform_input_mouse_button_set_state(mouse_button_t button, mouse_button_state_t state) {
  if (button < MOUSE_BUTTON_COUNT && button >= MOUSE_BUTTON_NONE) {
    __platform_input_mouse_button_states[button] = state;

    return true;
  }

  return false;
}

b8 __platform_input_mouse_button_clear_state(void) {
  for (mouse_button_t button = MOUSE_BUTTON_NONE; button < MOUSE_BUTTON_COUNT; ++button) {
    if (__platform_input_mouse_button_states[button] == MOUSE_BUTTON_STATE_RELEASED) {
      __platform_input_mouse_button_states[button] = MOUSE_BUTTON_STATE_NONE;
    }
  }

  return true;
}

b8 __platform_input_mouse_scroll_clear_state(void) {
  __platform_input_mouse_scroll_current_state = MOUSE_SCROLL_STATE_NONE;

  return true;
}

b8 __platform_input_mouse_position_clear_state(void) {
  mouse_position_t current_position = __platform_input_mouse_current_position();

  b8 result = __platform_input_mouse_set_previous_position(current_position);

  assert_message(result == true, "__platform_input_mouse_set_previous_position failed");

  return true;
}

mouse_scroll_state_t __platform_input_mouse_scroll_state(void) {
  return __platform_input_mouse_scroll_current_state;
}

mouse_button_state_t __platform_input_mouse_button_state(mouse_button_t button) {
  if (button < MOUSE_BUTTON_COUNT && button >= MOUSE_BUTTON_NONE) {
    return __platform_input_mouse_button_states[button];
  }

  return MOUSE_BUTTON_STATE_NONE;
}

mouse_scroll_state_t __platform_input_mouse_scroll_direction_to_state(mouse_scroll_direction_t direction) {
  if (direction < MOUSE_SCROLL_DIRECTION_COUNT && direction >= MOUSE_SCROLL_DIRECTION_NONE) {
    return platform_input_mouse_scroll_direction_to_state_mapping[direction];
  }

  return MOUSE_SCROLL_STATE_NONE;
}

mouse_position_t platform_input_mouse_position(void) {
  return __platform_input_mouse_current_position();
}

b8 platform_input_mouse_has_moved(void) {
  mouse_position_t current_position = __platform_input_mouse_current_position();
  mouse_position_t previous_position = __platform_input_mouse_previous_position();

  if (current_position.x != previous_position.x || current_position.y != previous_position.y) {
    return true;
  }

  return false;
}

b8 platform_input_mouse_button_is_pressed(mouse_button_t button) {
  return __platform_input_mouse_button_state(button) == MOUSE_BUTTON_STATE_PRESSED;
}

b8 platform_input_mouse_button_is_released(mouse_button_t button) {
  return __platform_input_mouse_button_state(button) == MOUSE_BUTTON_STATE_RELEASED;
}

b8 platform_input_mouse_scroll_is_up(void) {
  return __platform_input_mouse_scroll_current_state == MOUSE_SCROLL_STATE_UP;
}

b8 platform_input_mouse_scroll_is_down(void) {
  return __platform_input_mouse_scroll_current_state == MOUSE_SCROLL_STATE_DOWN;
}

mouse_scroll_state_t platform_input_mouse_scroll_state(void) {
  return __platform_input_mouse_scroll_state();
}

mouse_button_state_t platform_input_mouse_button_state(mouse_button_t button) {
  return __platform_input_mouse_button_state(button);
}
