/* horus vulkan renderer layer */
#include <horus/renderer/vulkan/instance.h>
#include <horus/renderer/vulkan/platform.h>

/* horus base layer */
#include <horus/definitions.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus containers layer */
#include <horus/containers/array.h>

b8 renderer_vulkan_instance_create(renderer_t *renderer, application_t *application) {
  VkApplicationInfo application_info = (VkApplicationInfo){
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = application->name,
      .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
      .pEngineName = horus_project_name(),
      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
      .apiVersion = VK_API_VERSION_1_0,
  };

  array_t *layers = renderer_vulkan_instance_get_required_layers();
  array_t *extensions = renderer_vulkan_instance_get_required_extensions();

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

b8 renderer_vulkan_instance_destroy(renderer_t *renderer) {
  vkDestroyInstance(renderer->instance, NULL);

  return true;
}
