#include <stdlib.h>
#include <string.h>

/* horus platform layer */
#include <horus/platform/memory.h>

void *platform_memory_allocate(u64 size) {
  return malloc(size);
}

void *platform_memory_reallocate(void *data, u64 size) {
  return realloc(data, size);
}

void platform_memory_deallocate(void *data) {
  free(data);
}

void platform_memory_clear(void *data, u64 size) {
  memset(data, 0, size);
}
