/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/image.h>
#include <horus/renderer/vulkan/device.h>
#include <horus/renderer/vulkan/swapchain.h>
#include <horus/renderer/vulkan/synchronization.h>

/* horus core layer */
#include <horus/core/numeric.h>

/* horus logger layer */
#include <horus/logger/logger.h>

b8 renderer_vulkan_swapchain_create(renderer_t *renderer, platform_window_t *window) {
  vkDeviceWaitIdle(renderer->device);

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

  if (!renderer_vulkan_swapchain_image_views_create(renderer)) {
    logger_critical_format("<renderer:%p> <swapchain:%p> swapchain image views creation failed", renderer,
                           renderer->swapchain);

    array_destroy(renderer->swapchain_images);

    vkDestroySwapchainKHR(renderer->device, renderer->swapchain, NULL);

    return false;
  }

  VkImageCreateInfo depth_image_create_info = (VkImageCreateInfo){
      .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
      .flags = (VkImageCreateFlags)0,
      .imageType = VK_IMAGE_TYPE_2D,
      .extent =
          (VkExtent3D){
              .width = renderer->swapchain_extent.width,
              .height = renderer->swapchain_extent.height,
              .depth = 1,
          },
      .format = VK_FORMAT_D32_SFLOAT, /* TODO: select the best available format */
      .mipLevels = 1,
      .arrayLayers = 1,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .tiling = VK_IMAGE_TILING_OPTIMAL, /* better performance for shader access */
      .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .queueFamilyIndexCount = 0,
      .pQueueFamilyIndices = NULL,
      .pNext = NULL,
  };

  if (vkCreateImage(renderer->device, &depth_image_create_info, NULL, &renderer->depth_image) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> image creation failed", renderer);

    renderer_vulkan_swapchain_image_views_destroy(renderer);

    array_destroy(renderer->swapchain_images);

    vkDestroySwapchainKHR(renderer->device, renderer->swapchain, NULL);

    return false;
  }

  VkMemoryRequirements depth_image_memory_requirements;

  VkPhysicalDeviceMemoryProperties physical_device_memory_properties;

  VkMemoryPropertyFlags depth_image_memory_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

  vkGetImageMemoryRequirements(renderer->device, renderer->depth_image, &depth_image_memory_requirements);

  vkGetPhysicalDeviceMemoryProperties(renderer->physical_device, &physical_device_memory_properties);

  u32 depth_image_memory_type_index = 0;

  b8 has_required_depth_image_memory_type = false;

  for (u32 i = 0; i < physical_device_memory_properties.memoryTypeCount; i++) {
    if ((depth_image_memory_requirements.memoryTypeBits & (1 << i)) &&
        (physical_device_memory_properties.memoryTypes[i].propertyFlags & depth_image_memory_properties) ==
            depth_image_memory_properties) {
      depth_image_memory_type_index = i;

      has_required_depth_image_memory_type = true;

      break;
    }
  }

  if (!has_required_depth_image_memory_type) {
    logger_critical_format("<renderer:%p> desired memory type for depth image creation not found", renderer);

    vkDestroyImage(renderer->device, renderer->depth_image, NULL);

    renderer_vulkan_swapchain_image_views_destroy(renderer);

    array_destroy(renderer->swapchain_images);

    vkDestroySwapchainKHR(renderer->device, renderer->swapchain, NULL);

    return false;
  }

  VkMemoryAllocateInfo depth_image_memory_allocate_info = (VkMemoryAllocateInfo){
      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .allocationSize = depth_image_memory_requirements.size,
      .memoryTypeIndex = depth_image_memory_type_index,
  };

  /* TODO: implement a memory allocator to allow more than 4096 allocations */
  if (vkAllocateMemory(renderer->device, &depth_image_memory_allocate_info, NULL, &renderer->depth_image_memory) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <image:%p> depth image memory allocation failed", renderer,
                           renderer->depth_image);

    vkDestroyImage(renderer->device, renderer->depth_image, NULL);

    renderer_vulkan_swapchain_image_views_destroy(renderer);

    array_destroy(renderer->swapchain_images);

    vkDestroySwapchainKHR(renderer->device, renderer->swapchain, NULL);

    return false;
  }

  if (vkBindImageMemory(renderer->device, renderer->depth_image, renderer->depth_image_memory, 0) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <image:%p> depth image memory binding failed", renderer,
                           renderer->depth_image);

    vkFreeMemory(renderer->device, renderer->depth_image_memory, NULL);

    vkDestroyImage(renderer->device, renderer->depth_image, NULL);

    renderer_vulkan_swapchain_image_views_destroy(renderer);

    array_destroy(renderer->swapchain_images);

    vkDestroySwapchainKHR(renderer->device, renderer->swapchain, NULL);

    return false;
  }

  VkImageViewCreateInfo depth_image_view_create_info = (VkImageViewCreateInfo){
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .flags = (VkImageViewCreateFlags)0,
      .image = renderer->depth_image,
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = depth_image_create_info.format,
      .subresourceRange =
          (VkImageSubresourceRange){
              .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT, /* TODO: check for stencil test availability */
              .baseMipLevel = 0,
              .levelCount = depth_image_create_info.mipLevels,
              .baseArrayLayer = 0,
              .layerCount = depth_image_create_info.arrayLayers,
          },
      .pNext = NULL,
  };

  if (vkCreateImageView(renderer->device, &depth_image_view_create_info, NULL, &renderer->depth_image_view) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <image:%p> depth image view creation failed", renderer,
                           renderer->depth_image);

    vkFreeMemory(renderer->device, renderer->depth_image_memory, NULL);

    vkDestroyImage(renderer->device, renderer->depth_image, NULL);

    renderer_vulkan_swapchain_image_views_destroy(renderer);

    array_destroy(renderer->swapchain_images);

    vkDestroySwapchainKHR(renderer->device, renderer->swapchain, NULL);

    return false;
  }

  image_transition_info_t image_transition_info = (image_transition_info_t){
      .levels = depth_image_create_info.mipLevels,
      .layers = depth_image_create_info.arrayLayers,
      .image = renderer->depth_image,
      .old_layout = VK_IMAGE_LAYOUT_UNDEFINED,
      .new_layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
      .aspect_mask = VK_IMAGE_ASPECT_DEPTH_BIT, /* TODO: check for stencil test availability */
      .source_access_mask = (VkAccessFlags2)0,
      .destination_access_mask =
          VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      .source_stage_mask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      .destination_stage_mask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
  };

  if (!__renderer_vulkan_transition_image_layout(renderer, image_transition_info)) {
    logger_critical_format("<renderer:%p> <image:%p> depth image transition failed", renderer, renderer->depth_image);

    vkDestroyImageView(renderer->device, renderer->depth_image_view, NULL);

    vkFreeMemory(renderer->device, renderer->depth_image_memory, NULL);

    vkDestroyImage(renderer->device, renderer->depth_image, NULL);

    renderer_vulkan_swapchain_image_views_destroy(renderer);

    array_destroy(renderer->swapchain_images);

    vkDestroySwapchainKHR(renderer->device, renderer->swapchain, NULL);

    return false;
  }

  return true;
}

b8 renderer_vulkan_swapchain_update(renderer_t *renderer, platform_window_t *window) {
  platform_window_size_t window_size = platform_window_size(window);

  if (window_size.width == 0 || window_size.height == 0) {
    return false;
  }

  renderer_vulkan_swapchain_destroy(renderer);

  renderer_vulkan_physical_device_update_surface_information(renderer);

  renderer_vulkan_swapchain_create(renderer, window);

  return true;
}

b8 renderer_vulkan_swapchain_destroy(renderer_t *renderer) {
  vkDeviceWaitIdle(renderer->device);

  vkDestroyImageView(renderer->device, renderer->depth_image_view, NULL);

  vkFreeMemory(renderer->device, renderer->depth_image_memory, NULL);

  vkDestroyImage(renderer->device, renderer->depth_image, NULL);

  if (!renderer_vulkan_swapchain_image_views_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <swapchain:%p> swapchain image views destruction failed", renderer,
                           renderer->swapchain);

    return false;
  }

  array_destroy(renderer->swapchain_images);

  vkDestroySwapchainKHR(renderer->device, renderer->swapchain, NULL);

  return true;
}

b8 renderer_vulkan_swapchain_build_extent(renderer_t *renderer, platform_window_t *window) {
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

b8 renderer_vulkan_swapchain_image_views_create(renderer_t *renderer) {
  VkImageViewCreateInfo image_view_create_info = (VkImageViewCreateInfo){
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = renderer->surface_format.format,
      .components =
          (VkComponentMapping){
              .b = VK_COMPONENT_SWIZZLE_IDENTITY,
              .g = VK_COMPONENT_SWIZZLE_IDENTITY,
              .r = VK_COMPONENT_SWIZZLE_IDENTITY,
              .a = VK_COMPONENT_SWIZZLE_IDENTITY,
          },
      .subresourceRange =
          (VkImageSubresourceRange){
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .baseMipLevel = 0,
              .levelCount = 1,
              .baseArrayLayer = 0,
              .layerCount = 1,
          },
  };

  renderer->swapchain_image_views = array_create(renderer->swapchain_images->count, sizeof(VkImageView));

  for (u64 i = 0; i < renderer->swapchain_images->count; i++) {
    VkImage image;
    VkImageView image_view;

    array_retrieve(renderer->swapchain_images, i, (void *)&image);

    image_view_create_info.image = image;

    if (vkCreateImageView(renderer->device, &image_view_create_info, NULL, &image_view) != VK_SUCCESS) {
      logger_critical_format("<renderer:%p> <swapchain:%p> <image:%p> image views creation failed for swapchain image",
                             renderer, renderer->swapchain, image);

      renderer_vulkan_swapchain_image_views_destroy(renderer);

      return false;
    }

    array_insert(renderer->swapchain_image_views, &image_view);
  }

  return true;
}

b8 renderer_vulkan_swapchain_image_views_destroy(renderer_t *renderer) {
  for (u64 i = 0; i < renderer->swapchain_image_views->count; i++) {
    VkImageView view;

    array_retrieve(renderer->swapchain_image_views, i, (void *)&view);

    vkDestroyImageView(renderer->device, view, NULL);
  }

  array_destroy(renderer->swapchain_image_views);

  return true;
}

b8 renderer_vulkan_swapchain_image_transition(swapchain_image_transition_info_t info) {
  VkImageMemoryBarrier2 barrier = (VkImageMemoryBarrier2){
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

      .srcStageMask = info.source_stage_mask,
      .srcAccessMask = info.source_access_mask,
      .dstStageMask = info.destination_stage_mask,
      .dstAccessMask = info.destination_access_mask,
      .oldLayout = info.old_layout,
      .newLayout = info.new_layout,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image = info.image,
      .subresourceRange =
          (VkImageSubresourceRange){
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .baseMipLevel = 0,
              .levelCount = 1,
              .baseArrayLayer = 0,
              .layerCount = 1,
          },
  };

  VkDependencyInfo dependency_info = (VkDependencyInfo){
      .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
      .dependencyFlags = (VkDependencyFlags)0,
      .imageMemoryBarrierCount = 1,
      .pImageMemoryBarriers = &barrier,
  };

  vkCmdPipelineBarrier2(info.command_buffer, &dependency_info);

  return true;
}
