/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/renderer.h>

b8 renderer_context_create(renderer_t *renderer, renderer_context_create_info_t info, platform_window_t *window) {
  (void)info;   /* unused */
  (void)window; /* unused */

  renderer->context = platform_memory_allocate(sizeof(renderer_context_t));

  *renderer->context = (renderer_context_t){
      .implementation = RENDERER_IMPLEMENTATION_OPENGL,
  };

  return true;
}

b8 renderer_context_destroy(renderer_t *renderer) {
  platform_memory_deallocate(renderer->context);

  return true;
}

/* TODO: improve for multiple windows support */
b8 renderer_context_record_commands(renderer_t *renderer) {
  /* TODO: improve window retrieval for multiple windows support */
  platform_window_t *window = platform_window();
  platform_window_size_t window_size = platform_window_size(platform_window());

  if (window_size.width == 0 || window_size.height == 0) {
    return false;
  }

  return true;
}

/* TODO: improve for multiple windows support */
b8 renderer_context_submit_commands(renderer_t *renderer) {
  (void)renderer; /* unused */

  return true;
}

/* TODO: improve for multiple window support */
b8 renderer_context_draw(renderer_t *renderer, u32 vertices, u32 instances) {
  (void)renderer;  /* unused */
  (void)vertices;  /* unused */
  (void)instances; /* unused */

  return true;
}

/* TODO: improve for multiple window support */
b8 renderer_context_draw_indexed(renderer_t *renderer, u32 indices, u32 instances) {
  (void)indices;   /* unused */
  (void)renderer;  /* unused */
  (void)instances; /* unused */

  return true;
}

renderer_implementation_t renderer_context_implementation(renderer_t *renderer) {
  return renderer->context->implementation;
}
