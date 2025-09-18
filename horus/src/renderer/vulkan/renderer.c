#include <vulkan/vulkan.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>

/* horus vulkan renderer layer */
#include <horus/renderer/vulkan/loader.h>
#include <horus/renderer/vulkan/renderer.h>

/* horus vulkan renderer layer */
#include <horus/renderer/vulkan/platform.h>

/* horus base layer */
#include <horus/definitions.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus containers layer */
#include <horus/containers/array.h>

const static logger_level_t
    vulkan_debug_message_severity_to_logger_level[VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT] = {
        [VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT] = LOGGER_LEVEL_INFO,
        [VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT] = LOGGER_LEVEL_TRACE,
        [VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT] = LOGGER_LEVEL_WARNING,
        [VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT] = LOGGER_LEVEL_ERROR,
};

VKAPI_ATTR VkBool32 VKAPI_CALL renderer_vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                              VkDebugUtilsMessageTypeFlagsEXT type,
                                                              const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                                                              void *user_data) {
  renderer_t *renderer = (renderer_t *)user_data;

  logger_level_t level = vulkan_debug_message_severity_to_logger_level[severity];

  __logger_general(level, "<renderer:%p> <implementation:%s> %s", renderer, renderer->implementation_string,
                   callback_data->pMessage);

  return VK_FALSE;
}

renderer_t *renderer_create(application_t *application, platform_window_t *window) {
  renderer_t *renderer = platform_memory_allocate(sizeof(renderer_t));

  *renderer = (renderer_t){
      .implementation = RENDERER_IMPLEMENTATION_VULKAN,
      .implementation_string = __renderer_implementation_string(RENDERER_IMPLEMENTATION_VULKAN),
  };

  if (!renderer_vulkan_create_instance(renderer, application)) {
    logger_critical("<renderer:%p> <implementation:%s> VkInstance creation failed", renderer,
                    renderer->implementation_string);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug("<renderer:%p> <implementation:%s> <instance:%p> VkInstance created", renderer,
               renderer->implementation_string, renderer->instance);

  if (!renderer_vulkan_create_debug_messenger(renderer)) {
    logger_critical("<renderer:%p> <implementation:%s> VkDebugUtilsMessengerEXT creation failed", renderer,
                    renderer->implementation_string);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug("<renderer:%p> <implementation:%s> <instance:%p> VkDebugUtilsMessengerEXT created", renderer,
               renderer->implementation_string, renderer->instance);

  return renderer;
}

b8 renderer_destroy(renderer_t *renderer) {
  if (!renderer_vulkan_destroy_instance(renderer)) {
    logger_critical("<renderer:%p> <implementation:%s> <instance:%p> VkInstance destruction failed", renderer,
                    renderer->implementation_string, renderer->instance);
  }

  logger_debug("<renderer:%p> <implementation:%s> <instance:%p> VkInstance destroyed", renderer,
               renderer->implementation_string, renderer->instance);

  if (!renderer_vulkan_destroy_debug_messenger(renderer)) {
    logger_critical("<renderer:%p> <implementation:%s> <instance:%p> VkDebugUtilsMessengerEXT destruction failed",
                    renderer, renderer->implementation_string, renderer->instance);
  }

  logger_debug("<renderer:%p> <implementation:%s> <instance:%p> VkDebugUtilsMessengerEXT destroyed", renderer,
               renderer->implementation_string, renderer->instance);

  platform_memory_deallocate(renderer);

  return true;
}

renderer_implementation_t renderer_implementation(renderer_t *renderer) {
  return renderer->implementation;
}

const char *renderer_implementation_string(renderer_t *renderer) {
  return renderer->implementation_string;
}

b8 renderer_vulkan_create_instance(renderer_t *renderer, application_t *application) {
  VkApplicationInfo application_info = (VkApplicationInfo){
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = application->name,
      .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
      .pEngineName = horus_project_name(),
      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
      .apiVersion = VK_API_VERSION_1_0,
  };

  array_t *layers = renderer_vulkan_get_instance_required_layers();
  array_t *extensions = renderer_vulkan_get_instance_required_extensions();

  logger_debug("<renderer:%p> <implementation:%s> <count:%lu> required instance layers", renderer,
               renderer->implementation_string, layers->count);

  for (u64 i = 0; i < layers->count; i++) {
    char *name;

    array_retrieve(layers, i, (void *)&name);

    logger_debug("| [ %s ]", name);
  }

  logger_debug("<renderer:%p> <implementation:%s> <count:%lu> required instance extensions", renderer,
               renderer->implementation_string, extensions->count);

  for (u64 i = 0; i < extensions->count; i++) {
    char *name;

    array_retrieve(extensions, i, (void *)&name);

    logger_debug("| [ %s ]", name);
  }

  VkInstanceCreateInfo instance_create_info = (VkInstanceCreateInfo){
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &application_info,
      .enabledLayerCount = layers->count,
      .ppEnabledLayerNames = layers->buffer,
      .enabledExtensionCount = extensions->count,
      .ppEnabledExtensionNames = extensions->buffer,
  };

  VkResult result = vkCreateInstance(&instance_create_info, NULL, &renderer->instance);

  array_destroy(layers);
  array_destroy(extensions);

  if (result != VK_SUCCESS) {
    return false;
  }

  return true;
}

b8 renderer_vulkan_destroy_instance(renderer_t *renderer) {
  vkDestroyInstance(renderer->instance, NULL);

  return true;
}

b8 renderer_vulkan_create_debug_messenger(renderer_t *renderer) {
  VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info = (VkDebugUtilsMessengerCreateInfoEXT){
      .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
      .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
      .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
      .pfnUserCallback = renderer_vulkan_debug_callback,
      .pUserData = renderer};

  VkResult result =
      vkCreateDebugUtilsMessengerEXT(renderer->instance, &debug_messenger_create_info, NULL, &renderer->messenger);

  if (result != VK_SUCCESS) {
    return false;
  }

  return true;
}

b8 renderer_vulkan_destroy_debug_messenger(renderer_t *renderer) {
  vkDestroyDebugUtilsMessengerEXT(renderer->instance, renderer->messenger, NULL);

  return true;
}
