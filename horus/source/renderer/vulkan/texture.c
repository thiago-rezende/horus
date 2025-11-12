#include <ktx.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/detection.h>
#include <horus/platform/filesystem.h>

/* horus containers layer */
#include <horus/containers/array.h>

/* horus renderer layer */
#include <horus/renderer/texture.h>

/* horus renderer layer [ vulkan ]*/
#include <horus/renderer/vulkan/image.h>
#include <horus/renderer/vulkan/command.h>
#include <horus/renderer/vulkan/texture.h>
#include <horus/renderer/vulkan/pipeline.h>
#include <horus/renderer/vulkan/renderer.h>
#include <horus/renderer/vulkan/descriptors.h>

#define DEFAULT_DIFFUSE_SAMPLER_BINDING 2

static const u32 texture_context_bindings[TEXTURE_ROLE_COUNT] = {
    [TEXTURE_ROLE_DIFFUSE] = DEFAULT_DIFFUSE_SAMPLER_BINDING,
};

ktx_transcode_fmt_e __select_ktx_transcode_format(renderer_t *renderer);

/* TODO: refactor into multipe functions for usage within other types texture_context */
texture_context_t *texture_context_create(renderer_t *renderer, texture_role_t role, u8 *binary, u64 size) {
  ktxTexture2 *ktx_texture = NULL;

  if (ktxTexture2_CreateFromMemory((ktx_uint8_t *)binary, (ktx_size_t)size, KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
                                   &ktx_texture) != KTX_SUCCESS) {
    logger_critical_format("<renderer:%p> ktx texture_context creation failed", renderer);

    return NULL;
  }

  if (ktxTexture2_NeedsTranscoding(ktx_texture)) {
    ktx_transcode_fmt_e transcode_format = __select_ktx_transcode_format(renderer);

    if (ktxTexture2_TranscodeBasis(ktx_texture, transcode_format, 0) != KTX_SUCCESS) {
      logger_critical_format("<renderer:%p> ktx texture_context transcoding failed", renderer);

      ktxTexture2_Destroy(ktx_texture);

      return NULL;
    }
  }

  u32 texture_width = ktx_texture->baseWidth;
  u32 texture_height = ktx_texture->baseHeight;
  u32 texture_depth = ktx_texture->baseDepth;

  ktx_size_t texture_size = ktxTexture_GetDataSize((ktxTexture *)ktx_texture);
  ktx_uint8_t *texture_data = ktxTexture_GetData((ktxTexture *)ktx_texture);

  texture_context_t *texture_context = platform_memory_allocate(sizeof(texture_context_t));

  *texture_context = (texture_context_t){
      .size = (u64)texture_size,
      .device = renderer->device,
      .binding = renderer_vulkan_texture_binding(role),
  };

  VkImageCreateInfo image_create_info = (VkImageCreateInfo){
      .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
      .flags = (VkImageCreateFlags)0,
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
      .pNext = NULL,
  };

  VkBufferCreateInfo staging_create_info = (VkBufferCreateInfo){
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .flags = (VkBufferCreateFlags)0,
      .size = texture_context->size,
      .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
  };

  if (renderer->graphics_queue_family_index != renderer->transfer_queue_family_index) {
    staging_create_info.sharingMode = VK_SHARING_MODE_CONCURRENT;
  }

  if (vkCreateImage(texture_context->device, &image_create_info, NULL, &texture_context->image) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> image creation failed", renderer);

    ktxTexture2_Destroy(ktx_texture);

    platform_memory_deallocate(texture_context);

    return NULL;
  }

  if (vkCreateBuffer(texture_context->device, &staging_create_info, NULL, &texture_context->staging) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> buffer creation failed", renderer);

    ktxTexture2_Destroy(ktx_texture);

    vkDestroyImage(texture_context->device, texture_context->image, NULL);

    platform_memory_deallocate(texture_context);

    return NULL;
  }

  VkMemoryRequirements image_memory_requirements;
  VkMemoryRequirements staging_memory_requirements;
  VkPhysicalDeviceMemoryProperties physical_device_memory_properties;

  VkMemoryPropertyFlags image_memory_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  VkMemoryPropertyFlags staging_memory_properties =
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

  vkGetImageMemoryRequirements(texture_context->device, texture_context->image, &image_memory_requirements);
  vkGetBufferMemoryRequirements(texture_context->device, texture_context->staging, &staging_memory_requirements);

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
        "<renderer:%p> <texture_context:%p> desired memory type for image and staging buffer creation not found",
        renderer, texture_context);

    ktxTexture2_Destroy(ktx_texture);

    vkDestroyImage(texture_context->device, texture_context->image, NULL);
    vkDestroyBuffer(texture_context->device, texture_context->staging, NULL);

    platform_memory_deallocate(texture_context);

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
  if (vkAllocateMemory(texture_context->device, &image_memory_allocate_info, NULL, &texture_context->memory) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture_context:%p> image memory allocation failed", renderer,
                           texture_context);

    ktxTexture2_Destroy(ktx_texture);

    vkDestroyImage(texture_context->device, texture_context->image, NULL);
    vkDestroyBuffer(texture_context->device, texture_context->staging, NULL);

    platform_memory_deallocate(texture_context);

    return NULL;
  }

  /* TODO: implement a memory allocator to allow more than 4096 allocations */
  if (vkAllocateMemory(texture_context->device, &staging_memory_allocate_info, NULL,
                       &texture_context->staging_memory) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture_context:%p> staging buffer memory allocation failed", renderer,
                           texture_context);

    ktxTexture2_Destroy(ktx_texture);

    vkFreeMemory(texture_context->device, texture_context->memory, NULL);

    vkDestroyImage(texture_context->device, texture_context->image, NULL);
    vkDestroyBuffer(texture_context->device, texture_context->staging, NULL);

    platform_memory_deallocate(texture_context);

    return NULL;
  }

  if (vkBindImageMemory(texture_context->device, texture_context->image, texture_context->memory, 0) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture_context:%p> image memory binding failed", renderer, texture_context);

    ktxTexture2_Destroy(ktx_texture);

    vkFreeMemory(texture_context->device, texture_context->memory, NULL);
    vkFreeMemory(texture_context->device, texture_context->staging_memory, NULL);

    vkDestroyImage(texture_context->device, texture_context->image, NULL);
    vkDestroyBuffer(texture_context->device, texture_context->staging, NULL);

    platform_memory_deallocate(texture_context);

    return NULL;
  }

  if (vkBindBufferMemory(texture_context->device, texture_context->staging, texture_context->staging_memory, 0) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture_context:%p> staging buffer memory binding failed", renderer,
                           texture_context);

    ktxTexture2_Destroy(ktx_texture);

    vkFreeMemory(texture_context->device, texture_context->memory, NULL);
    vkFreeMemory(texture_context->device, texture_context->staging_memory, NULL);

    vkDestroyImage(texture_context->device, texture_context->image, NULL);
    vkDestroyBuffer(texture_context->device, texture_context->staging, NULL);

    platform_memory_deallocate(texture_context);

    return NULL;
  }

  void *mapped = NULL;

  if (vkMapMemory(texture_context->device, texture_context->staging_memory, 0, staging_create_info.size, 0, &mapped) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture_context:%p> buffer memory mapping failed", renderer,
                           texture_context);

    ktxTexture2_Destroy(ktx_texture);

    vkFreeMemory(texture_context->device, texture_context->memory, NULL);
    vkFreeMemory(texture_context->device, texture_context->staging_memory, NULL);

    vkDestroyImage(texture_context->device, texture_context->image, NULL);
    vkDestroyBuffer(texture_context->device, texture_context->staging, NULL);

    platform_memory_deallocate(texture_context);

    return NULL;
  }

  platform_memory_copy(mapped, texture_data, (u64)texture_size);

  vkUnmapMemory(texture_context->device, texture_context->staging_memory);

  image_transition_info_t image_transition_info = (image_transition_info_t){
      .levels = (u32)ktx_texture->numLevels,
      .layers = (u32)ktx_texture->numLayers,
      .image = texture_context->image,
      .old_layout = VK_IMAGE_LAYOUT_UNDEFINED,
      .new_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      .aspect_mask = VK_IMAGE_ASPECT_COLOR_BIT,
      .source_access_mask = (VkAccessFlags2)0,
      .destination_access_mask = VK_ACCESS_TRANSFER_WRITE_BIT,
      .source_stage_mask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      .destination_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT,
  };

  if (!renderer_vulkan_image_transition_layout(renderer, image_transition_info)) {
    logger_critical_format("<renderer:%p> <texture_context:%p> image transition failed", renderer, texture_context);

    ktxTexture2_Destroy(ktx_texture);

    vkFreeMemory(texture_context->device, texture_context->memory, NULL);
    vkFreeMemory(texture_context->device, texture_context->staging_memory, NULL);

    vkDestroyImage(texture_context->device, texture_context->image, NULL);
    vkDestroyBuffer(texture_context->device, texture_context->staging, NULL);

    platform_memory_deallocate(texture_context);

    return NULL;
  }

  VkCommandBuffer transfer_command_buffer =
      renderer_vulkan_command_buffer_create(texture_context->device, renderer->transfer_command_pool);

  VkCommandBufferBeginInfo transfer_command_buffer_begin_info = (VkCommandBufferBeginInfo){
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
  };

  if (vkBeginCommandBuffer(transfer_command_buffer, &transfer_command_buffer_begin_info) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture_context:%p> transfer command buffer beginning failed", renderer,
                           texture_context);

    ktxTexture2_Destroy(ktx_texture);

    renderer_vulkan_command_buffer_destroy(texture_context->device, transfer_command_buffer,
                                           renderer->transfer_command_pool);

    vkFreeMemory(texture_context->device, texture_context->memory, NULL);
    vkFreeMemory(texture_context->device, texture_context->staging_memory, NULL);

    vkDestroyImage(texture_context->device, texture_context->image, NULL);
    vkDestroyBuffer(texture_context->device, texture_context->staging, NULL);

    platform_memory_deallocate(texture_context);

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

  vkCmdCopyBufferToImage(transfer_command_buffer, texture_context->staging, texture_context->image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, regions->count, regions->buffer);

  if (vkEndCommandBuffer(transfer_command_buffer) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture_context:%p> transfer command buffer ending failed", renderer,
                           texture_context);

    ktxTexture2_Destroy(ktx_texture);

    renderer_vulkan_command_buffer_destroy(texture_context->device, transfer_command_buffer,
                                           renderer->transfer_command_pool);

    vkFreeMemory(texture_context->device, texture_context->memory, NULL);
    vkFreeMemory(texture_context->device, texture_context->staging_memory, NULL);

    vkDestroyImage(texture_context->device, texture_context->image, NULL);
    vkDestroyBuffer(texture_context->device, texture_context->staging, NULL);

    array_destroy(regions);

    platform_memory_deallocate(texture_context);

    return NULL;
  }

  VkSubmitInfo transfer_submit_info = (VkSubmitInfo){
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers = &transfer_command_buffer,
  };

  if (vkQueueSubmit(renderer->transfer_queue, 1, &transfer_submit_info, VK_NULL_HANDLE) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture_context:%p> transfer queue submission failed", renderer,
                           texture_context);

    ktxTexture2_Destroy(ktx_texture);

    renderer_vulkan_command_buffer_destroy(texture_context->device, transfer_command_buffer,
                                           renderer->transfer_command_pool);

    vkFreeMemory(texture_context->device, texture_context->memory, NULL);
    vkFreeMemory(texture_context->device, texture_context->staging_memory, NULL);

    vkDestroyImage(texture_context->device, texture_context->image, NULL);
    vkDestroyBuffer(texture_context->device, texture_context->staging, NULL);

    array_destroy(regions);

    platform_memory_deallocate(texture_context);

    return NULL;
  }

  vkQueueWaitIdle(renderer->transfer_queue);

  image_transition_info.old_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  image_transition_info.new_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  image_transition_info.source_access_mask = VK_ACCESS_TRANSFER_WRITE_BIT;
  image_transition_info.destination_access_mask = VK_ACCESS_SHADER_READ_BIT;
  image_transition_info.source_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT;
  image_transition_info.destination_stage_mask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

  if (!renderer_vulkan_image_transition_layout(renderer, image_transition_info)) {
    logger_critical_format("<renderer:%p> <texture_context:%p> image transition failed", renderer, texture_context);

    ktxTexture2_Destroy(ktx_texture);

    vkFreeMemory(texture_context->device, texture_context->memory, NULL);
    vkFreeMemory(texture_context->device, texture_context->staging_memory, NULL);

    vkDestroyImage(texture_context->device, texture_context->image, NULL);
    vkDestroyBuffer(texture_context->device, texture_context->staging, NULL);

    array_destroy(regions);

    platform_memory_deallocate(texture_context);

    return NULL;
  }

  renderer_vulkan_command_buffer_destroy(texture_context->device, transfer_command_buffer,
                                         renderer->transfer_command_pool);

  vkFreeMemory(texture_context->device, texture_context->staging_memory, NULL);

  vkDestroyBuffer(texture_context->device, texture_context->staging, NULL);

  array_destroy(regions);

  VkImageViewCreateInfo image_view_create_info = (VkImageViewCreateInfo){
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .flags = (VkImageViewCreateFlags)0,
      .image = texture_context->image,
      .viewType = texture_depth > 1 ? VK_IMAGE_VIEW_TYPE_3D : VK_IMAGE_VIEW_TYPE_2D,
      .format = ktx_texture->vkFormat,
      .subresourceRange =
          (VkImageSubresourceRange){
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .baseMipLevel = 0,
              .levelCount = ktx_texture->numLevels,
              .baseArrayLayer = 0,
              .layerCount = ktx_texture->numLayers,
          },
      .pNext = NULL,
  };

  if (vkCreateImageView(texture_context->device, &image_view_create_info, NULL, &texture_context->image_view) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <texture_context:%p> image view creation failed", renderer, texture_context);

    ktxTexture2_Destroy(ktx_texture);

    vkFreeMemory(texture_context->device, texture_context->memory, NULL);

    vkDestroyImage(texture_context->device, texture_context->image, NULL);

    platform_memory_deallocate(texture_context);

    return NULL;
  }

  VkSamplerCreateInfo sampler_create_info = (VkSamplerCreateInfo){
      .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
      .magFilter = VK_FILTER_LINEAR,
      .minFilter = VK_FILTER_LINEAR,
      .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
      .mipLodBias = 0.0f,
      .minLod = 0.0f,
      .maxLod = ktx_texture->numLevels - 1.0f,
      .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT, /* TODO: control with user choice */
      .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT, /* TODO: control with user choice */
      .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT, /* TODO: control with user choice */
      .anisotropyEnable = renderer->physical_device_features.samplerAnisotropy,
      .maxAnisotropy = renderer->physical_device_properties.limits.maxSamplerAnisotropy,
      .compareEnable = VK_FALSE,
      .compareOp = VK_COMPARE_OP_ALWAYS,
      .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
      .unnormalizedCoordinates = VK_FALSE, /* ! always using normalized texture_context coordinates */
  };

  if (vkCreateSampler(texture_context->device, &sampler_create_info, NULL, &texture_context->sampler) != VK_SUCCESS) {
    ktxTexture2_Destroy(ktx_texture);

    vkFreeMemory(texture_context->device, texture_context->memory, NULL);

    vkDestroyImage(texture_context->device, texture_context->image, NULL);
    vkDestroyImageView(texture_context->device, texture_context->image_view, NULL);

    platform_memory_deallocate(texture_context);

    return NULL;
  }

  return texture_context;
}

b8 texture_context_bind(texture_context_t *context, graphics_pipeline_t *pipeline, renderer_t *renderer) {
  VkDescriptorSet descriptor_set;
  VkCommandBuffer graphics_command_buffer;

  array_retrieve(pipeline->descriptor_sets, renderer->current_frame_in_flight_index, &descriptor_set);
  array_retrieve(renderer->graphics_command_buffers, renderer->current_frame_in_flight_index, &graphics_command_buffer);

  renderer_vulkan_descriptor_set_update_sampler(renderer->device, descriptor_set,
                                                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, context->binding,
                                                context->sampler, context->image_view);

  vkCmdBindDescriptorSets(graphics_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout, 0, 1,
                          &descriptor_set, 0, NULL);

  return true;
}

b8 texture_context_destroy(texture_context_t *context) {
  vkDestroySampler(context->device, context->sampler, NULL);

  vkDestroyImageView(context->device, context->image_view, NULL);

  vkFreeMemory(context->device, context->memory, NULL);

  vkDestroyImage(context->device, context->image, NULL);

  platform_memory_deallocate(context);

  return true;
}

u32 renderer_vulkan_texture_binding(texture_role_t role) {
  if (role < TEXTURE_ROLE_COUNT && role >= TEXTURE_ROLE_DIFFUSE) {
    return texture_context_bindings[role];
  }

  return max_u32;
}

ktx_transcode_fmt_e __select_ktx_transcode_format(renderer_t *renderer) {
  if (renderer->physical_device_features.textureCompressionBC) {
    return KTX_TTF_BC7_RGBA;
  }

  if (renderer->physical_device_features.textureCompressionASTC_LDR) {
    return KTX_TTF_ASTC_4x4_RGBA;
  }

  if (renderer->physical_device_features.textureCompressionETC2) {
    return KTX_TTF_ETC2_RGBA;
  }

  return KTX_TTF_RGBA32;
}
