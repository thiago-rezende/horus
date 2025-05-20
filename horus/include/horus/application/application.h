#pragma once

#include <horus/core/types.h>
#include <horus/events/base.h>
#include <horus/application/configuration.h>

typedef struct __application {
  char *name;

  configuration_t configuration;

  b8 (*on_event)(event_t *event);
  b8 (*on_render)(void);
  b8 (*on_update)(f64 timestep);
} application_t;
