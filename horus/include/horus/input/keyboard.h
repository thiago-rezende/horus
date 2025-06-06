#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus events layer */
#include <horus/events/base.h>

typedef enum __keyboard_keycode {
  KEYBOARD_KEYCODE_NONE = 0,

  /* letters */
  KEYBOARD_KEYCODE_A,
  KEYBOARD_KEYCODE_B,
  KEYBOARD_KEYCODE_C,
  KEYBOARD_KEYCODE_D,
  KEYBOARD_KEYCODE_E,
  KEYBOARD_KEYCODE_F,
  KEYBOARD_KEYCODE_G,
  KEYBOARD_KEYCODE_H,
  KEYBOARD_KEYCODE_I,
  KEYBOARD_KEYCODE_J,
  KEYBOARD_KEYCODE_K,
  KEYBOARD_KEYCODE_L,
  KEYBOARD_KEYCODE_M,
  KEYBOARD_KEYCODE_N,
  KEYBOARD_KEYCODE_O,
  KEYBOARD_KEYCODE_P,
  KEYBOARD_KEYCODE_Q,
  KEYBOARD_KEYCODE_R,
  KEYBOARD_KEYCODE_S,
  KEYBOARD_KEYCODE_T,
  KEYBOARD_KEYCODE_U,
  KEYBOARD_KEYCODE_V,
  KEYBOARD_KEYCODE_W,
  KEYBOARD_KEYCODE_X,
  KEYBOARD_KEYCODE_Y,
  KEYBOARD_KEYCODE_Z,

  /* numbers */
  KEYBOARD_KEYCODE_0,
  KEYBOARD_KEYCODE_1,
  KEYBOARD_KEYCODE_2,
  KEYBOARD_KEYCODE_3,
  KEYBOARD_KEYCODE_4,
  KEYBOARD_KEYCODE_5,
  KEYBOARD_KEYCODE_6,
  KEYBOARD_KEYCODE_7,
  KEYBOARD_KEYCODE_8,
  KEYBOARD_KEYCODE_9,

  /* general */
  KEYBOARD_KEYCODE_TAB,
  KEYBOARD_KEYCODE_ENTER,
  KEYBOARD_KEYCODE_SPACE,
  KEYBOARD_KEYCODE_ESCAPE,
  KEYBOARD_KEYCODE_BACKSPACE,

  /* arrows */
  KEYBOARD_KEYCODE_UP,
  KEYBOARD_KEYCODE_DOWN,
  KEYBOARD_KEYCODE_LEFT,
  KEYBOARD_KEYCODE_RIGHT,

  /* modifiers */
  KEYBOARD_KEYCODE_PRINT,
  KEYBOARD_KEYCODE_PAGE_UP,
  KEYBOARD_KEYCODE_LEFT_ALT,
  KEYBOARD_KEYCODE_NUM_LOCK,
  KEYBOARD_KEYCODE_CAPS_LOCK,
  KEYBOARD_KEYCODE_LEFT_META,
  KEYBOARD_KEYCODE_PAGE_DOWN,
  KEYBOARD_KEYCODE_RIGHT_ALT,
  KEYBOARD_KEYCODE_LEFT_SHIFT,
  KEYBOARD_KEYCODE_LEFT_SUPER,
  KEYBOARD_KEYCODE_RIGHT_META,
  KEYBOARD_KEYCODE_RIGHT_SHIFT,
  KEYBOARD_KEYCODE_RIGHT_SUPER,
  KEYBOARD_KEYCODE_SCROLL_LOCK,
  KEYBOARD_KEYCODE_LEFT_BRACKET,
  KEYBOARD_KEYCODE_LEFT_CONTROL,
  KEYBOARD_KEYCODE_RIGHT_BRACKET,
  KEYBOARD_KEYCODE_RIGHT_CONTROL,

  /* functions */
  KEYBOARD_KEYCODE_F1,
  KEYBOARD_KEYCODE_F2,
  KEYBOARD_KEYCODE_F3,
  KEYBOARD_KEYCODE_F4,
  KEYBOARD_KEYCODE_F5,
  KEYBOARD_KEYCODE_F6,
  KEYBOARD_KEYCODE_F7,
  KEYBOARD_KEYCODE_F8,
  KEYBOARD_KEYCODE_F9,
  KEYBOARD_KEYCODE_F10,
  KEYBOARD_KEYCODE_F11,
  KEYBOARD_KEYCODE_F12,

  /* general */
  KEYBOARD_KEYCODE_END,
  KEYBOARD_KEYCODE_HOME,
  KEYBOARD_KEYCODE_DELETE,
  KEYBOARD_KEYCODE_INSERT,
  KEYBOARD_KEYCODE_PAGEUP,
  KEYBOARD_KEYCODE_PAGEDOWN,

  /* general */
  KEYBOARD_KEYCODE_NUMLOCK,
  KEYBOARD_KEYCODE_CAPSLOCK,
  KEYBOARD_KEYCODE_SCROLLLOCK,

  /* general */
  KEYBOARD_KEYCODE_PAUSE,
  KEYBOARD_KEYCODE_PRINTSCREEN,

  /* symbols */
  KEYBOARD_KEYCODE_COMMA,
  KEYBOARD_KEYCODE_GRAVE,
  KEYBOARD_KEYCODE_MINUS,
  KEYBOARD_KEYCODE_SLASH,
  KEYBOARD_KEYCODE_EQUALS,
  KEYBOARD_KEYCODE_PERIOD,
  KEYBOARD_KEYCODE_BACKSLASH,
  KEYBOARD_KEYCODE_SEMICOLON,
  KEYBOARD_KEYCODE_APOSTROPHE,

  /* keycodes count */
  KEYBOARD_KEYCODE_COUNT,
} keyboard_keycode_t;

typedef enum __keyboard_keycode_state {
  KEYBOARD_KEYCODE_STATE_NONE = 0,
  KEYBOARD_KEYCODE_STATE_HELD,
  KEYBOARD_KEYCODE_STATE_PRESSED,
  KEYBOARD_KEYCODE_STATE_RELEASED,
  KEYBOARD_KEYCODE_STATE_COUNT,
} keyboard_keycode_state_t;

const char *input_keyboard_keycode_string(keyboard_keycode_t keycode);

const char *input_keyboard_keycode_state_string(keyboard_keycode_state_t state);

b8 input_keyboard_keycode_is_held(keyboard_keycode_t keycode);
b8 input_keyboard_keycode_is_pressed(keyboard_keycode_t keycode);
b8 input_keyboard_keycode_is_released(keyboard_keycode_t keycode);

keyboard_keycode_state_t input_keyboard_keycode_state(keyboard_keycode_t keycode);
