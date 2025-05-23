#pragma once

/* horus core layer */
#include <horus/core/types.h>

typedef struct __mouse_position {
  u32 x;
  u32 y;
} mouse_position_t;

typedef enum __mouse_button {
  MOUSE_BUTTON_NONE = 0,
  MOUSE_BUTTON_LEFT,
  MOUSE_BUTTON_RIGHT,
  MOUSE_BUTTON_MIDDLE,
  MOUSE_BUTTON_SIDE_FORWARD,
  MOUSE_BUTTON_SIDE_BACKWARD,
  MOUSE_BUTTON_COUNT,
} mouse_button_t;

typedef enum __mouse_scroll_direction {
  MOUSE_SCROLL_DIRECTION_NONE = 0,
  MOUSE_SCROLL_DIRECTION_UP,
  MOUSE_SCROLL_DIRECTION_DOWN,
  MOUSE_SCROLL_DIRECTION_LEFT,
  MOUSE_SCROLL_DIRECTION_RIGHT,
  MOUSE_SCROLL_DIRECTION_COUNT,
} mouse_scroll_direction_t;

const char *mouse_button_string(mouse_button_t button);
const char *mouse_scroll_direction_string(mouse_scroll_direction_t direction);
