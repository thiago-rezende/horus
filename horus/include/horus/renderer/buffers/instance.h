#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus math layer */
#include <horus/math/matrix.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>
#include <horus/renderer/pipeline.h>

typedef struct __instance_buffer instance_buffer_t;

typedef struct __instance_buffer_object {
  matrix4f32_t model;
} instance_buffer_object_t;

instance_buffer_t *instance_buffer_create(renderer_t *renderer, instance_buffer_object_t *objects, u32 count);

b8 instance_buffer_bind(instance_buffer_t *buffer, graphics_pipeline_t *pipeline, renderer_t *renderer);

b8 instance_buffer_update(instance_buffer_t *buffer, instance_buffer_object_t *objects, u32 count);

b8 instance_buffer_destroy(instance_buffer_t *buffer);
