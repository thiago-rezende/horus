#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#define HORUS_ENTRYPOINT_DISABLE
#include <horus/horus.h>

static void test_quaternionf32_add(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  quaternionf32_t a = (quaternionf32_t){.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 0.0f};
  quaternionf32_t b = (quaternionf32_t){.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 0.0f};

  /* expected result */
  quaternionf32_t expected = (quaternionf32_t){.x = 2.0f, .y = 2.0f, .z = 2.0f, .w = 0.0f};

  /* function call */
  quaternionf32_t result = quaternionf32_add(a, b);

  /* general assertion */
  assert_memory_equal(&result, &expected, sizeof(quaternionf32_t));

  /* union assertions */
  assert_float_equal(result.x, expected.x, epsilon_f32);
  assert_float_equal(result.y, expected.y, epsilon_f32);
  assert_float_equal(result.z, expected.z, epsilon_f32);
  assert_float_equal(result.w, expected.w, epsilon_f32);
}

static void test_quaternionf32_identity(void **state) {
  (void)state; /* unused */

  /* expected result */
  quaternionf32_t expected = (quaternionf32_t){.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f};

  /* function call */
  quaternionf32_t result = quaternionf32_identity();

  /* general assertion */
  assert_memory_equal(&result, &expected, sizeof(quaternionf32_t));

  /* union assertions */
  assert_float_equal(result.x, expected.x, epsilon_f32);
  assert_float_equal(result.y, expected.y, epsilon_f32);
  assert_float_equal(result.z, expected.z, epsilon_f32);
  assert_float_equal(result.w, expected.w, epsilon_f32);
}

static void test_quaternionf32_multiply(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  quaternionf32_t a = (quaternionf32_t){.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 0.0f};
  quaternionf32_t b = (quaternionf32_t){.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 0.0f};

  /* expected result */
  quaternionf32_t expected = (quaternionf32_t){.x = 1.0f, .y = -2.0f, .z = 1.0f, .w = -6.0f};

  /* function call */
  quaternionf32_t result = quaternionf32_multiply(a, b);

  /* general assertion */
  assert_memory_equal(&result, &expected, sizeof(quaternionf32_t));

  /* union assertions */
  assert_float_equal(result.x, expected.x, epsilon_f32);
  assert_float_equal(result.y, expected.y, epsilon_f32);
  assert_float_equal(result.z, expected.z, epsilon_f32);
  assert_float_equal(result.w, expected.w, epsilon_f32);
}

static void test_quaternionf32_to_conjugate(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  quaternionf32_t quaternion = (quaternionf32_t){.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};

  /* expected result */
  quaternionf32_t expected = (quaternionf32_t){.x = -1.0f, .y = -1.0f, .z = -1.0f, .w = 1.0f};

  /* function call */
  quaternionf32_t result = quaternionf32_to_conjugate(quaternion);

  /* general assertion */
  assert_memory_equal(&result, &expected, sizeof(quaternionf32_t));

  /* union assertions */
  assert_float_equal(result.x, expected.x, epsilon_f32);
  assert_float_equal(result.y, expected.y, epsilon_f32);
  assert_float_equal(result.z, expected.z, epsilon_f32);
  assert_float_equal(result.w, expected.w, epsilon_f32);
}

static void test_quaternionf32_rotate_euler(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  quaternionf32_t quaternion = (quaternionf32_t){.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f};
  vector3f32_t degrees = (vector3f32_t){.x = 180.0f, .y = 0.0f, .z = 0.0f, .__padding = 0.0f};

  /* expected result */
  quaternionf32_t expected = (quaternionf32_t){.x = 1.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f};

  /* function call */
  quaternionf32_t result = quaternionf32_rotate_euler(quaternion, degrees);

  /* union assertions */
  assert_float_equal(result.x, expected.x, epsilon_f32);
  assert_float_equal(result.y, expected.y, epsilon_f32);
  assert_float_equal(result.z, expected.z, epsilon_f32);
  assert_float_equal(result.w, expected.w, epsilon_f32);
}

static void test_quaternionf32_to_matrix(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  quaternionf32_t quaternion = (quaternionf32_t){.x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f};

  /* expected result */
  matrix4f32_t expected = (matrix4f32_t){
      .column0 = {-1, 0, 0, 0},
      .column1 = {0, 1, 0, 0},
      .column2 = {0, 0, -1, 0},
      .column3 = {0, 0, 0, 1},
  };

  /* function call */
  matrix4f32_t result = quaternionf32_to_matrix(quaternion);

  /* general assertion */
  assert_memory_equal(&result, &expected, sizeof(quaternionf32_t));

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
      /* operations */
      cmocka_unit_test(test_quaternionf32_add),
      cmocka_unit_test(test_quaternionf32_identity),
      cmocka_unit_test(test_quaternionf32_multiply),
      cmocka_unit_test(test_quaternionf32_rotate_euler),
      /* conversions */
      cmocka_unit_test(test_quaternionf32_to_matrix),
      cmocka_unit_test(test_quaternionf32_to_conjugate),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
