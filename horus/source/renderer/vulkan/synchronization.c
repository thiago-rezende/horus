/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/synchronization.h>

/* horus logger layer */
#include <horus/logger/logger.h>

b8 renderer_vulkan_synchronization_create(renderer_t *renderer) {
  VkFenceCreateInfo fence_create_info = (VkFenceCreateInfo){
      .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
      .flags = VK_FENCE_CREATE_SIGNALED_BIT,
  };

  VkSemaphoreCreateInfo semaphore_create_info = (VkSemaphoreCreateInfo){
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
  };

  if (vkCreateFence(renderer->device, &fence_create_info, NULL, &renderer->render_complete_fence) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> render complete fence creation failed", renderer);

    return false;
  }

  if (vkCreateSemaphore(renderer->device, &semaphore_create_info, NULL, &renderer->render_complete_semaphore) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> render complete semaphore creation failed", renderer);

    vkDestroyFence(renderer->device, renderer->render_complete_fence, NULL);

    return false;
  }

  if (vkCreateSemaphore(renderer->device, &semaphore_create_info, NULL, &renderer->present_complete_semaphore) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> present complete semaphore creation failed", renderer);

    vkDestroyFence(renderer->device, renderer->render_complete_fence, NULL);
    vkDestroySemaphore(renderer->device, renderer->render_complete_semaphore, NULL);

    return false;
  }

  return true;
}

b8 renderer_vulkan_synchronization_destroy(renderer_t *renderer) {
  vkDestroyFence(renderer->device, renderer->render_complete_fence, NULL);
  vkDestroySemaphore(renderer->device, renderer->render_complete_semaphore, NULL);
  vkDestroySemaphore(renderer->device, renderer->present_complete_semaphore, NULL);

  return true;
}
