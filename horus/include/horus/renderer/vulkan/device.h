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

b8 renderer_vulkan_physical_device_select(renderer_t *renderer);

physical_device_score_t renderer_vulkan_physical_device_get_score(VkPhysicalDevice device);
