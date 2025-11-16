#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#define HORUS_ENTRYPOINT_DISABLE
#include <horus/horus.h>

static void test_hash_fnv1a64(void **state) {
  (void)state; /* unused */

  /* default arguments */
  const char *string = "fnv1a64";
  const u64 string_lenght = 7;

  /* expected results */
  u64 expected_hash = 0XCD8A4416206A3AB9ULL;

  /* function call */
  u64 result_hash = hash_fnv1a64((const void *)string, string_lenght);

  /* assertions */
  assert_memory_equal(&result_hash, &expected_hash, sizeof(u64));
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_hash_fnv1a64),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
