#pragma once

/* horus platform layer */
#include <horus/platform/window.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>

typedef struct __swapchain_image_transition_info {
  VkImage image;
  VkImageLayout old_layout;
  VkImageLayout new_layout;
  VkAccessFlags2 source_access_mask;
  VkAccessFlags2 destination_access_mask;
  VkPipelineStageFlags2 source_stage_mask;
  VkPipelineStageFlags2 destination_stage_mask;
  VkCommandBuffer command_buffer;
} swapchain_image_transition_info_t;

b8 renderer_vulkan_swapchain_create(renderer_t *renderer, platform_window_t *window);
b8 renderer_vulkan_swapchain_update(renderer_t *renderer, platform_window_t *window);
b8 renderer_vulkan_swapchain_destroy(renderer_t *renderer);

b8 renderer_vulkan_swapchain_build_extent(renderer_t *renderer, platform_window_t *window);

b8 renderer_vulkan_swapchain_image_views_create(renderer_t *renderer);
b8 renderer_vulkan_swapchain_image_views_destroy(renderer_t *renderer);

b8 renderer_vulkan_swapchain_image_transition(swapchain_image_transition_info_t info);
