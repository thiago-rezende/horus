#pragma once

/* horus platform layer */
#include <horus/platform/window.h>

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>

b8 renderer_vulkan_swapchain_create(renderer_t *renderer, platform_window_t *window);
b8 renderer_vulkan_swapchain_update(renderer_t *renderer, platform_window_t *window);
b8 renderer_vulkan_swapchain_destroy(renderer_t *renderer);

b8 renderer_vulkan_swapchain_build_extent(renderer_t *renderer, platform_window_t *window);
