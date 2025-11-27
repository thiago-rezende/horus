/* horus core layer */
#include <horus/core/types.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/image.h>
#include <horus/renderer/vulkan/command.h>
#include <horus/renderer/vulkan/renderer.h>

b8 renderer_vulkan_image_transition_layout(renderer_t *renderer, image_transition_info_t info) {
  VkCommandBuffer graphics_command_buffer =
      renderer_vulkan_command_buffer_create(renderer->context->device, renderer->context->graphics_command_pool);

  VkCommandBufferBeginInfo graphics_command_buffer_begin_info = (VkCommandBufferBeginInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
  };

  if (vkBeginCommandBuffer(graphics_command_buffer, &graphics_command_buffer_begin_info) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> graphics command buffer beginning failed", renderer,
                           graphics_command_buffer);

    renderer_vulkan_command_buffer_destroy(renderer->context->device, graphics_command_buffer,
                                           renderer->context->graphics_command_pool);

    return false;
  }

  VkImageMemoryBarrier2 barrier = (VkImageMemoryBarrier2){
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

      .srcStageMask = info.source_stage_mask,
      .srcAccessMask = info.source_access_mask,
      .dstStageMask = info.destination_stage_mask,
      .dstAccessMask = info.destination_access_mask,
      .oldLayout = info.old_layout,
      .newLayout = info.new_layout,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, /* TODO: handle queue ownership transfer */
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, /* TODO: handle queue ownership transfer */
      .image = info.image,
      .subresourceRange =
          (VkImageSubresourceRange){
              .aspectMask = info.aspect_mask,
              .baseMipLevel = 0,
              .levelCount = info.levels,
              .baseArrayLayer = 0,
              .layerCount = info.layers,
          },
  };

  VkDependencyInfo dependency_info = (VkDependencyInfo){
      .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
      .dependencyFlags = (VkDependencyFlags)0,
      .imageMemoryBarrierCount = 1,
      .pImageMemoryBarriers = &barrier,
  };

  vkCmdPipelineBarrier2(graphics_command_buffer, &dependency_info);

  if (vkEndCommandBuffer(graphics_command_buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> graphics command buffer ending failed", renderer,
                           graphics_command_buffer);

    renderer_vulkan_command_buffer_destroy(renderer->context->device, graphics_command_buffer,
                                           renderer->context->graphics_command_pool);

    return false;
  }

  VkSubmitInfo graphics_submit_info = (VkSubmitInfo){
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers = &graphics_command_buffer,
  };

  if (vkQueueSubmit(renderer->context->graphics_queue, 1, &graphics_submit_info, VK_NULL_HANDLE) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <queue:%p> graphics queue submission failed", renderer,
                           renderer->context->graphics_queue);

    renderer_vulkan_command_buffer_destroy(renderer->context->device, graphics_command_buffer,
                                           renderer->context->graphics_command_pool);

    return false;
  }

  vkQueueWaitIdle(renderer->context->graphics_queue);

  renderer_vulkan_command_buffer_destroy(renderer->context->device, graphics_command_buffer,
                                         renderer->context->graphics_command_pool);

  return true;
}

b8 renderer_vulkan_format_has_stencil_component(VkFormat format) {
  return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkFormat renderer_vulkan_image_find_supported_format(renderer_t *renderer,
                                                     array_t *candidates,
                                                     VkImageTiling tiling,
                                                     VkFormatFeatureFlags features) {
  for (u64 i = 0; i < candidates->count; i++) {
    VkFormat format;

    array_retrieve(candidates, i, (void *)&format);

    VkFormatProperties format_properties;

    vkGetPhysicalDeviceFormatProperties(renderer->context->physical_device, format, &format_properties);

    if (tiling == VK_IMAGE_TILING_LINEAR && (format_properties.linearTilingFeatures & features) == features) {
      return format;
    }

    if (tiling == VK_IMAGE_TILING_OPTIMAL && (format_properties.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  return VK_FORMAT_UNDEFINED;
}
