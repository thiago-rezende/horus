#pragma once

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>

b8 renderer_vulkan_command_pool_create(renderer_t *renderer);
b8 renderer_vulkan_command_pool_destroy(renderer_t *renderer);
