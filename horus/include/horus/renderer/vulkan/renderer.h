#pragma once

#include <vulkan/vulkan.h>

/* horus application layer */
#include <horus/application/application.h>

/* horus renderer layer */
#include <horus/renderer/renderer.h>

struct __renderer {
  renderer_implementation_t implementation;
  const char *implementation_string;

  VkInstance instance;
};

b8 renderer_vulkan_create_instance(renderer_t *renderer, application_t *application);
b8 renderer_vulkan_destroy_instance(renderer_t *renderer);
