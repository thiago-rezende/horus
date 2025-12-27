/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/renderer.h>
#include <horus/renderer/opengl/platform.h>

/* horus renderer loader layer [ opengl ] */
#include <horus/renderer/opengl/glad/gl.h>

GLuint global_vertex_array_object = 0;

b8 renderer_context_create(renderer_t *renderer, renderer_context_create_info_t info, platform_window_t *window) {
  (void)info;   /* unused */
  (void)window; /* unused */

  renderer->context = platform_memory_allocate(sizeof(renderer_context_t));

  *renderer->context = (renderer_context_t){
      .implementation = RENDERER_IMPLEMENTATION_OPENGL,
  };

  if (!renderer_opengl_context_create(renderer, window)) {
    logger_critical_format("<renderer:%p> <window:%p> renderer_opengl_context_create failed", renderer, window);

    platform_memory_deallocate(renderer->context);

    return false;
  }

  glGenVertexArrays(1, &global_vertex_array_object);
  glBindVertexArray(global_vertex_array_object);

  return true;
}

b8 renderer_context_destroy(renderer_t *renderer) {
  glDeleteVertexArrays(1, &global_vertex_array_object);

  if (!renderer_opengl_context_destroy(renderer)) {
    logger_critical_format("<renderer:%p> renderer_opengl_context_destroy failed", renderer);

    return false;
  }

  platform_memory_deallocate(renderer->context);

  return true;
}

/* TODO: improve for multiple windows support */
b8 renderer_context_record_commands(renderer_t *renderer) {
  /* TODO: improve window retrieval for multiple windows support */
  platform_window_t *window = platform_window();
  platform_window_size_t window_size = platform_window_size(window);

  if (window_size.width == 0 || window_size.height == 0) {
    return false;
  }

  /* TODO: current context update is needed for multiple windows */
  /* renderer_opengl_context_make_current(renderer); */

  glViewport(0, 0, window_size.width, window_size.height);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return true;
}

/* TODO: improve for multiple windows support */
b8 renderer_context_submit_commands(renderer_t *renderer) {
  renderer_opengl_context_swap_buffers(renderer);

  return true;
}

/* TODO: improve for multiple window support */
b8 renderer_context_draw(renderer_t *renderer, u32 vertices, u32 instances) {
  (void)renderer; /* unused */

  glDrawArraysInstanced(GL_TRIANGLES, 0, vertices, instances);

  return true;
}

/* TODO: improve for multiple window support */
b8 renderer_context_draw_indexed(renderer_t *renderer, u32 indices, u32 instances) {
  (void)renderer; /* unused */

  glDrawElementsInstanced(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0, instances);

  return true;
}

renderer_implementation_t renderer_context_implementation(renderer_t *renderer) {
  return renderer->context->implementation;
}
