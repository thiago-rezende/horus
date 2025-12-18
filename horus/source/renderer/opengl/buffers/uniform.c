/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/renderer.h>
#include <horus/renderer/opengl/pipeline.h>

/* horus renderer buffers layer [ opengl ] */
#include <horus/renderer/opengl/buffers/uniform.h>

uniform_buffer_t *uniform_buffer_create(renderer_t *renderer, uniform_buffer_object_t *object) {
  (void)object;   /* unused */
  (void)renderer; /* unused */

  uniform_buffer_t *buffer = platform_memory_allocate(sizeof(uniform_buffer_t));

  *buffer = (uniform_buffer_t){};

  return buffer;
}

b8 uniform_buffer_bind(uniform_buffer_t *buffer, graphics_pipeline_t *pipeline, renderer_t *renderer) {
  (void)buffer;   /* unused */
  (void)pipeline; /* unused */
  (void)renderer; /* unused */

  return true;
}

b8 uniform_buffer_update(uniform_buffer_t *buffer, uniform_buffer_object_t *object) {
  (void)buffer; /* unused */
  (void)object; /* unused */

  return true;
}

b8 uniform_buffer_destroy(uniform_buffer_t *buffer) {
  platform_memory_deallocate(buffer);

  return true;
}
