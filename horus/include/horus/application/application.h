#pragma once

#include <horus/core/types.h>
#include <horus/application/configuration.h>

typedef struct __application {
  char *name;

  configuration_t configuration;

  b8 (*on_render)(void);
  b8 (*on_update)(u64 timestep);
} application_t;
