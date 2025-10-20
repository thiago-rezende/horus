#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#define HORUS_ENTRYPOINT_DISABLE
#include <horus/horus.h>

static void test_vector3f32_add(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  vector3f32_t a = (vector3f32_t){.x = 1.0f, .y = 1.0f, .z = 1.0f, .__padding = 0};
  vector3f32_t b = (vector3f32_t){.x = 1.0f, .y = 1.0f, .z = 1.0f, .__padding = 0};

  /* expected result */
  vector3f32_t expected = (vector3f32_t){.x = 2.0f, .y = 2.0f, .z = 2.0f, .__padding = 0};

  /* function call */
  vector3f32_t result = vector3f32_add(a, b);

  /* general assertion */
  assert_memory_equal(&result, &expected, sizeof(vector3f32_t));

  /* union assertions */
  assert_float_equal(result.x, expected.r, epsilon_f32);
  assert_float_equal(result.y, expected.g, epsilon_f32);
  assert_float_equal(result.z, expected.b, epsilon_f32);
}

static void test_vector4f32_add(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  vector4f32_t a = (vector4f32_t){.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 0};
  vector4f32_t b = (vector4f32_t){.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 0};

  /* expected result */
  vector4f32_t expected = (vector4f32_t){.x = 2.0f, .y = 2.0f, .z = 2.0f, .w = 0};

  /* function call */
  vector4f32_t result = vector4f32_add(a, b);

  /* general assertion */
  assert_memory_equal(&result, &expected, sizeof(vector4f32_t));

  /* union assertions */
  assert_float_equal(result.x, expected.r, epsilon_f32);
  assert_float_equal(result.y, expected.g, epsilon_f32);
  assert_float_equal(result.z, expected.b, epsilon_f32);
  assert_float_equal(result.w, expected.a, epsilon_f32);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_vector3f32_add),
      cmocka_unit_test(test_vector4f32_add),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
