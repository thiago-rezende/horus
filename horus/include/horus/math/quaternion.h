#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus math layer */
#include <horus/math/matrix.h>
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

/* TODO: inline all possible quaternion functions */
quaternionf32_t quaternionf32_multiply(quaternionf32_t a, quaternionf32_t b);

/* TODO: inline all possible quaternion functions */
quaternionf32_t quaternionf32_rotate_euler(quaternionf32_t quaternion, vector3f32_t degrees);

/* TODO: inline all possible quaternion functions */
matrix4f32_t quaternionf32_to_matrix(quaternionf32_t quaternion);
