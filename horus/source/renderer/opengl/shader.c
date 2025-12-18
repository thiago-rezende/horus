/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/shader.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/filesystem.h>

shader_module_t *shader_module_create(renderer_t *renderer, shader_stage_flags_t stages, u8 *code, u64 size) {
  (void)code;   /* unused */
  (void)size;   /* unused */
  (void)stages; /* unused */

  shader_module_t *module = platform_memory_allocate(sizeof(shader_module_t));

  *module = (shader_module_t){};

  return module;
}

shader_module_t *shader_module_create_from_binary(renderer_t *renderer, shader_stage_flags_t stages, char *path) {
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
  platform_memory_deallocate(module);

  return true;
}
