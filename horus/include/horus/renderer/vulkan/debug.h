#pragma once

/* horus vulkan renderer layer */
#include <horus/renderer/vulkan/renderer.h>

b8 renderer_vulkan_debug_messenger_create(renderer_t *renderer);
b8 renderer_vulkan_debug_messenger_destroy(renderer_t *renderer);

VKAPI_ATTR VkBool32 VKAPI_CALL renderer_vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                              VkDebugUtilsMessageTypeFlagsEXT type,
                                                              const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                                                              void *user_data);
