/* horus vulkan renderer layer */
#include <horus/renderer/vulkan/device.h>
#include <horus/renderer/vulkan/platform.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus containers layer */
#include <horus/containers/array.h>

b8 renderer_vulkan_physical_device_select(renderer_t *renderer) {
  u32 physical_device_count = 0;

  vkEnumeratePhysicalDevices(renderer->instance, &physical_device_count, NULL);

  if (physical_device_count == 0) {
    logger_critical("<renderer:%p> <instance:%p> no physical devices found", renderer, renderer->instance);

    return false;
  }

  array_t *devices = array_create(physical_device_count, sizeof(VkPhysicalDevice));

  devices->count = physical_device_count;

  vkEnumeratePhysicalDevices(renderer->instance, &physical_device_count, devices->buffer);

  logger_debug("<renderer:%p> <count:%lu> physical devices", renderer, devices->count);

  physical_device_score_t current_physical_device_score = {
      .score = 0,
      .device = VK_NULL_HANDLE,
  };

  for (u64 i = 0; i < devices->count; i++) {
    VkPhysicalDevice device;

    array_retrieve(devices, i, (void *)&device);

    physical_device_score_t device_score = renderer_vulkan_physical_device_get_score(device);

    queue_family_indices_t queue_family_indices = renderer_vulkan_physical_device_get_queue_family_indices(device);

    if (!queue_family_indices.has_compute_family_index) {
      continue;
    }

    if (!queue_family_indices.has_graphics_family_index) {
      continue;
    }

    if (!queue_family_indices.has_transfer_family_index) {
      continue;
    }

    device_score.queues = queue_family_indices;

    if (device_score.score > current_physical_device_score.score) {
      current_physical_device_score = device_score;
    }
  }

  array_destroy(devices);

  if (current_physical_device_score.device == VK_NULL_HANDLE) {
    logger_critical("<renderer:%p> <instance:%p> no suitable physical devices found", renderer, renderer->instance);

    return false;
  }

  renderer->physical_device = current_physical_device_score.device;
  renderer->physical_device_features = current_physical_device_score.features;
  renderer->physical_device_properties = current_physical_device_score.properties;

  renderer->compute_queue_family_index = current_physical_device_score.queues.compute_family_index;
  renderer->graphics_queue_family_index = current_physical_device_score.queues.graphics_family_index;
  renderer->transfer_queue_family_index = current_physical_device_score.queues.transfer_family_index;

  return true;
}

physical_device_score_t renderer_vulkan_physical_device_get_score(VkPhysicalDevice device) {
  u64 score = 0;

  VkPhysicalDeviceFeatures features;
  VkPhysicalDeviceProperties properties;

  char *device_type_string = "unknown";

  vkGetPhysicalDeviceFeatures(device, &features);
  vkGetPhysicalDeviceProperties(device, &properties);

  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU) {
    score += 10;

    device_type_string = "virtual_gpu";
  }

  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU) {
    score += 100;

    device_type_string = "cpu";
  }

  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
    score += 1000;

    device_type_string = "integrated_gpu";
  }

  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 10000;

    device_type_string = "discrete_gpu";
  }

  score += features.wideLines ? 50 : 0;
  score += features.depthBounds ? 150 : 0;
  score += features.multiViewport ? 10 : 0;
  score += features.fillModeNonSolid ? 50 : 0;
  score += features.samplerAnisotropy ? 200 : 0;
  score += features.tessellationShader ? 10 : 0;

  score += properties.limits.maxImageDimension1D / 1024;
  score += properties.limits.maxImageDimension2D / 1024;
  score += properties.limits.maxImageDimension3D / 128;
  score += properties.limits.maxSamplerAnisotropy > 8.0f ? 100 : 0;
  score += properties.limits.maxPushConstantsSize > 128 ? 100 : 0;
  score += properties.limits.maxMemoryAllocationCount / 1000;
  score += properties.limits.maxVertexInputAttributes > 16 ? 100 : 0;

  logger_debug("|- [ %lu ] <score:%lu> <type:%s> %s", properties.deviceID, score, device_type_string,
               properties.deviceName);
  logger_debug("|- |- [ properties ]");
  logger_debug("|- |- |- [ wide lines ] %s", features.wideLines ? "true" : "false");
  logger_debug("|- |- |- [ depth bounds ] %s", features.depthBounds ? "true" : "false");
  logger_debug("|- |- |- [ multi viewport ] %s", features.multiViewport ? "true" : "false");
  logger_debug("|- |- |- [ fill mode non solid ] %s", features.fillModeNonSolid ? "true" : "false");
  logger_debug("|- |- |- [ sampler anisotropy ] %s", features.samplerAnisotropy ? "true" : "false");
  logger_debug("|- |- |- [ tessellation shader ] %s", features.tessellationShader ? "true" : "false");
  logger_debug("|- |- [ limits ]");
  logger_debug("|- |- |- [ max image dimension 1D ] %lu", properties.limits.maxImageDimension1D);
  logger_debug("|- |- |- [ max image dimension 2D ] %lu", properties.limits.maxImageDimension2D);
  logger_debug("|- |- |- [ max image dimension 3D ] %lu", properties.limits.maxImageDimension3D);
  logger_debug("|- |- |- [ max sampler anisotropy ] %lu", properties.limits.maxSamplerAnisotropy);
  logger_debug("|- |- |- [ max push constants size ] %lu", properties.limits.maxPushConstantsSize);
  logger_debug("|- |- |- [ max memory allocation count ] %lu", properties.limits.maxMemoryAllocationCount);
  logger_debug("|- |- |- [ max vertex input attributes ] %lu", properties.limits.maxVertexInputAttributes);

  if (!features.geometryShader) {
    score = 0;
  }

  return (physical_device_score_t){
      .score = score,
      .device = device,
      .features = features,
      .properties = properties,
      .device_type_string = device_type_string,
  };
}

queue_family_indices_t renderer_vulkan_physical_device_get_queue_family_indices(VkPhysicalDevice device) {
  queue_family_indices_t indices = (queue_family_indices_t){
      .compute_family_index = 0,
      .graphics_family_index = 0,
      .transfer_family_index = 0,
      .has_compute_family_index = false,
      .has_graphics_family_index = false,
      .has_transfer_family_index = false,
  };

  u32 queue_family_count = 0;

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);

  if (queue_family_count == 0) {
    logger_critical("<device:%p> no queue families found for device", device);

    return indices;
  }

  array_t *families = array_create(queue_family_count, sizeof(VkQueueFamilyProperties));

  families->count = queue_family_count;

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, families->buffer);

  logger_debug("<device:%p> <count:%lu> physical device queue families", device, families->count);

  /* TODO: improve queue family selection to prioritize dedicated queues and fallback for the general one */
  for (u64 i = 0; i < families->count; i++) {
    VkQueueFamilyProperties family;

    array_retrieve(families, i, (void *)&family);

    if (family.queueFlags & VK_QUEUE_COMPUTE_BIT) {
      indices.compute_family_index = i;
      indices.has_compute_family_index = true;
    }

    if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphics_family_index = i;
      indices.has_graphics_family_index = true;
    }

    if (family.queueFlags & VK_QUEUE_TRANSFER_BIT) {
      indices.transfer_family_index = i;
      indices.has_transfer_family_index = true;
    }

    logger_debug("|- [ queue ] <index:%lu>", i);
    logger_debug("|- |- [ compute ] %s", family.queueFlags & VK_QUEUE_COMPUTE_BIT ? "true" : "false");
    logger_debug("|- |- [ graphics ] %s", family.queueFlags & VK_QUEUE_GRAPHICS_BIT ? "true" : "false");
    logger_debug("|- |- [ transfer ] %s", family.queueFlags & VK_QUEUE_TRANSFER_BIT ? "true" : "false");
  }

  array_destroy(families);

  return indices;
}

b8 renderer_vulkan_device_create(renderer_t *renderer) {
  const static u64 queue_count = 3;
  const static float queue_priority = 1.0f;

  VkDeviceQueueCreateInfo compute_queue_create_info = (VkDeviceQueueCreateInfo){
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueCount = 1,
      .queueFamilyIndex = renderer->compute_queue_family_index,
      .pQueuePriorities = &queue_priority,
  };

  VkDeviceQueueCreateInfo graphics_queue_create_info = (VkDeviceQueueCreateInfo){
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueCount = 1,
      .queueFamilyIndex = renderer->graphics_queue_family_index,
      .pQueuePriorities = &queue_priority,
  };

  VkDeviceQueueCreateInfo transfer_queue_create_info = (VkDeviceQueueCreateInfo){
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueCount = 1,
      .queueFamilyIndex = renderer->transfer_queue_family_index,
      .pQueuePriorities = &queue_priority,
  };

  array_t *queue_create_infos = array_create(queue_count, sizeof(VkDeviceQueueCreateInfo));

  array_insert(queue_create_infos, &compute_queue_create_info);
  array_insert(queue_create_infos, &graphics_queue_create_info);
  array_insert(queue_create_infos, &transfer_queue_create_info);

  VkPhysicalDeviceFeatures device_features = (VkPhysicalDeviceFeatures){
      .wideLines = VK_TRUE,
      .depthBounds = VK_TRUE,
      .multiViewport = VK_TRUE,
      .fillModeNonSolid = VK_TRUE,
      .samplerAnisotropy = VK_TRUE,
      .tessellationShader = VK_TRUE,
  };

  VkDeviceCreateInfo device_create_info = (VkDeviceCreateInfo){
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .queueCreateInfoCount = queue_create_infos->count,
      .pQueueCreateInfos = queue_create_infos->buffer,
      .pEnabledFeatures = &device_features,
  };

  if (vkCreateDevice(renderer->physical_device, &device_create_info, NULL, &renderer->device) != VK_SUCCESS) {
    logger_critical("<physical_device:%p> logical device creation failed", renderer->physical_device);

    array_destroy(queue_create_infos);

    return false;
  }

  vkGetDeviceQueue(renderer->device, renderer->compute_queue_family_index, 0, &renderer->compute_queue);
  vkGetDeviceQueue(renderer->device, renderer->graphics_queue_family_index, 0, &renderer->graphics_queue);
  vkGetDeviceQueue(renderer->device, renderer->transfer_queue_family_index, 0, &renderer->transfer_queue);

  array_destroy(queue_create_infos);

  return true;
}

b8 renderer_vulkan_device_destroy(renderer_t *renderer) {
  vkDestroyDevice(renderer->device, NULL);

  return true;
}
