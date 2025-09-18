#pragma once

#include <vulkan/vulkan.h>

/* horus application layer */
#include <horus/application/application.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>

struct __renderer {
  renderer_implementation_t implementation;
  const char *implementation_string;

  VkInstance instance;

  VkDebugUtilsMessengerEXT messenger;
};

b8 renderer_vulkan_create_instance(renderer_t *renderer, application_t *application);
b8 renderer_vulkan_destroy_instance(renderer_t *renderer);

b8 renderer_vulkan_create_debug_messenger(renderer_t *renderer);
b8 renderer_vulkan_destroy_debug_messenger(renderer_t *renderer);

VKAPI_ATTR VkBool32 VKAPI_CALL renderer_vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                              VkDebugUtilsMessageTypeFlagsEXT type,
                                                              const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                                                              void *user_data);
