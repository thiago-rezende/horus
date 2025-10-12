#include <vulkan/vulkan.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/command.h>
#include <horus/renderer/vulkan/renderer.h>
#include <horus/renderer/vulkan/pipeline.h>

/* horus renderer buffers layer [ vulkan ] */
#include <horus/renderer/buffers/vertex.h>

struct __vertex_buffer {
  VkDevice device;

  VkBuffer buffer;
  VkBuffer staging;

  VkDeviceSize size;
  VkDeviceMemory memory;
  VkDeviceMemory staging_memory;
};

vertex_buffer_t *vertex_buffer_create(renderer_t *renderer, vertex_t *vertices, u64 count) {
  vertex_buffer_t *buffer = platform_memory_allocate(sizeof(vertex_buffer_t));

  *buffer = (vertex_buffer_t){
      .size = sizeof(vertex_t) * count,
      .device = renderer->device,
  };

  VkBufferCreateInfo buffer_create_info = (VkBufferCreateInfo){
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .flags = (VkBufferCreateFlags)0,
      .size = buffer->size,
      .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
  };

  VkBufferCreateInfo staging_create_info = (VkBufferCreateInfo){
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .flags = (VkBufferCreateFlags)0,
      .size = buffer->size,
      .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
  };

  if (renderer->graphics_queue_family_index != renderer->transfer_queue_family_index) {
    buffer_create_info.sharingMode = VK_SHARING_MODE_CONCURRENT;
    staging_create_info.sharingMode = VK_SHARING_MODE_CONCURRENT;
  }

  if (vkCreateBuffer(renderer->device, &buffer_create_info, NULL, &buffer->buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> buffer creation failed", renderer);

    vkDestroyBuffer(buffer->device, buffer->staging, NULL);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  if (vkCreateBuffer(renderer->device, &staging_create_info, NULL, &buffer->staging) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> staging buffer creation failed", renderer);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  VkMemoryRequirements buffer_memory_requirements;
  VkMemoryRequirements staging_memory_requirements;
  VkPhysicalDeviceMemoryProperties physical_device_memory_properties;

  VkMemoryPropertyFlags buffer_memory_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  VkMemoryPropertyFlags staging_memory_properties =
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

  vkGetBufferMemoryRequirements(buffer->device, buffer->buffer, &buffer_memory_requirements);
  vkGetBufferMemoryRequirements(buffer->device, buffer->staging, &staging_memory_requirements);

  vkGetPhysicalDeviceMemoryProperties(renderer->physical_device, &physical_device_memory_properties);

  u32 buffer_memory_type_index = 0;
  u32 staging_memory_type_index = 0;

  b8 has_required_buffer_memory_type = false;
  b8 has_required_staging_memory_type = false;

  for (u32 i = 0; i < physical_device_memory_properties.memoryTypeCount; i++) {
    if ((buffer_memory_requirements.memoryTypeBits & (1 << i)) &&
        (physical_device_memory_properties.memoryTypes[i].propertyFlags & buffer_memory_properties) ==
            buffer_memory_properties) {
      buffer_memory_type_index = i;

      has_required_buffer_memory_type = true;

      break;
    }
  }

  for (u32 i = 0; i < physical_device_memory_properties.memoryTypeCount; i++) {
    if ((staging_memory_requirements.memoryTypeBits & (1 << i)) &&
        (physical_device_memory_properties.memoryTypes[i].propertyFlags & staging_memory_properties) ==
            staging_memory_properties) {
      staging_memory_type_index = i;

      has_required_staging_memory_type = true;

      break;
    }
  }

  if (!has_required_buffer_memory_type || !has_required_staging_memory_type) {
    logger_critical_format("<renderer:%p> <buffer:%p> desired memory type for buffers creation not found", renderer,
                           buffer);

    vkDestroyBuffer(buffer->device, buffer->buffer, NULL);
    vkDestroyBuffer(buffer->device, buffer->staging, NULL);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  VkMemoryAllocateInfo buffer_memory_allocate_info = (VkMemoryAllocateInfo){
      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .allocationSize = buffer_memory_requirements.size,
      .memoryTypeIndex = buffer_memory_type_index,
  };

  VkMemoryAllocateInfo staging_memory_allocate_info = (VkMemoryAllocateInfo){
      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .allocationSize = staging_memory_requirements.size,
      .memoryTypeIndex = staging_memory_type_index,
  };

  /* TODO: implement a memory allocator to allow more than 4096 allocations */
  if (vkAllocateMemory(buffer->device, &buffer_memory_allocate_info, NULL, &buffer->memory) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <buffer:%p> buffer memory allocation failed", renderer, buffer);

    vkDestroyBuffer(buffer->device, buffer->buffer, NULL);
    vkDestroyBuffer(buffer->device, buffer->staging, NULL);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  /* TODO: implement a memory allocator to allow more than 4096 allocations */
  if (vkAllocateMemory(buffer->device, &staging_memory_allocate_info, NULL, &buffer->staging_memory) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <buffer:%p> staging buffer memory allocation failed", renderer, buffer);

    vkFreeMemory(buffer->device, buffer->memory, NULL);

    vkDestroyBuffer(buffer->device, buffer->buffer, NULL);
    vkDestroyBuffer(buffer->device, buffer->staging, NULL);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  if (vkBindBufferMemory(buffer->device, buffer->buffer, buffer->memory, 0) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <buffer:%p> staging buffer memory binding failed", renderer, buffer);

    vkFreeMemory(buffer->device, buffer->memory, NULL);
    vkFreeMemory(buffer->device, buffer->staging_memory, NULL);

    vkDestroyBuffer(buffer->device, buffer->buffer, NULL);
    vkDestroyBuffer(buffer->device, buffer->staging, NULL);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  if (vkBindBufferMemory(buffer->device, buffer->staging, buffer->staging_memory, 0) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <buffer:%p> staging buffer memory binding failed", renderer, buffer);

    vkFreeMemory(buffer->device, buffer->memory, NULL);
    vkFreeMemory(buffer->device, buffer->staging_memory, NULL);

    vkDestroyBuffer(buffer->device, buffer->buffer, NULL);
    vkDestroyBuffer(buffer->device, buffer->staging, NULL);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  void *mapped = NULL;

  if (vkMapMemory(buffer->device, buffer->staging_memory, 0, staging_create_info.size, 0, &mapped) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <buffer:%p> buffer memory mapping failed", renderer, buffer);

    vkFreeMemory(buffer->device, buffer->memory, NULL);
    vkFreeMemory(buffer->device, buffer->staging_memory, NULL);

    vkDestroyBuffer(buffer->device, buffer->buffer, NULL);
    vkDestroyBuffer(buffer->device, buffer->staging, NULL);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  platform_memory_copy(mapped, vertices, (u64)buffer->size);

  vkUnmapMemory(buffer->device, buffer->staging_memory);

  VkCommandBuffer transfer_command_buffer =
      renderer_vulkan_command_buffer_create(buffer->device, renderer->transfer_command_pool);

  VkCommandBufferBeginInfo transfer_command_buffer_begin_info = (VkCommandBufferBeginInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
  };

  if (vkBeginCommandBuffer(transfer_command_buffer, &transfer_command_buffer_begin_info) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <buffer:%p> transfer command buffer beginning failed", renderer, buffer);

    renderer_vulkan_command_buffer_destroy(buffer->device, transfer_command_buffer, renderer->transfer_command_pool);

    vkFreeMemory(buffer->device, buffer->memory, NULL);
    vkFreeMemory(buffer->device, buffer->staging_memory, NULL);

    vkDestroyBuffer(buffer->device, buffer->buffer, NULL);
    vkDestroyBuffer(buffer->device, buffer->staging, NULL);

    return NULL;
  }

  VkBufferCopy buffer_copy_region = (VkBufferCopy){
      .srcOffset = 0,
      .dstOffset = 0,
      .size = buffer->size,
  };

  vkCmdCopyBuffer(transfer_command_buffer, buffer->staging, buffer->buffer, 1, &buffer_copy_region);

  if (vkEndCommandBuffer(transfer_command_buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <buffer:%p> transfer command buffer ending failed", renderer, buffer);

    renderer_vulkan_command_buffer_destroy(buffer->device, transfer_command_buffer, renderer->transfer_command_pool);

    vkFreeMemory(buffer->device, buffer->memory, NULL);
    vkFreeMemory(buffer->device, buffer->staging_memory, NULL);

    vkDestroyBuffer(buffer->device, buffer->buffer, NULL);
    vkDestroyBuffer(buffer->device, buffer->staging, NULL);

    return NULL;
  }

  VkSubmitInfo transfer_submit_info = (VkSubmitInfo){
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers = &transfer_command_buffer,
  };

  if (vkQueueSubmit(renderer->transfer_queue, 1, &transfer_submit_info, VK_NULL_HANDLE) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <buffer:%p> transfer command buffer ending failed", renderer, buffer);

    renderer_vulkan_command_buffer_destroy(buffer->device, transfer_command_buffer, renderer->transfer_command_pool);

    vkFreeMemory(buffer->device, buffer->memory, NULL);
    vkFreeMemory(buffer->device, buffer->staging_memory, NULL);

    vkDestroyBuffer(buffer->device, buffer->buffer, NULL);
    vkDestroyBuffer(buffer->device, buffer->staging, NULL);

    return NULL;
  }

  vkQueueWaitIdle(renderer->transfer_queue);

  renderer_vulkan_command_buffer_destroy(buffer->device, transfer_command_buffer, renderer->transfer_command_pool);

  vkFreeMemory(buffer->device, buffer->staging_memory, NULL);

  vkDestroyBuffer(buffer->device, buffer->staging, NULL);

  return buffer;
}

b8 vertex_buffer_bind(vertex_buffer_t *buffer, graphics_pipeline_t *pipeline, renderer_t *renderer) {
  VkCommandBuffer graphics_command_buffer;

  array_retrieve(renderer->graphics_command_buffers, renderer->current_frame_in_flight_index, &graphics_command_buffer);

  VkDeviceSize offsets[] = {0};

  vkCmdBindVertexBuffers(graphics_command_buffer, 0, 1, &buffer->buffer, offsets);

  return true;
}

b8 vertex_buffer_destroy(vertex_buffer_t *buffer) {
  vkDeviceWaitIdle(buffer->device);

  vkFreeMemory(buffer->device, buffer->memory, NULL);

  vkDestroyBuffer(buffer->device, buffer->buffer, NULL);

  platform_memory_deallocate(buffer);

  return true;
}
