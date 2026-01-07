#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus platform layer */
#include <horus/platform/window.h>

/* horus events layer */
#include <horus/events/base.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>

/* horus application layer */
#include <horus/application/version.h>
#include <horus/application/configuration.h>

typedef struct __application application_t;

struct __application {
  char *name;

  version_t version;

  renderer_t *renderer;

  configuration_t configuration;

  /* TODO: improve for multiple windows support */
  platform_window_t *window;

  b8 running;

  b8 (*on_create)(application_t *application, platform_window_t *window, renderer_t *renderer);
  b8 (*on_destroy)(application_t *application, platform_window_t *window, renderer_t *renderer);

  b8 (*on_event)(event_t *event);
  b8 (*on_render)(renderer_t *renderer);
  b8 (*on_update)(f64 timestep);
};

application_t *application(void);

b8 __application_set_global_instance(application_t *application);
