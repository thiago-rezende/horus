#pragma once

#include <vulkan/vulkan.h>

/* horus renderer layer */
#include <horus/renderer/buffers/uniform.h>

struct __uniform_buffer {
  VkDevice device;

  VkDeviceSize size;

  array_t *buffers;
  array_t *memories;
  array_t *mappings;

  u8 *current_frame_in_flight_index;
};
