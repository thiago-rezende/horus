#include <xcb/xcb.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xcb.h>

/* horus platform layer [ linux ] */
#include <horus/platform/linux/window.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/platform.h>

#define VULKAN_LAYERS_COUNT 1
#define VULKAN_EXTENSIONS_COUNT 3

#define VK_KHRONOS_VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"

const char *vk_khronos_validation_layer_name = VK_KHRONOS_VALIDATION_LAYER_NAME;

const char *vk_khr_surface_extension_name = VK_KHR_SURFACE_EXTENSION_NAME;
const char *vk_khr_xcb_surface_extension_name = VK_KHR_XCB_SURFACE_EXTENSION_NAME;

const char *vk_ext_debug_utils_extension_name = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

array_t *renderer_vulkan_instance_get_required_layers(void) {
  array_t *array = array_create(VULKAN_LAYERS_COUNT, sizeof(char *));

  array_insert(array, &vk_khronos_validation_layer_name);

  return array;
}

array_t *renderer_vulkan_instance_get_required_extensions(void) {
  array_t *array = array_create(VULKAN_EXTENSIONS_COUNT, sizeof(char *));

  array_insert(array, &vk_khr_surface_extension_name);
  array_insert(array, &vk_khr_xcb_surface_extension_name);

  array_insert(array, &vk_ext_debug_utils_extension_name);

  return array;
}

b8 renderer_vulkan_surface_create(renderer_t *renderer, platform_window_t *window) {
  platform_window_context_t *context = platform_window_context(window);

  VkXcbSurfaceCreateInfoKHR surface_create_info = (VkXcbSurfaceCreateInfoKHR){
      .sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
      .connection = context->connection,
      .window = context->window,
  };

  if (vkCreateXcbSurfaceKHR(renderer->instance, &surface_create_info, NULL, &renderer->surface) != VK_SUCCESS) {
    logger_critical("<renderer:%p> <instance:%p> surface creation failed", renderer, renderer->instance);

    return false;
  }

  return true;
}

b8 renderer_vulkan_surface_destroy(renderer_t *renderer) {
  vkDestroySurfaceKHR(renderer->instance, renderer->surface, NULL);

  return true;
}
