#pragma once

/* horus renderer layer */
#include <horus/renderer/general.h>

/* horus core layer */
#include <horus/core/types.h>

/* horus application layer */
#include <horus/application/application.h>

/* horus platform layer */
#include <horus/platform/window.h>

typedef struct __renderer renderer_t;

renderer_t *renderer_create(application_t *application, platform_window_t *window);
b8 renderer_destroy(renderer_t *renderer);

renderer_implementation_t renderer_implementation(renderer_t *renderer);
const char *renderer_implementation_string(renderer_t *renderer);
