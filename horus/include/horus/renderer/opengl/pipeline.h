#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus containers layer */
#include <horus/containers/array.h>

/* horus renderer layer */
#include <horus/renderer/pipeline.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/renderer.h>

struct __graphics_pipeline {
  shader_module_t *module;
};
