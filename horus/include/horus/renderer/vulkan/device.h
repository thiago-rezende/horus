#pragma once

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>

/* horus containers layer */
#include <horus/containers/array.h>

typedef struct __queue_family_indices {
  u32 compute_family_index;
  u32 present_family_index;
  u32 graphics_family_index;
  u32 transfer_family_index;

  b8 has_compute_family_index;
  b8 has_present_family_index;
  b8 has_graphics_family_index;
  b8 has_transfer_family_index;
} queue_family_indices_t;

typedef struct __surface_information {
  VkPresentModeKHR present_mode;
  VkSurfaceFormatKHR format;
  VkSurfaceCapabilitiesKHR capabilities;

  b8 has_desired_format;
  b8 has_desired_capabilities;
  b8 has_desired_present_mode;
} surface_information_t;

typedef struct __physical_device_score {
  u64 score;

  VkPhysicalDevice device;
  VkPhysicalDeviceFeatures features;
  VkPhysicalDeviceProperties properties;

  queue_family_indices_t queues;

  surface_information_t surface_information;

  char *device_type_string;
} physical_device_score_t;

b8 renderer_vulkan_physical_device_select(renderer_t *renderer);

physical_device_score_t renderer_vulkan_physical_device_get_score(VkPhysicalDevice device,
                                                                  VkSurfaceKHR surface,
                                                                  array_t *extensions);

queue_family_indices_t renderer_vulkan_physical_device_get_queue_family_indices(VkPhysicalDevice device,
                                                                                VkSurfaceKHR surface);

surface_information_t renderer_vulkan_physical_device_get_surface_information(VkPhysicalDevice device,
                                                                              VkSurfaceKHR surface);

b8 renderer_vulkan_device_create(renderer_t *renderer);
b8 renderer_vulkan_device_destroy(renderer_t *renderer);
