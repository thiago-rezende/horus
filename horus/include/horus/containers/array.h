#pragma once

/* horus core layer */
#include <horus/core/types.h>

typedef struct __array {
  void *buffer;

  u64 count;
  u64 stride;
  u64 capacity;
} array_t;

b8 array_create(u64 capacity, u64 stride, array_t **output);
b8 array_destroy(array_t *array);

b8 array_insert(array_t *array, void *item);
b8 array_retrieve(array_t *array, u64 index, void *item);
