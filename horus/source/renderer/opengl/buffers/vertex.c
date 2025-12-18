/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/renderer.h>
#include <horus/renderer/opengl/pipeline.h>

/* horus renderer buffers layer [ opengl ] */
#include <horus/renderer/opengl/buffers/vertex.h>

vertex_buffer_t *vertex_buffer_create(renderer_t *renderer, vertex_t *vertices, u64 count) {
  (void)count;    /* unused */
  (void)vertices; /* unused */
  (void)renderer; /* unused */

  vertex_buffer_t *buffer = platform_memory_allocate(sizeof(vertex_buffer_t));

  *buffer = (vertex_buffer_t){};

  return buffer;
}

b8 vertex_buffer_bind(vertex_buffer_t *buffer, renderer_t *renderer) {
  (void)buffer;   /* unused */
  (void)renderer; /* unused */

  return true;
}

b8 vertex_buffer_destroy(vertex_buffer_t *buffer) {
  platform_memory_deallocate(buffer);

  return true;
}
