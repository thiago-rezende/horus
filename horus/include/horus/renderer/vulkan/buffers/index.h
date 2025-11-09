#pragma once

#include <vulkan/vulkan.h>

/* horus core layer */
#include <horus/core/types.h>

/* horus renderer buffers layer */
#include <horus/renderer/buffers/index.h>

struct __index_buffer {
  VkDevice device;

  VkBuffer buffer;
  VkBuffer staging;

  VkDeviceSize size;
  VkDeviceMemory memory;
  VkDeviceMemory staging_memory;
};
