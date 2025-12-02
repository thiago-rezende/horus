#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus containers layer */
#include <horus/containers/array.h>

/* horus renderer layer */
#include <horus/renderer/shader.h>
#include <horus/renderer/texture.h>
#include <horus/renderer/pipeline.h>
#include <horus/renderer/renderer.h>

typedef enum __material_type {
  MATERIAL_TYPE_DEFAULT = 0,
  MATERIAL_TYPE_UNLIT,
  MATERIAL_TYPE_COUNT,
} material_type_t;

typedef struct __material {
  material_type_t type;

  array_t *textures;

  shader_module_t *shader;

  graphics_pipeline_t *pipeline;
} material_t;

material_t *material_create(material_type_t type, renderer_t *renderer);

b8 material_destroy(material_t *material);

b8 material_bind(material_t *material, renderer_t *renderer);

const char *material_type_string(material_type_t type);
