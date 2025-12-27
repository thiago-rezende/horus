/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/filesystem.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/shader.h>

b8 __renderer_opengl_shader_module_destroy(GLuint vertex, GLuint compute, GLuint fragment, GLuint program);

shader_module_t *shader_module_create(renderer_t *renderer, shader_stage_flags_t stages, u8 *code, u64 size) {
  (void)stages; /* unused */

  GLuint program = 0;

  GLuint vertex = 0;
  GLuint compute = 0;
  GLuint fragment = 0;

  b8 has_vertex = false;
  b8 has_compute = false;
  b8 has_fragment = false;

  program = glCreateProgram();

  if (program == 0) {
    logger_critical_format("<renderer:%p> vertex shader program creation failed", renderer);

    __renderer_opengl_shader_module_destroy(vertex, compute, fragment, program);

    return NULL;
  }

  if (stages & SHADER_STAGE_VERTEX) {
    has_vertex = true;

    vertex = glCreateShader(GL_VERTEX_SHADER);

    if (vertex == 0) {
      logger_critical_format("<renderer:%p> vertex shader creation failed", renderer);

      __renderer_opengl_shader_module_destroy(vertex, compute, fragment, program);

      return NULL;
    }

    glShaderBinary(1, &vertex, GL_SHADER_BINARY_FORMAT_SPIR_V, code, size);

    glSpecializeShader(vertex, RENDERER_DEFAULT_SHADER_MODULE_VERTEX_STAGE_ENTRYPOINT, 0, NULL, NULL);

    GLint vertex_shader_compilation_status = 0;

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &vertex_shader_compilation_status);

    if (!vertex_shader_compilation_status) {
      logger_critical_format("<renderer:%p> vertex shader compilation failed", renderer);

      __renderer_opengl_shader_module_destroy(vertex, compute, fragment, program);

      return NULL;
    }

    glAttachShader(program, vertex);
  }

  if (stages & SHADER_STAGE_COMPUTE) {
    has_compute = true;

    compute = glCreateShader(GL_COMPUTE_SHADER);

    if (compute == 0) {
      logger_critical_format("<renderer:%p> compute shader creation failed", renderer);

      __renderer_opengl_shader_module_destroy(compute, compute, fragment, program);

      return NULL;
    }

    glShaderBinary(1, &compute, GL_SHADER_BINARY_FORMAT_SPIR_V, code, size);

    glSpecializeShader(compute, RENDERER_DEFAULT_SHADER_MODULE_COMPUTE_STAGE_ENTRYPOINT, 0, NULL, NULL);

    GLint compute_shader_compilation_status = 0;

    glGetShaderiv(compute, GL_COMPILE_STATUS, &compute_shader_compilation_status);

    if (!compute_shader_compilation_status) {
      logger_critical_format("<renderer:%p> compute shader compilation failed", renderer);

      __renderer_opengl_shader_module_destroy(compute, compute, fragment, program);

      return NULL;
    }

    glAttachShader(program, compute);
  }

  if (stages & SHADER_STAGE_FRAGMENT) {
    has_fragment = true;

    fragment = glCreateShader(GL_FRAGMENT_SHADER);

    if (fragment == 0) {
      logger_critical_format("<renderer:%p> fragment shader creation failed", renderer);

      __renderer_opengl_shader_module_destroy(fragment, fragment, fragment, program);

      return NULL;
    }

    glShaderBinary(1, &fragment, GL_SHADER_BINARY_FORMAT_SPIR_V, code, size);

    glSpecializeShader(fragment, RENDERER_DEFAULT_SHADER_MODULE_FRAGMENT_STAGE_ENTRYPOINT, 0, NULL, NULL);

    GLint fragment_shader_compilation_status = 0;

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &fragment_shader_compilation_status);

    if (!fragment_shader_compilation_status) {
      logger_critical_format("<renderer:%p> fragment shader compilation failed", renderer);

      __renderer_opengl_shader_module_destroy(fragment, fragment, fragment, program);

      return NULL;
    }

    glAttachShader(program, fragment);
  }

  glLinkProgram(program);

  GLint shader_program_linking_status = 0;

  glGetProgramiv(program, GL_LINK_STATUS, &shader_program_linking_status);

  if (!shader_program_linking_status) {
    logger_critical_format("<renderer:%p> shader program linking failed", renderer);

    __renderer_opengl_shader_module_destroy(vertex, compute, fragment, program);

    return NULL;
  }

  shader_module_t *module = platform_memory_allocate(sizeof(shader_module_t));

  *module = (shader_module_t){
      .program = program,

      .vertex = vertex,
      .compute = compute,
      .fragment = fragment,

      .has_vertex = has_vertex,
      .has_compute = has_compute,
      .has_fragment = has_fragment,
  };

  return module;
}

shader_module_t *shader_module_create_from_binary(renderer_t *renderer, shader_stage_flags_t stages, char *path) {
  platform_file_t *binary = platform_file_open(path);

  if (binary == NULL) {
    logger_critical_format("<renderer:%p> <path:%s> shader module opening failed", renderer, path);

    return NULL;
  }

  u64 size = platform_file_size(binary);

  u8 *buffer = platform_memory_allocate(sizeof(u8) * size);

  platform_memory_clear((void *)buffer, size);

  u64 bytes_read = platform_file_read(binary, buffer, size);

  if (bytes_read == 0) {
    logger_critical_format("<renderer:%p> <path:%s> shader module reading failed", renderer, path);

    platform_memory_deallocate(buffer);

    platform_file_close(binary);

    return NULL;
  }

  platform_file_close(binary);

  shader_module_t *module = shader_module_create(renderer, stages, buffer, size);

  if (module == NULL) {
    logger_critical_format("<renderer:%p> <path:%s> shader module creation failed", renderer, path);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  platform_memory_deallocate(buffer);

  return module;
}

b8 shader_module_destroy(shader_module_t *module) {
  __renderer_opengl_shader_module_destroy(module->vertex, module->compute, module->fragment, module->program);

  platform_memory_deallocate(module);

  return true;
}

b8 __renderer_opengl_shader_module_destroy(GLuint vertex, GLuint compute, GLuint fragment, GLuint program) {
  if (vertex != 0) {
    glDeleteShader(vertex);
  }

  if (compute != 0) {
    glDeleteShader(compute);
  }

  if (fragment != 0) {
    glDeleteShader(fragment);
  }

  if (program != 0) {
    glDeleteProgram(program);
  }

  return true;
}
