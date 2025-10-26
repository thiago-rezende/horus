#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* compiler vectorization types [ vector_size should be a power of 2 ] */
typedef u8 __v4u8 __attribute__((vector_size(sizeof(u8) * 4)));

typedef f32 __v3f32 __attribute__((vector_size(sizeof(f32) * 4)));
typedef f32 __v4f32 __attribute__((vector_size(sizeof(f32) * 4)));

typedef union __vector4u8 {
  u8 buffer[4];

  __v4u8 vector;

  /* clang-format off */
  struct {
    union { u8 x, r; };
    union { u8 y, g; };
    union { u8 z, b; };
    union { u8 w, a; };
  };
  /* clang-format on */
} vector4u8_t;

typedef union __vector3f32 {
  f32 buffer[4];

  __v3f32 vector;

  /* clang-format off */
  struct {
    union { f32 x, r, u; };

    union { f32 y, g, v; };

    union { f32 z, b, w; };

    f32 __padding;
  };
  /* clang-format on */
} vector3f32_t;

typedef union __vector4f32 {
  f32 buffer[4];

  __v4f32 vector;

  /* clang-format off */
  struct {
    union { f32 x, r; };
    union { f32 y, g; };
    union { f32 z, b; };
    union { f32 w, a; };
  };
  /* clang-format on */
} vector4f32_t;

/* TODO: inline all possible vector functions */
vector4u8_t vector4u8_add(vector4u8_t a, vector4u8_t b);

/* TODO: inline all possible vector functions */
vector3f32_t vector3f32_add(vector3f32_t a, vector3f32_t b);

/* TODO: inline all possible vector functions */
f32 vector3f32_dot(vector3f32_t a, vector3f32_t b);

/* TODO: inline all possible vector functions */
vector3f32_t vector3f32_cross(vector3f32_t a, vector3f32_t b);

/* TODO: inline all possible vector functions */
vector3f32_t vector3f32_scalar(vector3f32_t vector, f32 scalar);

/* TODO: inline all possible vector functions */
vector3f32_t vector3f32_hadamard(vector3f32_t a, vector3f32_t b);

/* TODO: inline all possible vector functions */
vector4f32_t vector4f32_add(vector4f32_t a, vector4f32_t b);
