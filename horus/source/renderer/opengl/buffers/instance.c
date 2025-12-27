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
#include <horus/renderer/opengl/buffers/instance.h>

instance_buffer_t *instance_buffer_create(renderer_t *renderer, instance_buffer_object_t *objects, u32 count) {
  (void)renderer; /* unused */

  GLuint instance_buffer = 0;

  glGenBuffers(1, &instance_buffer);

  if (instance_buffer == 0) {
    logger_critical_format("<renderer:%p> uniform buffer creation failed", renderer);

    return NULL;
  }

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, instance_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, RENDERER_DEFAULT_INSTANCE_BUFFER_BINDING, instance_buffer);

  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(instance_buffer_object_t) * count, NULL, GL_DYNAMIC_DRAW);

  GLenum error = glGetError();

  if (error != GL_NO_ERROR) {
    logger_critical_format("<renderer:%p> instance buffer allocation failed", renderer);

    glDeleteBuffers(1, &instance_buffer);

    return NULL;
  }

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  instance_buffer_t *buffer = platform_memory_allocate(sizeof(instance_buffer_t));

  *buffer = (instance_buffer_t){
      .instance_buffer = instance_buffer,
  };

  return buffer;
}

b8 instance_buffer_bind(instance_buffer_t *buffer, graphics_pipeline_t *pipeline, renderer_t *renderer) {
  (void)pipeline; /* unused */
  (void)renderer; /* unused */

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer->instance_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, RENDERER_DEFAULT_INSTANCE_BUFFER_BINDING, buffer->instance_buffer);

  return true;
}

b8 instance_buffer_update(instance_buffer_t *buffer, instance_buffer_object_t *objects, u32 count) {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer->instance_buffer);

  glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(instance_buffer_object_t) * count, objects);

  GLenum error = glGetError();

  if (error != GL_NO_ERROR) {
    logger_critical_format("<buffer:%p> instnace buffer data transfer failed", buffer);

    return false;
  }

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  return true;
}

b8 instance_buffer_destroy(instance_buffer_t *buffer) {
  glDeleteBuffers(1, &buffer->instance_buffer);

  platform_memory_deallocate(buffer);

  return true;
}
