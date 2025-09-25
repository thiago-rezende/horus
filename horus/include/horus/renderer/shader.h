#pragma once

/* horus renderer layer */
#include <horus/renderer/renderer.h>

#define SHADER_MODULE_MAXIMUM_STAGE_COUNT 3

typedef enum __shader_module_stage {
  SHADER_MODULE_STAGE_NONE = 0,
  SHADER_MODULE_STAGE_VERTEX = (1 << 0),
  SHADER_MODULE_STAGE_COMPUTE = (1 << 1),
  SHADER_MODULE_STAGE_FRAGMENT = (1 << 2),
} shader_module_stage_t;

typedef struct __shader_module shader_module_t;

shader_module_t *shader_module_create(renderer_t *renderer, shader_module_stage_t stages, u8 *code, u64 size);
shader_module_t *shader_module_create_from_binary(renderer_t *renderer, shader_module_stage_t stages, char *path);

b8 shader_module_destroy(shader_module_t *module);
