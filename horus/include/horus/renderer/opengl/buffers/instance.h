#pragma once

/* horus renderer buffers layer */
#include <horus/renderer/buffers/instance.h>

/* horus renderer loader layer [ opengl ] */
#include <horus/renderer/opengl/glad/gl.h>

#define RENDERER_DEFAULT_INSTANCE_BUFFER_BINDING 1

struct __instance_buffer {
  GLuint instance_buffer;
};
