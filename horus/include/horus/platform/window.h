#pragma once

#include <horus/core/types.h>
#include <horus/events/base.h>

typedef struct __platform_window platform_window_t;

typedef b8 (*platform_window_event_callback_t)(event_t *event);

platform_window_t *platform_window_create(char *title, u16 width, u16 height, b8 windowed);

void platform_window_destroy(platform_window_t *window);

void platform_window_process_events(platform_window_t *window);

b8 platform_window_should_close(platform_window_t *window);

b8 platform_window_has_focus(platform_window_t *window);

void platform_window_size(platform_window_t *window, u16 *width, u16 *height);

b8 platform_window_set_size(platform_window_t *window, u16 width, u16 height);

b8 platform_window_set_title(platform_window_t *window, char *title);

b8 platform_window_is_fullscreen(platform_window_t *window);

b8 platform_window_set_fullscreen(platform_window_t *window, b8 fullscreen);

b8 platform_window_set_event_callback(platform_window_t *window, platform_window_event_callback_t callback);
