/* horus core layer */
#include <horus/core/types.h>

/* horus platform layer */
#include <horus/platform/time.h>

/* horus platform layer [ windows ] */
#include <horus/platform/windows/windows.h>

f64 platform_absolute_time(void) {
  static b8 initialized = false;

  static LARGE_INTEGER frequency;

  if (!initialized) {
    QueryPerformanceFrequency(&frequency);
  }

  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);

  return (f64)counter.QuadPart / (f64)frequency.QuadPart;
}

b8 platform_sleep(u64 ms) {
  Sleep((DWORD)ms);

  return true;
}
