#include <vulkan/vulkan.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>

/* horus vulkan renderer layer */
#include <horus/renderer/vulkan/debug.h>
#include <horus/renderer/vulkan/device.h>
#include <horus/renderer/vulkan/loader.h>
#include <horus/renderer/vulkan/instance.h>
#include <horus/renderer/vulkan/renderer.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus logger layer */
#include <horus/logger/logger.h>

renderer_t *renderer_create(application_t *application, platform_window_t *window) {
  renderer_t *renderer = platform_memory_allocate(sizeof(renderer_t));

  *renderer = (renderer_t){
      .implementation = RENDERER_IMPLEMENTATION_VULKAN,
      .implementation_string = __renderer_implementation_string(RENDERER_IMPLEMENTATION_VULKAN),
  };

  if (!renderer_vulkan_instance_create(renderer, application)) {
    logger_critical("<renderer:%p> <implementation:%s> VkInstance creation failed", renderer,
                    renderer->implementation_string);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug("<renderer:%p> <implementation:%s> <instance:%p> VkInstance created", renderer,
               renderer->implementation_string, renderer->instance);

  if (!renderer_vulkan_debug_messenger_create(renderer)) {
    logger_critical("<renderer:%p> <implementation:%s> VkDebugUtilsMessengerEXT creation failed", renderer,
                    renderer->implementation_string);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug("<renderer:%p> <implementation:%s> <instance:%p> VkDebugUtilsMessengerEXT created", renderer,
               renderer->implementation_string, renderer->instance);

  if (!renderer_vulkan_physical_device_select(renderer)) {
    logger_critical("<renderer:%p> <implementation:%s> VkPhysicalDevice selection failed", renderer,
                    renderer->implementation_string);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug("<renderer:%p> <implementation:%s> <instance:%p> VkPhysicalDevice selected", renderer,
               renderer->implementation_string, renderer->instance);
  logger_debug("|- [ %lu ] %s", renderer->physical_device_properties.deviceID,
               renderer->physical_device_properties.deviceName);

  return renderer;
}

b8 renderer_destroy(renderer_t *renderer) {
  if (!renderer_vulkan_debug_messenger_destroy(renderer)) {
    logger_critical("<renderer:%p> <implementation:%s> <instance:%p> VkDebugUtilsMessengerEXT destruction failed",
                    renderer, renderer->implementation_string, renderer->instance);
  }

  logger_debug("<renderer:%p> <implementation:%s> <instance:%p> VkDebugUtilsMessengerEXT destroyed", renderer,
               renderer->implementation_string, renderer->instance);

  if (!renderer_vulkan_instance_destroy(renderer)) {
    logger_critical("<renderer:%p> <implementation:%s> <instance:%p> VkInstance destruction failed", renderer,
                    renderer->implementation_string, renderer->instance);
  }

  logger_debug("<renderer:%p> <implementation:%s> <instance:%p> VkInstance destroyed", renderer,
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
