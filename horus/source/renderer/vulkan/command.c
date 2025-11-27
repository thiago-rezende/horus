/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/command.h>

/* horus logger layer */
#include <horus/logger/logger.h>

b8 renderer_vulkan_command_pools_create(renderer_t *renderer) {
  VkCommandPoolCreateInfo command_pool_create_info = (VkCommandPoolCreateInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
  };

  command_pool_create_info.queueFamilyIndex = renderer->context->compute_queue_family_index;

  if (vkCreateCommandPool(renderer->context->device, &command_pool_create_info, NULL,
                          &renderer->context->compute_command_pool) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> compute command pool creation failed", (void *)renderer);

    return false;
  }

  command_pool_create_info.queueFamilyIndex = renderer->context->present_queue_family_index;

  if (vkCreateCommandPool(renderer->context->device, &command_pool_create_info, NULL,
                          &renderer->context->present_command_pool) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> present command pool creation failed", (void *)renderer);

    vkDestroyCommandPool(renderer->context->device, renderer->context->compute_command_pool, NULL);

    return false;
  }

  command_pool_create_info.queueFamilyIndex = renderer->context->graphics_queue_family_index;

  if (vkCreateCommandPool(renderer->context->device, &command_pool_create_info, NULL,
                          &renderer->context->graphics_command_pool) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> graphics command pool creation failed", (void *)renderer);

    vkDestroyCommandPool(renderer->context->device, renderer->context->compute_command_pool, NULL);
    vkDestroyCommandPool(renderer->context->device, renderer->context->present_command_pool, NULL);

    return false;
  }

  command_pool_create_info.queueFamilyIndex = renderer->context->transfer_queue_family_index;

  if (vkCreateCommandPool(renderer->context->device, &command_pool_create_info, NULL,
                          &renderer->context->transfer_command_pool) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> transfer command pool creation failed", (void *)renderer);

    vkDestroyCommandPool(renderer->context->device, renderer->context->compute_command_pool, NULL);
    vkDestroyCommandPool(renderer->context->device, renderer->context->present_command_pool, NULL);
    vkDestroyCommandPool(renderer->context->device, renderer->context->graphics_command_pool, NULL);

    return false;
  }

  renderer->context->compute_command_buffers = renderer_vulkan_command_buffers_create(
      renderer->context->device, renderer->context->compute_command_pool, RENDERER_VULKAN_FRAMES_IN_FLIGHT);

  renderer->context->present_command_buffers = renderer_vulkan_command_buffers_create(
      renderer->context->device, renderer->context->present_command_pool, RENDERER_VULKAN_FRAMES_IN_FLIGHT);

  renderer->context->graphics_command_buffers = renderer_vulkan_command_buffers_create(
      renderer->context->device, renderer->context->graphics_command_pool, RENDERER_VULKAN_FRAMES_IN_FLIGHT);

  renderer->context->transfer_command_buffers = renderer_vulkan_command_buffers_create(
      renderer->context->device, renderer->context->transfer_command_pool, RENDERER_VULKAN_FRAMES_IN_FLIGHT);

  return true;
}

b8 renderer_vulkan_command_pools_destroy(renderer_t *renderer) {
  renderer_vulkan_command_buffers_destroy(renderer->context->compute_command_buffers, renderer->context->device,
                                          renderer->context->compute_command_pool);

  renderer_vulkan_command_buffers_destroy(renderer->context->present_command_buffers, renderer->context->device,
                                          renderer->context->present_command_pool);

  renderer_vulkan_command_buffers_destroy(renderer->context->graphics_command_buffers, renderer->context->device,
                                          renderer->context->graphics_command_pool);

  renderer_vulkan_command_buffers_destroy(renderer->context->transfer_command_buffers, renderer->context->device,
                                          renderer->context->transfer_command_pool);

  vkDestroyCommandPool(renderer->context->device, renderer->context->compute_command_pool, NULL);
  vkDestroyCommandPool(renderer->context->device, renderer->context->present_command_pool, NULL);
  vkDestroyCommandPool(renderer->context->device, renderer->context->graphics_command_pool, NULL);
  vkDestroyCommandPool(renderer->context->device, renderer->context->transfer_command_pool, NULL);

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
