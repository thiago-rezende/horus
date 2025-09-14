/* horus containers layer */
#include <horus/containers/array.h>

/* horus platform layer */
#include <horus/platform/memory.h>

b8 array_create(u64 capacity, u64 stride, array_t **output) {
  if (output == NULL) {
    return false;
  }

  array_t *array = (array_t *)platform_memory_allocate(sizeof(array_t));

  if (array == NULL) {
    return false;
  }

  *array = (array_t){
      .buffer = platform_memory_allocate(capacity * stride),
      .count = 0,
      .stride = stride,
      .capacity = capacity,
  };

  if (array->buffer == NULL) {
    return false;
  }

  if (!platform_memory_clear(array->buffer, capacity * stride)) {
    return false;
  }

  *output = array;

  return true;
}

b8 array_destroy(array_t *array) {
  if (array != NULL) {
    platform_memory_deallocate(array->buffer);
  }

  platform_memory_deallocate(array);

  return true;
}

b8 array_retrieve(array_t *array, u64 index, void *item) {
  if (array == NULL || item == NULL) {
    return false;
  }

  if (index >= array->count) {
    return false;
  }

  void *offset = array->buffer + (index * array->stride);

  if (!platform_memory_copy(item, offset, array->stride)) {
    return false;
  }

  return true;
}

b8 array_insert(array_t *array, void *item) {
  if (array == NULL || item == NULL) {
    return false;
  }

  if (array->count >= array->capacity) {
    return false;
  }

  void *offset = array->buffer + (array->count * array->stride);

  if (!platform_memory_copy(offset, item, array->stride)) {
    return false;
  }

  array->count++;

  return true;
}
