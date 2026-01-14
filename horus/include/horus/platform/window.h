#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus events layer */
#include <horus/events/base.h>

typedef struct __platform_window platform_window_t;
typedef struct __platform_window_context platform_window_context_t;

typedef b8 (*platform_window_event_callback_t)(event_t *event);

typedef struct __platform_window_size_t {
  u16 width;
  u16 height;
} platform_window_size_t;

platform_window_t *platform_window_create(char *title, platform_window_size_t size, b8 fullscreen);

b8 platform_window_destroy(platform_window_t *window);

b8 platform_window_process_events(platform_window_t *window);

b8 platform_window_should_close(platform_window_t *window);

b8 platform_window_set_should_close(platform_window_t *window, b8 should_close);

b8 platform_window_has_focus(platform_window_t *window);

b8 platform_window_has_resized(platform_window_t *window);

platform_window_size_t platform_window_size(platform_window_t *window);

b8 platform_window_set_size(platform_window_t *window, platform_window_size_t size);

b8 platform_window_set_title(platform_window_t *window, char *title);

b8 platform_window_is_fullscreen(platform_window_t *window);

b8 platform_window_set_fullscreen(platform_window_t *window, b8 fullscreen);

b8 platform_window_set_event_callback(platform_window_t *window, platform_window_event_callback_t callback);

platform_window_context_t *platform_window_context(platform_window_t *window);

platform_window_t *platform_window_get_global_instance(void);

b8 __platform_window_set_global_instance(platform_window_t *window);

b8 __platform_window_resized_clear_state(platform_window_t *window);
