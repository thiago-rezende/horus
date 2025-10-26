#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus platform layer */
#include <horus/platform/window.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>

/* horus application layer */
#include <horus/application/application.h>

application_t *application_create(void);
b8 application_destroy(application_t *application);

b8 on_create(application_t *application, platform_window_t *window, renderer_t *renderer);
b8 on_destroy(application_t *application, platform_window_t *window, renderer_t *renderer);

b8 on_event(event_t *event);
b8 on_update(f64 timestep);
b8 on_render(renderer_t *renderer);
