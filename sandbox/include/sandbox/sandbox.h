#pragma once

#include <horus/application/application.h>

typedef struct __sandbox_application {
  application_t application;

  void *data;
} sandbox_application_t;

application_t *application_create();
void application_destroy(application_t *application);
