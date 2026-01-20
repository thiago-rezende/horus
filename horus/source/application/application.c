/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/time.h>

/* horus input layer */
#include <horus/input/mouse.h>
#include <horus/input/keyboard.h>

/* horus application layer */
#include <horus/application/application.h>

application_t *__application = NULL;

b8 application_run(application_t *application) {
  f64 timestep = 0;
  f64 current_absolute_time = platform_absolute_time();
  f64 previous_absolute_time = platform_absolute_time();

  input_mouse_clear_state();
  input_keyboard_clear_state();

  while (application->running) {
    platform_window_process_events(application->window);

    /* TODO: improve the 'running' state handling to allow headless applications */
    if (platform_window_should_close(application->window)) {
      application->running = false;

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
    if (renderer_record_commands(application->renderer)) {
      if (application->on_render) {
        if (!application->on_render(application->renderer)) {
          logger_error_format("<application:%p> <on_render> failed", (void *)application);
        }
      }

      renderer_submit_commands(application->renderer);
    }
  }

  return true;
}

b8 application_setup(application_t *application) {
  configuration_t configuration = application->configuration;

  application->running = true;

  platform_window_size_t window_size = (platform_window_size_t){
      .width = configuration.resolution.width,
      .height = configuration.resolution.height,
  };

  application->window = platform_window_create(application->name, window_size, application->configuration.fullscreen);

  if (!application->window) {
    logger_error_format("<application:%p> <window:%p> failed to create window", (void *)application,
                        (void *)application->window);

    return false;
  }

  logger_info_format("<window:%p> <title:%s> <width:%u> <height:%u> <fullscreen:%u> created",
                     (void *)application->window, application->name, window_size.width, window_size.height,
                     application->configuration.fullscreen);

  renderer_context_create_info_t renderer_context_create_info = (renderer_context_create_info_t){
      .name = application->name,
      .version = application->version,
  };

  application->renderer = renderer_create(renderer_context_create_info, application->window);

  if (!application->renderer) {
    logger_error_format("<application:%p> <renderer:%p> failed to create renderer", (void *)application,
                        (void *)application->renderer);
    platform_window_destroy(application->window);

    return false;
  }

  logger_info_format("<renderer:%p> <implementation:%s> created", (void *)application->renderer,
                     renderer_implementation_string(application->renderer->implementation));

  if (application->on_event) {
    if (!platform_window_set_event_callback(application->window, application->on_event)) {
      logger_error_format("<application:%p> <on_event> failed", (void *)application);

      renderer_destroy(application->renderer);
      platform_window_destroy(application->window);

      return false;
    }
  }

  if (application->on_create) {
    if (!application->on_create(application, application->window, application->renderer)) {
      logger_error_format("<application:%p> <on_create> failed", (void *)application);

      renderer_destroy(application->renderer);
      platform_window_destroy(application->window);

      return false;
    }
  }

  __application_set_global_instance(application);
  __platform_window_set_global_instance(application->window);

  return true;
}

b8 application_teardown(application_t *application) {
  if (application->on_destroy) {
    if (!application->on_destroy(application, application->window, application->renderer)) {
      logger_error_format("<application:%p> <on_destroy> failed", (void *)application);
    }
  }

  renderer_destroy(application->renderer);

  logger_info_format("<application:%p> <renderer:%p> destroyed", (void *)application, (void *)application->renderer);

  platform_window_destroy(application->window);

  logger_info_format("<application:%p> <window:%p> destroyed", (void *)application, (void *)application->window);

  return true;
}

application_t *application_get_global_instance(void) {
  return __application;
}

b8 __application_set_global_instance(application_t *application) {
  if (!__application && application) {
    __application = application;

    return true;
  }

  return false;
}
