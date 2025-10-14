#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>

b8 renderer_vulkan_descriptor_pool_create(renderer_t *renderer);
b8 renderer_vulkan_descriptor_pool_destroy(renderer_t *renderer);

VkDescriptorSet renderer_vulkan_descriptor_set_create(VkDevice device,
                                                      VkDescriptorPool descriptor_pool,
                                                      VkDescriptorSetLayout layout);

b8 renderer_vulkan_descriptor_set_update(VkDevice device, VkDescriptorSet descriptor_set, VkBuffer buffer, u64 size);

b8 renderer_vulkan_descriptor_set_destroy(VkDevice device,
                                          VkDescriptorPool descriptor_pool,
                                          VkDescriptorSet descriptor_set);
