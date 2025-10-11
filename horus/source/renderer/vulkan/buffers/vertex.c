#include <vulkan/vulkan.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>
#include <horus/renderer/vulkan/pipeline.h>

/* horus renderer buffers layer [ vulkan ] */
#include <horus/renderer/buffers/vertex.h>

struct __vertex_buffer {
  VkDevice device;

  VkBuffer buffer;

  VkDeviceSize size;
  VkDeviceMemory memory;
};

vertex_buffer_t *vertex_buffer_create(renderer_t *renderer, vertex_t *vertices, u64 count) {
  vertex_buffer_t *buffer = platform_memory_allocate(sizeof(vertex_buffer_t));

  *buffer = (vertex_buffer_t){
      .device = renderer->device,
      .size = sizeof(vertex_t) * count,
  };

  VkBufferCreateInfo buffer_create_info = (VkBufferCreateInfo){
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .flags = (VkBufferCreateFlags)0,
      .size = buffer->size,
      .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
  };

  if (vkCreateBuffer(renderer->device, &buffer_create_info, NULL, &buffer->buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> buffer creation failed", renderer);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  VkMemoryRequirements memory_requirements;
  VkPhysicalDeviceMemoryProperties physical_device_memory_properties;

  VkMemoryPropertyFlags memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

  vkGetBufferMemoryRequirements(buffer->device, buffer->buffer, &memory_requirements);

  vkGetPhysicalDeviceMemoryProperties(renderer->physical_device, &physical_device_memory_properties);

  u32 memory_type_index = 0;
  b8 has_required_memory_type = false;

  for (u32 i = 0; i < physical_device_memory_properties.memoryTypeCount; i++) {
    if ((memory_requirements.memoryTypeBits & (1 << i)) &&
        (physical_device_memory_properties.memoryTypes[i].propertyFlags & memory_properties) == memory_properties) {
      memory_type_index = i;

      has_required_memory_type = true;

      break;
    }
  }

  if (!has_required_memory_type) {
    logger_critical_format("<renderer:%p> <buffer:%p> desired memory type for buffer creation not found", renderer,
                           buffer);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  VkMemoryAllocateInfo memory_allocate_info = (VkMemoryAllocateInfo){
      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .allocationSize = memory_requirements.size,
      .memoryTypeIndex = memory_type_index,
  };

  if (vkAllocateMemory(buffer->device, &memory_allocate_info, NULL, &buffer->memory) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <buffer:%p> buffer memory allocation failed", renderer, buffer);

    vkDestroyBuffer(buffer->device, buffer->buffer, NULL);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  if (vkBindBufferMemory(buffer->device, buffer->buffer, buffer->memory, 0) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <buffer:%p> buffer memory binding failed", renderer, buffer);

    vkDestroyBuffer(buffer->device, buffer->buffer, NULL);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  void *mapped = NULL;

  if (vkMapMemory(buffer->device, buffer->memory, 0, buffer_create_info.size, 0, &mapped) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <buffer:%p> buffer memory mapping failed", renderer, buffer);

    vkDestroyBuffer(buffer->device, buffer->buffer, NULL);

    platform_memory_deallocate(buffer);
  }

  platform_memory_copy(mapped, vertices, (u64)buffer->size);

  vkUnmapMemory(buffer->device, buffer->memory);

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
