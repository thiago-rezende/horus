/* horus input layer */
#include <horus/input/mouse.h>

static const char *mouse_button_strings[MOUSE_BUTTON_COUNT] = {
    [MOUSE_BUTTON_NONE] = "none",
    [MOUSE_BUTTON_LEFT] = "left",
    [MOUSE_BUTTON_RIGHT] = "right",
    [MOUSE_BUTTON_MIDDLE] = "middle",
};

const char *mouse_button_string(mouse_button_t button) {
  if (button < MOUSE_BUTTON_COUNT && button >= 0) {
    return mouse_button_strings[button];
  }

  return "unknown";
}
