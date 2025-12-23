#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus renderer buffers layer */
#include <horus/renderer/buffers/index.h>

/* horus renderer loader layer [ opengl ] */
#include <horus/renderer/opengl/glad/gl.h>

struct __index_buffer {
  GLuint element_buffer;
};
