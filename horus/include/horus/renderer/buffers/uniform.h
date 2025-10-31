#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus math layer */
#include <horus/math/matrix.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>
#include <horus/renderer/pipeline.h>

typedef struct __uniform_buffer uniform_buffer_t;

typedef struct __uniform_buffer_object {
  f32 time;

  matrix4f32_t view;
  matrix4f32_t projection;

  vector3f32_t camera_position;
} uniform_buffer_object_t;

uniform_buffer_t *uniform_buffer_create(renderer_t *renderer, uniform_buffer_object_t *object);

b8 uniform_buffer_bind(uniform_buffer_t *buffer, graphics_pipeline_t *pipeline, renderer_t *renderer);

b8 uniform_buffer_update(uniform_buffer_t *buffer, uniform_buffer_object_t *object);

b8 uniform_buffer_destroy(uniform_buffer_t *buffer);
