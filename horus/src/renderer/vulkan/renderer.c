/* horus renderer layer */
#include <horus/renderer/renderer.h>

/* horus base layer */
#include <horus/definitions.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus logger layer */
#include <horus/logger/logger.h>

#include <vulkan/vulkan.h>

struct __renderer {
  renderer_implementation_t implementation;
  const char *implementation_string;

  VkInstance instance;
};

b8 renderer_create(application_t *application, platform_window_t *window, renderer_t **output) {
  if (output == NULL) {
    return false;
  }

  renderer_t *renderer = platform_memory_allocate(sizeof(renderer_t));

  *renderer = (renderer_t){
      .implementation = RENDERER_IMPLEMENTATION_VULKAN,
      .implementation_string = __renderer_implementation_string(RENDERER_IMPLEMENTATION_VULKAN),
  };

  VkApplicationInfo application_info = (VkApplicationInfo){
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = application->name,
      .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
      .pEngineName = horus_project_name(),
      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
      .apiVersion = VK_API_VERSION_1_0,
  };

  VkInstanceCreateInfo instance_create_info = (VkInstanceCreateInfo){
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &application_info,
      .enabledLayerCount = 0,
      .ppEnabledLayerNames = NULL,
      .enabledExtensionCount = 0,
      .ppEnabledExtensionNames = NULL,
  };

  VkResult result = vkCreateInstance(&instance_create_info, NULL, &renderer->instance);

  if (result != VK_SUCCESS) {
    logger_critical("<renderer:%p> <implementation:%s> vkCreateInstance failed", renderer,
                    renderer->implementation_string);

    platform_memory_deallocate(renderer);

    return false;
  }

  logger_debug("<renderer:%p> <implementation:%s> <instance:%p> VkInstance created", renderer,
               renderer->implementation_string, renderer->instance);

  *output = renderer;

  return true;
}

b8 renderer_destroy(renderer_t *renderer) {
  if (renderer == NULL) {
    return false;
  }

  vkDestroyInstance(renderer->instance, NULL);

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
