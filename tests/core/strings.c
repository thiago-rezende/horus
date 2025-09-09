#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#define HORUS_ENTRYPOINT_DISABLE
#include <horus/horus.h>

static void test_string_length(void **state) {
  (void)state; /* unused */

  const char *string = "horus";
  u64 length = 5;

  u64 result = string_length((char *)string);

  assert_int_equal(result, length);
}

static void test_string_length_secure(void **state) {
  (void)state; /* unused */

  const char *string = "horus_secure";
  u64 length = 12;
  u64 limit = 5;

  u64 result = string_length_secure((char *)string, length);
  u64 result_limited = string_length_secure((char *)string, limit);

  assert_int_equal(result, length);
  assert_int_equal(result_limited, limit);
}

static void test_string_compare(void **state) {
  (void)state; /* unused */

  const char *string_a = "horus";
  const char *string_b = "horus";
  const char *string_c = "sandbox";

  b8 result_success = string_compare((char *)string_a, (char *)string_b);
  b8 result_failure = string_compare((char *)string_a, (char *)string_c);

  assert_int_equal(result_success, true);
  assert_int_equal(result_failure, false);
}

static void test_string_compare_secure(void **state) {
  (void)state; /* unused */

  const char *string_a = "horus";
  const char *string_b = "horus_secure";
  const char *string_c = "sandbox";

  u64 limit = 5;

  b8 result_success = string_compare_secure((char *)string_a, (char *)string_b, limit);
  b8 result_failure = string_compare_secure((char *)string_a, (char *)string_c, limit);

  assert_int_equal(result_success, true);
  assert_int_equal(result_failure, false);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_string_length),
      cmocka_unit_test(test_string_length_secure),
      cmocka_unit_test(test_string_compare),
      cmocka_unit_test(test_string_compare_secure),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
