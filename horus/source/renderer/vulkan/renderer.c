#include <vulkan/vulkan.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/debug.h>
#include <horus/renderer/vulkan/device.h>
#include <horus/renderer/vulkan/loader.h>
#include <horus/renderer/vulkan/instance.h>
#include <horus/renderer/vulkan/platform.h>
#include <horus/renderer/vulkan/renderer.h>
#include <horus/renderer/vulkan/swapchain.h>

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
    logger_critical_format("<renderer:%p> VkInstance creation failed", (void *)renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug_format("<renderer:%p> <instance:%p> VkInstance created", (void *)renderer, (void *)renderer->instance);

  if (!renderer_vulkan_debug_messenger_create(renderer)) {
    logger_critical_format("<renderer:%p> VkDebugUtilsMessengerEXT creation failed", (void *)renderer);

    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug_format("<renderer:%p> <messenger:%p> VkDebugUtilsMessengerEXT created", (void *)renderer,
                      (void *)renderer->messenger);

  if (!renderer_vulkan_surface_create(renderer, window)) {
    logger_critical_format("<renderer:%p> VkSurfaceKHR creation failed", (void *)renderer);

    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug_format("<renderer:%p> <surface:%p> VkSurfaceKHR created", (void *)renderer, (void *)renderer->surface);

  if (!renderer_vulkan_physical_device_select(renderer)) {
    logger_critical_format("<renderer:%p> VkPhysicalDevice selection failed", (void *)renderer);

    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug_format("<renderer:%p> <physical_device:%p> VkPhysicalDevice selected", (void *)renderer,
                      (void *)renderer->physical_device);
  logger_debug_format("|- [ %u ] %s", renderer->physical_device_properties.deviceID,
                      renderer->physical_device_properties.deviceName);
  logger_debug_format("|- |- [ queues family indices ] <compute:%u> <present:%u> <graphics:%u> <transfer:%u>",
                      renderer->compute_queue_family_index, renderer->present_queue_family_index,
                      renderer->graphics_queue_family_index, renderer->transfer_queue_family_index);

  if (!renderer_vulkan_device_create(renderer)) {
    logger_critical_format("<renderer:%p> VkDevice creation failed", (void *)renderer);

    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug_format("<renderer:%p> <device:%p> VkDevice created", (void *)renderer, (void *)renderer->device);

  if (!renderer_vulkan_swapchain_create(renderer)) {
    logger_critical_format("<renderer:%p> VkSwapchainKHR creation failed", (void *)renderer);

    renderer_vulkan_device_destroy(renderer);
    renderer_vulkan_surface_destroy(renderer);
    renderer_vulkan_debug_messenger_destroy(renderer);
    renderer_vulkan_instance_destroy(renderer);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug_format("<renderer:%p> <swapchain:%p> VkSwapchainKHR created", (void *)renderer,
                      (void *)renderer->swapchain);
  logger_debug_format("|- [ image count ] %lu", renderer->swapchain_images->count);

  return renderer;
}

b8 renderer_destroy(renderer_t *renderer) {
  if (!renderer_vulkan_swapchain_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <swapchain:%p> VkSwapchainKHR destruction failed", (void *)renderer,
                           (void *)renderer->swapchain);

    return false;
  }

  logger_debug_format("<renderer:%p> <swapchain:%p> VkSwapchainKHR destroyed", (void *)renderer,
                      (void *)renderer->swapchain);

  if (!renderer_vulkan_device_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <device:%p> VkDevice destruction failed", (void *)renderer,
                           (void *)renderer->device);

    return false;
  }

  logger_debug_format("<renderer:%p> <device:%p> VkDevice destroyed", (void *)renderer, (void *)renderer->device);

  if (!renderer_vulkan_surface_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <surface:%p> VkSurfaceKHR destruction failed", (void *)renderer,
                           (void *)renderer->surface);

    return false;
  }

  logger_debug_format("<renderer:%p> <surface:%p> VkSurfaceKHR destroyed", (void *)renderer, (void *)renderer->surface);

  if (!renderer_vulkan_debug_messenger_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <messenger:%p> VkDebugUtilsMessengerEXT destruction failed", (void *)renderer,
                           (void *)renderer->messenger);

    return false;
  }

  logger_debug_format("<renderer:%p> <messenger:%p> VkDebugUtilsMessengerEXT destroyed", (void *)renderer,
                      (void *)renderer->messenger);

  if (!renderer_vulkan_instance_destroy(renderer)) {
    logger_critical_format("<renderer:%p> <instance:%p> VkInstance destruction failed", (void *)renderer,
                           (void *)renderer->instance);

    return false;
  }

  logger_debug_format("<renderer:%p> <instance:%p> VkInstance destroyed", (void *)renderer, (void *)renderer->instance);

  platform_memory_deallocate(renderer);

  return true;
}

renderer_implementation_t renderer_implementation(renderer_t *renderer) {
  return renderer->implementation;
}

const char *renderer_implementation_string(renderer_t *renderer) {
  return renderer->implementation_string;
}
