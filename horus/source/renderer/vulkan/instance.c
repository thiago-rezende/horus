/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/instance.h>
#include <horus/renderer/vulkan/platform.h>

/* horus base layer */
#include <horus/definitions.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus containers layer */
#include <horus/containers/array.h>

b8 renderer_vulkan_instance_create(renderer_t *renderer, application_t *application) {
  unsigned int engine_version_major = horus_project_version_major();
  unsigned int engine_version_minor = horus_project_version_minor();
  unsigned int engine_version_patch = horus_project_version_patch();

  VkApplicationInfo application_info = (VkApplicationInfo){
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = application->name,
      .applicationVersion =
          VK_MAKE_VERSION(application->version.major, application->version.minor, application->version.patch),
      .pEngineName = horus_project_name(),
      .engineVersion = VK_MAKE_VERSION(engine_version_major, engine_version_minor, engine_version_patch),
      .apiVersion = VK_API_VERSION_1_3,
  };

  array_t *layers = renderer_vulkan_instance_get_required_layers();
  array_t *extensions = renderer_vulkan_instance_get_required_extensions();

  logger_debug_format("<renderer:%p> <count:%llu> required instance layers", (void *)renderer, layers->count);

  for (u64 i = 0; i < layers->count; i++) {
    char *name;

    array_retrieve(layers, i, (void *)&name);

    logger_debug_format("|- [ %s ]", name);
  }

  logger_debug_format("<renderer:%p> <count:%llu> required instance extensions", (void *)renderer, extensions->count);

  for (u64 i = 0; i < extensions->count; i++) {
    char *name;

    array_retrieve(extensions, i, (void *)&name);

    logger_debug_format("|- [ %s ]", name);
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
