#pragma once

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>

b8 renderer_vulkan_command_pools_create(renderer_t *renderer);
b8 renderer_vulkan_command_pools_destroy(renderer_t *renderer);

VkCommandBuffer renderer_vulkan_command_buffer_create(VkCommandPool pool, VkDevice device);

b8 renderer_vulkan_command_buffer_reset(VkCommandBuffer buffer);
b8 renderer_vulkan_command_buffer_destroy(VkCommandBuffer buffer, VkCommandPool pool, VkDevice device);
