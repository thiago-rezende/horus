#include <ktx.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/filesystem.h>

/* horus containers layer */
#include <horus/containers/array.h>

/* horus renderer layer [ vulkan ]*/
#include <horus/renderer/vulkan/command.h>
#include <horus/renderer/vulkan/renderer.h>

/* horus renderer textures layer */
#include <horus/renderer/textures/texture.h>

/* horus renderer textures layer [ vulkan ] */
#include <horus/renderer/vulkan/textures/texture.h>

struct __texture {
  VkDevice device;

  VkImage image;

  VkBuffer staging;

  VkDeviceSize size;
  VkDeviceMemory memory;
  VkDeviceMemory staging_memory;
};

/* TODO: refactor into multipe functions for usage within other types texture */
texture_t *texture_create(renderer_t *renderer, u8 *binary, u64 size) {
  ktxTexture2 *ktx_texture = NULL;

  if (ktxTexture2_CreateFromMemory((ktx_uint8_t *)binary, (ktx_size_t)size, KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
                                   &ktx_texture) != KTX_SUCCESS) {
    logger_critical_format("<renderer:%p> ktx texture creation failed", renderer);

    return NULL;
  }

  logger_debug_format("<ktx_texture:%p> <width:%lu> <height:%lu> <depth:%lu> <levels:%lu> <layers:%lu>", ktx_texture,
                      ktx_texture->baseWidth, ktx_texture->baseHeight, ktx_texture->baseDepth, ktx_texture->numLevels,
                      ktx_texture->numLayers);

  /* TODO: transcode if needed for better performance and compatibility */

  u32 texture_width = ktx_texture->baseWidth;
  u32 texture_height = ktx_texture->baseHeight;
  u32 texture_depth = ktx_texture->baseDepth;

  ktx_size_t texture_size = ktxTexture_GetDataSize((ktxTexture *)ktx_texture);
  ktx_uint8_t *texture_data = ktxTexture_GetData((ktxTexture *)ktx_texture);

  texture_t *texture = platform_memory_allocate(sizeof(texture_t));

  *texture = (texture_t){
      .size = (u64)texture_size,
      .device = renderer->device,
  };

  VkImageCreateInfo image_create_info = (VkImageCreateInfo){
      .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
      .flags = (VkImageCreateFlags)0,
      .pNext = NULL,
      .imageType = texture_depth > 1 ? VK_IMAGE_TYPE_3D : VK_IMAGE_TYPE_2D,
      .extent = (VkExtent3D){texture_width, texture_height, texture_depth},
      .format = ktx_texture->vkFormat,
      .mipLevels = ktx_texture->numLevels,
      .arrayLayers = ktx_texture->numLayers,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .tiling = VK_IMAGE_TILING_OPTIMAL, /* better performance for shader access */
      .usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .queueFamilyIndexCount = 0,
      .pQueueFamilyIndices = NULL,
  };

  VkBufferCreateInfo staging_create_info = (VkBufferCreateInfo){
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .flags = (VkBufferCreateFlags)0,
      .size = texture->size,
      .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
  };

  if (renderer->graphics_queue_family_index != renderer->transfer_queue_family_index) {
    staging_create_info.sharingMode = VK_SHARING_MODE_CONCURRENT;
  }

  if (vkCreateImage(texture->device, &image_create_info, NULL, &texture->image) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> image creation failed", renderer);

    ktxTexture2_Destroy(ktx_texture);

    platform_memory_deallocate(texture);

    return NULL;
  }

  if (vkCreateBuffer(texture->device, &staging_create_info, NULL, &texture->staging) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> buffer creation failed", renderer);

    ktxTexture2_Destroy(ktx_texture);

    vkDestroyImage(texture->device, texture->image, NULL);

    platform_memory_deallocate(texture);

    return NULL;
  }

  VkMemoryRequirements image_memory_requirements;
  VkMemoryRequirements staging_memory_requirements;
  VkPhysicalDeviceMemoryProperties physical_device_memory_properties;

  VkMemoryPropertyFlags image_memory_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  VkMemoryPropertyFlags staging_memory_properties =
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

  vkGetImageMemoryRequirements(texture->device, texture->image, &image_memory_requirements);
  vkGetBufferMemoryRequirements(texture->device, texture->staging, &staging_memory_requirements);

  vkGetPhysicalDeviceMemoryProperties(renderer->physical_device, &physical_device_memory_properties);

  u32 image_memory_type_index = 0;
  u32 staging_memory_type_index = 0;

  b8 has_required_image_memory_type = false;
  b8 has_required_staging_memory_type = false;

  for (u32 i = 0; i < physical_device_memory_properties.memoryTypeCount; i++) {
    if ((image_memory_requirements.memoryTypeBits & (1 << i)) &&
        (physical_device_memory_properties.memoryTypes[i].propertyFlags & image_memory_properties) ==
            image_memory_properties) {
      image_memory_type_index = i;

      has_required_image_memory_type = true;

      break;
    }
  }

  for (u32 i = 0; i < physical_device_memory_properties.memoryTypeCount; i++) {
    if ((staging_memory_requirements.memoryTypeBits & (1 << i)) &&
        (physical_device_memory_properties.memoryTypes[i].propertyFlags & staging_memory_properties) ==
            staging_memory_properties) {
      staging_memory_type_index = i;

      has_required_staging_memory_type = true;

      break;
    }
  }

  if (!has_required_image_memory_type || !has_required_staging_memory_type) {
    logger_critical_format(
        "<renderer:%p> <texture:%p> desired memory type for image and staging buffer creation not found", renderer,
        texture);

    ktxTexture2_Destroy(ktx_texture);

    vkDestroyImage(texture->device, texture->image, NULL);
    vkDestroyBuffer(texture->device, texture->staging, NULL);

    platform_memory_deallocate(texture);

    return NULL;
  }

  VkMemoryAllocateInfo image_memory_allocate_info = (VkMemoryAllocateInfo){
      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .allocationSize = image_memory_requirements.size,
      .memoryTypeIndex = image_memory_type_index,
  };

  VkMemoryAllocateInfo staging_memory_allocate_info = (VkMemoryAllocateInfo){
      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .allocationSize = staging_memory_requirements.size,
      .memoryTypeIndex = staging_memory_type_index,
  };

  /* TODO: implement a memory allocator to allow more than 4096 allocations */
  if (vkAllocateMemory(texture->device, &image_memory_allocate_info, NULL, &texture->memory) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture:%p> image memory allocation failed", renderer, texture);

    ktxTexture2_Destroy(ktx_texture);

    vkDestroyImage(texture->device, texture->image, NULL);
    vkDestroyBuffer(texture->device, texture->staging, NULL);

    platform_memory_deallocate(texture);

    return NULL;
  }

  /* TODO: implement a memory allocator to allow more than 4096 allocations */
  if (vkAllocateMemory(texture->device, &staging_memory_allocate_info, NULL, &texture->staging_memory) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture:%p> staging buffer memory allocation failed", renderer, texture);

    ktxTexture2_Destroy(ktx_texture);

    vkFreeMemory(texture->device, texture->memory, NULL);

    vkDestroyImage(texture->device, texture->image, NULL);
    vkDestroyBuffer(texture->device, texture->staging, NULL);

    platform_memory_deallocate(texture);

    return NULL;
  }

  if (vkBindImageMemory(texture->device, texture->image, texture->memory, 0) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture:%p> image memory binding failed", renderer, texture);

    ktxTexture2_Destroy(ktx_texture);

    vkFreeMemory(texture->device, texture->memory, NULL);
    vkFreeMemory(texture->device, texture->staging_memory, NULL);

    vkDestroyImage(texture->device, texture->image, NULL);
    vkDestroyBuffer(texture->device, texture->staging, NULL);

    platform_memory_deallocate(texture);

    return NULL;
  }

  if (vkBindBufferMemory(texture->device, texture->staging, texture->staging_memory, 0) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture:%p> staging buffer memory binding failed", renderer, texture);

    ktxTexture2_Destroy(ktx_texture);

    vkFreeMemory(texture->device, texture->memory, NULL);
    vkFreeMemory(texture->device, texture->staging_memory, NULL);

    vkDestroyImage(texture->device, texture->image, NULL);
    vkDestroyBuffer(texture->device, texture->staging, NULL);

    platform_memory_deallocate(texture);

    return NULL;
  }

  void *mapped = NULL;

  if (vkMapMemory(texture->device, texture->staging_memory, 0, staging_create_info.size, 0, &mapped) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture:%p> buffer memory mapping failed", renderer, texture);

    ktxTexture2_Destroy(ktx_texture);

    vkFreeMemory(texture->device, texture->memory, NULL);
    vkFreeMemory(texture->device, texture->staging_memory, NULL);

    vkDestroyImage(texture->device, texture->image, NULL);
    vkDestroyBuffer(texture->device, texture->staging, NULL);

    platform_memory_deallocate(texture);

    return NULL;
  }

  platform_memory_copy(mapped, texture_data, (u64)texture_size);

  vkUnmapMemory(texture->device, texture->staging_memory);

  image_transition_info_t image_transition_info = (image_transition_info_t){
      .levels = (u32)ktx_texture->numLevels,
      .layers = (u32)ktx_texture->numLayers,
      .image = texture->image,
      .old_layout = VK_IMAGE_LAYOUT_UNDEFINED,
      .new_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      .source_access_mask = (VkAccessFlags2)0,
      .destination_access_mask = VK_ACCESS_TRANSFER_WRITE_BIT,
      .source_stage_mask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      .destination_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT,
  };

  if (!__renderer_vulkan_transition_image_layout(renderer, image_transition_info)) {
    logger_critical_format("<renderer:%p> <texture:%p> image transition failed", renderer, texture);

    ktxTexture2_Destroy(ktx_texture);

    vkFreeMemory(texture->device, texture->memory, NULL);
    vkFreeMemory(texture->device, texture->staging_memory, NULL);

    vkDestroyImage(texture->device, texture->image, NULL);
    vkDestroyBuffer(texture->device, texture->staging, NULL);

    return NULL;
  }

  VkCommandBuffer transfer_command_buffer =
      renderer_vulkan_command_buffer_create(texture->device, renderer->transfer_command_pool);

  VkCommandBufferBeginInfo transfer_command_buffer_begin_info = (VkCommandBufferBeginInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
  };

  if (vkBeginCommandBuffer(transfer_command_buffer, &transfer_command_buffer_begin_info) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture:%p> transfer command buffer beginning failed", renderer, texture);

    ktxTexture2_Destroy(ktx_texture);

    renderer_vulkan_command_buffer_destroy(texture->device, transfer_command_buffer, renderer->transfer_command_pool);

    vkFreeMemory(texture->device, texture->memory, NULL);
    vkFreeMemory(texture->device, texture->staging_memory, NULL);

    vkDestroyImage(texture->device, texture->image, NULL);
    vkDestroyBuffer(texture->device, texture->staging, NULL);

    return NULL;
  }

  array_t *regions = array_create(ktx_texture->numLevels, sizeof(VkBufferImageCopy));

  VkBufferImageCopy buffer_image_copy = (VkBufferImageCopy){
      /* .bufferOffset = 0, */
      .bufferRowLength = 0,
      .bufferImageHeight = 0,
      .imageSubresource =
          (VkImageSubresourceLayers){
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              /* .mipLevel = 1, */
              .baseArrayLayer = 0,
              .layerCount = ktx_texture->numLayers,
          },
      .imageOffset = (VkOffset3D){0, 0, 0},
      .imageExtent = (VkExtent3D){texture_width, texture_height, texture_depth},
  };

  for (u32 level = 0; level < (u32)ktx_texture->numLevels; level++) {
    ktx_size_t offset;

    u32 mipmap_width = texture_width >> level;
    u32 mipmap_height = texture_height >> level;

    mipmap_width = mipmap_width > 0 ? mipmap_width : 1;
    mipmap_height = mipmap_height > 0 ? mipmap_height : 1;

    ktxTexture2_GetImageOffset(ktx_texture, level, 0, 0, &offset);

    buffer_image_copy.bufferOffset = (VkDeviceSize)offset;
    buffer_image_copy.imageExtent = (VkExtent3D){mipmap_width, mipmap_height, texture_depth};
    buffer_image_copy.imageSubresource.mipLevel = level;

    array_insert(regions, &buffer_image_copy);
  }

  vkCmdCopyBufferToImage(transfer_command_buffer, texture->staging, texture->image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, regions->count, regions->buffer);

  if (vkEndCommandBuffer(transfer_command_buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture:%p> transfer command buffer ending failed", renderer, texture);

    ktxTexture2_Destroy(ktx_texture);

    renderer_vulkan_command_buffer_destroy(texture->device, transfer_command_buffer, renderer->transfer_command_pool);

    vkFreeMemory(texture->device, texture->memory, NULL);
    vkFreeMemory(texture->device, texture->staging_memory, NULL);

    vkDestroyImage(texture->device, texture->image, NULL);
    vkDestroyBuffer(texture->device, texture->staging, NULL);

    array_destroy(regions);

    return NULL;
  }

  VkSubmitInfo transfer_submit_info = (VkSubmitInfo){
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers = &transfer_command_buffer,
  };

  if (vkQueueSubmit(renderer->transfer_queue, 1, &transfer_submit_info, VK_NULL_HANDLE) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture:%p> transfer queue submission failed", renderer, texture);

    ktxTexture2_Destroy(ktx_texture);

    renderer_vulkan_command_buffer_destroy(texture->device, transfer_command_buffer, renderer->transfer_command_pool);

    vkFreeMemory(texture->device, texture->memory, NULL);
    vkFreeMemory(texture->device, texture->staging_memory, NULL);

    vkDestroyImage(texture->device, texture->image, NULL);
    vkDestroyBuffer(texture->device, texture->staging, NULL);

    array_destroy(regions);

    return NULL;
  }

  vkQueueWaitIdle(renderer->transfer_queue);

  image_transition_info.source_access_mask = VK_ACCESS_TRANSFER_WRITE_BIT;
  image_transition_info.destination_access_mask = VK_ACCESS_SHADER_READ_BIT;
  image_transition_info.source_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT;
  image_transition_info.destination_stage_mask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

  if (!__renderer_vulkan_transition_image_layout(renderer, image_transition_info)) {
    logger_critical_format("<renderer:%p> <texture:%p> image transition failed", renderer, texture);

    ktxTexture2_Destroy(ktx_texture);

    vkFreeMemory(texture->device, texture->memory, NULL);
    vkFreeMemory(texture->device, texture->staging_memory, NULL);

    vkDestroyImage(texture->device, texture->image, NULL);
    vkDestroyBuffer(texture->device, texture->staging, NULL);

    return NULL;
  }

  ktxTexture2_Destroy(ktx_texture);

  renderer_vulkan_command_buffer_destroy(texture->device, transfer_command_buffer, renderer->transfer_command_pool);

  vkFreeMemory(texture->device, texture->staging_memory, NULL);

  vkDestroyBuffer(texture->device, texture->staging, NULL);

  array_destroy(regions);

  return texture;
}

texture_t *texture_create_from_binary(renderer_t *renderer, char *path) {
  platform_file_t *binary = platform_file_open(path);

  if (binary == NULL) {
    logger_critical_format("<renderer:%p> <path:%s> texture binary opening failed", renderer, path);

    return NULL;
  }

  u64 size = platform_file_size(binary);

  u8 *buffer = platform_memory_allocate(sizeof(u8) * size);

  platform_memory_clear((void *)buffer, size);

  u64 bytes_read = platform_file_read(binary, buffer, size);

  if (bytes_read == 0) {
    logger_critical_format("<renderer:%p> <path:%s> texture binary reading failed", renderer, path);

    platform_memory_deallocate(buffer);

    platform_file_close(binary);

    return NULL;
  }

  platform_file_close(binary);

  texture_t *texture = texture_create(renderer, buffer, size);

  if (texture == NULL) {
    logger_critical_format("<renderer:%p> <path:%s> texture creation failed", renderer, path);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  platform_memory_deallocate(buffer);

  return texture;
}

b8 texture_destroy(texture_t *texture) {
  vkFreeMemory(texture->device, texture->memory, NULL);

  vkDestroyImage(texture->device, texture->image, NULL);

  return true;
}

b8 __renderer_vulkan_transition_image_layout(renderer_t *renderer, image_transition_info_t info) {
  VkCommandBuffer graphics_command_buffer =
      renderer_vulkan_command_buffer_create(renderer->device, renderer->graphics_command_pool);

  VkCommandBufferBeginInfo graphics_command_buffer_begin_info = (VkCommandBufferBeginInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
  };

  if (vkBeginCommandBuffer(graphics_command_buffer, &graphics_command_buffer_begin_info) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> graphics command buffer beginning failed", renderer,
                           graphics_command_buffer);

    renderer_vulkan_command_buffer_destroy(renderer->device, graphics_command_buffer, renderer->graphics_command_pool);

    return false;
  }

  VkImageMemoryBarrier2 barrier = (VkImageMemoryBarrier2){
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,

      .srcStageMask = info.source_stage_mask,
      .srcAccessMask = info.source_access_mask,
      .dstStageMask = info.destination_stage_mask,
      .dstAccessMask = info.destination_access_mask,
      .oldLayout = info.old_layout,
      .newLayout = info.new_layout,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, /* TODO: handle queue ownership transfer */
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, /* TODO: handle queue ownership transfer */
      .image = info.image,
      .subresourceRange =
          (VkImageSubresourceRange){
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .baseMipLevel = 0,
              .levelCount = info.levels,
              .baseArrayLayer = 0,
              .layerCount = info.layers,
          },
  };

  VkDependencyInfo dependency_info = (VkDependencyInfo){
      .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
      .dependencyFlags = (VkDependencyFlags)0,
      .imageMemoryBarrierCount = 1,
      .pImageMemoryBarriers = &barrier,
  };

  vkCmdPipelineBarrier2(graphics_command_buffer, &dependency_info);

  if (vkEndCommandBuffer(graphics_command_buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <command_buffer:%p> graphics command buffer ending failed", renderer,
                           graphics_command_buffer);

    renderer_vulkan_command_buffer_destroy(renderer->device, graphics_command_buffer, renderer->graphics_command_pool);

    return false;
  }

  VkSubmitInfo graphics_submit_info = (VkSubmitInfo){
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers = &graphics_command_buffer,
  };

  if (vkQueueSubmit(renderer->graphics_queue, 1, &graphics_submit_info, VK_NULL_HANDLE) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <queue:%p> graphics queue submission failed", renderer,
                           renderer->graphics_queue);

    renderer_vulkan_command_buffer_destroy(renderer->device, graphics_command_buffer, renderer->graphics_command_pool);

    return false;
  }

  vkQueueWaitIdle(renderer->graphics_queue);

  renderer_vulkan_command_buffer_destroy(renderer->device, graphics_command_buffer, renderer->graphics_command_pool);

  return true;
}
