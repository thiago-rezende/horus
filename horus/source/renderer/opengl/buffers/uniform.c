/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/renderer.h>
#include <horus/renderer/opengl/pipeline.h>

/* horus renderer loader layer [ opengl ] */
#include <horus/renderer/opengl/glad/gl.h>

/* horus renderer buffers layer [ opengl ] */
#include <horus/renderer/opengl/buffers/uniform.h>

uniform_buffer_t *uniform_buffer_create(renderer_t *renderer, uniform_buffer_object_t *object) {
  (void)renderer; /* unused */

  GLuint uniform_buffer = 0;

  glGenBuffers(1, &uniform_buffer);

  if (uniform_buffer == 0) {
    logger_critical_format("<renderer:%p> uniform buffer creation failed", renderer);

    return NULL;
  }

  glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer);
  glBindBufferBase(GL_UNIFORM_BUFFER, RENDERER_DEFAULT_UNIFORM_BUFFER_OBJECT_BINDING, uniform_buffer);

  glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform_buffer_object_t), NULL, GL_STATIC_DRAW);

  GLenum error = glGetError();

  if (error != GL_NO_ERROR) {
    logger_critical_format("<renderer:%p> uniform buffer allocation failed", renderer);

    glDeleteBuffers(1, &uniform_buffer);

    return NULL;
  }

  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  uniform_buffer_t *buffer = platform_memory_allocate(sizeof(uniform_buffer_t));

  *buffer = (uniform_buffer_t){
      .uniform_buffer = uniform_buffer,
  };

  return buffer;
}

b8 uniform_buffer_bind(uniform_buffer_t *buffer, graphics_pipeline_t *pipeline, renderer_t *renderer) {
  (void)pipeline; /* unused */
  (void)renderer; /* unused */

  glBindBuffer(GL_UNIFORM_BUFFER, buffer->uniform_buffer);
  glBindBufferBase(GL_UNIFORM_BUFFER, RENDERER_DEFAULT_UNIFORM_BUFFER_OBJECT_BINDING, buffer->uniform_buffer);

  return true;
}

b8 uniform_buffer_update(uniform_buffer_t *buffer, uniform_buffer_object_t *object) {
  glBindBuffer(GL_UNIFORM_BUFFER, buffer->uniform_buffer);

  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(uniform_buffer_object_t), object);

  GLenum error = glGetError();

  if (error != GL_NO_ERROR) {
    logger_critical_format("<buffer:%p> uniform buffer data transfer failed", buffer);

    return false;
  }

  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  return true;
}

b8 uniform_buffer_destroy(uniform_buffer_t *buffer) {
  glDeleteBuffers(1, &buffer->uniform_buffer);

  platform_memory_deallocate(buffer);

  return true;
}
