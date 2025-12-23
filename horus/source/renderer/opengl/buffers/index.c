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
  (void)renderer; /* unused */

  GLuint element_buffer = 0;

  glGenBuffers(1, &element_buffer);

  if (element_buffer == 0) {
    logger_critical_format("<renderer:%p> index buffer creation failed", renderer);

    return NULL;
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * count, indices, GL_STATIC_DRAW);

  GLenum error = glGetError();

  if (error != GL_NO_ERROR) {
    logger_critical_format("<renderer:%p> index buffer data transfer failed", renderer);

    glDeleteBuffers(1, &element_buffer);

    return NULL;
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  index_buffer_t *buffer = platform_memory_allocate(sizeof(index_buffer_t));

  *buffer = (index_buffer_t){
      .element_buffer = element_buffer,
  };

  return buffer;
}

b8 index_buffer_bind(index_buffer_t *buffer, renderer_t *renderer) {
  (void)renderer; /* unused */

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->element_buffer);

  return true;
}

b8 index_buffer_destroy(index_buffer_t *buffer) {
  glDeleteBuffers(1, &buffer->element_buffer);

  platform_memory_deallocate(buffer);

  return true;
}
