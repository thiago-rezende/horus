#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus containers layer */
#include <horus/containers/array.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>

b8 renderer_vulkan_descriptor_pool_create(renderer_t *renderer);
b8 renderer_vulkan_descriptor_pool_destroy(renderer_t *renderer);

VkDescriptorSet renderer_vulkan_descriptor_set_create(VkDevice device,
                                                      VkDescriptorPool descriptor_pool,
                                                      VkDescriptorSetLayout layout);

array_t *renderer_vulkan_descriptor_sets_create(VkDevice device,
                                                VkDescriptorPool descriptor_pool,
                                                VkDescriptorSetLayout layout,
                                                u8 count);

b8 renderer_vulkan_descriptor_set_update_buffer(VkDevice device,
                                                VkDescriptorSet descriptor_set,
                                                VkDescriptorType descriptor_type,
                                                u32 binding,
                                                VkBuffer buffer,
                                                u64 size);

b8 renderer_vulkan_descriptor_set_update_sampler(VkDevice device,
                                                 VkDescriptorSet descriptor_set,
                                                 VkDescriptorType descriptor_type,
                                                 u32 binding,
                                                 VkSampler sampler,
                                                 VkImageView image_view);

b8 renderer_vulkan_descriptor_set_destroy(VkDevice device,
                                          VkDescriptorPool descriptor_pool,
                                          VkDescriptorSet descriptor_set);

b8 renderer_vulkan_descriptor_sets_destroy(array_t *descriptor_sets, VkDevice device, VkDescriptorPool descriptor_pool);
