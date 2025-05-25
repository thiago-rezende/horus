#pragma once

/* horus input layer */
#include <horus/input/mouse.h>

mouse_position_t platform_input_mouse_position();

b8 platform_input_mouse_has_moved();

b8 platform_input_mouse_button_is_pressed(mouse_button_t button);
b8 platform_input_mouse_button_is_released(mouse_button_t button);

b8 platform_input_mouse_scroll_is_up();
b8 platform_input_mouse_scroll_is_down();

mouse_scroll_state_t platform_input_mouse_scroll_state();
mouse_button_state_t platform_input_mouse_button_state(mouse_button_t button);
