#pragma once

#include <horus/logger/logger.h>

extern void* application_create();

int main(int argc, char** argv, char** envp) {
  HINFO("<horus:%s> <platform:%s> initializing\n", "v0.1.0", "linux");

  void* application = application_create();

  (void)application; /* unused */

  HDEBUG("<application:%p> created\n", application);

  HINFO("<horus> terminating\n");

  return 0;
}
