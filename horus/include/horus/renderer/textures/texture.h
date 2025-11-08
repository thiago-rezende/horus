#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus renderer layer */
#include <horus/renderer/pipeline.h>
#include <horus/renderer/renderer.h>

typedef struct __texture texture_t;

texture_t *texture_create(renderer_t *renderer, u8 *binary, u64 size);
texture_t *texture_create_from_binary(renderer_t *renderer, char *path);

b8 texture_bind(texture_t *texture, graphics_pipeline_t *pipeline, renderer_t *renderer);

b8 texture_destroy(texture_t *texture);
