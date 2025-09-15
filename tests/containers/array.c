#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#define HORUS_ENTRYPOINT_DISABLE
#include <horus/horus.h>

static void test_array_create_and_destroy(void **state) {
  (void)state; /* unused */

  /* default attributes */
  const u64 count = 0;
  const u64 stride = sizeof(u64);
  const u64 capacity = 10;

  /* array object */
  array_t *array = NULL;

  /* array creation */
  b8 result = array_create(capacity, stride, &array);

  /* creation result assertion */
  assert_true(result);

  /* array object assertion */
  assert_non_null(array);

  /* array attribute assertions */
  assert_int_equal(array->count, count);
  assert_int_equal(array->stride, stride);
  assert_int_equal(array->capacity, capacity);

  /* array destruction */
  result = array_destroy(array);

  /* destruction result assertion */
  assert_true(result);
}

static void test_array_insert_and_retrieve(void **state) {
  (void)state; /* unused */

  /* default attributes */
  const u64 index = 0;
  const u64 stride = sizeof(u64);
  const u64 capacity = 10;

  /* default values */
  const u64 item_to_insert = 100;
  u64 item_to_retrieve = 0;

  /* array object */
  array_t *array = NULL;

  /* array creation */
  array_create(capacity, stride, &array);

  /* array intertion */
  b8 result = array_insert(array, (void *)&item_to_insert);

  /* insertion result assertion */
  assert_true(result);

  /* array retrieval */
  result = array_retrieve(array, index, (void *)&item_to_retrieve);

  /* retrieval result assertion */
  assert_true(result);

  /* item comparison assertion */
  assert_int_equal(item_to_insert, item_to_retrieve);

  /* array destruction */
  array_destroy(array);
}

static void test_array_insert_beyond_capacity(void **state) {
  (void)state; /* unused */

  /* default attributes */
  const u64 stride = sizeof(u64);
  const u64 capacity = 1;

  /* default values */
  const u64 item_to_insert = 100;

  /* array object */
  array_t *array = NULL;

  /* array creation */
  array_create(capacity, stride, &array);

  /* array intertion */
  b8 result = array_insert(array, (void *)&item_to_insert);

  /* insertion result assertion */
  assert_true(result);

  /* array insertion */
  result = array_insert(array, (void *)&item_to_insert);

  /* insertion result assertion */
  assert_false(result);

  /* array destruction */
  array_destroy(array);
}

static void test_array_retrieve_beyond_count(void **state) {
  (void)state; /* unused */

  /* default attributes */
  const u64 index = 1;
  const u64 stride = sizeof(u64);
  const u64 capacity = 2;

  /* default values */
  const u64 item_to_insert = 100;

  /* array object */
  array_t *array = NULL;

  /* array creation */
  array_create(capacity, stride, &array);

  /* array retrieval */
  b8 result = array_retrieve(array, index, (void *)&item_to_insert);

  /* insertion result assertion */
  assert_false(result);

  /* array destruction */
  array_destroy(array);
}

static void test_array_buffer_direct_usage(void **state) {
  (void)state; /* unused */

  /* default attributes */
  const u64 stride = sizeof(u64);
  const u64 capacity = 5;

  /* default values */
  const u64 buffer[5] = {1, 2, 3, 4, 5};

  /* array object */
  array_t *array = NULL;

  /* array creation */
  array_create(capacity, stride, &array);

  /* array insertion */
  for (size_t index = 0; index < capacity; index++) {
    b8 result = array_insert(array, (void *)&buffer[index]);

    /* insertion result assertion */
    assert_true(result);
  }

  /* array retrieval */
  for (size_t index = 0; index < capacity; index++) {
    /* retrieved object */
    u64 item_to_retrieve = ((u64 *)array->buffer)[index];

    /* retrieved object assertion */
    assert_int_equal(item_to_retrieve, buffer[index]);
  }

  /* array destruction */
  array_destroy(array);
}

static void test_array_clear(void **state) {
  (void)state; /* unused */

  /* default attributes */
  const u64 index = 0;
  const u64 stride = sizeof(u64);
  const u64 capacity = 10;

  /* default values */
  const u64 item_to_insert = 100;
  u64 item_to_retrieve = 0;

  /* array object */
  array_t *array = NULL;

  /* array creation */
  array_create(capacity, stride, &array);

  /* array intertion */
  array_insert(array, (void *)&item_to_insert);

  /* array retrieval */
  b8 result = array_retrieve(array, index, (void *)&item_to_retrieve);

  /* retrieval result assertion */
  assert_true(result);

  /* item comparison assertion */
  assert_int_equal(item_to_insert, item_to_retrieve);

  /* array clear */
  result = array_clear(array);

  /* clear result assertion */
  assert_true(result);

  /* array retrieval */
  result = array_retrieve(array, index, (void *)&item_to_retrieve);

  /* retrieval result assertion */
  assert_false(result);

  /* array destruction */
  array_destroy(array);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_array_create_and_destroy),     cmocka_unit_test(test_array_insert_and_retrieve),
      cmocka_unit_test(test_array_insert_beyond_capacity), cmocka_unit_test(test_array_retrieve_beyond_count),
      cmocka_unit_test(test_array_buffer_direct_usage),    cmocka_unit_test(test_array_clear),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
