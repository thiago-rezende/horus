#pragma once

#include <horus/core/types.h>
#include <horus/events/base.h>

typedef enum __mouse_button {
  MOUSE_BUTTON_NONE = 0,
  MOUSE_BUTTON_LEFT,
  MOUSE_BUTTON_RIGHT,
  MOUSE_BUTTON_MIDDLE,
  MOUSE_BUTTON_X1,
  MOUSE_BUTTON_X2,
  MOUSE_BUTTON_COUNT,
} mouse_button_t;

typedef struct __mouse_position {
  u32 x;
  u32 y;
} mouse_position_t;

typedef struct __mouse_move_event {
  event_type_t type;

  mouse_position_t position;
} mouse_move_event_t;
