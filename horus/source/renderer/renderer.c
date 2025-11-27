/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>

const char *renderer_implementation_strings[RENDERER_IMPLEMENTATION_COUNT] = {
    [RENDERER_IMPLEMENTATION_NONE] = "none",
    [RENDERER_IMPLEMENTATION_VULKAN] = "vulkan",
};

renderer_t *renderer_create(renderer_context_create_info_t info, platform_window_t *window) {
  renderer_t *renderer = platform_memory_allocate(sizeof(renderer_t));

  if (!renderer_context_create(renderer, info, window)) {
    logger_critical_format("<renderer:%p> context creation failed", renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  renderer->implementation = renderer_context_implementation(renderer);

  return renderer;
}

b8 renderer_destroy(renderer_t *renderer) {
  if (!renderer_context_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <context:%p> context destruction failed", renderer, renderer);

    return false;
  }

  platform_memory_deallocate(renderer);

  return true;
}

const char *renderer_implementation_string(renderer_implementation_t renderer_implementation) {
  if (renderer_implementation < RENDERER_IMPLEMENTATION_COUNT &&
      renderer_implementation >= RENDERER_IMPLEMENTATION_NONE) {
    return renderer_implementation_strings[renderer_implementation];
  }

  return "unknown";
}

b8 renderer_record_commands(renderer_t *renderer) {
  return renderer_context_record_commands(renderer);
}
b8 renderer_submit_commands(renderer_t *renderer) {
  return renderer_context_submit_commands(renderer);
}

b8 renderer_draw(renderer_t *renderer, u32 vertices, u32 instances) {
  return renderer_context_draw(renderer, vertices, instances);
}

b8 renderer_draw_indexed(renderer_t *renderer, u32 indices, u32 instances) {
  return renderer_context_draw_indexed(renderer, indices, instances);
}
