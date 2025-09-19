#pragma once

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>

b8 renderer_vulkan_instance_create(renderer_t *renderer, application_t *application);
b8 renderer_vulkan_instance_destroy(renderer_t *renderer);
