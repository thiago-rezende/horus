/* horus logger layer */
#include <horus/logger/logger.h>

/* horus application layer */
#include <horus/application/application.h>

application_t *__application = NULL;

b8 application_run(application_t *application) {
  logger_warning_format("<application:%p> application_run() not implemented", (void *)application);

  return false;
}

b8 application_setup(application_t *application) {
  logger_warning_format("<application:%p> application_setup() not implemented", (void *)application);

  return false;
}

b8 application_teardown(application_t *application) {
  logger_warning_format("<application:%p> application_teardown() not implemented", (void *)application);

  return false;
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
