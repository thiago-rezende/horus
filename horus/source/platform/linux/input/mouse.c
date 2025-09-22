/* horus core layer */
#include <horus/core/assert.h>

/* horus input layer [ linux ] */
#include <horus/platform/linux/input/mouse.h>

#define PLATFORM_MOUSE_BUTTON_LEFT 1
#define PLATFORM_MOUSE_BUTTON_MIDDLE 2
#define PLATFORM_MOUSE_BUTTON_RIGHT 3
#define PLATFORM_MOUSE_BUTTON_SIDE_BACKWARD 8
#define PLATFORM_MOUSE_BUTTON_SIDE_FORWARD 9

#define PLATFORM_MOUSE_SCROLL_DIRECTION_UP 4
#define PLATFORM_MOUSE_SCROLL_DIRECTION_DOWN 5

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

mouse_button_t __platform_input_mouse_button(xcb_button_t button) {
  if (button == PLATFORM_MOUSE_BUTTON_LEFT) {
    return MOUSE_BUTTON_LEFT;
  }

  if (button == PLATFORM_MOUSE_BUTTON_RIGHT) {
    return MOUSE_BUTTON_RIGHT;
  }

  if (button == PLATFORM_MOUSE_BUTTON_MIDDLE) {
    return MOUSE_BUTTON_MIDDLE;
  }

  if (button == PLATFORM_MOUSE_BUTTON_SIDE_BACKWARD) {
    return MOUSE_BUTTON_SIDE_BACKWARD;
  }

  if (button == PLATFORM_MOUSE_BUTTON_SIDE_FORWARD) {
    return MOUSE_BUTTON_SIDE_FORWARD;
  }

  return MOUSE_BUTTON_NONE;
}

mouse_scroll_direction_t __platform_input_mouse_scroll_direction(xcb_button_t button) {
  if (button == PLATFORM_MOUSE_SCROLL_DIRECTION_UP) {
    return MOUSE_SCROLL_DIRECTION_UP;
  }

  if (button == PLATFORM_MOUSE_SCROLL_DIRECTION_DOWN) {
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
