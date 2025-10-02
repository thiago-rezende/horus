#pragma once

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>

/* horus containers layer */
#include <horus/containers/array.h>

b8 renderer_vulkan_synchronization_create(renderer_t *renderer);
b8 renderer_vulkan_synchronization_destroy(renderer_t *renderer);

array_t *renderer_vulkan_fences_create(VkDevice device, VkFenceCreateInfo *fence_create_info, u8 count);
b8 renderer_vulkan_fences_destroy(VkDevice device, array_t *fences);

array_t *renderer_vulkan_semaphores_create(VkDevice device, VkSemaphoreCreateInfo *semaphore_create_info, u8 count);
b8 renderer_vulkan_semaphores_destroy(VkDevice device, array_t *semaphores);
