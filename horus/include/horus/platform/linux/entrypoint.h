#pragma once

#include <horus/logger/logger.h>
#include <horus/platform/window.h>

extern void* application_create();

int main(int argc, char** argv, char** envp) {
  HINFO("<horus:%s> <platform:%s> initializing", "v0.1.0", "linux");

  void* application = application_create();

  (void)application; /* unused */

  HDEBUG("<application:%p> created", application);

  platform_window_t* window = platform_window_create("[ horus ]", 1280, 720);

  HDEBUG("<window:%p> created", window);

  while (!platform_window_should_close(window)) {
    platform_window_process_events(window);
  }

  platform_window_destroy(window);

  HDEBUG("<window:%p> destroyed", window);

  HINFO("<horus> terminating");

  return 0;
}
