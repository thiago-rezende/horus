#pragma once

/* horus input layer */
#include <horus/input/mouse.h>

/* horus events layer */
#include <horus/events/base.h>

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
