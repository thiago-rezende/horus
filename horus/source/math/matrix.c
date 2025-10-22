/* horus math layer */
#include <horus/math/matrix.h>

/* horus platform layer */
#include <horus/platform/memory.h>

matrix4f32_t matrix4f32_scale(matrix4f32_t matrix, vector3f32_t vector) {
  matrix4f32_t result = {0};

  platform_memory_copy(&result, &matrix, sizeof(matrix4f32_t));

  result.column0 *= (__v4f32){vector.x, vector.x, vector.x, 1.0f};

  result.column1 *= (__v4f32){vector.y, vector.y, vector.y, 1.0f};

  result.column2 *= (__v4f32){vector.z, vector.z, vector.z, 1.0f};

  return result;
}

matrix4f32_t matrix4f32_identity(void) {
  matrix4f32_t matrix = (matrix4f32_t){0};

  matrix.column0 = (__v4f32){1, 0, 0, 0};
  matrix.column1 = (__v4f32){0, 1, 0, 0};
  matrix.column2 = (__v4f32){0, 0, 1, 0};
  matrix.column3 = (__v4f32){0, 0, 0, 1};

  return matrix;
}

matrix4f32_t matrix4f32_multiply(matrix4f32_t a, matrix4f32_t b) {
  matrix4f32_t matrix = (matrix4f32_t){0};

  for (u8 i = 0; i < 4; ++i) {
    __v4f32 b_column = b.columns[i];

    matrix.columns[i] = a.column0 * (__v4f32){b_column[0], b_column[0], b_column[0], b_column[0]} +
                        a.column1 * (__v4f32){b_column[1], b_column[1], b_column[1], b_column[1]} +
                        a.column2 * (__v4f32){b_column[2], b_column[2], b_column[2], b_column[2]} +
                        a.column3 * (__v4f32){b_column[3], b_column[3], b_column[3], b_column[3]};
  }

  return matrix;
}

matrix4f32_t matrix4f32_translate(matrix4f32_t matrix, vector3f32_t vector) {
  matrix4f32_t result = {0};

  platform_memory_copy(&result, &matrix, sizeof(matrix4f32_t));

  result.column3 += (__v4f32){vector.x, vector.y, vector.z, 0.0f};

  return result;
}
