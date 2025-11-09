#pragma once

#include <vulkan/vulkan.h>

/* horus core layer */
#include <horus/core/types.h>

/* horus containers layer */
#include <horus/containers/array.h>

/* horus renderer layery [ vulkan ]*/
#include <horus/renderer/vulkan/renderer.h>

typedef struct __image_transition_info {
  u32 levels;
  u32 layers;
  VkImage image;
  VkImageLayout old_layout;
  VkImageLayout new_layout;
  VkAccessFlags2 source_access_mask;
  VkAccessFlags2 destination_access_mask;
  VkImageAspectFlags aspect_mask;
  VkPipelineStageFlags2 source_stage_mask;
  VkPipelineStageFlags2 destination_stage_mask;
} image_transition_info_t;

b8 renderer_vulkan_image_transition_layout(renderer_t *renderer, image_transition_info_t info);

b8 renderer_vulkan_format_has_stencil_component(VkFormat format);

VkFormat renderer_vulkan_image_find_supported_format(renderer_t *renderer,
                                                     array_t *candidates,
                                                     VkImageTiling tiling,
                                                     VkFormatFeatureFlags features);
