#pragma once

#include <horus/core/types.h>

typedef struct __platform_window platform_window_t;

platform_window_t* platform_window_create(char* title, u16 width, u16 height);

void platform_window_destroy(platform_window_t* window);

void platform_window_process_events(platform_window_t* window);

b8 platform_window_should_close(platform_window_t* window);
