#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>

#define HORUS_ENTRYPOINT_DISABLE
#include <horus/horus.h>

static void test_matrix4f32_identity(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  matrix4f32_t expected = (matrix4f32_t){
      .column0 = {1, 0, 0, 0},
      .column1 = {0, 1, 0, 0},
      .column2 = {0, 0, 1, 0},
      .column3 = {0, 0, 0, 1},
  };

  /* function call */
  matrix4f32_t result = matrix4f32_identity();

  /* general assertion */
  assert_memory_equal(&result, &expected, sizeof(matrix4f32_t));

  /* union assertions */
  assert_float_equal(result.x0, expected.x0, epsilon_f32);
  assert_float_equal(result.y0, expected.y0, epsilon_f32);
  assert_float_equal(result.z0, expected.z0, epsilon_f32);
  assert_float_equal(result.w0, expected.w0, epsilon_f32);

  assert_float_equal(result.x1, expected.x1, epsilon_f32);
  assert_float_equal(result.y1, expected.y1, epsilon_f32);
  assert_float_equal(result.z1, expected.z1, epsilon_f32);
  assert_float_equal(result.w1, expected.w1, epsilon_f32);

  assert_float_equal(result.x2, expected.x2, epsilon_f32);
  assert_float_equal(result.y2, expected.y2, epsilon_f32);
  assert_float_equal(result.z2, expected.z2, epsilon_f32);
  assert_float_equal(result.w2, expected.w2, epsilon_f32);

  assert_float_equal(result.x3, expected.x3, epsilon_f32);
  assert_float_equal(result.y3, expected.y3, epsilon_f32);
  assert_float_equal(result.z3, expected.z3, epsilon_f32);
  assert_float_equal(result.w3, expected.w3, epsilon_f32);
}

static void test_matrix4f32_multiply(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  matrix4f32_t a = (matrix4f32_t){
      .column0 = {1, 0, 0, 1},
      .column1 = {0, 2, 2, 0},
      .column2 = {0, 3, 3, 0},
      .column3 = {4, 0, 0, 4},
  };

  matrix4f32_t b = (matrix4f32_t){
      .column0 = {1, 0, 0, 1},
      .column1 = {0, 2, 2, 0},
      .column2 = {0, 3, 3, 0},
      .column3 = {4, 0, 0, 4},
  };

  /* expected result */
  matrix4f32_t expected = (matrix4f32_t){
      .column0 = {5, 0, 0, 5},
      .column1 = {0, 10, 10, 0},
      .column2 = {0, 15, 15, 0},
      .column3 = {20, 0, 0, 20},
  };

  /* function call */
  matrix4f32_t result = matrix4f32_multiply(a, b);

  /* general assertion */
  assert_memory_equal(&result, &expected, sizeof(matrix4f32_t));

  /* union assertions */
  assert_float_equal(result.x0, expected.x0, epsilon_f32);
  assert_float_equal(result.y0, expected.y0, epsilon_f32);
  assert_float_equal(result.z0, expected.z0, epsilon_f32);
  assert_float_equal(result.w0, expected.w0, epsilon_f32);

  assert_float_equal(result.x1, expected.x1, epsilon_f32);
  assert_float_equal(result.y1, expected.y1, epsilon_f32);
  assert_float_equal(result.z1, expected.z1, epsilon_f32);
  assert_float_equal(result.w1, expected.w1, epsilon_f32);

  assert_float_equal(result.x2, expected.x2, epsilon_f32);
  assert_float_equal(result.y2, expected.y2, epsilon_f32);
  assert_float_equal(result.z2, expected.z2, epsilon_f32);
  assert_float_equal(result.w2, expected.w2, epsilon_f32);

  assert_float_equal(result.x3, expected.x3, epsilon_f32);
  assert_float_equal(result.y3, expected.y3, epsilon_f32);
  assert_float_equal(result.z3, expected.z3, epsilon_f32);
  assert_float_equal(result.w3, expected.w3, epsilon_f32);
}

static void test_matrix4f32_translate(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  vector3f32_t vector = (vector3f32_t){{2, 4, 8}};
  matrix4f32_t identity = (matrix4f32_t){
      .column0 = {1, 0, 0, 0},
      .column1 = {0, 1, 0, 0},
      .column2 = {0, 0, 1, 0},
      .column3 = {0, 0, 0, 1},
  };

  /* expected result */
  matrix4f32_t expected = (matrix4f32_t){
      .column0 = {1, 0, 0, 0},
      .column1 = {0, 1, 0, 0},
      .column2 = {0, 0, 1, 0},
      .column3 = {2, 4, 8, 1},
  };

  /* function call */
  matrix4f32_t result = matrix4f32_translate(identity, vector);

  /* general assertion */
  assert_memory_equal(&result, &expected, sizeof(matrix4f32_t));

  /* union assertions */
  assert_float_equal(result.x0, expected.x0, epsilon_f32);
  assert_float_equal(result.y0, expected.y0, epsilon_f32);
  assert_float_equal(result.z0, expected.z0, epsilon_f32);
  assert_float_equal(result.w0, expected.w0, epsilon_f32);

  assert_float_equal(result.x1, expected.x1, epsilon_f32);
  assert_float_equal(result.y1, expected.y1, epsilon_f32);
  assert_float_equal(result.z1, expected.z1, epsilon_f32);
  assert_float_equal(result.w1, expected.w1, epsilon_f32);

  assert_float_equal(result.x2, expected.x2, epsilon_f32);
  assert_float_equal(result.y2, expected.y2, epsilon_f32);
  assert_float_equal(result.z2, expected.z2, epsilon_f32);
  assert_float_equal(result.w2, expected.w2, epsilon_f32);

  assert_float_equal(result.x3, expected.x3, epsilon_f32);
  assert_float_equal(result.y3, expected.y3, epsilon_f32);
  assert_float_equal(result.z3, expected.z3, epsilon_f32);
  assert_float_equal(result.w3, expected.w3, epsilon_f32);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_matrix4f32_identity),
      cmocka_unit_test(test_matrix4f32_multiply),
      cmocka_unit_test(test_matrix4f32_translate),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
