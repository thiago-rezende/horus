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
  (void)renderer; /* unused */

  GLuint array_buffer = 0;

  glGenBuffers(1, &array_buffer);

  if (array_buffer == 0) {
    logger_critical_format("<renderer:%p> vertex buffer creation failed", renderer);

    return NULL;
  }

  glBindBuffer(GL_ARRAY_BUFFER, array_buffer);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * count, vertices, GL_STATIC_DRAW);

  GLenum error = glGetError();

  if (error != GL_NO_ERROR) {
    logger_critical_format("<renderer:%p> vertex buffer data transfer failed", renderer);

    glDeleteBuffers(1, &array_buffer);

    return NULL;
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  vertex_buffer_t *buffer = platform_memory_allocate(sizeof(vertex_buffer_t));

  *buffer = (vertex_buffer_t){
      .array_buffer = array_buffer,
  };

  return buffer;
}

b8 vertex_buffer_bind(vertex_buffer_t *buffer, renderer_t *renderer) {
  (void)renderer; /* unused */

  glBindBuffer(GL_ARRAY_BUFFER, buffer->array_buffer);

  return true;
}

b8 vertex_buffer_destroy(vertex_buffer_t *buffer) {
  glDeleteBuffers(1, &buffer->array_buffer);

  platform_memory_deallocate(buffer);

  return true;
}
