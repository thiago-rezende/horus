#pragma once

#include <horus/platform/console.h>

extern void* application_create();

int main(int argc, char** argv, char** envp) {
  platform_console_write("[ horus ] <linux> v0.1.0\n");

  void* application = application_create();
  (void)application; /* unused */

  platform_console_write("[ application ] created\n");

  return 0;
}
