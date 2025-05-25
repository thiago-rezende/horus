#pragma once

/* horus input layer */
#include <horus/input/keyboard.h>

b8 platform_input_keyboard_keycode_is_held(keyboard_keycode_t keycode);
b8 platform_input_keyboard_keycode_is_pressed(keyboard_keycode_t keycode);
b8 platform_input_keyboard_keycode_is_released(keyboard_keycode_t keycode);

keyboard_keycode_state_t platform_input_keyboard_keycode_state(keyboard_keycode_t keycode);
