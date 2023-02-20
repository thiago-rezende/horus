#define _POSIX_C_SOURCE 199309L
#include <time.h>

#include <horus/platform/time.h>

f64 platform_absolute_time() {
  struct timespec current;

  clock_gettime(CLOCK_MONOTONIC_RAW, &current);

  return current.tv_sec + current.tv_nsec * 0.000000001;
}

void platform_sleep(u64 ms) {
  struct timespec ts;

  ts.tv_sec = ms / 1000;

  ts.tv_nsec = (ms % 1000) * 1000 * 1000;

  nanosleep(&ts, 0);
}
