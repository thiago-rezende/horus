#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus math layer */
#include <horus/math/vector.h>

typedef union __quaternionf32 {
  f32 buffer[4];

  __v4f32 vector;

  /* clang-format off */
  struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
  };
  /* clang-format on */
} quaternionf32_t;

/* TODO: inline all possible quaternion functions */
quaternionf32_t quaternionf32_add(quaternionf32_t a, quaternionf32_t b);
