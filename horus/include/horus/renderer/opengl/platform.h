#pragma once

/* horus platform layer */
#include <horus/platform/window.h>

/* horus containers layer */
#include <horus/containers/array.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/renderer.h>

b8 renderer_opengl_context_create(renderer_t *renderer, platform_window_t *window);
b8 renderer_opengl_context_destroy(renderer_t *renderer);

b8 renderer_opengl_context_make_current(renderer_t *renderer);
b8 renderer_opengl_context_swap_buffers(renderer_t *renderer);
