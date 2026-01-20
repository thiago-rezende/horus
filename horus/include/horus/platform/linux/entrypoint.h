#pragma once

/* horus base layer */
#include <horus/definitions.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/time.h>
#include <horus/platform/window.h>

/* horus input layer */
#include <horus/input/mouse.h>
#include <horus/input/keyboard.h>

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

  logger_info_format("<application:%p> <name:%s> created", (void *)application, application->name);

  if (!application_setup(application)) {
    logger_critical_format("<application:%p> setup failed", (void *)application);

    application_destroy(application);

    return 1;
  }

  if (!application_run(application)) {
    logger_critical_format("<application:%p> execution failed", (void *)application);
  }

  if (!application_teardown(application)) {
    logger_critical_format("<application:%p> teardown failed", (void *)application);

    application_destroy(application);

    return 1;
  }

  if (!application_destroy(application)) {
    logger_critical_format("<application:%p> destruction failed", (void *)application);

    return 1;
  }

  logger_info_format("<application:%p> destroyed", (void *)application);

  logger_info_format("<%s:v%s> <platform:%s> terminating", horus_project_name(), horus_project_version(),
                     horus_platform());

  return 0;
}
