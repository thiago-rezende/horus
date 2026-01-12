#include <horus/application/application.h>

application_t *__application = NULL;

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
