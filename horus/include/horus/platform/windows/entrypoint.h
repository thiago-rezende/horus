#pragma once

/* horus base layer */
#include <horus/definitions.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/time.h>
#include <horus/platform/window.h>

/* horus platform layer [ windows ] */
#include <horus/platform/windows/windows.h>

/* horus application layer */
#include <horus/application/application.h>

extern application_t *application_create(void);
extern b8 application_destroy(application_t *application);

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
  logger_info("<%s:v%s> <platform:%s> initializing", horus_project_name(), horus_project_version(), horus_platform());

  application_t *application = application_create();

  if (!application) {
    logger_critical("<application:%p> creation failed", application);

    return 1;
  }

  __application_set_global_instance(application);

  logger_info("<application:%p> <name:%s> created", application, application->name);

  configuration_t *configuration = &application->configuration;
  resolution_t *resolution = &configuration->resolution;

  platform_window_size_t window_size = (platform_window_size_t){
      .width = resolution->width,
      .height = resolution->height,
  };

  platform_window_t *window = platform_window_create(application->name, window_size, configuration->fullscreen);

  __platform_window_set_global_instance(window);

  logger_info("<window:%p> <title:%s> <width:%u> <height:%u> <fullscreen:%u> created", window, application->name,
              resolution->width, resolution->height, configuration->fullscreen);

  if (application->on_event) {
    if (!platform_window_set_event_callback(window, application->on_event)) {
      logger_error("<application:%p> <on_event> failed", application);
    }
  }

  f64 timestep = 0;
  f64 current_absolute_time = platform_absolute_time();
  f64 previous_absolute_time = platform_absolute_time();

  while (!platform_window_should_close(window)) {
    previous_absolute_time = current_absolute_time;
    current_absolute_time = platform_absolute_time();

    timestep = current_absolute_time - previous_absolute_time;

    if (application->on_update) {
      if (!application->on_update(timestep)) {
        logger_error("<application:%p> <on_update> failed", application);
      }
    }

    if (application->on_render) {
      if (!application->on_render()) {
        logger_error("<application:%p> <on_render> failed", application);
      }
    }

    platform_window_process_events(window);
  }

  platform_window_destroy(window);

  logger_info("<window:%p> destroyed", window);

  if (!application_destroy(application)) {
    logger_critical("<application:%p> destruction failed", application);

    return 1;
  }

  logger_info("<application:%p> destroyed", application);

  logger_info("<%s:v%s> <platform:%s> terminating", horus_project_name(), horus_project_version(), horus_platform());

  return 0;
}
