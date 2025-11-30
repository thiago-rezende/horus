#pragma once

/* horus core layer */
#include <horus/core/types.h>

typedef struct __resolution {
  u16 width;
  u16 height;
} resolution_t;

typedef struct __configuration {
  b8 fullscreen;

  resolution_t resolution;
} configuration_t;
