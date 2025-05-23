#pragma once

#include <xcb/xcb.h>

/* horus input layer */
#include <horus/input/mouse.h>

mouse_button_t platform_mouse_button(xcb_button_t button);
mouse_scroll_direction_t platform_mouse_scroll_direction(xcb_button_t button);
