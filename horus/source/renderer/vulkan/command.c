/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/command.h>

/* horus logger layer */
#include <horus/logger/logger.h>

b8 renderer_vulkan_command_pool_create(renderer_t *renderer) {
  VkCommandPoolCreateInfo command_pool_create_info = (VkCommandPoolCreateInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
      .queueFamilyIndex = renderer->graphics_queue_family_index,
  };

  if (vkCreateCommandPool(renderer->device, &command_pool_create_info, NULL, &renderer->command_pool) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> command buffer creation failed", (void *)renderer);

    return false;
  }

  return true;
}

b8 renderer_vulkan_command_pool_destroy(renderer_t *renderer) {
  vkDestroyCommandPool(renderer->device, renderer->command_pool, NULL);

  return true;
}
