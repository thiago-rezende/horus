#pragma once

#include <vulkan/vulkan.h>

/* horus renderer layer */
#include <horus/renderer/buffers/vertex.h>

struct __vertex_buffer {
  VkDevice device;

  VkBuffer buffer;
  VkBuffer staging;

  VkDeviceSize size;
  VkDeviceMemory memory;
  VkDeviceMemory staging_memory;
};
