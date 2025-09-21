/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/device.h>
#include <horus/renderer/vulkan/platform.h>

/* horus core layer */
#include <horus/core/strings.h>

/* horus logger layer */
#include <horus/logger/logger.h>

#define MINIMUM_EXTRA_DESIRED_SURFACE_IMAGES 1

b8 __queue_family_index_is_unique(u32 index, u32 others[], u8 others_count);
b8 __physical_device_has_required_extensions(VkPhysicalDevice device, array_t *extensions);

array_t *__build_queue_create_infos(renderer_t *renderer);

b8 renderer_vulkan_physical_device_select(renderer_t *renderer) {
  u32 physical_device_count = 0;

  vkEnumeratePhysicalDevices(renderer->instance, &physical_device_count, NULL);

  if (physical_device_count == 0) {
    logger_critical("<renderer:%p> <instance:%p> no physical devices found", renderer, renderer->instance);

    return false;
  }

  array_t *extensions = renderer_vulkan_device_get_required_extensions();

  logger_debug("<renderer:%p> <count:%lu> required device extensions", renderer, extensions->count);

  for (u64 i = 0; i < extensions->count; i++) {
    char *name;

    array_retrieve(extensions, i, (void *)&name);

    logger_debug("|- [ %s ]", name);
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

    physical_device_score_t device_score =
        renderer_vulkan_physical_device_get_score(device, renderer->surface, extensions);

    if (device_score.score > current_physical_device_score.score) {
      current_physical_device_score = device_score;
    }
  }

  array_destroy(devices);
  array_destroy(extensions);

  if (current_physical_device_score.device == VK_NULL_HANDLE) {
    logger_critical("<renderer:%p> <instance:%p> no suitable physical devices found", renderer, renderer->instance);

    return false;
  }

  renderer->physical_device = current_physical_device_score.device;
  renderer->physical_device_features = current_physical_device_score.features;
  renderer->physical_device_properties = current_physical_device_score.properties;

  renderer->compute_queue_family_index = current_physical_device_score.queues.compute_family_index;
  renderer->present_queue_family_index = current_physical_device_score.queues.present_family_index;
  renderer->graphics_queue_family_index = current_physical_device_score.queues.graphics_family_index;
  renderer->transfer_queue_family_index = current_physical_device_score.queues.transfer_family_index;

  renderer->surface_format = current_physical_device_score.surface_information.format;
  renderer->surface_capabilities = current_physical_device_score.surface_information.capabilities;
  renderer->surface_present_mode = current_physical_device_score.surface_information.present_mode;

  return true;
}

physical_device_score_t renderer_vulkan_physical_device_get_score(VkPhysicalDevice device,
                                                                  VkSurfaceKHR surface,
                                                                  array_t *extensions) {
  u64 score = 0;

  VkPhysicalDeviceFeatures features;
  VkPhysicalDeviceProperties properties;

  surface_information_t surface_information = {0};

  b8 has_required_extensions = __physical_device_has_required_extensions(device, extensions);

  queue_family_indices_t queue_family_indices =
      renderer_vulkan_physical_device_get_queue_family_indices(device, surface);

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

  score += properties.limits.maxImageDimension1D / 1024;
  score += properties.limits.maxImageDimension2D / 1024;
  score += properties.limits.maxImageDimension3D / 128;
  score += properties.limits.maxSamplerAnisotropy > 8.0f ? 100 : 0;
  score += properties.limits.maxPushConstantsSize > 128 ? 100 : 0;
  score += properties.limits.maxMemoryAllocationCount / 1000;
  score += properties.limits.maxVertexInputAttributes > 16 ? 100 : 0;

  if (has_required_extensions) {
    surface_information = renderer_vulkan_physical_device_get_surface_information(device, surface);

    if (surface_information.has_desired_format) {
      score += 10;
    }

    if (surface_information.has_desired_capabilities) {
      score += 10;
    }

    if (surface_information.has_desired_present_mode) {
      score += 10;
    }
  }

  if (!features.wideLines) {
    score = 0;
  }

  if (!features.depthBounds) {
    score = 0;
  }

  if (!features.multiViewport) {
    score = 0;
  }

  if (!features.geometryShader) {
    score = 0;
  }

  if (!features.fillModeNonSolid) {
    score = 0;
  }

  if (!features.samplerAnisotropy) {
    score = 0;
  }

  if (!features.tessellationShader) {
    score = 0;
  }

  if (!has_required_extensions) {
    score = 0;
  }

  if (!queue_family_indices.has_compute_family_index) {
    score = 0;
  }

  if (!queue_family_indices.has_present_family_index) {
    score = 0;
  }

  if (!queue_family_indices.has_graphics_family_index) {
    score = 0;
  }

  if (!queue_family_indices.has_transfer_family_index) {
    score = 0;
  }

  logger_debug("|- [ %lu ] <score:%lu> <type:%s> %s", properties.deviceID, score, device_type_string,
               properties.deviceName);

  return (physical_device_score_t){
      .score = score,
      .device = device,
      .queues = queue_family_indices,
      .features = features,
      .properties = properties,
      .device_type_string = device_type_string,
      .surface_information = surface_information,
  };
}

queue_family_indices_t renderer_vulkan_physical_device_get_queue_family_indices(VkPhysicalDevice device,
                                                                                VkSurfaceKHR surface) {
  queue_family_indices_t indices = (queue_family_indices_t){
      .compute_family_index = 0,
      .present_family_index = 0,
      .graphics_family_index = 0,
      .transfer_family_index = 0,
      .has_compute_family_index = false,
      .has_present_family_index = false,
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

  /* TODO: improve queue family selection to prioritize dedicated queues and fallback for the general one */
  for (u64 i = 0; i < families->count; i++) {
    VkQueueFamilyProperties family;

    array_retrieve(families, i, (void *)&family);

    VkBool32 has_present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &has_present_support);

    if (!indices.has_compute_family_index && family.queueFlags & VK_QUEUE_COMPUTE_BIT) {
      indices.compute_family_index = i;
      indices.has_compute_family_index = true;
    }

    if (!indices.has_present_family_index && has_present_support) {
      indices.present_family_index = i;
      indices.has_present_family_index = true;
    }

    if (!indices.has_graphics_family_index && family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphics_family_index = i;
      indices.has_graphics_family_index = true;
    }

    if (!indices.has_transfer_family_index && family.queueFlags & VK_QUEUE_TRANSFER_BIT) {
      indices.transfer_family_index = i;
      indices.has_transfer_family_index = true;
    }
  }

  array_destroy(families);

  return indices;
}

surface_information_t renderer_vulkan_physical_device_get_surface_information(VkPhysicalDevice device,
                                                                              VkSurfaceKHR surface) {
  VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
  VkSurfaceFormatKHR format;
  VkSurfaceCapabilitiesKHR capabilities;

  b8 has_desired_format = false;
  b8 has_desired_capabilities = false;
  b8 has_desired_present_mode = false;

  u32 format_count = 0;
  u32 present_mode_count = 0;

  u32 minimum_desired_images = 0;

  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, NULL);
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, NULL);
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);

  array_t *formats = array_create(format_count, sizeof(VkSurfaceFormatKHR));
  array_t *present_modes = array_create(format_count, sizeof(VkPresentModeKHR));

  formats->count = format_count;
  present_modes->count = present_mode_count;

  /* TODO: rank the available formats in case of desired format is missing */
  array_retrieve(formats, 0, (void *)&format);

  for (u64 i = 0; i < formats->count; i++) {
    VkSurfaceFormatKHR current;

    array_retrieve(formats, i, (void *)&current);

    if (current.format == VK_FORMAT_B8G8R8A8_SRGB && current.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      format = current;

      has_desired_format = true;

      break;
    }
  }

  for (u64 i = 0; i < present_modes->count; i++) {
    VkPresentModeKHR current;

    array_retrieve(present_modes, i, (void *)&current);

    if (current == VK_PRESENT_MODE_MAILBOX_KHR) {
      present_mode = current;

      has_desired_present_mode = true;

      break;
    }
  }

  minimum_desired_images = capabilities.minImageCount + MINIMUM_EXTRA_DESIRED_SURFACE_IMAGES;

  if (capabilities.maxImageCount > 0 && minimum_desired_images <= capabilities.maxImageCount) {
    has_desired_capabilities = true;
  }

  array_destroy(formats);
  array_destroy(present_modes);

  return (surface_information_t){
      .format = format,
      .capabilities = capabilities,
      .present_mode = present_mode,
      .has_desired_format = has_desired_format,
      .has_desired_capabilities = has_desired_capabilities,
      .has_desired_present_mode = has_desired_present_mode,
  };
}

b8 renderer_vulkan_device_create(renderer_t *renderer) {
  array_t *queue_create_infos = __build_queue_create_infos(renderer);

  array_t *extensions = renderer_vulkan_device_get_required_extensions();

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
      .enabledExtensionCount = extensions->count,
      .ppEnabledExtensionNames = extensions->buffer,
  };

  if (vkCreateDevice(renderer->physical_device, &device_create_info, NULL, &renderer->device) != VK_SUCCESS) {
    logger_critical("<physical_device:%p> logical device creation failed", renderer->physical_device);

    array_destroy(extensions);
    array_destroy(queue_create_infos);

    return false;
  }

  vkGetDeviceQueue(renderer->device, renderer->compute_queue_family_index, 0, &renderer->compute_queue);
  vkGetDeviceQueue(renderer->device, renderer->present_queue_family_index, 0, &renderer->present_queue);
  vkGetDeviceQueue(renderer->device, renderer->graphics_queue_family_index, 0, &renderer->graphics_queue);
  vkGetDeviceQueue(renderer->device, renderer->transfer_queue_family_index, 0, &renderer->transfer_queue);

  array_destroy(extensions);
  array_destroy(queue_create_infos);

  return true;
}

b8 renderer_vulkan_device_destroy(renderer_t *renderer) {
  vkDestroyDevice(renderer->device, NULL);

  return true;
}

b8 __queue_family_index_is_unique(u32 index, u32 others[], u8 others_count) {
  for (u8 i = 0; i < others_count; i++) {
    if (index == others[i]) {
      return false;
    }
  }

  return true;
}

b8 __physical_device_has_required_extensions(VkPhysicalDevice device, array_t *extensions) {
  u32 device_extension_property_count = 0;

  vkEnumerateDeviceExtensionProperties(device, NULL, &device_extension_property_count, NULL);

  array_t *device_extensions = array_create(device_extension_property_count, sizeof(VkExtensionProperties));
  device_extensions->count = device_extension_property_count;

  vkEnumerateDeviceExtensionProperties(device, NULL, &device_extension_property_count, device_extensions->buffer);

  for (u64 i = 0; i < extensions->count; i++) {
    b8 found = false;
    char *name;

    array_retrieve(extensions, i, (void *)&name);

    for (u64 j = 0; j < device_extensions->count; j++) {
      VkExtensionProperties properties;

      array_retrieve(device_extensions, j, (void *)&properties);

      if (string_compare_secure(properties.extensionName, name, VK_MAX_EXTENSION_NAME_SIZE)) {
        found = true;

        break;
      }
    }

    if (!found) {
      array_destroy(device_extensions);

      return false;
    }
  }

  array_destroy(device_extensions);

  return true;
}

array_t *__build_queue_create_infos(renderer_t *renderer) {
  const static u64 queue_count = 4;
  const static float queue_priority = 1.0f;

  VkDeviceQueueCreateInfo compute_queue_create_info = (VkDeviceQueueCreateInfo){
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueCount = 1,
      .queueFamilyIndex = renderer->compute_queue_family_index,
      .pQueuePriorities = &queue_priority,
  };

  VkDeviceQueueCreateInfo present_queue_create_info = (VkDeviceQueueCreateInfo){
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueCount = 1,
      .queueFamilyIndex = renderer->present_queue_family_index,
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

  u8 queue_others_count = 2;

  u32 present_queue_others[] = {renderer->compute_queue_family_index, renderer->graphics_queue_family_index,
                                renderer->transfer_queue_family_index};
  u32 graphics_queue_others[] = {renderer->compute_queue_family_index, renderer->present_queue_family_index,
                                 renderer->transfer_queue_family_index};
  u32 transfer_queue_others[] = {renderer->compute_queue_family_index, renderer->present_queue_family_index,
                                 renderer->graphics_queue_family_index};

  b8 should_include_compute_queue = true;

  b8 should_include_present_queue =
      __queue_family_index_is_unique(renderer->present_queue_family_index, present_queue_others, queue_others_count);

  b8 should_include_graphics_queue =
      __queue_family_index_is_unique(renderer->graphics_queue_family_index, graphics_queue_others, queue_others_count);

  b8 should_include_transfer_queue =
      __queue_family_index_is_unique(renderer->transfer_queue_family_index, transfer_queue_others, queue_others_count);

  if (should_include_compute_queue) {
    array_insert(queue_create_infos, &compute_queue_create_info);
  }

  if (should_include_present_queue) {
    array_insert(queue_create_infos, &present_queue_create_info);
  }

  if (should_include_graphics_queue) {
    array_insert(queue_create_infos, &graphics_queue_create_info);
  }

  if (should_include_transfer_queue) {
    array_insert(queue_create_infos, &transfer_queue_create_info);
  }

  return queue_create_infos;
}
