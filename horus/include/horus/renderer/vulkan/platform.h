#pragma once

/* horus vulkan renderer layer */
#include <horus/renderer/vulkan/renderer.h>

/* horus containers layer */
#include <horus/containers/array.h>

/* horus platform layer [ linux ] */
#include <horus/platform/windows/window.h>

array_t *renderer_vulkan_instance_get_required_layers(void);
array_t *renderer_vulkan_instance_get_required_extensions(void);

b8 renderer_vulkan_surface_create(renderer_t *renderer, platform_window_context_t *context);
b8 renderer_vulkan_surface_destroy(renderer_t *renderer);
