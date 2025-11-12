#pragma once

#include <vulkan/vulkan.h>

/* horus core layer */
#include <horus/core/types.h>

/* horus renderer layer */
#include <horus/renderer/texture.h>

struct __texture_context {
  VkDevice device;

  u32 binding;

  VkImage image;
  VkImageView image_view;

  VkSampler sampler;

  VkBuffer staging;

  VkDeviceSize size;
  VkDeviceMemory memory;
  VkDeviceMemory staging_memory;
};

u32 renderer_vulkan_texture_binding(texture_role_t role);
