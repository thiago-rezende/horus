#pragma once

typedef enum __mouse_button {
  MOUSE_BUTTON_NONE = 0,
  MOUSE_BUTTON_LEFT,
  MOUSE_BUTTON_RIGHT,
  MOUSE_BUTTON_MIDDLE,
  MOUSE_BUTTON_COUNT,
} mouse_button_t;

const char *mouse_button_string(mouse_button_t button);
