#pragma once

#include <vulkan/vulkan.h>

/* horus application layer */
#include <horus/application/application.h>

/* horus containers layer */
#include <horus/containers/array.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>

struct __renderer {
  renderer_implementation_t implementation;
  const char *implementation_string;

  VkInstance instance;

  VkPhysicalDevice physical_device;
  VkPhysicalDeviceFeatures physical_device_features;
  VkPhysicalDeviceProperties physical_device_properties;

  VkDevice device;

  VkQueue compute_queue;
  VkQueue present_queue;
  VkQueue graphics_queue;
  VkQueue transfer_queue;

  u32 compute_queue_family_index;
  u32 present_queue_family_index;
  u32 graphics_queue_family_index;
  u32 transfer_queue_family_index;

  VkSurfaceKHR surface;

  VkPresentModeKHR surface_present_mode;
  VkSurfaceFormatKHR surface_format;
  VkSurfaceCapabilitiesKHR surface_capabilities;

  VkSwapchainKHR swapchain;

  VkExtent2D swapchain_extent;

  array_t *swapchain_images;
  array_t *swapchain_image_views;

  VkCommandPool compute_command_pool;
  VkCommandPool present_command_pool;
  VkCommandPool graphics_command_pool;
  VkCommandPool transfer_command_pool;

  VkCommandBuffer compute_command_buffer;
  VkCommandBuffer present_command_buffer;
  VkCommandBuffer graphics_command_buffer;
  VkCommandBuffer transfer_command_buffer;

  VkFence render_complete_fence;

  VkSemaphore render_complete_semaphore;
  VkSemaphore present_complete_semaphore;

  VkDebugUtilsMessengerEXT messenger;
};
