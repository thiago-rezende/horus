#pragma once

#include <horus/definitions.h>
#include <horus/logger/logger.h>
#include <horus/platform/window.h>
#include <horus/application/application.h>

extern application_t *application_create();
extern void application_destroy(application_t *application);

int main(int argc, char **argv, char **envp) {
  HINFO("<%s:v%s> <platform:%s> initializing", horus_project_name(), horus_project_version(), horus_platform());

  application_t *application = application_create();

  HDEBUG("<application:%p> <name:%s> created", application, application->name);

  configuration_t *configuration = &application->configuration;
  resolution_t *resolution = &configuration->resolution;

  platform_window_t *window =
      platform_window_create(application->name, resolution->width, resolution->height, configuration->windowed);

  HDEBUG("<window:%p> created", window);

  while (!platform_window_should_close(window)) {
    if (application->on_update) {
      if (!application->on_update((u64)0)) {
        HERROR("<application:%p> <on_update> failed", application);
      }
    }

    if (application->on_render) {
      if (!application->on_render()) {
        HERROR("<application:%p> <on_render> failed", application);
      }
    }

    platform_window_process_events(window);
  }

  platform_window_destroy(window);

  HDEBUG("<window:%p> destroyed", window);

  application_destroy(application);

  HDEBUG("<application:%p> <name:%s> destroyed", application, application->name);

  HINFO("<%s:v%s> <platform:%s> terminating", horus_project_name(), horus_project_version(), horus_platform());

  return 0;
}
