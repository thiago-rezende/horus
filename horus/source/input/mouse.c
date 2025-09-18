/* horus input layer */
#include <horus/input/mouse.h>

/* horus platform layer */
#include <horus/platform/input/mouse.h>

static const char *mouse_button_strings[MOUSE_BUTTON_COUNT] = {
    [MOUSE_BUTTON_NONE] = "none",
    [MOUSE_BUTTON_LEFT] = "left",
    [MOUSE_BUTTON_RIGHT] = "right",
    [MOUSE_BUTTON_MIDDLE] = "middle",
    [MOUSE_BUTTON_SIDE_FORWARD] = "side_forward",
    [MOUSE_BUTTON_SIDE_BACKWARD] = "side_backward",
};

static const char *mouse_button_state_strings[MOUSE_BUTTON_STATE_COUNT] = {
    [MOUSE_BUTTON_STATE_NONE] = "none",
    [MOUSE_BUTTON_STATE_PRESSED] = "pressed",
    [MOUSE_BUTTON_STATE_RELEASED] = "released",
};

const char *mouse_scroll_direction_strings[MOUSE_SCROLL_DIRECTION_COUNT] = {
    [MOUSE_SCROLL_DIRECTION_NONE] = "none",
    [MOUSE_SCROLL_DIRECTION_UP] = "up",
    [MOUSE_SCROLL_DIRECTION_DOWN] = "down",
};

static const char *mouse_scroll_state_strings[MOUSE_SCROLL_STATE_COUNT] = {
    [MOUSE_SCROLL_STATE_NONE] = "none",
    [MOUSE_SCROLL_STATE_UP] = "up",
    [MOUSE_SCROLL_STATE_DOWN] = "down",
};

const char *input_mouse_button_string(mouse_button_t button) {
  if (button < MOUSE_BUTTON_COUNT && button >= MOUSE_BUTTON_NONE) {
    return mouse_button_strings[button];
  }

  return "unknown";
}

const char *input_mouse_button_state_string(mouse_button_state_t state) {
  if (state < MOUSE_BUTTON_STATE_COUNT && state >= MOUSE_BUTTON_STATE_NONE) {
    return mouse_button_state_strings[state];
  }

  return "unknown";
}

const char *input_mouse_scroll_direction_string(mouse_scroll_direction_t direction) {
  if (direction < MOUSE_SCROLL_DIRECTION_COUNT && direction >= MOUSE_SCROLL_DIRECTION_NONE) {
    return mouse_scroll_direction_strings[direction];
  }

  return "unknown";
}

const char *input_mouse_scroll_state_string(mouse_scroll_state_t state) {
  if (state < MOUSE_SCROLL_STATE_COUNT && state >= MOUSE_SCROLL_STATE_NONE) {
    return mouse_scroll_state_strings[state];
  }

  return "unknown";
}

mouse_position_t input_mouse_position(void) {
  return platform_input_mouse_position();
}

b8 input_mouse_has_moved(void) {
  return platform_input_mouse_has_moved();
}

b8 input_mouse_button_is_pressed(mouse_button_t button) {
  return platform_input_mouse_button_is_pressed(button);
}

b8 input_mouse_button_is_released(mouse_button_t button) {
  return platform_input_mouse_button_is_released(button);
}

mouse_button_state_t input_mouse_button_state(mouse_button_t button) {
  return platform_input_mouse_button_state(button);
}

b8 input_mouse_scroll_is_up(void) {
  return platform_input_mouse_scroll_is_up();
}

b8 input_mouse_scroll_is_down(void) {
  return platform_input_mouse_scroll_is_down();
}

mouse_scroll_state_t input_mouse_scroll_state(void) {
  return platform_input_mouse_scroll_state();
}
