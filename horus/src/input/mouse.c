/* horus input layer */
#include <horus/input/mouse.h>

static const char *mouse_button_strings[MOUSE_BUTTON_COUNT] = {
    [MOUSE_BUTTON_NONE] = "none",                   /* */
    [MOUSE_BUTTON_LEFT] = "left",                   /* */
    [MOUSE_BUTTON_RIGHT] = "right",                 /* */
    [MOUSE_BUTTON_MIDDLE] = "middle",               /* */
    [MOUSE_BUTTON_SIDE_FORWARD] = "side_forward",   /* */
    [MOUSE_BUTTON_SIDE_BACKWARD] = "side_backward", /* */
};

const char *mouse_scroll_direction_strings[MOUSE_SCROLL_DIRECTION_COUNT] = {
    [MOUSE_SCROLL_DIRECTION_NONE] = "none",   /* */
    [MOUSE_SCROLL_DIRECTION_UP] = "up",       /* */
    [MOUSE_SCROLL_DIRECTION_DOWN] = "down",   /* */
    [MOUSE_SCROLL_DIRECTION_LEFT] = "left",   /* */
    [MOUSE_SCROLL_DIRECTION_RIGHT] = "right", /* */
};

const char *mouse_button_string(mouse_button_t button) {
  if (button < MOUSE_BUTTON_COUNT && button >= 0) {
    return mouse_button_strings[button];
  }

  return "unknown";
}

const char *mouse_scroll_direction_string(mouse_scroll_direction_t direction) {
  if (direction < MOUSE_SCROLL_DIRECTION_COUNT && direction >= 0) {
    return mouse_scroll_direction_strings[direction];
  }

  return "unknown";
}
