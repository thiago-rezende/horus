#pragma once

/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/renderer.h>

/* horus containers layer */
#include <horus/containers/array.h>

/* horus platform layer */
#include <horus/platform/window.h>

array_t *renderer_vulkan_instance_get_required_layers(void);
array_t *renderer_vulkan_instance_get_required_extensions(void);

array_t *renderer_vulkan_device_get_required_extensions(void);

b8 renderer_vulkan_surface_create(renderer_t *renderer, platform_window_t *window);
b8 renderer_vulkan_surface_destroy(renderer_t *renderer);
