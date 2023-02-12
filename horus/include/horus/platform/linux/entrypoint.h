#pragma once

#include <horus/logger/logger.h>
#include <horus/platform/window.h>
#include <horus/application/application.h>

extern application_t *application_create();

int main(int argc, char **argv, char **envp) {
  HINFO("<horus:%s> <platform:%s> initializing", "v0.1.0", "linux");

  application_t *application = application_create();

  HDEBUG("<application:%p> <name:%s> created", application, application->name);

  platform_window_t *window = platform_window_create(application->name, 1280, 720);

  HDEBUG("<window:%p> created", window);

  while (!platform_window_should_close(window)) {
    platform_window_process_events(window);
  }

  platform_window_destroy(window);

  HDEBUG("<window:%p> destroyed", window);

  platform_memory_deallocate(application);

  HDEBUG("<application:%p> <name:%s> destroyed", application, application->name);

  HINFO("<horus> terminating");

  return 0;
}
