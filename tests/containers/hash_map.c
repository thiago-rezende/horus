#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#define HORUS_ENTRYPOINT_DISABLE
#include <horus/horus.h>

static void test_hash_map_create_and_destroy(void **state) {
  (void)state; /* unused */

  /* default attributes */
  const u64 size = 10;
  const u64 stride = sizeof(u64);

  /* expected values */
  const u64 count = 0;

  /* map creation */
  hash_map_t *map = hash_map_create(size, stride);

  /* creation result assertion */
  assert_non_null(map);

  /* map attribute assertions */
  assert_int_equal(map->size, size);
  assert_int_equal(map->count, count);
  assert_int_equal(map->stride, stride);

  /* map destruction */
  b8 result = hash_map_destroy(map);

  /* destruction result assertion */
  assert_true(result);
}

static void test_hash_map_insert_and_remove(void **state) {
  (void)state; /* unused */

  /* default attributes */
  const u64 size = 10;
  const u64 stride = sizeof(u64);

  /* default values */
  u64 count = 0;
  u64 value_to_insert = 100;

  const char *key_to_insert = "string";

  /* output values */
  u64 value_to_remove = 0;

  /* map creation */
  hash_map_t *map = hash_map_create(size, stride);

  /* map attribute assertions */
  assert_int_equal(map->size, size);
  assert_int_equal(map->count, count);
  assert_int_equal(map->stride, stride);

  /* map inrestion */
  b8 insert_retult = hash_map_insert(map, key_to_insert, &value_to_insert);

  /* intert assertions */
  assert_true(insert_retult);

  /* map attribute assertions */
  assert_int_equal(map->size, size);
  assert_int_equal(map->count, count + 1);
  assert_int_equal(map->stride, stride);

  /* map removal */
  b8 removal_result = hash_map_remove(map, key_to_insert, &value_to_remove);

  /* intert assertions */
  assert_true(removal_result);

  assert_int_equal(value_to_insert, value_to_remove);

  /* map attribute assertions */
  assert_int_equal(map->size, size);
  assert_int_equal(map->count, count);
  assert_int_equal(map->stride, stride);

  /* map destruction */
  hash_map_destroy(map);
}

static void test_hash_map_insert_and_retrieve(void **state) {
  (void)state; /* unused */

  /* default attributes */
  const u64 size = 10;
  const u64 stride = sizeof(u64);

  /* default values */
  u64 count = 0;
  u64 value_to_insert = 100;

  const char *key_to_insert = "string";

  /* output values */
  u64 value_to_retrieve = 0;

  /* map creation */
  hash_map_t *map = hash_map_create(size, stride);

  /* map attribute assertions */
  assert_int_equal(map->size, size);
  assert_int_equal(map->count, count);
  assert_int_equal(map->stride, stride);

  /* map inrestion */
  b8 insert_retult = hash_map_insert(map, key_to_insert, &value_to_insert);

  /* intert assertions */
  assert_true(insert_retult);

  /* map attribute assertions */
  assert_int_equal(map->size, size);
  assert_int_equal(map->count, count + 1);
  assert_int_equal(map->stride, stride);

  /* map retrieval */
  b8 retrieval_result = hash_map_retrieve(map, key_to_insert, &value_to_retrieve);

  /* intert assertions */
  assert_true(retrieval_result);

  assert_int_equal(value_to_insert, value_to_retrieve);

  /* map attribute assertions */
  assert_int_equal(map->size, size);
  assert_int_equal(map->count, count + 1);
  assert_int_equal(map->stride, stride);

  /* map destruction */
  hash_map_destroy(map);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_hash_map_insert_and_remove),
      cmocka_unit_test(test_hash_map_create_and_destroy),
      cmocka_unit_test(test_hash_map_insert_and_retrieve),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
