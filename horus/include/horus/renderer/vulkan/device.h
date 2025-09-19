#pragma once

/* horus vulkan renderer layer */
#include <horus/renderer/vulkan/renderer.h>

typedef struct __physical_device_score {
  u64 score;

  VkPhysicalDevice device;
  VkPhysicalDeviceFeatures features;
  VkPhysicalDeviceProperties properties;

  char *device_type_string;
} physical_device_score_t;

typedef struct __queue_family_indices {
  u32 compute_family_index;
  u32 graphics_family_index;
  u32 transfer_family_index;

  b8 has_compute_family_index;
  b8 has_graphics_family_index;
  b8 has_transfer_family_index;
} queue_family_indices_t;

b8 renderer_vulkan_physical_device_select(renderer_t *renderer);

physical_device_score_t renderer_vulkan_physical_device_get_score(VkPhysicalDevice device);
queue_family_indices_t renderer_vulkan_physical_device_get_queue_family_indices(VkPhysicalDevice device);
