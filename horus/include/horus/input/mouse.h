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

typedef enum __mouse_button_state {
  MOUSE_BUTTON_STATE_NONE = 0,
  MOUSE_BUTTON_STATE_PRESSED,
  MOUSE_BUTTON_STATE_RELEASED,
  MOUSE_BUTTON_STATE_COUNT,
} mouse_button_state_t;

typedef enum __mouse_scroll_direction {
  MOUSE_SCROLL_DIRECTION_NONE = 0,
  MOUSE_SCROLL_DIRECTION_UP,
  MOUSE_SCROLL_DIRECTION_DOWN,
  MOUSE_SCROLL_DIRECTION_COUNT,
} mouse_scroll_direction_t;

typedef enum __mouse_scroll_state {
  MOUSE_SCROLL_STATE_NONE = 0,
  MOUSE_SCROLL_STATE_UP,
  MOUSE_SCROLL_STATE_DOWN,
  MOUSE_SCROLL_STATE_COUNT,
} mouse_scroll_state_t;

const char *input_mouse_button_string(mouse_button_t button);
const char *input_mouse_button_state_string(mouse_button_state_t state);

const char *input_mouse_scroll_state_string(mouse_scroll_state_t state);
const char *input_mouse_scroll_direction_string(mouse_scroll_direction_t direction);

mouse_position_t input_mouse_position(void);

b8 input_mouse_has_moved(void);

b8 input_mouse_button_is_pressed(mouse_button_t button);
b8 input_mouse_button_is_released(mouse_button_t button);

mouse_button_state_t input_mouse_button_state(mouse_button_t button);

b8 input_mouse_scroll_is_up(void);
b8 input_mouse_scroll_is_down(void);

mouse_scroll_state_t input_mouse_scroll_state(void);

b8 input_mouse_clear_state(void);
