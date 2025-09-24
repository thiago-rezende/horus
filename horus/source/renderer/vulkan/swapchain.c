/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/swapchain.h>

/* horus core layer */
#include <horus/core/numeric.h>

/* horus logger layer */
#include <horus/logger/logger.h>

b8 renderer_vulkan_swapchain_create(renderer_t *renderer, platform_window_t *window) {
  u32 minimum_image_count = renderer->surface_capabilities.minImageCount + 1;

  if (renderer->surface_capabilities.maxImageCount > 0 &&
      minimum_image_count > renderer->surface_capabilities.maxImageCount) {
    minimum_image_count = renderer->surface_capabilities.maxImageCount;
  }

  if (!renderer_vulkan_swapchain_build_extent(renderer, window)) {
    logger_critical_format("<renderer:%p> <window:%p> swapchain extent creation failed", (void *)renderer,
                           (void *)window);

    return false;
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

b8 renderer_vulkan_swapchain_update(renderer_t *renderer, platform_window_t *window) {
  return false;
}

b8 renderer_vulkan_swapchain_destroy(renderer_t *renderer) {
  vkDestroySwapchainKHR(renderer->device, renderer->swapchain, NULL);

  return true;
}

b8 renderer_vulkan_swapchain_build_extent(renderer_t *renderer, platform_window_t *window) {
  if (renderer->surface_capabilities.currentExtent.width != max_u32) {
    renderer->swapchain_extent = renderer->surface_capabilities.currentExtent;

    return true;
  }

  platform_window_size_t window_size = platform_window_size(window);

  u32 selected_width = numeric_clamp_u32((u32)window_size.width, renderer->surface_capabilities.minImageExtent.width,
                                         renderer->surface_capabilities.maxImageExtent.width);
  u32 selected_height = numeric_clamp_u32((u32)window_size.height, renderer->surface_capabilities.minImageExtent.height,
                                          renderer->surface_capabilities.maxImageExtent.height);
  ;

  renderer->swapchain_extent = (VkExtent2D){
      .width = selected_width,
      .height = selected_height,
  };

  return true;
}
