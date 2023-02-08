#include <horus/horus.h>

#include <sandbox/sandbox.h>

void* application_create() {
  HCRITICAL("that is a critical logger message\n");
  HERROR("that is an error logger message\n");
  HWARNING("that is a warning logger message\n");
  HINFO("that is an info logger message\n");
  HDEBUG("that is a debug logger message\n");
  HTRACE("that is a trace logger message\n");

  void* data = platform_memory_allocate(sizeof(u8) * 256);
  HDEBUG("<memory:%p> allocated 256 bytes\n", data);

  data = platform_memory_reallocate(data, sizeof(u8) * 512);
  HDEBUG("<memory:%p> reallocated to 512 bytes\n", data);

  platform_memory_deallocate(data);
  HDEBUG("<memory:%p> deallocated 512 bytes\n", data);

  return (void*)0;
}
