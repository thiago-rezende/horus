#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus types layer */
#include <horus/types/vertex.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>
#include <horus/renderer/pipeline.h>

typedef struct __index_buffer index_buffer_t;

index_buffer_t *index_buffer_create(renderer_t *renderer, u32 *indices, u64 count);

b8 index_buffer_bind(index_buffer_t *buffer, renderer_t *renderer);

b8 index_buffer_destroy(index_buffer_t *buffer);
