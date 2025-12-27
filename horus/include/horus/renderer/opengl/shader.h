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
  GLuint program;

  GLuint vertex;
  GLuint compute;
  GLuint fragment;

  b8 has_vertex;
  b8 has_compute;
  b8 has_fragment;
};
