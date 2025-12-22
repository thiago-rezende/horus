#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus containers layer */
#include <horus/containers/array.h>

/* horus renderer layer */
#include <horus/renderer/shader.h>

/* horus renderer loader layer [ opengl ] */
#include <horus/renderer/opengl/glad/gl.h>

struct __shader_module {
  GLuint shader_program;

  GLuint vertex_shader;
  GLuint compute_shader;
  GLuint fragment_shader;

  b8 has_vertex_shader;
  b8 has_compute_shader;
  b8 has_fragment_shader;
};
