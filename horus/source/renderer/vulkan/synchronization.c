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

  renderer->context->render_complete_fences =
      renderer_vulkan_fences_create(renderer->context->device, &fence_create_info, RENDERER_VULKAN_FRAMES_IN_FLIGHT);

  renderer->context->transfer_complete_fences =
      renderer_vulkan_fences_create(renderer->context->device, &fence_create_info, RENDERER_VULKAN_FRAMES_IN_FLIGHT);

  renderer->context->render_complete_semaphores = renderer_vulkan_semaphores_create(
      renderer->context->device, &semaphore_create_info, renderer->context->swapchain_images->count);

  renderer->context->present_complete_semaphores = renderer_vulkan_semaphores_create(
      renderer->context->device, &semaphore_create_info, renderer->context->swapchain_images->count);

  return true;
}

b8 renderer_vulkan_synchronization_destroy(renderer_t *renderer) {
  vkDeviceWaitIdle(renderer->context->device);

  renderer_vulkan_fences_destroy(renderer->context->device, renderer->context->render_complete_fences);
  renderer_vulkan_fences_destroy(renderer->context->device, renderer->context->transfer_complete_fences);

  renderer_vulkan_semaphores_destroy(renderer->context->device, renderer->context->render_complete_semaphores);
  renderer_vulkan_semaphores_destroy(renderer->context->device, renderer->context->present_complete_semaphores);

  return true;
}

array_t *renderer_vulkan_fences_create(VkDevice device, VkFenceCreateInfo *fence_create_info, u8 count) {
  array_t *fences = array_create(count, sizeof(VkFence));

  for (u8 i = 0; i < count; i++) {
    VkFence fence;

    if (vkCreateFence(device, fence_create_info, NULL, &fence) != VK_SUCCESS) {
      logger_critical_format("<device:%p> fence creation failed", device);

      renderer_vulkan_fences_destroy(device, fences);

      return NULL;
    }

    array_insert(fences, &fence);
  }

  return fences;
}

b8 renderer_vulkan_fences_destroy(VkDevice device, array_t *fences) {
  for (u8 i = 0; i < fences->count; i++) {
    VkFence fence;

    array_retrieve(fences, i, &fence);

    vkDestroyFence(device, fence, NULL);
  }

  array_destroy(fences);

  return true;
}

array_t *renderer_vulkan_semaphores_create(VkDevice device, VkSemaphoreCreateInfo *semaphore_create_info, u8 count) {
  array_t *semaphores = array_create(count, sizeof(VkSemaphore));

  for (u8 i = 0; i < count; i++) {
    VkSemaphore semaphore;

    if (vkCreateSemaphore(device, semaphore_create_info, NULL, &semaphore) != VK_SUCCESS) {
      logger_critical_format("<device:%p> semaphore creation failed", device);

      renderer_vulkan_semaphores_destroy(device, semaphores);

      return NULL;
    }

    array_insert(semaphores, &semaphore);
  }

  return semaphores;
}

b8 renderer_vulkan_semaphores_destroy(VkDevice device, array_t *semaphores) {
  for (u8 i = 0; i < semaphores->count; i++) {
    VkSemaphore semaphore;

    array_retrieve(semaphores, i, &semaphore);

    vkDestroySemaphore(device, semaphore, NULL);
  }

  array_destroy(semaphores);

  return true;
}
