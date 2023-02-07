#include <stdio.h>

#include <horus/platform/console.h>

b8 platform_console_write(char* message) {
  u32 written = fprintf(stdout, "%s", message);

  return written < 0 ? false : true;
}

b8 platform_console_error(char* message) {
  u32 written = fprintf(stderr, "%s", message);

  return written < 0 ? false : true;
}
