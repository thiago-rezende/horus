/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/shader.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/filesystem.h>

shader_module_t *shader_module_create(renderer_t *renderer, shader_module_stage_t stages, u8 *code, u64 size) {
  shader_module_t *module = platform_memory_allocate(sizeof(shader_module_t));

  *module = (shader_module_t){
      .device = renderer->device,
      .module = VK_NULL_HANDLE,

      .vertex_stage_create_info = (VkPipelineShaderStageCreateInfo){0},
      .compute_stage_create_info = (VkPipelineShaderStageCreateInfo){0},
      .fragment_stage_create_info = (VkPipelineShaderStageCreateInfo){0},

      .has_vertex_stage_create_info = false,
      .has_compute_stage_create_info = false,
      .has_fragment_stage_create_info = false,
  };

  VkShaderModuleCreateInfo shader_module_create_info = (VkShaderModuleCreateInfo){
      .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
      .codeSize = size * sizeof(u8),
      .pCode = (const u32 *)code,
  };

  if (vkCreateShaderModule(module->device, &shader_module_create_info, NULL, &module->module) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <device:%p> shader module creation failed", renderer, module->device);

    platform_memory_deallocate(module);

    return NULL;
  }

  VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info = (VkPipelineShaderStageCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .module = module->module,
  };

  if (stages & SHADER_MODULE_STAGE_VERTEX) {
    pipeline_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    pipeline_shader_stage_create_info.pName = RENDERER_DEFAULT_SHADER_MODULE_VERTEX_STAGE_ENTRYPOINT;

    module->vertex_stage_create_info = pipeline_shader_stage_create_info;
    module->has_vertex_stage_create_info = true;
  }

  if (stages & SHADER_MODULE_STAGE_COMPUTE) {
    pipeline_shader_stage_create_info.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    pipeline_shader_stage_create_info.pName = RENDERER_DEFAULT_SHADER_MODULE_COMPUTE_STAGE_ENTRYPOINT;

    module->compute_stage_create_info = pipeline_shader_stage_create_info;
    module->has_compute_stage_create_info = true;
  }

  if (stages & SHADER_MODULE_STAGE_FRAGMENT) {
    pipeline_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    pipeline_shader_stage_create_info.pName = RENDERER_DEFAULT_SHADER_MODULE_FRAGMENT_STAGE_ENTRYPOINT;

    module->fragment_stage_create_info = pipeline_shader_stage_create_info;
    module->has_fragment_stage_create_info = true;
  }

  return module;
}

shader_module_t *shader_module_create_from_binary(renderer_t *renderer, shader_module_stage_t stages, char *path) {
  platform_file_t *binary = platform_file_open(path);

  if (binary == NULL) {
    logger_critical_format("<renderer:%p> <path:%s> shader module opening failed", renderer, path);

    return NULL;
  }

  u64 size = platform_file_size(binary);

  u8 *buffer = platform_memory_allocate(sizeof(u8) * size);

  platform_memory_clear((void *)buffer, size);

  u64 bytes_read = platform_file_read(binary, buffer, size);

  if (bytes_read == 0) {
    logger_critical_format("<renderer:%p> <path:%s> shader module reading failed", renderer, path);

    platform_memory_deallocate(buffer);

    platform_file_close(binary);

    return NULL;
  }

  platform_file_close(binary);

  shader_module_t *module = shader_module_create(renderer, stages, buffer, size);

  if (module == NULL) {
    logger_critical_format("<renderer:%p> <path:%s> shader module creation failed", renderer, path);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  platform_memory_deallocate(buffer);

  return module;
}

b8 shader_module_destroy(shader_module_t *module) {
  vkDestroyShaderModule(module->device, module->module, NULL);

  platform_memory_deallocate(module);

  return true;
}

array_t *renderer_vulkan_shader_module_stage_create_infos(shader_module_t *module) {
  array_t *shader_stage_create_infos =
      array_create(SHADER_MODULE_MAXIMUM_STAGE_COUNT, sizeof(VkPipelineShaderStageCreateInfo));

  if (module->has_vertex_stage_create_info) {
    array_insert(shader_stage_create_infos, &module->vertex_stage_create_info);
  }

  if (module->has_compute_stage_create_info) {
    array_insert(shader_stage_create_infos, &module->compute_stage_create_info);
  }

  if (module->has_fragment_stage_create_info) {
    array_insert(shader_stage_create_infos, &module->fragment_stage_create_info);
  }

  return shader_stage_create_infos;
}
