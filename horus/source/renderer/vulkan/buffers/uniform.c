#include <vulkan/vulkan.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/time.h>
#include <horus/platform/memory.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/command.h>
#include <horus/renderer/vulkan/renderer.h>
#include <horus/renderer/vulkan/pipeline.h>
#include <horus/renderer/vulkan/descriptors.h>

/* horus renderer buffers layer [ vulkan ] */
#include <horus/renderer/buffers/uniform.h>

#define DEFAULT_UNIFORM_BUFFER_BINDING 0

struct __uniform_buffer {
  VkDevice device;

  VkDeviceSize size;

  array_t *buffers;
  array_t *memories;
  array_t *mappings;

  u8 *current_frame_in_flight_index;
};

/* TODO: refactor into multipe functions for usage within other types of buffer */
uniform_buffer_t *uniform_buffer_create(renderer_t *renderer, uniform_buffer_object_t *object) {
  uniform_buffer_t *buffer = platform_memory_allocate(sizeof(uniform_buffer_t));

  *buffer = (uniform_buffer_t){
      .size = sizeof(uniform_buffer_object_t),
      .device = renderer->device,
      .current_frame_in_flight_index = &renderer->current_frame_in_flight_index,
  };

  buffer->buffers = array_create(RENDERER_VULKAN_FRAMES_IN_FLIGHT, sizeof(VkBuffer));
  buffer->memories = array_create(RENDERER_VULKAN_FRAMES_IN_FLIGHT, sizeof(VkDeviceMemory));
  buffer->mappings = array_create(RENDERER_VULKAN_FRAMES_IN_FLIGHT, sizeof(void *));

  for (u8 i = 0; i < RENDERER_VULKAN_FRAMES_IN_FLIGHT; i++) {
    void *current_mapped;
    VkBuffer current_buffer;
    VkDeviceMemory current_memory;

    VkBufferCreateInfo buffer_create_info = (VkBufferCreateInfo){
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .flags = (VkBufferCreateFlags)0,
        .size = buffer->size,
        .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    if (vkCreateBuffer(buffer->device, &buffer_create_info, NULL, &current_buffer) != VK_SUCCESS) {
      logger_critical_format("<renderer:%p> buffer creation failed", renderer);

      platform_memory_deallocate(buffer);

      return NULL;
    }

    array_insert(buffer->buffers, (void *)&current_buffer);

    VkMemoryRequirements buffer_memory_requirements;
    VkPhysicalDeviceMemoryProperties physical_device_memory_properties;

    VkMemoryPropertyFlags buffer_memory_properties =
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    vkGetBufferMemoryRequirements(buffer->device, current_buffer, &buffer_memory_requirements);

    vkGetPhysicalDeviceMemoryProperties(renderer->physical_device, &physical_device_memory_properties);

    u32 buffer_memory_type_index = 0;

    b8 has_required_buffer_memory_type = false;

    for (u32 i = 0; i < physical_device_memory_properties.memoryTypeCount; i++) {
      if ((buffer_memory_requirements.memoryTypeBits & (1 << i)) &&
          (physical_device_memory_properties.memoryTypes[i].propertyFlags & buffer_memory_properties) ==
              buffer_memory_properties) {
        buffer_memory_type_index = i;

        has_required_buffer_memory_type = true;

        break;
      }
    }

    if (!has_required_buffer_memory_type) {
      logger_critical_format("<renderer:%p> <buffer:%p> desired memory type for buffers creation not found", renderer,
                             buffer);

      uniform_buffer_destroy(buffer);

      return NULL;
    }

    VkMemoryAllocateInfo buffer_memory_allocate_info = (VkMemoryAllocateInfo){
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = buffer_memory_requirements.size,
        .memoryTypeIndex = buffer_memory_type_index,
    };

    /* TODO: implement a memory allocator to allow more than 4096 allocations */
    if (vkAllocateMemory(buffer->device, &buffer_memory_allocate_info, NULL, &current_memory) != VK_SUCCESS) {
      logger_critical_format("<renderer:%p> <buffer:%p> buffer memory allocation failed", renderer, buffer);

      uniform_buffer_destroy(buffer);

      return NULL;
    }

    array_insert(buffer->memories, (void *)&current_memory);

    if (vkBindBufferMemory(buffer->device, current_buffer, current_memory, 0) != VK_SUCCESS) {
      logger_critical_format("<renderer:%p> <buffer:%p> staging buffer memory binding failed", renderer, buffer);

      uniform_buffer_destroy(buffer);

      return NULL;
    }

    if (vkMapMemory(buffer->device, current_memory, 0, buffer_create_info.size, 0, &current_mapped) != VK_SUCCESS) {
      logger_critical_format("<renderer:%p> <buffer:%p> buffer memory mapping failed", renderer, buffer);

      uniform_buffer_destroy(buffer);

      return NULL;
    }

    array_insert(buffer->mappings, (void *)&current_mapped);

    platform_memory_copy(current_mapped, object, sizeof(uniform_buffer_object_t));
  }

  return buffer;
}

b8 uniform_buffer_bind(uniform_buffer_t *buffer, graphics_pipeline_t *pipeline, renderer_t *renderer) {
  VkBuffer current_buffer;
  VkDescriptorSet descriptor_set;
  VkCommandBuffer graphics_command_buffer;

  array_retrieve(buffer->buffers, renderer->current_frame_in_flight_index, &current_buffer);
  array_retrieve(pipeline->descriptor_sets, renderer->current_frame_in_flight_index, &descriptor_set);
  array_retrieve(renderer->graphics_command_buffers, renderer->current_frame_in_flight_index, &graphics_command_buffer);

  renderer_vulkan_descriptor_set_update_buffer(renderer->device, descriptor_set, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                               DEFAULT_UNIFORM_BUFFER_BINDING, current_buffer, buffer->size);

  vkCmdBindDescriptorSets(graphics_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout, 0, 1,
                          &descriptor_set, 0, NULL);

  return true;
}

b8 uniform_buffer_update(uniform_buffer_t *buffer, uniform_buffer_object_t *object) {
  void *mapped = NULL;

  array_retrieve(buffer->mappings, *buffer->current_frame_in_flight_index, &mapped);

  platform_memory_copy(mapped, object, sizeof(uniform_buffer_object_t));

  return true;
}

b8 uniform_buffer_destroy(uniform_buffer_t *buffer) {
  vkDeviceWaitIdle(buffer->device);

  array_destroy(buffer->mappings);

  for (u8 i = 0; i < buffer->memories->count; i++) {
    VkDeviceMemory memory = VK_NULL_HANDLE;

    array_retrieve(buffer->memories, i, (void *)&memory);

    vkUnmapMemory(buffer->device, memory);

    vkFreeMemory(buffer->device, memory, NULL);
  }

  array_destroy(buffer->memories);

  for (u8 i = 0; i < buffer->buffers->count; i++) {
    VkBuffer current_buffer = VK_NULL_HANDLE;

    array_retrieve(buffer->buffers, i, (void *)&current_buffer);

    vkDestroyBuffer(buffer->device, current_buffer, NULL);
  }

  array_destroy(buffer->buffers);

  platform_memory_deallocate(buffer);

  return true;
}
