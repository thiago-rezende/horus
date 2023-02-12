#pragma once

#include <horus/application/configuration.h>
typedef struct __application {
  char *name;

  configuration_t configuration;
} application_t;
