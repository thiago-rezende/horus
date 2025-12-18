/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/renderer.h>
#include <horus/renderer/opengl/pipeline.h>

/* horus renderer buffers layer [ opengl ] */
#include <horus/renderer/opengl/buffers/index.h>

index_buffer_t *index_buffer_create(renderer_t *renderer, u32 *indices, u64 count) {
  (void)count;    /* unused */
  (void)indices;  /* unused */
  (void)renderer; /* unused */

  index_buffer_t *buffer = platform_memory_allocate(sizeof(index_buffer_t));

  *buffer = (index_buffer_t){};

  return buffer;
}

b8 index_buffer_bind(index_buffer_t *buffer, renderer_t *renderer) {
  (void)buffer;   /* unused */
  (void)renderer; /* unused */

  return true;
}

b8 index_buffer_destroy(index_buffer_t *buffer) {
  platform_memory_deallocate(buffer);

  return true;
}
