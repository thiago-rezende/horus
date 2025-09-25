#pragma once

/* horus containers layer */
#include <horus/containers/array.h>

/* horus renderer layer */
#include <horus/renderer/shader.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>

#define RENDERER_DEFAULT_SHADER_MODULE_VERTEX_STAGE_ENTRYPOINT "vertex_entrypoint"
#define RENDERER_DEFAULT_SHADER_MODULE_COMPUTE_STAGE_ENTRYPOINT "compute_entrypoint"
#define RENDERER_DEFAULT_SHADER_MODULE_FRAGMENT_STAGE_ENTRYPOINT "fragment_entrypoint"

struct __shader_module {
  VkDevice device;

  VkShaderModule module;

  VkPipelineShaderStageCreateInfo vertex_stage_create_info;
  VkPipelineShaderStageCreateInfo compute_stage_create_info;
  VkPipelineShaderStageCreateInfo fragment_stage_create_info;

  b8 has_vertex_stage_create_info;
  b8 has_compute_stage_create_info;
  b8 has_fragment_stage_create_info;
};

array_t *renderer_vulkan_shader_module_stage_create_infos(shader_module_t *module);
