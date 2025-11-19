#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#define HORUS_ENTRYPOINT_DISABLE
#include <horus/horus.h>

static void test_linked_list_create_and_destroy(void **state) {
  (void)state; /* unused */

  /* default attributes */
  const u64 stride = sizeof(u64);

  /* expected values */
  const u64 count = 0;

  /* list creation */
  linked_list_t *list = linked_list_create(stride);

  /* creation result assertion */
  assert_non_null(list);

  /* list attribute assertions */
  assert_int_equal(list->count, count);
  assert_int_equal(list->stride, stride);

  /* list destruction */
  b8 result = linked_list_destroy(list);

  /* destruction result assertion */
  assert_true(result);
}

static void test_list_insert_and_remove(void **state) {
  (void)state; /* unused */

  /* default attributes */
  const u64 stride = sizeof(u64);

  /* default values */
  const u64 item_to_insert_0 = 100;
  const u64 item_to_insert_1 = 101;
  const u64 item_to_insert_2 = 102;

  /* default output variables */
  u64 item_removed = 0;

  /* expected values */
  const u64 count = 2;

  /* list creation */
  linked_list_t *list = linked_list_create(stride);

  /* list insertion */
  b8 insert_result_0 = linked_list_insert(list, 0, (void *)&item_to_insert_0);
  b8 insert_result_1 = linked_list_insert(list, 1, (void *)&item_to_insert_1);
  b8 insert_result_2 = linked_list_insert(list, 1, (void *)&item_to_insert_2);

  /* insertion result assertion */
  assert_true(insert_result_0);
  assert_true(insert_result_1);
  assert_true(insert_result_2);

  /* list retrieval */
  b8 remove_result = linked_list_remove(list, 1, (void *)&item_removed);

  /* retrieval result assertion */
  assert_true(remove_result);

  /* item comparison assertion */
  assert_int_equal(item_to_insert_2, item_removed);

  /* list attrubute assertions */
  assert_int_equal(list->count, count);
  assert_int_equal(list->stride, stride);

  /* list links assertions */
  assert_non_null(list->head);
  assert_non_null(list->tail);

  assert_null(list->head->previous);
  assert_null(list->tail->next);

  assert_ptr_equal(list->tail, list->head->next);
  assert_ptr_equal(list->head, list->tail->previous);

  assert_int_equal(*(u64 *)list->head->data, item_to_insert_0);
  assert_int_equal(*(u64 *)list->tail->data, item_to_insert_1);

  /* list destruction */
  linked_list_destroy(list);
}

static void test_linked_list_insert_and_retrieve(void **state) {
  (void)state; /* unused */

  /* default attributes */
  const u64 stride = sizeof(u64);

  /* default values */
  const u64 item_to_insert_0 = 100;
  const u64 item_to_insert_1 = 101;
  const u64 item_to_insert_2 = 102;

  /* default output variables */
  u64 item_to_retrieve = 0;

  /* expected values */
  const u64 count = 3;

  /* list creation */
  linked_list_t *list = linked_list_create(stride);

  /* list insertion */
  b8 insert_result_0 = linked_list_insert(list, 0, (void *)&item_to_insert_0);
  b8 insert_result_1 = linked_list_insert(list, 1, (void *)&item_to_insert_1);
  b8 insert_result_2 = linked_list_insert(list, 1, (void *)&item_to_insert_2);

  /* insertion result assertion */
  assert_true(insert_result_0);
  assert_true(insert_result_1);
  assert_true(insert_result_2);

  /* list retrieval */
  b8 retrieve_result = linked_list_retrieve(list, 2, (void *)&item_to_retrieve);

  /* retrieval result assertion */
  assert_true(retrieve_result);

  /* item comparison assertion */
  assert_int_equal(item_to_insert_1, item_to_retrieve);

  /* list attrubute assertions */
  assert_int_equal(list->count, count);
  assert_int_equal(list->stride, stride);

  /* list links assertions */
  assert_non_null(list->head);
  assert_non_null(list->tail);

  assert_null(list->head->previous);
  assert_null(list->tail->next);

  assert_ptr_equal(list->head->next, list->tail->previous);

  assert_int_equal(*(u64 *)list->head->data, item_to_insert_0);
  assert_int_equal(*(u64 *)list->tail->data, item_to_insert_1);

  /* list destruction */
  linked_list_destroy(list);
}

static void test_linked_list_clear(void **state) {
  (void)state; /* unused */

  /* default attributes */
  const u64 stride = sizeof(u64);

  /* default values */
  const u64 item_to_insert = 100;

  /* expected values */
  const u64 count = 6;

  /* list creation */
  linked_list_t *list = linked_list_create(stride);

  /* list insertion */
  linked_list_insert(list, 0, (void *)&item_to_insert);
  linked_list_insert(list, 0, (void *)&item_to_insert);
  linked_list_insert(list, 0, (void *)&item_to_insert);
  linked_list_insert(list, 0, (void *)&item_to_insert);
  linked_list_insert(list, 0, (void *)&item_to_insert);
  linked_list_insert(list, 0, (void *)&item_to_insert);

  /* list attribute assertions */
  assert_int_equal(list->count, count);
  assert_int_equal(list->stride, stride);

  /* list clear */
  b8 clear_result = linked_list_clear(list);

  /* clear result assertion */
  assert_true(clear_result);

  /* list attribute assertions after clear */
  assert_int_equal(list->count, 0);
  assert_int_equal(list->stride, stride);

  assert_null(list->head);
  assert_null(list->tail);

  /* list destruction */
  linked_list_destroy(list);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_linked_list_clear),
      cmocka_unit_test(test_list_insert_and_remove),
      cmocka_unit_test(test_linked_list_create_and_destroy),
      cmocka_unit_test(test_linked_list_insert_and_retrieve),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
