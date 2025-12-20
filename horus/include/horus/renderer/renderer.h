#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus platform layer */
#include <horus/platform/window.h>

/* horus application layer */
#include <horus/application/version.h>

typedef enum __renderer_implementation {
  RENDERER_IMPLEMENTATION_NONE = 0,
  RENDERER_IMPLEMENTATION_OPENGL,
  RENDERER_IMPLEMENTATION_COUNT
} renderer_implementation_t;

typedef struct __renderer_context renderer_context_t;

typedef struct __renderer {
  renderer_implementation_t implementation;

  renderer_context_t *context;
} renderer_t;

typedef struct __renderer_context_create_info {
  char *name;
  version_t version;
} renderer_context_create_info_t;

renderer_t *renderer_create(renderer_context_create_info_t info, platform_window_t *window);
b8 renderer_destroy(renderer_t *renderer);

const char *renderer_implementation_string(renderer_implementation_t implementation);

b8 renderer_record_commands(renderer_t *renderer);
b8 renderer_submit_commands(renderer_t *renderer);

b8 renderer_draw(renderer_t *renderer, u32 vertices, u32 instances);
b8 renderer_draw_indexed(renderer_t *renderer, u32 indices, u32 instances);

b8 renderer_context_create(renderer_t *renderer, renderer_context_create_info_t info, platform_window_t *window);
b8 renderer_context_destroy(renderer_t *renderer);

renderer_implementation_t renderer_context_implementation(renderer_t *renderer);

b8 renderer_context_record_commands(renderer_t *renderer);
b8 renderer_context_submit_commands(renderer_t *renderer);

b8 renderer_context_draw(renderer_t *renderer, u32 vertices, u32 instances);
b8 renderer_context_draw_indexed(renderer_t *renderer, u32 indices, u32 instances);
