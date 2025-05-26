#pragma once

#include <xcb/xcb.h>

/* horus input layer */
#include <horus/input/keyboard.h>

keyboard_keycode_t __platform_input_keyboard_keycode(xcb_keycode_t keycode);

b8 __platform_input_keyboard_keycode_clear_state(void);

b8 __platform_input_keyboard_keycode_set_state(keyboard_keycode_t keycode, keyboard_keycode_state_t state);

keyboard_keycode_state_t __platform_input_keyboard_keycode_state(keyboard_keycode_t keycode);

keyboard_keycode_state_t __platform_input_keyboard_keycode_pressed_state(keyboard_keycode_t keycode);
