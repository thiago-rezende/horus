#pragma once

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>

/* horus containers layer */
#include <horus/containers/array.h>

b8 renderer_vulkan_command_pools_create(renderer_t *renderer);
b8 renderer_vulkan_command_pools_destroy(renderer_t *renderer);

VkCommandBuffer renderer_vulkan_command_buffer_create(VkDevice device, VkCommandPool pool);

b8 renderer_vulkan_command_buffer_reset(VkCommandBuffer buffer);
b8 renderer_vulkan_command_buffer_destroy(VkDevice device, VkCommandBuffer buffer, VkCommandPool pool);

array_t *renderer_vulkan_command_buffers_create(VkDevice device, VkCommandPool pool, u8 count);
b8 renderer_vulkan_command_buffers_destroy(array_t *buffers, VkDevice device, VkCommandPool pool);
