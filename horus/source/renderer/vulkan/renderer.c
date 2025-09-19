#include <vulkan/vulkan.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>

/* horus vulkan renderer layer */
#include <horus/renderer/vulkan/debug.h>
#include <horus/renderer/vulkan/device.h>
#include <horus/renderer/vulkan/loader.h>
#include <horus/renderer/vulkan/instance.h>
#include <horus/renderer/vulkan/platform.h>
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
    logger_critical("<renderer:%p> VkInstance creation failed", renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug("<renderer:%p> <instance:%p> VkInstance created", renderer, renderer->instance);

  if (!renderer_vulkan_debug_messenger_create(renderer)) {
    logger_critical("<renderer:%p> VkDebugUtilsMessengerEXT creation failed", renderer);

    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug("<renderer:%p> <messenger:%p> VkDebugUtilsMessengerEXT created", renderer, renderer->messenger);

  if (!renderer_vulkan_surface_create(renderer, platform_window_context(window))) {
    logger_critical("<renderer:%p> VkSurfaceKHR creation failed", renderer);

    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug("<renderer:%p> <surface:%p> VkSurfaceKHR created", renderer, renderer->surface);

  if (!renderer_vulkan_physical_device_select(renderer)) {
    logger_critical("<renderer:%p> VkPhysicalDevice selection failed", renderer);

    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug("<renderer:%p> <physical_device:%p> VkPhysicalDevice selected", renderer, renderer->physical_device);
  logger_debug("|- [ %lu ] %s", renderer->physical_device_properties.deviceID,
               renderer->physical_device_properties.deviceName);
  logger_debug("|- |- [ queues family indices ] <compute:%lu> <present:%lu> <graphics:%lu> <transfer:%lu>",
               renderer->compute_queue_family_index, renderer->present_queue_family_index,
               renderer->graphics_queue_family_index, renderer->transfer_queue_family_index);

  if (!renderer_vulkan_device_create(renderer)) {
    logger_critical("<renderer:%p> VkDevice creation failed", renderer);

    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  return renderer;
}

b8 renderer_destroy(renderer_t *renderer) {
  if (!renderer_vulkan_device_destroy(renderer)) {
    logger_critical("<renderer:%p> <device:%p> VkDevice destruction failed", renderer, renderer->device);

    return false;
  }

  logger_debug("<renderer:%p> <device:%p> VkDevice destroyed", renderer, renderer->device);

  if (!renderer_vulkan_surface_destroy(renderer)) {
    logger_critical("<renderer:%p> <surface:%p> VkSurfaceKHR destruction failed", renderer, renderer->surface);

    return false;
  }

  logger_debug("<renderer:%p> <surface:%p> VkSurfaceKHR destroyed", renderer, renderer->surface);

  if (!renderer_vulkan_debug_messenger_destroy(renderer)) {
    logger_critical("<renderer:%p> <messenger:%p> VkDebugUtilsMessengerEXT destruction failed", renderer,
                    renderer->messenger);

    return false;
  }

  logger_debug("<renderer:%p> <messenger:%p> VkDebugUtilsMessengerEXT destroyed", renderer, renderer->messenger);

  if (!renderer_vulkan_instance_destroy(renderer)) {
    logger_critical("<renderer:%p> <instance:%p> VkInstance destruction failed", renderer, renderer->instance);

    return false;
  }

  logger_debug("<renderer:%p> <instance:%p> VkInstance destroyed", renderer, renderer->instance);

  platform_memory_deallocate(renderer);

  return true;
}

renderer_implementation_t renderer_implementation(renderer_t *renderer) {
  return renderer->implementation;
}

const char *renderer_implementation_string(renderer_t *renderer) {
  return renderer->implementation_string;
}
