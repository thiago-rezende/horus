#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus containers layer */
#include <horus/containers/array.h>

/* horus renderer layer */
#include <horus/renderer/pipeline.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/shader.h>
#include <horus/renderer/vulkan/renderer.h>

struct __graphics_pipeline {
  VkDevice device;

  array_t *dynamic_states;

  shader_module_t *module;

  VkPipelineLayout layout;

  VkPipeline pipeline;
};
