#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus math layer */
#include <horus/math/vector.h>

typedef struct __vertex {
  vector3f32_t position;

  vector4f32_t color;
} vertex_t;
