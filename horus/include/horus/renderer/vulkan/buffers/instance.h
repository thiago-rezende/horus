#pragma once

#include <vulkan/vulkan.h>

/* horus renderer buffers layer */
#include <horus/renderer/buffers/instance.h>

/* TODO: update to use staging buffers enabling high speed memory */
struct __instance_buffer {
  VkDevice device;

  VkDeviceSize size;

  array_t *buffers;
  array_t *memories;
  array_t *mappings;

  u8 *current_frame_in_flight_index;
};
