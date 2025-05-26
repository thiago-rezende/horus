#pragma once

/* horus input layer */
#include <horus/input/mouse.h>

mouse_position_t platform_input_mouse_position(void);

b8 platform_input_mouse_has_moved(void);

b8 platform_input_mouse_button_is_pressed(mouse_button_t button);
b8 platform_input_mouse_button_is_released(mouse_button_t button);

b8 platform_input_mouse_scroll_is_up(void);
b8 platform_input_mouse_scroll_is_down(void);

mouse_scroll_state_t platform_input_mouse_scroll_state(void);
mouse_button_state_t platform_input_mouse_button_state(mouse_button_t button);
