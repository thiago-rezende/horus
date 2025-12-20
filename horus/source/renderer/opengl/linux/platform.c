/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/window.h>

/* horus platform layer [ linux ] */
#include <horus/platform/linux/window.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/renderer.h>
#include <horus/renderer/opengl/platform.h>

struct __platform_renderer_context {};

/* TODO: imporve for multiple windows support */
b8 renderer_opengl_context_create(renderer_t *renderer, platform_window_t *window) {
  platform_window_context_t *window_context = platform_window_context(window);

  renderer->context->platform_context = platform_memory_allocate(sizeof(platform_renderer_context_t));

  *renderer->context->platform_context = (platform_renderer_context_t){};

  return true;
}

/* TODO: imporve for multiple windows support */
b8 renderer_opengl_context_destroy(renderer_t *renderer) {
  platform_window_t *window = platform_window();
  platform_window_context_t *window_context = platform_window_context(window);

  platform_renderer_context_t *platform_renderer_context = renderer->context->platform_context;

  platform_memory_deallocate(platform_renderer_context);

  return true;
}

/* TODO: imporve for multiple windows support */
b8 renderer_opengl_context_make_current(renderer_t *renderer) {
  (void)renderer; /* unused */

  return true;
}

/* TODO: imporve for multiple windows support */
b8 renderer_opengl_context_swap_buffers(renderer_t *renderer) {
  (void)renderer; /* unused */

  return true;
}
