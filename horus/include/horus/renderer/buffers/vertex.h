#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus types layer */
#include <horus/types/vertex.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>
#include <horus/renderer/pipeline.h>

typedef struct __vertex_buffer vertex_buffer_t;

vertex_buffer_t *vertex_buffer_create(renderer_t *renderer, vertex_t *vertices, u64 count);

b8 vertex_buffer_bind(vertex_buffer_t *buffer, graphics_pipeline_t *pipeline, renderer_t *renderer);

b8 vertex_buffer_destroy(vertex_buffer_t *buffer);
