#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus renderer layer */
#include <horus/renderer/pipeline.h>
#include <horus/renderer/renderer.h>

#define RENDERER_DEFAULT_ALBEDO_TEXTURE_SAMPLER_BINDING 2

/* TODO: implement texture types */

typedef enum __texture_role {
  TEXTURE_ROLE_ALBEDO = 0,
  TEXTURE_ROLE_COUNT,
} texture_role_t;

typedef struct __texture_context texture_context_t;

typedef struct __texture {
  texture_role_t role;

  texture_context_t *context;
} texture_t;

texture_t *texture_create(renderer_t *renderer, texture_role_t role, u8 *binary, u64 size);
texture_t *texture_create_from_binary(renderer_t *renderer, texture_role_t role, char *path);

b8 texture_bind(texture_t *texture, graphics_pipeline_t *pipeline, renderer_t *renderer);

b8 texture_destroy(texture_t *texture);

texture_context_t *texture_context_create(renderer_t *renderer, texture_role_t role, u8 *binary, u64 size);

b8 texture_context_bind(texture_context_t *context, graphics_pipeline_t *pipeline, renderer_t *renderer);

b8 texture_context_destroy(texture_context_t *context);

const char *texture_role_string(texture_role_t role);
