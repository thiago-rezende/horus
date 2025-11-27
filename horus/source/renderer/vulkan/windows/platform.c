/* horus platform layer [ windows ] */
#include <horus/platform/windows/windows.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

/* horus platform layer [ windows ] */
#include <horus/platform/windows/window.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/platform.h>

#define VULKAN_INSTANCE_LAYERS_COUNT 1
#define VULKAN_INSTANCE_EXTENSIONS_COUNT 3

#define VULKAN_DEVICE_EXTENSIONS_COUNT 4

#define VK_KHRONOS_VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"

/* instance layers */
const char *vk_khronos_validation_layer_name = VK_KHRONOS_VALIDATION_LAYER_NAME;

/* instance debug extensions */
const char *vk_ext_debug_utils_extension_name = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

/* instance platform extensions */
const char *vk_khr_win32_surface_extension_name = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;

/* instance extensions */
const char *vk_khr_surface_extension_name = VK_KHR_SURFACE_EXTENSION_NAME;

/* device extensions */
const char *vk_khr_spirv_1_4_extension_name = VK_KHR_SPIRV_1_4_EXTENSION_NAME;
const char *vk_khr_swapchain_extension_name = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
const char *vk_khr_synchronization_2_extension_name = VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME;
const char *vk_khr_create_renderpass_2_extension_name = VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME;

array_t *renderer_vulkan_instance_get_required_layers(void) {
  array_t *array = array_create(VULKAN_INSTANCE_LAYERS_COUNT, sizeof(char *));

  array_insert(array, &vk_khronos_validation_layer_name);

  return array;
}

array_t *renderer_vulkan_instance_get_required_extensions(void) {
  array_t *array = array_create(VULKAN_INSTANCE_EXTENSIONS_COUNT, sizeof(char *));

  array_insert(array, &vk_khr_surface_extension_name);
  array_insert(array, &vk_khr_win32_surface_extension_name);

  array_insert(array, &vk_ext_debug_utils_extension_name);

  return array;
}

array_t *renderer_vulkan_device_get_required_extensions(void) {
  array_t *array = array_create(VULKAN_DEVICE_EXTENSIONS_COUNT, sizeof(char *));

  array_insert(array, &vk_khr_spirv_1_4_extension_name);
  array_insert(array, &vk_khr_swapchain_extension_name);
  array_insert(array, &vk_khr_synchronization_2_extension_name);
  array_insert(array, &vk_khr_create_renderpass_2_extension_name);

  return array;
}

b8 renderer_vulkan_surface_create(renderer_t *renderer, platform_window_t *window) {
  platform_window_context_t *context = platform_window_context(window);

  VkWin32SurfaceCreateInfoKHR surface_create_info = (VkWin32SurfaceCreateInfoKHR){
      .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
      .hwnd = context->window,
      .hinstance = context->hinstance,
  };

  if (vkCreateWin32SurfaceKHR(renderer->context->instance, &surface_create_info, NULL, &renderer->context->surface) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <instance:%p> surface creation failed", (void *)renderer,
                           (void *)renderer->context->instance);

    return false;
  }

  return true;
}

b8 renderer_vulkan_surface_destroy(renderer_t *renderer) {
  vkDestroySurfaceKHR(renderer->context->instance, renderer->context->surface, NULL);

  return true;
}
