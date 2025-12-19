#pragma once

/* horus containers layer */
#include <horus/containers/array.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>

typedef struct __platform_renderer_context platform_renderer_context_t;

struct __renderer_context {
  renderer_implementation_t implementation;

  platform_renderer_context_t *platform_context;
};
