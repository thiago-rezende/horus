#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus math layer */
#include <horus/math/vector.h>

typedef union __matrix4f32 {
  f32 buffer[16];

  f32 matrix[4][4];

  __v4f32 columns[4];

  struct {
    __v4f32 column0;
    __v4f32 column1;
    __v4f32 column2;
    __v4f32 column3;
  };

  /* clang-format off */
  struct {
    union { float a00, x0; };
    union { float a10, y0; };
    union { float a20, z0; };
    union { float a30, w0; };

    union { float a01, x1; };
    union { float a11, y1; };
    union { float a21, z1; };
    union { float a31, w1; };

    union { float a02, x2; };
    union { float a12, y2; };
    union { float a22, z2; };
    union { float a32, w2; };

    union { float a03, x3; };
    union { float a13, y3; };
    union { float a23, z3; };
    union { float a33, w3; };
  };
  /* clang-format on */
} matrix4f32_t;

/* TODO: inline all possible matrix functions */
matrix4f32_t matrix4f32_identity(void);

/* TODO: inline all possible matrix functions */
matrix4f32_t matrix4f32_multiply(matrix4f32_t a, matrix4f32_t b);

/* TODO: inline all possible matrix functions */
matrix4f32_t matrix4f32_translate(matrix4f32_t matrix, vector3f32_t vector);
