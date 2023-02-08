#include <horus/horus.h>

#include <sandbox/sandbox.h>

void* application_create() {
  void* data = platform_memory_allocate(sizeof(u8) * 256);

  platform_memory_reallocate(data, sizeof(u8) * 512);

  platform_memory_deallocate(data);

  return (void*)0;
}
