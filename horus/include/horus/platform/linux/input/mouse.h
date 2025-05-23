#pragma once

#include <xcb/xcb.h>

/* horus input layer */
#include <horus/input/mouse.h>

mouse_button_t platform_input_mouse_button(xcb_button_t button);

mouse_scroll_direction_t platform_input_mouse_scroll_direction(xcb_button_t button);

mouse_scroll_state_t platform_input_mouse_scroll_state();
mouse_button_state_t platform_input_mouse_button_state(mouse_button_t button);

b8 platform_input_mouse_button_is_pressed(mouse_button_t button);
b8 platform_input_mouse_button_is_released(mouse_button_t button);

b8 platform_input_mouse_scroll_is_up();
b8 platform_input_mouse_scroll_is_down();

b8 __platform_input_mouse_scroll_set_state();
b8 __platform_input_mouse_button_set_state(mouse_button_t button, mouse_button_state_t state);

b8 __platform_input_mouse_button_clear_state();
b8 __platform_input_mouse_scroll_clear_state();

mouse_scroll_state_t __platform_input_mouse_scroll_state();
mouse_button_state_t __platform_input_mouse_button_state(mouse_button_t button);
mouse_scroll_state_t __platform_input_mouse_scroll_direction_to_state(mouse_scroll_direction_t direction);
