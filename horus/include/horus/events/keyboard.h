#pragma once

/* horus input layer */
#include <horus/input/keyboard.h>

/* horus events layer */
#include <horus/events/base.h>

typedef struct __keyboard_press_event {
  event_t base;

  keyboard_keycode_t keycode;
  keyboard_keysymbol_t keysymbol;
} keyboard_press_event_t;

typedef struct __keyboard_release_event {
  event_t base;

  keyboard_keycode_t keycode;
  keyboard_keysymbol_t keysymbol;
} keyboard_release_event_t;

typedef struct __keyboard_hold_event {
  event_t base;

  keyboard_keycode_t keycode;
  keyboard_keysymbol_t keysymbol;
} keyboard_hold_event_t;
