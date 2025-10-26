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

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_quaternionf32_add),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
