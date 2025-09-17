#include <vulkan/vulkan.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>
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

renderer_t *renderer_create(application_t *application, platform_window_t *window) {
  renderer_t *renderer = platform_memory_allocate(sizeof(renderer_t));

  *renderer = (renderer_t){
      .implementation = RENDERER_IMPLEMENTATION_VULKAN,
      .implementation_string = __renderer_implementation_string(RENDERER_IMPLEMENTATION_VULKAN),
  };

  if (!renderer_vulkan_create_instance(renderer, application)) {
    logger_critical("<renderer:%p> <implementation:%s> vkCreateInstance failed", renderer,
                    renderer->implementation_string);

    platform_memory_deallocate(renderer);

    return NULL;
  }

  logger_debug("<renderer:%p> <implementation:%s> <instance:%p> VkInstance created", renderer,
               renderer->implementation_string, renderer->instance);

  return renderer;
}

b8 renderer_destroy(renderer_t *renderer) {
  if (renderer == NULL) {
    return false;
  }

  if (!renderer_vulkan_destroy_instance(renderer)) {
    logger_critical("<renderer:%p> <implementation:%s> <instance:%p> VkInstance destruction failed", renderer,
                    renderer->implementation_string, renderer->instance);
  }

  logger_debug("<renderer:%p> <implementation:%s> <instance:%p> VkInstance destroyed", renderer,
               renderer->implementation_string, renderer->instance);

  platform_memory_deallocate(renderer);

  return true;
}

renderer_implementation_t renderer_implementation(renderer_t *renderer) {
  if (!renderer) {
    return RENDERER_IMPLEMENTATION_NONE;
  }

  return renderer->implementation;
}

const char *renderer_implementation_string(renderer_t *renderer) {
  if (!renderer) {
    return "unknown";
  }

  return renderer->implementation_string;
}

b8 renderer_vulkan_create_instance(renderer_t *renderer, application_t *application) {
  if (renderer == NULL) {
    return false;
  }

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
  if (renderer == NULL) {
    return false;
  }

  vkDestroyInstance(renderer->instance, NULL);

  return true;
}
