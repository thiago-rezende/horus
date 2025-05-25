#pragma once

/* horus events layer */
#include <horus/events/base.h>

/* horus input layer */
#include <horus/input/keyboard.h>

typedef struct __keyboard_press_event {
  event_t base;

  keyboard_keycode_t keycode;
} keyboard_press_event_t;

typedef struct __keyboard_release_event {
  event_t base;

  keyboard_keycode_t keycode;
} keyboard_release_event_t;

typedef struct __keyboard_hold_event {
  event_t base;

  keyboard_keycode_t keycode;
} keyboard_hold_event_t;
