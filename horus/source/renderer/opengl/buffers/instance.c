/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/renderer.h>
#include <horus/renderer/opengl/pipeline.h>

/* horus renderer buffers layer [ opengl ] */
#include <horus/renderer/opengl/buffers/instance.h>

#define DEFAULT_INSTANCE_BUFFER_BINDING 1

instance_buffer_t *instance_buffer_create(renderer_t *renderer, instance_buffer_object_t *objects, u32 count) {
  (void)count;    /* unused */
  (void)objects;  /* unused */
  (void)renderer; /* unused */

  instance_buffer_t *buffer = platform_memory_allocate(sizeof(instance_buffer_t));

  *buffer = (instance_buffer_t){};

  return buffer;
}

b8 instance_buffer_bind(instance_buffer_t *buffer, graphics_pipeline_t *pipeline, renderer_t *renderer) {
  (void)buffer;   /* unused */
  (void)pipeline; /* unused */
  (void)renderer; /* unused */

  return true;
}

b8 instance_buffer_update(instance_buffer_t *buffer, instance_buffer_object_t *objects, u32 count) {
  (void)count;   /* unused */
  (void)buffer;  /* unused */
  (void)objects; /* unused */

  return true;
}

b8 instance_buffer_destroy(instance_buffer_t *buffer) {
  platform_memory_deallocate(buffer);

  return true;
}
