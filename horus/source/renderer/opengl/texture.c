#include <ktx.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus renderer layer */
#include <horus/renderer/texture.h>

/* horus renderer layer [ opengl ]*/
#include <horus/renderer/opengl/texture.h>

/* horus renderer loader layer [ opengl ]*/
#include <horus/renderer/opengl/glad/gl.h>

static const u32 texture_context_bindings[TEXTURE_ROLE_COUNT] = {
    [TEXTURE_ROLE_ALBEDO] = RENDERER_DEFAULT_ALBEDO_TEXTURE_SAMPLER_BINDING,
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

  GLenum target = 0;
  GLuint texture = 0;

  ktx_error_code_e ktx_texture_upload_result = ktxTexture_GLUpload((ktxTexture *)ktx_texture, &texture, &target, NULL);

  if (ktx_texture_upload_result != KTX_SUCCESS) {
    logger_critical_format("<renderer:%p> ktx texture upload failed", renderer);

    ktxTexture2_Destroy(ktx_texture);

    return NULL;
  }

  ktxTexture2_Destroy(ktx_texture);

  glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  /* texture mipmap bias */
  /* glTextureParameterf(texture, GL_TEXTURE_LOD_BIAS, -0.5f); */

  if (GLAD_GL_ARB_texture_filter_anisotropic) {
    GLfloat max_anisotropy = 0.0f;

    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);

    glTextureParameterf(texture, GL_TEXTURE_MAX_ANISOTROPY, max_anisotropy);
  }

  texture_context_t *texture_context = platform_memory_allocate(sizeof(texture_context_t));

  *texture_context = (texture_context_t){
      .target = target,
      .texture = texture,
      .binding = texture_context_bindings[role],
  };

  return texture_context;
}

b8 texture_context_bind(texture_context_t *context, graphics_pipeline_t *pipeline, renderer_t *renderer) {
  (void)renderer; /* unused */
  (void)pipeline; /* unused */

  glBindTextureUnit(context->binding, context->texture);

  return true;
}

b8 texture_context_destroy(texture_context_t *context) {
  glDeleteTextures(1, &context->texture);

  platform_memory_deallocate(context);

  return true;
}

ktx_transcode_fmt_e __select_ktx_transcode_format(renderer_t *renderer) {
  (void)renderer; /* unused */

  if (GLAD_GL_ARB_texture_compression_bptc) {
    return KTX_TTF_BC7_RGBA;
  }

  if (GLAD_GL_KHR_texture_compression_astc_ldr) {
    return KTX_TTF_ASTC_4x4_RGBA;
  }

  if (GLAD_GL_ARB_ES3_compatibility) {
    return KTX_TTF_ETC2_RGBA;
  }

  return KTX_TTF_BC1_OR_3;
}
