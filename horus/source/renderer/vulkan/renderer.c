#include <vulkan/vulkan.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/debug.h>
#include <horus/renderer/vulkan/device.h>
#include <horus/renderer/vulkan/loader.h>
#include <horus/renderer/vulkan/command.h>
#include <horus/renderer/vulkan/instance.h>
#include <horus/renderer/vulkan/platform.h>
#include <horus/renderer/vulkan/renderer.h>
#include <horus/renderer/vulkan/swapchain.h>
#include <horus/renderer/vulkan/synchronization.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus logger layer */
#include <horus/logger/logger.h>

renderer_t *renderer_create(application_t *application, platform_window_t *window) {
  renderer_t *renderer = platform_memory_allocate(sizeof(renderer_t));

  *renderer = (renderer_t){
      .implementation = RENDERER_IMPLEMENTATION_VULKAN,
      .implementation_string = __renderer_implementation_string(RENDERER_IMPLEMENTATION_VULKAN),
  };

  if (!renderer_vulkan_instance_create(renderer, application)) {
    logger_critical_format("<renderer:%p> VkInstance creation failed", (void *)renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug_format("<renderer:%p> <instance:%p> VkInstance created", (void *)renderer, (void *)renderer->instance);

  if (!renderer_vulkan_debug_messenger_create(renderer)) {
    logger_critical_format("<renderer:%p> VkDebugUtilsMessengerEXT creation failed", (void *)renderer);

    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug_format("<renderer:%p> <messenger:%p> VkDebugUtilsMessengerEXT created", (void *)renderer,
                      (void *)renderer->messenger);

  if (!renderer_vulkan_surface_create(renderer, window)) {
    logger_critical_format("<renderer:%p> VkSurfaceKHR creation failed", (void *)renderer);

    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug_format("<renderer:%p> <surface:%p> VkSurfaceKHR created", (void *)renderer, (void *)renderer->surface);

  if (!renderer_vulkan_physical_device_select(renderer)) {
    logger_critical_format("<renderer:%p> VkPhysicalDevice selection failed", (void *)renderer);

    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug_format("<renderer:%p> <physical_device:%p> VkPhysicalDevice selected", (void *)renderer,
                      (void *)renderer->physical_device);
  logger_debug_format("|- [ %u ] %s", renderer->physical_device_properties.deviceID,
                      renderer->physical_device_properties.deviceName);
  logger_debug_format("|- |- [ queues family indices ] <compute:%u> <present:%u> <graphics:%u> <transfer:%u>",
                      renderer->compute_queue_family_index, renderer->present_queue_family_index,
                      renderer->graphics_queue_family_index, renderer->transfer_queue_family_index);

  if (!renderer_vulkan_device_create(renderer)) {
    logger_critical_format("<renderer:%p> VkDevice creation failed", (void *)renderer);

    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug_format("<renderer:%p> <device:%p> VkDevice created", (void *)renderer, (void *)renderer->device);

  if (!renderer_vulkan_swapchain_create(renderer, window)) {
    logger_critical_format("<renderer:%p> VkSwapchainKHR creation failed", (void *)renderer);

    renderer_vulkan_device_destroy(renderer);
    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug_format("<renderer:%p> <swapchain:%p> VkSwapchainKHR created", (void *)renderer,
                      (void *)renderer->swapchain);
  logger_debug_format("|- [ image count ] %lu", renderer->swapchain_images->count);

  if (!renderer_vulkan_command_pools_create(renderer)) {
    logger_critical_format("<renderer:%p> VkCommandPool creation failed", (void *)renderer);

    renderer_vulkan_swapchain_destroy(renderer);
    renderer_vulkan_device_destroy(renderer);
    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug_format("<renderer:%p> VkCommandPools created", (void *)renderer);
  logger_debug_format("|- [ pools ] <compute:%p> <present:%p> <graphics:%p> <transfer:%p>",
                      (void *)renderer->compute_command_pool, (void *)renderer->present_command_pool,
                      (void *)renderer->graphics_command_pool, (void *)renderer->transfer_command_pool);

  if (!renderer_vulkan_synchronization_create(renderer)) {
    logger_critical_format("<renderer:%p> synchronization objects creation failed", (void *)renderer);

    renderer_vulkan_command_pools_destroy(renderer);
    renderer_vulkan_swapchain_destroy(renderer);
    renderer_vulkan_device_destroy(renderer);
    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug_format("<renderer:%p> synchronization objects created", (void *)renderer);

  return renderer;
}

b8 renderer_destroy(renderer_t *renderer) {
  if (!renderer_vulkan_synchronization_destroy(renderer)) {
    logger_critical_format("<renderer:%p> synchronization objects destruction failed", (void *)renderer);

    return false;
  }

  logger_debug_format("<renderer:%p> synchronization objects destroyed", (void *)renderer);

  if (!renderer_vulkan_command_pools_destroy(renderer)) {
    logger_critical_format("<renderer:%p> VkCommandPools destruction failed", (void *)renderer);

    return false;
  }

  logger_debug_format("<renderer:%p> VkCommandPool destroyed", (void *)renderer);

  if (!renderer_vulkan_swapchain_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <swapchain:%p> VkSwapchainKHR destruction failed", (void *)renderer,
                           (void *)renderer->swapchain);

    return false;
  }

  logger_debug_format("<renderer:%p> <swapchain:%p> VkSwapchainKHR destroyed", (void *)renderer,
                      (void *)renderer->swapchain);

  if (!renderer_vulkan_device_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <device:%p> VkDevice destruction failed", (void *)renderer,
                           (void *)renderer->device);

    return false;
  }

  logger_debug_format("<renderer:%p> <device:%p> VkDevice destroyed", (void *)renderer, (void *)renderer->device);

  if (!renderer_vulkan_surface_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <surface:%p> VkSurfaceKHR destruction failed", (void *)renderer,
                           (void *)renderer->surface);

    return false;
  }

  logger_debug_format("<renderer:%p> <surface:%p> VkSurfaceKHR destroyed", (void *)renderer, (void *)renderer->surface);

  if (!renderer_vulkan_debug_messenger_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <messenger:%p> VkDebugUtilsMessengerEXT destruction failed", (void *)renderer,
                           (void *)renderer->messenger);

    return false;
  }

  logger_debug_format("<renderer:%p> <messenger:%p> VkDebugUtilsMessengerEXT destroyed", (void *)renderer,
                      (void *)renderer->messenger);

  if (!renderer_vulkan_instance_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <instance:%p> VkInstance destruction failed", (void *)renderer,
                           (void *)renderer->instance);

    return false;
  }

  logger_debug_format("<renderer:%p> <instance:%p> VkInstance destroyed", (void *)renderer, (void *)renderer->instance);

  platform_memory_deallocate(renderer);

  return true;
}

/* TODO: improve for multiple windows support */
b8 renderer_record_commands(renderer_t *renderer) {
  /* TODO: improve window retrieval for multiple windows support */
  platform_window_t *window = platform_window();
  platform_window_size_t window_size = platform_window_size(platform_window());

  if (window_size.width == 0 || window_size.height == 0) {
    return false;
  }

  VkResult acquire_next_image_result =
      vkAcquireNextImageKHR(renderer->device, renderer->swapchain, max_u64, renderer->present_complete_semaphore,
                            VK_NULL_HANDLE, &renderer->current_swapchain_image_index);

  if (acquire_next_image_result == VK_ERROR_OUT_OF_DATE_KHR) {
    logger_warning_format("<renderer:%p> <swapchain:%p> swapchain images are outdated", renderer, renderer->swapchain);

    renderer_vulkan_swapchain_update(renderer, window);

    return false;
  }

  if (acquire_next_image_result != VK_SUCCESS && acquire_next_image_result != VK_SUBOPTIMAL_KHR) {
    logger_critical_format("<renderer:%p> <swapchain:%p> swapchain image retrieval failed", renderer,
                           renderer->swapchain);

    return false;
  }

  VkCommandBufferBeginInfo command_buffer_begin_info = (VkCommandBufferBeginInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = (VkCommandBufferUsageFlags)0,
      .pInheritanceInfo = VK_NULL_HANDLE,
  };

  if (vkBeginCommandBuffer(renderer->compute_command_buffer, &command_buffer_begin_info) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> compute command buffer begin failed", renderer,
                           renderer->compute_command_buffer);

    return false;
  }

  if (vkBeginCommandBuffer(renderer->present_command_buffer, &command_buffer_begin_info) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> present command buffer begin failed", renderer,
                           renderer->present_command_buffer);

    return false;
  }

  if (vkBeginCommandBuffer(renderer->graphics_command_buffer, &command_buffer_begin_info) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> graphics command buffer begin failed", renderer,
                           renderer->graphics_command_buffer);

    return false;
  }

  if (vkBeginCommandBuffer(renderer->transfer_command_buffer, &command_buffer_begin_info) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> transfer command buffer begin failed", renderer,
                           renderer->transfer_command_buffer);

    return false;
  }

  swapchain_image_transition_info_t swapchain_image_transition_info = (swapchain_image_transition_info_t){
      .old_layout = VK_IMAGE_LAYOUT_UNDEFINED,
      .new_layout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
      .source_access_mask = (VkAccessFlags2)0,
      .destination_access_mask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
      .source_stage_mask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
      .destination_stage_mask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
      .command_buffer = renderer->graphics_command_buffer,
  };

  array_retrieve(renderer->swapchain_images, renderer->current_swapchain_image_index,
                 &swapchain_image_transition_info.image);

  if (!renderer_vulkan_swapchain_image_transition(swapchain_image_transition_info)) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> swapchain image transition failed", renderer,
                           renderer->graphics_command_buffer);

    return false;
  }

  VkClearColorValue clear_color_value = (VkClearColorValue){
      .float32[0] = 0.01f,
      .float32[1] = 0.01f,
      .float32[2] = 0.01f,
      .float32[3] = 1.0f,
  };

  VkRenderingAttachmentInfo rendering_attachment_info = (VkRenderingAttachmentInfo){
      .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
      .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .clearValue = (VkClearValue)clear_color_value,
  };

  array_retrieve(renderer->swapchain_image_views, renderer->current_swapchain_image_index,
                 &rendering_attachment_info.imageView);

  VkRenderingInfo rendering_info = (VkRenderingInfo){
      .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
      .renderArea =
          (VkRect2D){
              .offset = (VkOffset2D){.x = 0, .y = 0},
              .extent = renderer->swapchain_extent,
          },
      .layerCount = 1,
      .colorAttachmentCount = 1,
      .pColorAttachments = &rendering_attachment_info,
  };

  vkCmdBeginRendering(renderer->graphics_command_buffer, &rendering_info);

  VkViewport viewport = (VkViewport){
      .x = 0,
      .y = 0,
      .width = renderer->swapchain_extent.width,
      .height = renderer->swapchain_extent.height,
      .minDepth = 0.0f,
      .maxDepth = 1.0f,
  };

  VkRect2D scissor = (VkRect2D){
      .offset = (VkOffset2D){.x = 0, .y = 0},
      .extent = renderer->swapchain_extent,
  };

  vkCmdSetViewport(renderer->graphics_command_buffer, 0, 1, &viewport);
  vkCmdSetScissor(renderer->graphics_command_buffer, 0, 1, &scissor);

  return true;
}

/* TODO: improve for multiple windows support */
b8 renderer_submit_commands(renderer_t *renderer) {
  /* TODO: improve window retrieval for multiple windows support */
  platform_window_t *window = platform_window();
  platform_window_size_t window_size = platform_window_size(platform_window());

  if (vkResetFences(renderer->device, 1, &renderer->render_complete_fence) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> fences reset failed", renderer);

    return false;
  }

  vkCmdEndRendering(renderer->graphics_command_buffer);

  swapchain_image_transition_info_t swapchain_image_transition_info = (swapchain_image_transition_info_t){
      .old_layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      .new_layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
      .source_access_mask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
      .destination_access_mask = (VkAccessFlags2)0,
      .source_stage_mask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
      .destination_stage_mask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
      .command_buffer = renderer->graphics_command_buffer,
  };

  array_retrieve(renderer->swapchain_images, renderer->current_swapchain_image_index,
                 &swapchain_image_transition_info.image);

  if (!renderer_vulkan_swapchain_image_transition(swapchain_image_transition_info)) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> swapchain image transition failed", renderer,
                           renderer->graphics_command_buffer);

    return false;
  }

  if (vkEndCommandBuffer(renderer->compute_command_buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> compute command buffer end failed", renderer,
                           renderer->compute_command_buffer);

    return false;
  }

  if (vkEndCommandBuffer(renderer->present_command_buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> present command buffer end failed", renderer,
                           renderer->present_command_buffer);

    return false;
  }

  if (vkEndCommandBuffer(renderer->graphics_command_buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> graphics command buffer end failed", renderer,
                           renderer->graphics_command_buffer);

    return false;
  }

  if (vkEndCommandBuffer(renderer->transfer_command_buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> transfer command buffer end failed", renderer,
                           renderer->transfer_command_buffer);

    return false;
  }

  VkPipelineStageFlags pipeline_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

  VkSubmitInfo submit_info = (VkSubmitInfo){
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = &renderer->present_complete_semaphore,
      .pWaitDstStageMask = &pipeline_stage_flags,
      .commandBufferCount = 1,
      .pCommandBuffers = &renderer->graphics_command_buffer,
      .signalSemaphoreCount = 1,
      .pSignalSemaphores = &renderer->render_complete_semaphore,
  };

  /* TODO: submit all the command buffers to their queues */
  if (vkQueueSubmit(renderer->graphics_queue, 1, &submit_info, renderer->render_complete_fence) != VK_SUCCESS) {
    logger_warning_format("<renderer:%p> <swapchain:%p> swapchain images are outdated", renderer, renderer->swapchain);

    return false;
  }

  while (vkWaitForFences(renderer->device, 1, &renderer->render_complete_fence, VK_TRUE, max_u64) == VK_TIMEOUT)
    ;

  renderer_vulkan_command_buffer_reset(renderer->compute_command_buffer);
  renderer_vulkan_command_buffer_reset(renderer->present_command_buffer);
  renderer_vulkan_command_buffer_reset(renderer->graphics_command_buffer);
  renderer_vulkan_command_buffer_reset(renderer->transfer_command_buffer);

  VkPresentInfoKHR present_info = (VkPresentInfoKHR){
      .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = &renderer->render_complete_semaphore,
      .swapchainCount = 1,
      .pSwapchains = &renderer->swapchain,
      .pImageIndices = &renderer->current_swapchain_image_index,
      .pResults = NULL,
  };

  VkResult queue_present_result = vkQueuePresentKHR(renderer->present_queue, &present_info);

  if (queue_present_result == VK_ERROR_OUT_OF_DATE_KHR || queue_present_result == VK_SUBOPTIMAL_KHR) {
    if (window_size.width == 0 || window_size.height == 0) {
      return false;
    }

    renderer_vulkan_swapchain_update(renderer, window);

    return false;
  }

  return true;
}

b8 renderer_draw(renderer_t *renderer, u32 vertices, u32 instances) {
  vkCmdDraw(renderer->graphics_command_buffer, vertices, instances, 0, 0);

  return true;
}

renderer_implementation_t renderer_implementation(renderer_t *renderer) {
  return renderer->implementation;
}

const char *renderer_implementation_string(renderer_t *renderer) {
  return renderer->implementation_string;
}
