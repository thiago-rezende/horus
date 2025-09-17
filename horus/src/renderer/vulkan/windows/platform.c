#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

/* horus vulkan renderer layer */
#include <horus/renderer/vulkan/platform.h>

#define VULKAN_LAYERS_COUNT 1
#define VULKAN_EXTENSIONS_COUNT 3

#define VK_KHRONOS_VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"

const char *vk_khronos_validation_layer_name = VK_KHRONOS_VALIDATION_LAYER_NAME;

const char *vk_khr_surface_extension_name = VK_KHR_SURFACE_EXTENSION_NAME;
const char *vk_khr_win32_surface_extension_name = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;

const char *vk_ext_debug_utils_extension_name = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

array_t *renderer_vulkan_get_instance_required_layers(void) {
  array_t *array = array_create(VULKAN_LAYERS_COUNT, sizeof(char *));

  array_insert(array, &vk_khronos_validation_layer_name);

  return array;
}

array_t *renderer_vulkan_get_instance_required_extensions(void) {
  array_t *array = array_create(VULKAN_EXTENSIONS_COUNT, sizeof(char *));

  array_insert(array, &vk_khr_surface_extension_name);
  array_insert(array, &vk_khr_win32_surface_extension_name);

  array_insert(array, &vk_ext_debug_utils_extension_name);

  return array;
}
