/* horus vulkan renderer layer */
#include <horus/renderer/vulkan/debug.h>

/* horus logger layer */
#include <horus/logger/logger.h>

VKAPI_ATTR VkBool32 VKAPI_CALL renderer_vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                              VkDebugUtilsMessageTypeFlagsEXT type,
                                                              const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                                                              void *user_data) {
  renderer_t *renderer = (renderer_t *)user_data;

  logger_level_t level = LOGGER_LEVEL_TRACE;

  switch (severity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
      level = LOGGER_LEVEL_TRACE;

      break;
    }

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
      level = LOGGER_LEVEL_INFO;

      break;
    }

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
      level = LOGGER_LEVEL_WARNING;

      break;
    }

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
      level = LOGGER_LEVEL_ERROR;

      break;
    }

    default: {
      level = LOGGER_LEVEL_INFO;
    }
  }

  __logger_general(level, "<renderer:%p> <implementation:%s> %s", renderer, renderer->implementation_string,
                   callback_data->pMessage);

  return VK_FALSE;
}

b8 renderer_vulkan_debug_messenger_create(renderer_t *renderer) {
  VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info = (VkDebugUtilsMessengerCreateInfoEXT){
      .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
      .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
      .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
      .pfnUserCallback = renderer_vulkan_debug_callback,
      .pUserData = renderer,
  };

  VkResult result =
      vkCreateDebugUtilsMessengerEXT(renderer->instance, &debug_messenger_create_info, NULL, &renderer->messenger);

  if (result != VK_SUCCESS) {
    return false;
  }

  return true;
}

b8 renderer_vulkan_debug_messenger_destroy(renderer_t *renderer) {
  vkDestroyDebugUtilsMessengerEXT(renderer->instance, renderer->messenger, NULL);

  return true;
}
