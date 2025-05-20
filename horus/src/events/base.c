#include <horus/events/base.h>

static const char *event_type_strings[EVENT_TYPE_COUNT] = {
    [EVENT_TYPE_NONE] = "none",
    /* window events */
    [EVENT_TYPE_WINDOW_CLOSE] = "window_close",
    [EVENT_TYPE_WINDOW_RESIZE] = "window_resize",
    [EVENT_TYPE_WINDOW_FOCUS] = "window_focus",
    [EVENT_TYPE_WINDOW_FOCUS_LOSS] = "window_focus_loss",
    /* keyboard events */
    [EVENT_TYPE_KEYBOARD_PRESS] = "keyboard_press",
    [EVENT_TYPE_KEYBOARD_RELEASE] = "keyboard_release",
    /* mouse events */
    [EVENT_TYPE_MOUSE_MOVE] = "mouse_move",
    [EVENT_TYPE_MOUSE_BUTTON_PRESS] = "mouse_button_press",
    [EVENT_TYPE_MOUSE_BUTTON_RELEASE] = "mouse_button_release",
};

const char *event_type_string(event_type_t type) {
  if (type < EVENT_TYPE_COUNT && type >= 0) {
    return event_type_strings[type];
  }

  return "unknown";
}
