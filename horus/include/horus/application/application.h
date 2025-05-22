#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus events layer */
#include <horus/events/base.h>

/* horus application layer */
#include <horus/application/configuration.h>

typedef struct __application {
  char *name;

  configuration_t configuration;

  b8 (*on_event)(event_t *event);
  b8 (*on_render)(void);
  b8 (*on_update)(f64 timestep);
} application_t;
