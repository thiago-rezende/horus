#include <ktx.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/filesystem.h>

/* horus containers layer */
#include <horus/containers/array.h>

/* horus renderer layer */
#include <horus/renderer/texture.h>

static const char *texture_role_strings[TEXTURE_ROLE_COUNT] = {
    [TEXTURE_ROLE_ALBEDO] = "albedo",
};

texture_t *texture_create(renderer_t *renderer, texture_role_t role, u8 *binary, u64 size) {
  texture_t *texture = platform_memory_allocate(sizeof(texture_t));

  *texture = (texture_t){
      .role = role,
      .context = texture_context_create(renderer, role, binary, size),
  };

  if (!texture->context) {
    logger_critical_format("<renderer:%p> texture context creation failed", renderer);

    platform_memory_deallocate(texture);

    return NULL;
  }

  return texture;
}

texture_t *texture_create_from_binary(renderer_t *renderer, texture_role_t role, char *path) {
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

  texture_t *texture = texture_create(renderer, role, buffer, size);

  if (texture == NULL) {
    logger_critical_format("<renderer:%p> <path:%s> texture creation failed", renderer, path);

    platform_memory_deallocate(buffer);

    return NULL;
  }

  platform_memory_deallocate(buffer);

  return texture;
}

b8 texture_bind(texture_t *texture, graphics_pipeline_t *pipeline, renderer_t *renderer) {
  return texture_context_bind(texture->context, pipeline, renderer);
}

b8 texture_destroy(texture_t *texture) {
  if (!texture_context_destroy(texture->context)) {
    logger_critical_format("<texture:%p> texture context destruction failed", texture);

    platform_memory_deallocate(texture);

    return false;
  }

  platform_memory_deallocate(texture);

  return true;
}

const char *texture_role_string(texture_role_t role) {
  if (role < TEXTURE_ROLE_COUNT && role >= TEXTURE_ROLE_ALBEDO) {
    return texture_role_strings[role];
  }

  return "unknown";
}
