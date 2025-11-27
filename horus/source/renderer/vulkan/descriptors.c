/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/descriptors.h>

/* horus logger layer */
#include <horus/logger/logger.h>

b8 renderer_vulkan_descriptor_pool_create(renderer_t *renderer) {
  VkDescriptorPoolSize descriptor_pool_sizes[3] = {
      (VkDescriptorPoolSize){
          .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
          .descriptorCount = RENDERER_VULKAN_FRAMES_IN_FLIGHT * 2,
      },
      (VkDescriptorPoolSize){
          .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
          .descriptorCount = RENDERER_VULKAN_FRAMES_IN_FLIGHT * 2,
      },
      (VkDescriptorPoolSize){
          .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
          .descriptorCount = RENDERER_VULKAN_FRAMES_IN_FLIGHT * 2,
      },
  };

  VkDescriptorPoolCreateInfo descriptor_pool_create_info = (VkDescriptorPoolCreateInfo){
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
      .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
      .maxSets = RENDERER_VULKAN_FRAMES_IN_FLIGHT,
      .poolSizeCount = 3,
      .pPoolSizes = descriptor_pool_sizes,
  };

  if (vkCreateDescriptorPool(renderer->context->device, &descriptor_pool_create_info, NULL,
                             &renderer->context->descriptor_pool) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <device:%p> descriptor pool creation failed", renderer,
                           renderer->context->device);

    return false;
  }

  return true;
}

b8 renderer_vulkan_descriptor_pool_destroy(renderer_t *renderer) {
  vkDeviceWaitIdle(renderer->context->device);

  vkDestroyDescriptorPool(renderer->context->device, renderer->context->descriptor_pool, NULL);

  return true;
}

VkDescriptorSet renderer_vulkan_descriptor_set_create(VkDevice device,
                                                      VkDescriptorPool descriptor_pool,
                                                      VkDescriptorSetLayout layout) {
  VkDescriptorSet descriptor_set = VK_NULL_HANDLE;

  VkDescriptorSetAllocateInfo descriptor_set_allocate_info = (VkDescriptorSetAllocateInfo){
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
      .descriptorPool = descriptor_pool,
      .descriptorSetCount = 1,
      .pSetLayouts = &layout,
  };

  if (vkAllocateDescriptorSets(device, &descriptor_set_allocate_info, &descriptor_set) != VK_SUCCESS) {
    logger_critical_format("<device:%p> <descriptor_pool:%p> descriptor set creation failed", device, descriptor_pool);

    return VK_NULL_HANDLE;
  }

  return descriptor_set;
}

array_t *renderer_vulkan_descriptor_sets_create(VkDevice device,
                                                VkDescriptorPool descriptor_pool,
                                                VkDescriptorSetLayout layout,
                                                u8 count) {
  array_t *descriptor_sets = array_create(count, sizeof(VkDescriptorSet));
  descriptor_sets->count = count;

  array_t *descriptor_sets_layouts = array_create(count, sizeof(VkDescriptorSetLayout));

  for (u8 i = 0; i < count; i++) {
    array_insert(descriptor_sets_layouts, (void *)&layout);
  }

  VkDescriptorSetAllocateInfo descriptor_set_allocate_info = (VkDescriptorSetAllocateInfo){
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
      .descriptorPool = descriptor_pool,
      .descriptorSetCount = count,
      .pSetLayouts = descriptor_sets_layouts->buffer,
  };

  if (vkAllocateDescriptorSets(device, &descriptor_set_allocate_info, descriptor_sets->buffer) != VK_SUCCESS) {
    logger_critical_format("<device:%p> <descriptor_pool:%p> descriptor sets creation failed", device, descriptor_pool);

    array_destroy(descriptor_sets_layouts);
    array_destroy(descriptor_sets);

    return NULL;
  }

  array_destroy(descriptor_sets_layouts);

  return descriptor_sets;
}

b8 renderer_vulkan_descriptor_set_update_buffer(VkDevice device,
                                                VkDescriptorSet descriptor_set,
                                                VkDescriptorType descriptor_type,
                                                u32 binding,
                                                VkBuffer buffer,
                                                u64 size) {
  VkDescriptorBufferInfo descriptor_buffer_info = (VkDescriptorBufferInfo){
      .buffer = buffer,
      .offset = 0,
      .range = size,
  };

  VkWriteDescriptorSet write_descriptor_set = (VkWriteDescriptorSet){
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .dstSet = descriptor_set,
      .dstBinding = binding,
      .dstArrayElement = 0,
      .descriptorCount = 1,
      .descriptorType = descriptor_type,
      .pBufferInfo = &descriptor_buffer_info,
  };

  vkUpdateDescriptorSets(device, 1, &write_descriptor_set, 0, NULL);

  return true;
}

b8 renderer_vulkan_descriptor_set_update_sampler(VkDevice device,
                                                 VkDescriptorSet descriptor_set,
                                                 VkDescriptorType descriptor_type,
                                                 u32 binding,
                                                 VkSampler sampler,
                                                 VkImageView image_view) {
  VkDescriptorImageInfo descriptor_image_info = (VkDescriptorImageInfo){
      .sampler = sampler,
      .imageView = image_view,
      .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
  };

  VkWriteDescriptorSet write_descriptor_set = (VkWriteDescriptorSet){
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .dstSet = descriptor_set,
      .dstBinding = binding,
      .dstArrayElement = 0,
      .descriptorCount = 1,
      .descriptorType = descriptor_type,
      .pImageInfo = &descriptor_image_info,
  };

  vkUpdateDescriptorSets(device, 1, &write_descriptor_set, 0, NULL);

  return true;
}

b8 renderer_vulkan_descriptor_set_destroy(VkDevice device,
                                          VkDescriptorPool descriptor_pool,
                                          VkDescriptorSet descriptor_set) {
  vkFreeDescriptorSets(device, descriptor_pool, 1, &descriptor_set);

  return true;
}

b8 renderer_vulkan_descriptor_sets_destroy(array_t *descriptor_sets,
                                           VkDevice device,
                                           VkDescriptorPool descriptor_pool) {
  vkFreeDescriptorSets(device, descriptor_pool, descriptor_sets->count, descriptor_sets->buffer);

  array_destroy(descriptor_sets);

  return true;
}
