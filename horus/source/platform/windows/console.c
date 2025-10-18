#include <stdio.h>

/* horus platform layer */
#include <horus/platform/console.h>

b8 platform_console_write(char *message) {
  int written = fprintf(stdout, "%s", message);

  fflush(stdout);

  return written < 0 ? false : true;
}

b8 platform_console_write_error(char *message) {
  int written = fprintf(stderr, "%s", message);

  fflush(stderr);

  return written < 0 ? false : true;
}
