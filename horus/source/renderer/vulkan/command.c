/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/command.h>

/* horus logger layer */
#include <horus/logger/logger.h>

b8 renderer_vulkan_command_pools_create(renderer_t *renderer) {
  VkCommandPoolCreateInfo command_pool_create_info = (VkCommandPoolCreateInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
  };

  command_pool_create_info.queueFamilyIndex = renderer->compute_queue_family_index;

  if (vkCreateCommandPool(renderer->device, &command_pool_create_info, NULL, &renderer->compute_command_pool) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> compute command pool creation failed", (void *)renderer);

    return false;
  }

  command_pool_create_info.queueFamilyIndex = renderer->present_queue_family_index;

  if (vkCreateCommandPool(renderer->device, &command_pool_create_info, NULL, &renderer->present_command_pool) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> present command pool creation failed", (void *)renderer);

    vkDestroyCommandPool(renderer->device, renderer->compute_command_pool, NULL);

    return false;
  }

  command_pool_create_info.queueFamilyIndex = renderer->graphics_queue_family_index;

  if (vkCreateCommandPool(renderer->device, &command_pool_create_info, NULL, &renderer->graphics_command_pool) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> graphics command pool creation failed", (void *)renderer);

    vkDestroyCommandPool(renderer->device, renderer->compute_command_pool, NULL);
    vkDestroyCommandPool(renderer->device, renderer->present_command_pool, NULL);

    return false;
  }

  command_pool_create_info.queueFamilyIndex = renderer->transfer_queue_family_index;

  if (vkCreateCommandPool(renderer->device, &command_pool_create_info, NULL, &renderer->transfer_command_pool) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> transfer command pool creation failed", (void *)renderer);

    vkDestroyCommandPool(renderer->device, renderer->compute_command_pool, NULL);
    vkDestroyCommandPool(renderer->device, renderer->present_command_pool, NULL);
    vkDestroyCommandPool(renderer->device, renderer->graphics_command_pool, NULL);

    return false;
  }

  renderer->compute_command_buffers = renderer_vulkan_command_buffers_create(
      renderer->device, renderer->compute_command_pool, RENDERER_VULKAN_FRAMES_IN_FLIGHT);

  renderer->present_command_buffers = renderer_vulkan_command_buffers_create(
      renderer->device, renderer->present_command_pool, RENDERER_VULKAN_FRAMES_IN_FLIGHT);

  renderer->graphics_command_buffers = renderer_vulkan_command_buffers_create(
      renderer->device, renderer->graphics_command_pool, RENDERER_VULKAN_FRAMES_IN_FLIGHT);

  renderer->transfer_command_buffers = renderer_vulkan_command_buffers_create(
      renderer->device, renderer->transfer_command_pool, RENDERER_VULKAN_FRAMES_IN_FLIGHT);

  return true;
}

b8 renderer_vulkan_command_pools_destroy(renderer_t *renderer) {
  renderer_vulkan_command_buffers_destroy(renderer->compute_command_buffers, renderer->device,
                                          renderer->compute_command_pool);

  renderer_vulkan_command_buffers_destroy(renderer->present_command_buffers, renderer->device,
                                          renderer->present_command_pool);

  renderer_vulkan_command_buffers_destroy(renderer->graphics_command_buffers, renderer->device,
                                          renderer->graphics_command_pool);

  renderer_vulkan_command_buffers_destroy(renderer->transfer_command_buffers, renderer->device,
                                          renderer->transfer_command_pool);

  vkDestroyCommandPool(renderer->device, renderer->compute_command_pool, NULL);
  vkDestroyCommandPool(renderer->device, renderer->present_command_pool, NULL);
  vkDestroyCommandPool(renderer->device, renderer->graphics_command_pool, NULL);
  vkDestroyCommandPool(renderer->device, renderer->transfer_command_pool, NULL);

  return true;
}

b8 renderer_vulkan_command_buffer_reset(VkCommandBuffer buffer) {
  vkResetCommandBuffer(buffer, 0);

  return true;
}

VkCommandBuffer renderer_vulkan_command_buffer_create(VkDevice device, VkCommandPool pool) {
  VkCommandBuffer buffer = VK_NULL_HANDLE;

  VkCommandBufferAllocateInfo command_buffer_allocate_info = (VkCommandBufferAllocateInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandPool = pool,
      .commandBufferCount = 1,
  };

  if (vkAllocateCommandBuffers(device, &command_buffer_allocate_info, &buffer) != VK_SUCCESS) {
    logger_critical_format("<command_pool:%p> command buffer creation failed for command pool", (void *)pool);

    return VK_NULL_HANDLE;
  }

  return buffer;
}

b8 renderer_vulkan_command_buffer_destroy(VkDevice device, VkCommandBuffer buffer, VkCommandPool pool) {
  vkFreeCommandBuffers(device, pool, 1, &buffer);

  return true;
}

/* TODO: call vkAllocateCommandBuffers directly since it can handle multiple buffers creation */
array_t *renderer_vulkan_command_buffers_create(VkDevice device, VkCommandPool pool, u8 count) {
  array_t *buffers = array_create(count, sizeof(VkCommandBuffer));

  for (u8 i = 0; i < count; i++) {
    VkCommandBuffer buffer = renderer_vulkan_command_buffer_create(device, pool);

    if (buffer == VK_NULL_HANDLE) {
      renderer_vulkan_command_buffers_destroy(buffers, device, pool);

      return NULL;
    }

    array_insert(buffers, &buffer);
  }

  return buffers;
}

/* TODO: call vkFreeCommandBuffers directly since it can handle multiple buffers creation */
b8 renderer_vulkan_command_buffers_destroy(array_t *buffers, VkDevice device, VkCommandPool pool) {
  for (u8 i = 0; i < buffers->count; i++) {
    VkCommandBuffer buffer;

    array_retrieve(buffers, i, &buffer);

    if (!renderer_vulkan_command_buffer_destroy(device, buffer, pool)) {
      array_destroy(buffers);

      return false;
    }
  }

  array_destroy(buffers);

  return true;
}
