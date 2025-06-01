#pragma once

/* horus input layer */
#include <horus/input/mouse.h>

mouse_button_t __platform_input_mouse_button(/* platform_button_t */ u8 button);

mouse_scroll_direction_t __platform_input_mouse_scroll_direction(/* platform_button_t */ u8 button);

mouse_position_t __platform_input_mouse_current_position(void);
mouse_position_t __platform_input_mouse_previous_position(void);

b8 __platform_input_mouse_set_current_position(mouse_position_t position);
b8 __platform_input_mouse_set_previous_position(mouse_position_t position);

b8 __platform_input_mouse_scroll_set_state(mouse_scroll_state_t state);
b8 __platform_input_mouse_button_set_state(mouse_button_t button, mouse_button_state_t state);

b8 __platform_input_mouse_button_clear_state(void);
b8 __platform_input_mouse_scroll_clear_state(void);
b8 __platform_input_mouse_position_clear_state(void);

mouse_scroll_state_t __platform_input_mouse_scroll_state(void);
mouse_button_state_t __platform_input_mouse_button_state(mouse_button_t button);

mouse_scroll_state_t __platform_input_mouse_scroll_direction_to_state(mouse_scroll_direction_t direction);
