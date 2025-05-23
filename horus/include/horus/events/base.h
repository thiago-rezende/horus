#pragma once

typedef enum __envent_type {
  EVENT_TYPE_NONE = 0,
  /* window events */
  EVENT_TYPE_WINDOW_CLOSE,
  EVENT_TYPE_WINDOW_FOCUS,
  EVENT_TYPE_WINDOW_RESIZE,
  EVENT_TYPE_WINDOW_FOCUS_LOSS,
  /* keyboard events */
  EVENT_TYPE_KEYBOARD_HOLD,
  EVENT_TYPE_KEYBOARD_PRESS,
  EVENT_TYPE_KEYBOARD_RELEASE,
  /* mouse events */
  EVENT_TYPE_MOUSE_MOVE,
  EVENT_TYPE_MOUSE_SCROLL,
  EVENT_TYPE_MOUSE_BUTTON_PRESS,
  EVENT_TYPE_MOUSE_BUTTON_RELEASE,
  EVENT_TYPE_COUNT,
} event_type_t;

typedef struct __event {
  event_type_t type;
} event_t;

const char *events_type_string(event_type_t type);
