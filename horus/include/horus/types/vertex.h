#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus math layer */
#include <horus/math/vector.h>

typedef struct __vertex {
  vector3f32_t position;

  vector4u8_t color;

  vector3f32_t normal;
} vertex_t;
