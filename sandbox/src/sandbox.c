#include <horus/horus.h>

#include <sandbox/sandbox.h>

void *application_create() {
  HCRITICAL("that is a critical logger message");
  HERROR("that is an error logger message");
  HWARNING("that is a warning logger message");
  HINFO("that is an info logger message");
  HDEBUG("that is a debug logger message");
  HTRACE("that is a trace logger message");

  void *data = platform_memory_allocate(sizeof(u8) * 256);
  HDEBUG("<memory:%p> allocated 256 bytes", data);

  data = platform_memory_reallocate(data, sizeof(u8) * 512);
  HDEBUG("<memory:%p> reallocated to 512 bytes", data);

  platform_memory_deallocate(data);
  HDEBUG("<memory:%p> deallocated 512 bytes", data);

  return (void *)0;
}
