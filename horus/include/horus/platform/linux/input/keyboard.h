#pragma once

#include <xcb/xcb.h>

/* horus input layer */
#include <horus/input/keyboard.h>

keyboard_keycode_t platform_keyboard_keycode(xcb_keycode_t keycode);
keyboard_keysymbol_t platform_keyboard_keysymbol(xcb_keysym_t keysymbol);
