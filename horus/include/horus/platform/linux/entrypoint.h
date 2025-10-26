#pragma once

/* horus base layer */
#include <horus/definitions.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/time.h>
#include <horus/platform/window.h>

/* horus application layer */
#include <horus/application/application.h>

/* horus renderer layer */
#include <horus/renderer/pipeline.h>
#include <horus/renderer/renderer.h>

extern application_t *application_create(void);
extern b8 application_destroy(application_t *application);

int main(int argc, char **argv, char **envp) {
  (void)argc; /* unused */
  (void)argv; /* unused */
  (void)envp; /* unused */

  logger_info_format("<%s:v%s> <platform:%s> initializing", horus_project_name(), horus_project_version(),
                     horus_platform());

  application_t *application = application_create();

  if (!application) {
    logger_critical_format("<application:%p> creation failed", (void *)application);

    return 1;
  }

  __application_set_global_instance(application);

  logger_info_format("<application:%p> <name:%s> created", (void *)application, application->name);

  configuration_t *configuration = &application->configuration;
  resolution_t *resolution = &configuration->resolution;

  platform_window_size_t window_size = (platform_window_size_t){
      .width = resolution->width,
      .height = resolution->height,
  };

  platform_window_t *window = platform_window_create(application->name, window_size, configuration->fullscreen);

  __platform_window_set_global_instance(window);

  logger_info_format("<window:%p> <title:%s> <width:%u> <height:%u> <fullscreen:%u> created", (void *)window,
                     application->name, resolution->width, resolution->height, configuration->fullscreen);

  renderer_create_info_t renderer_create_info = (renderer_create_info_t){
      .name = application->name,
      .version = application->version,
  };

  renderer_t *renderer = renderer_create(renderer_create_info, window);

  logger_info_format("<renderer:%p> <implementation:%s> created", (void *)renderer,
                     renderer_implementation_string(renderer));

  if (application->on_event) {
    if (!platform_window_set_event_callback(window, application->on_event)) {
      logger_error_format("<application:%p> <on_event> failed", (void *)application);
    }
  }

  if (application->on_create) {
    if (!application->on_create(application, window, renderer)) {
      logger_error_format("<application:%p> <on_create> failed", (void *)application);
    }
  }

  f64 timestep = 0;
  f64 current_absolute_time = platform_absolute_time();
  f64 previous_absolute_time = platform_absolute_time();

  while (!platform_window_should_close(window)) {
    platform_window_process_events(window);

    /* check window state before proceeding to prevent problems renderer problems */
    if (platform_window_should_close(window)) {
      break;
    }

    previous_absolute_time = current_absolute_time;
    current_absolute_time = platform_absolute_time();

    timestep = current_absolute_time - previous_absolute_time;

    if (application->on_update) {
      if (!application->on_update(timestep)) {
        logger_error_format("<application:%p> <on_update> failed", (void *)application);
      }
    }

    /* TODO: improve for multiple windows support */
    if (renderer_record_commands(renderer)) {
      if (application->on_render) {
        if (!application->on_render(renderer)) {
          logger_error_format("<application:%p> <on_render> failed", (void *)application);
        }
      }

      renderer_submit_commands(renderer);
    }

    f64 target_frame_time = (1.0 / (f64)configuration->framerate);
    f64 current_elapsed_time = current_absolute_time - platform_absolute_time();
    f64 remaining_frame_time = target_frame_time - current_elapsed_time;

    if (configuration->framerate && remaining_frame_time > 0) {
      platform_sleep((u64)(remaining_frame_time * 1000) - 1);
    }
  }

  if (application->on_destroy) {
    if (!application->on_destroy(application, window, renderer)) {
      logger_error_format("<application:%p> <on_destroy> failed", (void *)application);
    }
  }

  renderer_destroy(renderer);

  logger_info_format("<renderer:%p> destroyed", (void *)renderer);

  platform_window_destroy(window);

  logger_info_format("<window:%p> destroyed", (void *)window);

  if (!application_destroy(application)) {
    logger_critical_format("<application:%p> destruction failed", (void *)application);

    return 1;
  }

  logger_info_format("<application:%p> destroyed", (void *)application);

  logger_info_format("<%s:v%s> <platform:%s> terminating", horus_project_name(), horus_project_version(),
                     horus_platform());

  return 0;
}
