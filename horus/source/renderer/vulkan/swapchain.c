/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/swapchain.h>

/* horus logger layer */
#include <horus/logger/logger.h>

b8 renderer_vulkan_swapchain_create(renderer_t *renderer) {
  u32 minimum_image_count = renderer->surface_capabilities.minImageCount + 1;

  if (renderer->surface_capabilities.maxImageCount > 0 &&
      minimum_image_count > renderer->surface_capabilities.maxImageCount) {
    minimum_image_count = renderer->surface_capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR swapchain_create_info = (VkSwapchainCreateInfoKHR){
      .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
      .flags = (VkSwapchainCreateFlagsKHR){0},
      .surface = renderer->surface,
      .minImageCount = minimum_image_count,
      .imageFormat = renderer->surface_format.format,
      .imageColorSpace = renderer->surface_format.colorSpace,
      .imageExtent = renderer->surface_capabilities.currentExtent, /* TODO: build extent from window state */
      .imageArrayLayers = 1,
      .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, /* TODO: change to VK_IMAGE_USAGE_TRANSFER_DST_BIT */
      .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .preTransform = renderer->surface_capabilities.currentTransform,
      .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
      .presentMode = renderer->surface_present_mode,
      .clipped = VK_TRUE,
      .oldSwapchain = NULL,
      .queueFamilyIndexCount = 0,
      .pQueueFamilyIndices = NULL,
  };

  u32 queue_family_index_count = 2;
  u32 queue_family_indices[] = {renderer->graphics_queue_family_index, renderer->present_queue_family_index};

  if (renderer->graphics_queue_family_index != renderer->present_queue_family_index) {
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swapchain_create_info.queueFamilyIndexCount = queue_family_index_count;
    swapchain_create_info.pQueueFamilyIndices = queue_family_indices;
  }

  if (vkCreateSwapchainKHR(renderer->device, &swapchain_create_info, NULL, &renderer->swapchain) != VK_SUCCESS) {
    logger_critical_format("<device:%p> swapchain creation failed", (void *)renderer->device);

    return false;
  }

  u32 swapchain_image_count = 0;

  vkGetSwapchainImagesKHR(renderer->device, renderer->swapchain, &swapchain_image_count, NULL);

  renderer->swapchain_images = array_create(swapchain_image_count, sizeof(VkImage));
  renderer->swapchain_images->count = swapchain_image_count;

  vkGetSwapchainImagesKHR(renderer->device, renderer->swapchain, &swapchain_image_count,
                          renderer->swapchain_images->buffer);

  return true;
}

b8 renderer_vulkan_swapchain_update(renderer_t *renderer) {
  return false;
}

b8 renderer_vulkan_swapchain_destroy(renderer_t *renderer) {
  vkDestroySwapchainKHR(renderer->device, renderer->swapchain, NULL);

  return true;
}
