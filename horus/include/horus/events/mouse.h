#pragma once

/* horus events layer */
#include <horus/events/base.h>

/* horus input layer */
#include <horus/input/mouse.h>

typedef struct __mouse_button_press_event {
  event_t base;

  mouse_button_t button;
  mouse_position_t position;
} mouse_button_press_event_t;

typedef struct __mouse_button_release_event {
  event_t base;

  mouse_button_t button;
  mouse_position_t position;
} mouse_button_release_event_t;

typedef struct __mouse_move_event {
  event_t base;

  mouse_position_t position;
} mouse_move_event_t;

typedef struct __mouse_scroll_event {
  event_t base;

  mouse_position_t position;
  mouse_scroll_direction_t direction;
} mouse_scroll_event_t;
