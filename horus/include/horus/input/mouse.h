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
  MOUSE_BUTTON_COUNT,
} mouse_button_t;

const char *mouse_button_string(mouse_button_t button);
