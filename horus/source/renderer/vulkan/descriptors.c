/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/descriptors.h>

/* horus logger layer */
#include <horus/logger/logger.h>

b8 renderer_vulkan_descriptor_pool_create(renderer_t *renderer) {
  VkDescriptorPoolSize descriptor_pool_size = (VkDescriptorPoolSize){
      .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
      .descriptorCount = RENDERER_VULKAN_FRAMES_IN_FLIGHT,
  };

  VkDescriptorPoolCreateInfo descriptor_pool_create_info = (VkDescriptorPoolCreateInfo){
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
      .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
      .maxSets = RENDERER_VULKAN_FRAMES_IN_FLIGHT,
      .poolSizeCount = 1,
      .pPoolSizes = &descriptor_pool_size,
  };

  if (vkCreateDescriptorPool(renderer->device, &descriptor_pool_create_info, NULL, &renderer->descriptor_pool) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <device:%p> descriptor pool creation failed", renderer, renderer->device);

    return false;
  }

  return true;
}

b8 renderer_vulkan_descriptor_pool_destroy(renderer_t *renderer) {
  vkDeviceWaitIdle(renderer->device);

  vkDestroyDescriptorPool(renderer->device, renderer->descriptor_pool, NULL);

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

b8 renderer_vulkan_descriptor_set_update(VkDevice device, VkDescriptorSet descriptor_set, VkBuffer buffer, u64 size) {
  VkDescriptorBufferInfo descriptor_buffer_info = (VkDescriptorBufferInfo){
      .buffer = buffer,
      .offset = 0,
      .range = size,
  };

  VkWriteDescriptorSet write_descriptor_set = (VkWriteDescriptorSet){
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .dstSet = descriptor_set,
      .dstBinding = 0,
      .dstArrayElement = 0,
      .descriptorCount = 1,
      .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
      .pBufferInfo = &descriptor_buffer_info,
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
