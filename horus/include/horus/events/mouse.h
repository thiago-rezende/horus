#pragma once

#include <horus/core/types.h>
#include <horus/events/base.h>

typedef enum __mouse_button {
  MOUSE_BUTTON_NONE = 0,
  MOUSE_BUTTON_LEFT,
  MOUSE_BUTTON_RIGHT,
  MOUSE_BUTTON_MIDDLE,
  MOUSE_BUTTON_COUNT,
} mouse_button_t;

typedef struct __mouse_position {
  u32 x;
  u32 y;
} mouse_position_t;

typedef struct __mouse_button_press_event {
  event_type_t type;

  mouse_button_t button;
  mouse_position_t position;
} mouse_button_press_event_t;

typedef struct __mouse_button_release_event {
  event_type_t type;

  mouse_button_t button;
  mouse_position_t position;
} mouse_button_release_event_t;

typedef struct __mouse_move_event {
  event_type_t type;

  mouse_position_t position;
} mouse_move_event_t;

const char *mouse_button_string(mouse_button_t button);
