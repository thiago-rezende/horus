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
#include <horus/renderer/vulkan/descriptors.h>
#include <horus/renderer/vulkan/synchronization.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus logger layer */
#include <horus/logger/logger.h>

b8 renderer_context_create(renderer_t *renderer, renderer_context_create_info_t info, platform_window_t *window) {
  renderer->context = platform_memory_allocate(sizeof(renderer_context_t));

  *renderer->context = (renderer_context_t){
      .implementation = RENDERER_IMPLEMENTATION_VULKAN,
      .current_semaphore_index = 0,
      .current_frame_in_flight_index = 0,
  };

  if (!renderer_vulkan_instance_create(renderer, info)) {
    logger_critical_format("<renderer:%p> VkInstance creation failed", (void *)renderer);

    platform_memory_deallocate(renderer->context);

    return false;
  }

  logger_debug_format("<renderer:%p> <instance:%p> VkInstance created", (void *)renderer,
                      (void *)renderer->context->instance);

  if (!renderer_vulkan_debug_messenger_create(renderer)) {
    logger_critical_format("<renderer:%p> VkDebugUtilsMessengerEXT creation failed", (void *)renderer);

    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer->context);

    return false;
  }

  logger_debug_format("<renderer:%p> <messenger:%p> VkDebugUtilsMessengerEXT created", (void *)renderer,
                      (void *)renderer->context->messenger);

  if (!renderer_vulkan_surface_create(renderer, window)) {
    logger_critical_format("<renderer:%p> VkSurfaceKHR creation failed", (void *)renderer);

    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer->context);

    return false;
  }

  logger_debug_format("<renderer:%p> <surface:%p> VkSurfaceKHR created", (void *)renderer,
                      (void *)renderer->context->surface);

  if (!renderer_vulkan_physical_device_select(renderer)) {
    logger_critical_format("<renderer:%p> VkPhysicalDevice selection failed", (void *)renderer);

    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer->context);

    return false;
  }

  logger_debug_format("<renderer:%p> <physical_device:%p> VkPhysicalDevice selected", (void *)renderer,
                      (void *)renderer->context->physical_device);
  logger_debug_format("|- [ %u ] %s", renderer->context->physical_device_properties.deviceID,
                      renderer->context->physical_device_properties.deviceName);
  logger_debug_format("|- |- [ queues family indices ] <compute:%u> <present:%u> <graphics:%u> <transfer:%u>",
                      renderer->context->compute_queue_family_index, renderer->context->present_queue_family_index,
                      renderer->context->graphics_queue_family_index, renderer->context->transfer_queue_family_index);

  if (!renderer_vulkan_device_create(renderer)) {
    logger_critical_format("<renderer:%p> VkDevice creation failed", (void *)renderer);

    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer->context);

    return false;
  }

  logger_debug_format("<renderer:%p> <device:%p> VkDevice created", (void *)renderer,
                      (void *)renderer->context->device);

  if (!renderer_vulkan_command_pools_create(renderer)) {
    logger_critical_format("<renderer:%p> VkCommandPool creation failed", (void *)renderer);

    renderer_vulkan_swapchain_destroy(renderer);
    renderer_vulkan_device_destroy(renderer);
    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer->context);

    return false;
  }

  logger_debug_format("<renderer:%p> VkCommandPools created", (void *)renderer);
  logger_debug_format("|- [ pools ] <compute:%p> <present:%p> <graphics:%p> <transfer:%p>",
                      (void *)renderer->context->compute_command_pool, (void *)renderer->context->present_command_pool,
                      (void *)renderer->context->graphics_command_pool,
                      (void *)renderer->context->transfer_command_pool);

  if (!renderer_vulkan_swapchain_create(renderer, window)) {
    logger_critical_format("<renderer:%p> VkSwapchainKHR creation failed", (void *)renderer);

    renderer_vulkan_device_destroy(renderer);
    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer->context);

    return false;
  }

  logger_debug_format("<renderer:%p> <swapchain:%p> VkSwapchainKHR created", (void *)renderer,
                      (void *)renderer->context->swapchain);
  logger_debug_format("|- [ image count ] %lu", renderer->context->swapchain_images->count);

  if (!renderer_vulkan_synchronization_create(renderer)) {
    logger_critical_format("<renderer:%p> synchronization objects creation failed", (void *)renderer);

    renderer_vulkan_command_pools_destroy(renderer);
    renderer_vulkan_swapchain_destroy(renderer);
    renderer_vulkan_device_destroy(renderer);
    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer->context);

    return false;
  }

  logger_debug_format("<renderer:%p> synchronization objects created", (void *)renderer);

  if (!renderer_vulkan_descriptor_pool_create(renderer)) {
    renderer_vulkan_synchronization_destroy(renderer);
    renderer_vulkan_command_pools_destroy(renderer);
    renderer_vulkan_swapchain_destroy(renderer);
    renderer_vulkan_device_destroy(renderer);
    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer->context);

    return false;
  }

  logger_debug_format("<renderer:%p> <descriptor_pool:%p> VkDescriptorPool created", (void *)renderer,
                      (void *)renderer->context->descriptor_pool);

  return true;
}

b8 renderer_context_destroy(renderer_t *renderer) {
  if (!renderer_vulkan_descriptor_pool_destroy(renderer)) {
    logger_critical_format("<renderer:%p> VkDescriptorPool destruction failed", (void *)renderer);

    return false;
  }

  logger_debug_format("<renderer:%p> <descriptor_pool:%p> VkDescriptorPool destroyed", (void *)renderer,
                      (void *)renderer->context->descriptor_pool);

  if (!renderer_vulkan_synchronization_destroy(renderer)) {
    logger_critical_format("<renderer:%p> synchronization objects destruction failed", (void *)renderer);

    return false;
  }

  logger_debug_format("<renderer:%p> synchronization objects destroyed", (void *)renderer);

  if (!renderer_vulkan_swapchain_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <swapchain:%p> VkSwapchainKHR destruction failed", (void *)renderer,
                           (void *)renderer->context->swapchain);

    return false;
  }

  logger_debug_format("<renderer:%p> <swapchain:%p> VkSwapchainKHR destroyed", (void *)renderer,
                      (void *)renderer->context->swapchain);

  if (!renderer_vulkan_command_pools_destroy(renderer)) {
    logger_critical_format("<renderer:%p> VkCommandPools destruction failed", (void *)renderer);

    return false;
  }

  logger_debug_format("<renderer:%p> VkCommandPools destroyed", (void *)renderer);
  logger_debug_format("|- [ pools ] <compute:%p> <present:%p> <graphics:%p> <transfer:%p>",
                      (void *)renderer->context->compute_command_pool, (void *)renderer->context->present_command_pool,
                      (void *)renderer->context->graphics_command_pool,
                      (void *)renderer->context->transfer_command_pool);

  if (!renderer_vulkan_device_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <device:%p> VkDevice destruction failed", (void *)renderer,
                           (void *)renderer->context->device);

    return false;
  }

  logger_debug_format("<renderer:%p> <device:%p> VkDevice destroyed", (void *)renderer,
                      (void *)renderer->context->device);

  if (!renderer_vulkan_surface_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <surface:%p> VkSurfaceKHR destruction failed", (void *)renderer,
                           (void *)renderer->context->surface);

    return false;
  }

  logger_debug_format("<renderer:%p> <surface:%p> VkSurfaceKHR destroyed", (void *)renderer,
                      (void *)renderer->context->surface);

  if (!renderer_vulkan_debug_messenger_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <messenger:%p> VkDebugUtilsMessengerEXT destruction failed", (void *)renderer,
                           (void *)renderer->context->messenger);

    return false;
  }

  logger_debug_format("<renderer:%p> <messenger:%p> VkDebugUtilsMessengerEXT destroyed", (void *)renderer,
                      (void *)renderer->context->messenger);

  if (!renderer_vulkan_instance_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <instance:%p> VkInstance destruction failed", (void *)renderer,
                           (void *)renderer->context->instance);

    return false;
  }

  logger_debug_format("<renderer:%p> <instance:%p> VkInstance destroyed", (void *)renderer,
                      (void *)renderer->context->instance);

  platform_memory_deallocate(renderer->context);

  return true;
}

/* TODO: improve for multiple windows support */
b8 renderer_context_record_commands(renderer_t *renderer) {
  /* TODO: improve window retrieval for multiple windows support */
  platform_window_t *window = platform_window();
  platform_window_size_t window_size = platform_window_size(platform_window());

  if (window_size.width == 0 || window_size.height == 0) {
    return false;
  }

  VkFence render_complete_fence;
  VkSemaphore present_complete_semaphore;

  VkCommandBuffer compute_command_buffer;
  VkCommandBuffer present_command_buffer;
  VkCommandBuffer graphics_command_buffer;
  VkCommandBuffer transfer_command_buffer;

  array_retrieve(renderer->context->render_complete_fences, renderer->context->current_frame_in_flight_index,
                 &render_complete_fence);
  array_retrieve(renderer->context->present_complete_semaphores, renderer->context->current_semaphore_index,
                 &present_complete_semaphore);

  array_retrieve(renderer->context->compute_command_buffers, renderer->context->current_frame_in_flight_index,
                 &compute_command_buffer);
  array_retrieve(renderer->context->present_command_buffers, renderer->context->current_frame_in_flight_index,
                 &present_command_buffer);
  array_retrieve(renderer->context->graphics_command_buffers, renderer->context->current_frame_in_flight_index,
                 &graphics_command_buffer);
  array_retrieve(renderer->context->transfer_command_buffers, renderer->context->current_frame_in_flight_index,
                 &transfer_command_buffer);

  while (vkWaitForFences(renderer->context->device, 1, &render_complete_fence, VK_TRUE, max_u64) == VK_TIMEOUT)
    ;

  VkResult acquire_next_image_result = vkAcquireNextImageKHR(renderer->context->device, renderer->context->swapchain,
                                                             max_u64, present_complete_semaphore, VK_NULL_HANDLE,
                                                             &renderer->context->current_swapchain_image_index);

  if (acquire_next_image_result == VK_ERROR_OUT_OF_DATE_KHR) {
    logger_warning_format("<renderer:%p> <swapchain:%p> swapchain images are outdated", renderer,
                          renderer->context->swapchain);

    renderer_vulkan_swapchain_update(renderer, window);

    return false;
  }

  if (acquire_next_image_result != VK_SUCCESS && acquire_next_image_result != VK_SUBOPTIMAL_KHR) {
    logger_critical_format("<renderer:%p> <swapchain:%p> swapchain image retrieval failed", renderer,
                           renderer->context->swapchain);

    return false;
  }

  if (vkResetFences(renderer->context->device, 1, &render_complete_fence) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> fences reset failed", renderer);

    return false;
  }

  renderer_vulkan_command_buffer_reset(compute_command_buffer);
  renderer_vulkan_command_buffer_reset(present_command_buffer);
  renderer_vulkan_command_buffer_reset(graphics_command_buffer);
  renderer_vulkan_command_buffer_reset(transfer_command_buffer);

  VkCommandBufferBeginInfo command_buffer_begin_info = (VkCommandBufferBeginInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = (VkCommandBufferUsageFlags)0,
      .pInheritanceInfo = VK_NULL_HANDLE,
  };

  if (vkBeginCommandBuffer(compute_command_buffer, &command_buffer_begin_info) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> compute command buffer begin failed", renderer,
                           compute_command_buffer);

    return false;
  }

  if (vkBeginCommandBuffer(present_command_buffer, &command_buffer_begin_info) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> present command buffer begin failed", renderer,
                           present_command_buffer);

    return false;
  }

  if (vkBeginCommandBuffer(graphics_command_buffer, &command_buffer_begin_info) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> graphics command buffer begin failed", renderer,
                           graphics_command_buffer);

    return false;
  }

  if (vkBeginCommandBuffer(transfer_command_buffer, &command_buffer_begin_info) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> transfer command buffer begin failed", renderer,
                           transfer_command_buffer);

    return false;
  }

  swapchain_image_transition_info_t swapchain_image_transition_info = (swapchain_image_transition_info_t){
      .old_layout = VK_IMAGE_LAYOUT_UNDEFINED,
      .new_layout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
      .source_access_mask = (VkAccessFlags2)0,
      .destination_access_mask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
      .source_stage_mask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
      .destination_stage_mask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
      .command_buffer = graphics_command_buffer,
  };

  array_retrieve(renderer->context->swapchain_images, renderer->context->current_swapchain_image_index,
                 &swapchain_image_transition_info.image);

  if (!renderer_vulkan_swapchain_image_transition(swapchain_image_transition_info)) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> swapchain image transition failed", renderer,
                           graphics_command_buffer);

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
      .clearValue = (VkClearValue){clear_color_value},
  };

  VkRenderingAttachmentInfo depth_attachment_info = (VkRenderingAttachmentInfo){
      .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
      .imageView = renderer->context->depth_image_view,
      .imageLayout = renderer->context->depth_image_has_stencil_support
                         ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                         : VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .clearValue =
          (VkClearValue){
              .depthStencil =
                  {
                      .depth = 1.0f,
                      .stencil = 0,
                  },
          },
  };

  array_retrieve(renderer->context->swapchain_image_views, renderer->context->current_swapchain_image_index,
                 &rendering_attachment_info.imageView);

  VkRenderingInfo rendering_info = (VkRenderingInfo){
      .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
      .renderArea =
          (VkRect2D){
              .offset = (VkOffset2D){.x = 0, .y = 0},
              .extent = renderer->context->swapchain_extent,
          },
      .layerCount = 1,
      .colorAttachmentCount = 1,
      .pColorAttachments = &rendering_attachment_info,
      .pDepthAttachment = &depth_attachment_info,
      .pStencilAttachment = renderer->context->depth_image_has_stencil_support ? &depth_attachment_info : NULL,
  };

  vkCmdBeginRendering(graphics_command_buffer, &rendering_info);

  VkViewport viewport = (VkViewport){
      .x = 0,
      .y = 0,
      .width = renderer->context->swapchain_extent.width,
      .height = renderer->context->swapchain_extent.height,
      .minDepth = 0.0f,
      .maxDepth = 1.0f,
  };

  VkRect2D scissor = (VkRect2D){
      .offset = (VkOffset2D){.x = 0, .y = 0},
      .extent = renderer->context->swapchain_extent,
  };

  vkCmdSetViewport(graphics_command_buffer, 0, 1, &viewport);
  vkCmdSetScissor(graphics_command_buffer, 0, 1, &scissor);

  return true;
}

/* TODO: improve for multiple windows support */
b8 renderer_context_submit_commands(renderer_t *renderer) {
  VkFence render_complete_fence;
  VkFence transfer_complete_fence;
  VkSemaphore render_complete_semaphore;
  VkSemaphore present_complete_semaphore;

  VkCommandBuffer compute_command_buffer;
  VkCommandBuffer present_command_buffer;
  VkCommandBuffer graphics_command_buffer;
  VkCommandBuffer transfer_command_buffer;

  array_retrieve(renderer->context->render_complete_fences, renderer->context->current_frame_in_flight_index,
                 &render_complete_fence);
  array_retrieve(renderer->context->transfer_complete_fences, renderer->context->current_frame_in_flight_index,
                 &transfer_complete_fence);
  array_retrieve(renderer->context->present_complete_semaphores, renderer->context->current_semaphore_index,
                 &present_complete_semaphore);
  array_retrieve(renderer->context->render_complete_semaphores, renderer->context->current_swapchain_image_index,
                 &render_complete_semaphore);

  array_retrieve(renderer->context->compute_command_buffers, renderer->context->current_frame_in_flight_index,
                 &compute_command_buffer);
  array_retrieve(renderer->context->present_command_buffers, renderer->context->current_frame_in_flight_index,
                 &present_command_buffer);
  array_retrieve(renderer->context->graphics_command_buffers, renderer->context->current_frame_in_flight_index,
                 &graphics_command_buffer);
  array_retrieve(renderer->context->transfer_command_buffers, renderer->context->current_frame_in_flight_index,
                 &transfer_command_buffer);

  vkCmdEndRendering(graphics_command_buffer);

  swapchain_image_transition_info_t swapchain_image_transition_info = (swapchain_image_transition_info_t){
      .old_layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      .new_layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
      .source_access_mask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
      .destination_access_mask = (VkAccessFlags2)0,
      .source_stage_mask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
      .destination_stage_mask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
      .command_buffer = graphics_command_buffer,
  };

  array_retrieve(renderer->context->swapchain_images, renderer->context->current_swapchain_image_index,
                 &swapchain_image_transition_info.image);

  if (!renderer_vulkan_swapchain_image_transition(swapchain_image_transition_info)) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> swapchain image transition failed", renderer,
                           graphics_command_buffer);

    return false;
  }

  if (vkEndCommandBuffer(compute_command_buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> compute command buffer end failed", renderer,
                           compute_command_buffer);

    return false;
  }

  if (vkEndCommandBuffer(present_command_buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> present command buffer end failed", renderer,
                           present_command_buffer);

    return false;
  }

  if (vkEndCommandBuffer(graphics_command_buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> graphics command buffer end failed", renderer,
                           graphics_command_buffer);

    return false;
  }

  if (vkEndCommandBuffer(transfer_command_buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> transfer command buffer end failed", renderer,
                           transfer_command_buffer);

    return false;
  }

  /* TODO: submit all the command buffers to their queues */

  VkSubmitInfo transfer_queue_submit_info = (VkSubmitInfo){
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers = &transfer_command_buffer,
  };

  if (vkResetFences(renderer->context->device, 1, &transfer_complete_fence) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> fences reset failed", renderer);

    return false;
  }

  if (vkQueueSubmit(renderer->context->transfer_queue, 1, &transfer_queue_submit_info, transfer_complete_fence) !=
      VK_SUCCESS) {
    logger_warning_format("<renderer:%p> <swapchain:%p> swapchain images are outdated", renderer,
                          renderer->context->swapchain);

    return false;
  }

  while (vkWaitForFences(renderer->context->device, 1, &transfer_complete_fence, VK_TRUE, max_u64) == VK_TIMEOUT)
    ;

  VkPipelineStageFlags graphics_pipeline_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

  VkSubmitInfo graphics_queue_submit_info = (VkSubmitInfo){
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = &present_complete_semaphore,
      .pWaitDstStageMask = &graphics_pipeline_stage_flags,
      .commandBufferCount = 1,
      .pCommandBuffers = &graphics_command_buffer,
      .signalSemaphoreCount = 1,
      .pSignalSemaphores = &render_complete_semaphore,
  };

  if (vkQueueSubmit(renderer->context->graphics_queue, 1, &graphics_queue_submit_info, render_complete_fence) !=
      VK_SUCCESS) {
    logger_warning_format("<renderer:%p> <swapchain:%p> swapchain images are outdated", renderer,
                          renderer->context->swapchain);

    return false;
  }

  VkPresentInfoKHR present_info = (VkPresentInfoKHR){
      .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = &render_complete_semaphore,
      .swapchainCount = 1,
      .pSwapchains = &renderer->context->swapchain,
      .pImageIndices = &renderer->context->current_swapchain_image_index,
      .pResults = NULL,
  };

  VkResult queue_present_result = vkQueuePresentKHR(renderer->context->present_queue, &present_info);

  /* TODO: improve window retrieval for multiple windows support */
  platform_window_t *window = platform_window();
  platform_window_size_t window_size = platform_window_size(platform_window());

  if (queue_present_result == VK_ERROR_OUT_OF_DATE_KHR || queue_present_result == VK_SUBOPTIMAL_KHR) {
    if (window_size.width == 0 || window_size.height == 0) {
      return false;
    }

    renderer_vulkan_swapchain_update(renderer, window);

    return false;
  }

  renderer->context->current_semaphore_index =
      (renderer->context->current_semaphore_index + 1) % renderer->context->present_complete_semaphores->count;

  renderer->context->current_frame_in_flight_index =
      (renderer->context->current_frame_in_flight_index + 1) % RENDERER_VULKAN_FRAMES_IN_FLIGHT;

  return true;
}

/* TODO: improve for multiple window support */
b8 renderer_context_draw(renderer_t *renderer, u32 vertices, u32 instances) {
  VkCommandBuffer graphics_command_buffer;

  array_retrieve(renderer->context->graphics_command_buffers, renderer->context->current_frame_in_flight_index,
                 &graphics_command_buffer);

  vkCmdDraw(graphics_command_buffer, vertices, instances, 0, 0);

  return true;
}

/* TODO: improve for multiple window support */
b8 renderer_context_draw_indexed(renderer_t *renderer, u32 indices, u32 instances) {
  VkCommandBuffer graphics_command_buffer;

  array_retrieve(renderer->context->graphics_command_buffers, renderer->context->current_frame_in_flight_index,
                 &graphics_command_buffer);

  vkCmdDrawIndexed(graphics_command_buffer, indices, instances, 0, 0, 0);

  return true;
}

renderer_implementation_t renderer_context_implementation(renderer_t *renderer) {
  return renderer->context->implementation;
}
