/* horus vulkan renderer layer */
#include <horus/renderer/vulkan/device.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus containers layer */
#include <horus/containers/array.h>

b8 renderer_vulkan_physical_device_select(renderer_t *renderer) {
  u32 physical_device_count = 0;

  vkEnumeratePhysicalDevices(renderer->instance, &physical_device_count, NULL);

  if (physical_device_count == 0) {
    logger_critical("<renderer:%p> <implementation:%s> <instance:%p> no physical devices found", renderer,
                    renderer->implementation_string, renderer->instance);

    return false;
  }

  array_t *devices = array_create(physical_device_count, sizeof(VkPhysicalDevice));

  devices->count = physical_device_count;

  vkEnumeratePhysicalDevices(renderer->instance, &physical_device_count, devices->buffer);

  logger_debug("<renderer:%p> <implementation:%s> <instance:%p> <count:%lu> physical devices", renderer,
               renderer->implementation_string, renderer->instance, devices->count);

  u64 current_device_score = 0;

  VkPhysicalDevice current_device = VK_NULL_HANDLE;
  VkPhysicalDeviceFeatures current_device_features;
  VkPhysicalDeviceProperties current_device_properties;

  for (u64 i = 0; i < devices->count; i++) {
    u64 score = 0;

    VkPhysicalDevice device;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceProperties properties;

    char *device_type_string = "unknown";

    array_retrieve(devices, i, (void *)&device);

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

    logger_debug("| [ %lu ] <score:%lu> <type:%s> %s", properties.deviceID, score, device_type_string,
                 properties.deviceName);
    logger_debug("|- [ properties ]");
    logger_debug("|- |- [ wide lines ] %s", features.wideLines ? "true" : "false");
    logger_debug("|- |- [ depth bounds ] %s", features.depthBounds ? "true" : "false");
    logger_debug("|- |- [ multi viewport ] %s", features.multiViewport ? "true" : "false");
    logger_debug("|- |- [ fill mode non solid ] %s", features.fillModeNonSolid ? "true" : "false");
    logger_debug("|- |- [ sampler anisotropy ] %s", features.samplerAnisotropy ? "true" : "false");
    logger_debug("|- |- [ tessellation shader ] %s", features.tessellationShader ? "true" : "false");
    logger_debug("|- [ limits ]");
    logger_debug("|- |- [ max image dimension 1D ] %lu", properties.limits.maxImageDimension1D);
    logger_debug("|- |- [ max image dimension 2D ] %lu", properties.limits.maxImageDimension2D);
    logger_debug("|- |- [ max image dimension 3D ] %lu", properties.limits.maxImageDimension3D);
    logger_debug("|- |- [ max sampler anisotropy ] %lu", properties.limits.maxSamplerAnisotropy);
    logger_debug("|- |- [ max push constants size ] %lu", properties.limits.maxPushConstantsSize);
    logger_debug("|- |- [ max memory allocation count ] %lu", properties.limits.maxMemoryAllocationCount);
    logger_debug("|- |- [ max vertex input attributes ] %lu", properties.limits.maxVertexInputAttributes);

    if (!features.geometryShader) {
      continue;
    }

    if (score > current_device_score) {
      current_device_score = score;

      current_device = device;
      current_device_features = features;
      current_device_properties = properties;
    }
  }

  array_destroy(devices);

  if (current_device == VK_NULL_HANDLE) {
    logger_critical("<renderer:%p> <implementation:%s> <instance:%p> no suitable physical devices found", renderer,
                    renderer->implementation_string, renderer->instance);

    return false;
  }

  renderer->physical_device = current_device;
  renderer->physical_device_features = current_device_features;
  renderer->physical_device_properties = current_device_properties;

  return true;
}
