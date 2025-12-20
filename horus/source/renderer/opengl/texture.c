#include <ktx.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus renderer layer */
#include <horus/renderer/texture.h>

/* horus renderer layer [ opengl ]*/
#include <horus/renderer/opengl/texture.h>

#define DEFAULT_ALBEDO_SAMPLER_BINDING 2

static const u32 texture_context_bindings[TEXTURE_ROLE_COUNT] = {
    [TEXTURE_ROLE_ALBEDO] = DEFAULT_ALBEDO_SAMPLER_BINDING,
};

ktx_transcode_fmt_e __select_ktx_transcode_format(renderer_t *renderer);

/* TODO: refactor into multipe functions for usage within other types texture_context */
texture_context_t *texture_context_create(renderer_t *renderer, texture_role_t role, u8 *binary, u64 size) {
  (void)role; /* unused */

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

  *texture_context = (texture_context_t){};

  return texture_context;
}

b8 texture_context_bind(texture_context_t *context, graphics_pipeline_t *pipeline, renderer_t *renderer) {
  (void)context;  /* unused */
  (void)renderer; /* unused */
  (void)pipeline; /* unused */

  return true;
}

b8 texture_context_destroy(texture_context_t *context) {
  platform_memory_deallocate(context);

  return true;
}

ktx_transcode_fmt_e __select_ktx_transcode_format(renderer_t *renderer) {
  (void)renderer; /* unused */

  /* TODO: check if device supports BC7 */
  if (true) {
    return KTX_TTF_BC7_RGBA;
  }

  /* TODO: check if device supports ASTC */
  if (true) {
    return KTX_TTF_ASTC_4x4_RGBA;
  }

  /* TODO: check if device supports ETC2 */
  if (true) {
    return KTX_TTF_ETC2_RGBA;
  }

  return KTX_TTF_RGBA32;
}
