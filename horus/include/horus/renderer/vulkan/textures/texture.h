#pragma once

/* horus renderer textures layer */
#include <horus/renderer/textures/texture.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>

typedef struct __image_transition_info {
  u32 levels;
  u32 layers;
  VkImage image;
  VkImageLayout old_layout;
  VkImageLayout new_layout;
  VkAccessFlags2 source_access_mask;
  VkAccessFlags2 destination_access_mask;
  VkPipelineStageFlags2 source_stage_mask;
  VkPipelineStageFlags2 destination_stage_mask;
} image_transition_info_t;

b8 __renderer_vulkan_transition_image_layout(renderer_t *renderer, image_transition_info_t info);
