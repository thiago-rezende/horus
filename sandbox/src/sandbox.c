#include <horus/horus.h>

#include <sandbox/sandbox.h>

void* application_create() {
  void* data = platform_memory_allocate(sizeof(u8) * 256);
  HDEBUG("<memory:%p> allocated 256 bytes\n", data);

  data = platform_memory_reallocate(data, sizeof(u8) * 512);
  HDEBUG("<memory:%p> reallocated to 512 bytes\n", data);

  platform_memory_deallocate(data);
  HDEBUG("<memory:%p> deallocated 512 bytes\n", data);

  return (void*)0;
}
