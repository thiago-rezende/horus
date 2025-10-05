#pragma once

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>

b8 renderer_vulkan_instance_create(renderer_t *renderer, renderer_create_info_t info);
b8 renderer_vulkan_instance_destroy(renderer_t *renderer);
