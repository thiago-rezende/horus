#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus renderer layer */
#include <horus/renderer/texture.h>

/* horus renderer loader layer [ opengl ] */
#include <horus/renderer/opengl/glad/gl.h>

struct __texture_context {
  GLenum target;
  GLuint texture;
  GLuint binding;
};
