#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus renderer layer */
#include <horus/renderer/shader.h>
#include <horus/renderer/renderer.h>

typedef struct __graphics_pipeline graphics_pipeline_t;

graphics_pipeline_t *graphics_pipeline_create(renderer_t *renderer, shader_module_t *module);
b8 graphics_pipeline_destroy(graphics_pipeline_t *pipeline);

b8 graphics_pipeline_bind(graphics_pipeline_t *pipeline, renderer_t *renderer);
