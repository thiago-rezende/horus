#pragma once

/* horus renderer layer */
#include <horus/renderer/buffers/uniform.h>

/* horus renderer loader layer [ opengl ] */
#include <horus/renderer/opengl/glad/gl.h>

#define RENDERER_DEFAULT_UNIFORM_BUFFER_OBJECT_BINDING 0

struct __uniform_buffer {
  GLuint uniform_buffer;
};
