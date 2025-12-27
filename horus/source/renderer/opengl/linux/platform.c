#include <ktx.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/window.h>

/* horus platform layer [ linux ] */
#include <horus/platform/linux/window.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/debug.h>
#include <horus/renderer/opengl/renderer.h>
#include <horus/renderer/opengl/platform.h>

/* horus renderer loader layer [ opengl ] */
#include <horus/renderer/opengl/glad/gl.h>
#include <horus/renderer/opengl/glad/egl.h>

struct __platform_renderer_context {
  EGLDisplay display;
  EGLSurface *surface;
  EGLContext *context;
};

/* TODO: imporve for multiple windows support */
b8 renderer_opengl_context_create(renderer_t *renderer, platform_window_t *window) {
  if (!gladLoaderLoadEGL(NULL)) {
    logger_critical_format("<renderer:%p> <window:%p> gladLoaderLoadEGL failed", renderer, window);

    return false;
  }

  platform_window_context_t *window_context = platform_window_context(window);

  const EGLint display_attributes[] = {
      EGL_PLATFORM_XCB_SCREEN_EXT,
      0, /* TODO: fetch prefered screenp from xcb */
      EGL_NONE,
  };

  EGLDisplay display = eglGetPlatformDisplayEXT(EGL_PLATFORM_XCB_EXT, window_context->connection, display_attributes);

  if (display == EGL_NO_DISPLAY) {
    logger_critical_format("<renderer:%p> <window:%p> eglGetPlatformDisplayEXT failed", renderer, window);

    return false;
  }

  EGLint major;
  EGLint minor;

  if (!eglInitialize(display, &major, &minor)) {
    logger_critical_format("<renderer:%p> <window:%p> eglInitialize failed", renderer, window);

    return false;
  }

  if (major < 1 || (major == 1 && minor < 5)) {
    logger_critical_format("<renderer:%p> <window:%p> <version:%d.%d> EGL version 1.5 or higher is required", renderer,
                           window);

    return false;
  }

  if (!gladLoaderLoadEGL(display)) {
    logger_critical_format("<renderer:%p> <window:%p> gladLoaderLoadEGL with display failed", renderer, window);

    return false;
  }

  if (!eglBindAPI(EGL_OPENGL_API)) {
    logger_critical_format("<renderer:%p> <window:%p> eglBindAPI failed", renderer, window);

    gladLoaderUnloadEGL();

    return false;
  }

  EGLConfig config;
  EGLint config_count;

  /* clang-format off */
  EGLint config_attributes[] = {
      EGL_SURFACE_TYPE,      EGL_WINDOW_BIT,
      EGL_CONFORMANT,        EGL_OPENGL_BIT,
      EGL_RENDERABLE_TYPE,   EGL_OPENGL_BIT,
      EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,
      EGL_BUFFER_SIZE,       32,
      EGL_RED_SIZE,          8,
      EGL_GREEN_SIZE,        8,
      EGL_BLUE_SIZE,         8,
      EGL_ALPHA_SIZE,        8,
      EGL_DEPTH_SIZE,        24,
      EGL_STENCIL_SIZE,      8,
      /* multismaple            */
      /* EGL_SAMPLE_BUFFERS, 1, */
      /* EGL_SAMPLES,        4, */
      EGL_NONE,
  };
  /* clang-format on */

  if (!eglChooseConfig(display, config_attributes, &config, 1, &config_count) || config_count != 1) {
    logger_critical_format("<renderer:%p> <window:%p> eglChooseConfig failed", renderer, window);

    gladLoaderUnloadEGL();

    return false;
  }

  EGLSurface *surface = NULL;

  /* clang-format off */
  EGLint surface_attributes[] = {
    EGL_GL_COLORSPACE, EGL_GL_COLORSPACE_SRGB,
    EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
    EGL_NONE,
  };
  /* clang-format on */

  surface = eglCreateWindowSurface(display, config, window_context->window, surface_attributes);

  if (surface == EGL_NO_SURFACE) {
    logger_critical_format("<renderer:%p> <window:%p> eglCreateWindowSurface failed", renderer, window);

    gladLoaderUnloadEGL();

    return false;
  }

  EGLContext *context = NULL;

  /* clang-format off */
  EGLint context_attributes[] = {
    EGL_CONTEXT_MAJOR_VERSION, 4,
    EGL_CONTEXT_MINOR_VERSION, 6,
    EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
    EGL_CONTEXT_OPENGL_DEBUG, EGL_TRUE,
    EGL_NONE
  };
  /* clang-format on */

  context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);

  if (context == EGL_NO_CONTEXT) {
    logger_critical_format("<renderer:%p> <window:%p> eglCreateContext failed", renderer, window);

    eglDestroySurface(display, surface);

    gladLoaderUnloadEGL();

    return false;
  }

  if (!eglMakeCurrent(display, surface, surface, context)) {
    logger_critical_format("<renderer:%p> <window:%p> eglMakeCurrent failed", renderer, window);

    eglDestroyContext(display, context);

    eglDestroySurface(display, surface);

    gladLoaderUnloadEGL();

    return false;
  }

  if (!gladLoaderLoadGL()) {
    logger_critical_format("<renderer:%p> <window:%p> eglMakeCurrent failed", renderer, window);

    eglDestroyContext(display, context);

    eglDestroySurface(display, surface);

    gladLoaderUnloadEGL();

    return false;
  }

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(renderer_opengl_debug_callback, renderer);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

  glEnable(GL_DEPTH_TEST);

  eglSwapInterval(display, true);

  ktx_error_code_e ktx_load_opengl_result = ktxLoadOpenGL((PFNGLGETPROCADDRESS)eglGetProcAddress);

  if (ktx_load_opengl_result != KTX_SUCCESS) {
    logger_warning_format("<ktx> <eglGetProcAddress:%p> OpenGL functions loading failed", eglGetProcAddress);
  }

  renderer->context->platform_context = platform_memory_allocate(sizeof(platform_renderer_context_t));

  *renderer->context->platform_context = (platform_renderer_context_t){
      .display = display,
      .surface = surface,
      .context = context,
  };

  return true;
}

/* TODO: imporve for multiple windows support */
b8 renderer_opengl_context_destroy(renderer_t *renderer) {
  platform_renderer_context_t *platform_renderer_context = renderer->context->platform_context;

  eglDestroyContext(platform_renderer_context->display, platform_renderer_context->context);

  eglDestroySurface(platform_renderer_context->display, platform_renderer_context->surface);

  gladLoaderUnloadEGL();

  platform_memory_deallocate(platform_renderer_context);

  return true;
}

/* TODO: imporve for multiple windows support */
b8 renderer_opengl_context_make_current(renderer_t *renderer) {
  platform_renderer_context_t *platform_renderer_context = renderer->context->platform_context;

  if (!eglMakeCurrent(platform_renderer_context->display, platform_renderer_context->surface,
                      platform_renderer_context->surface, platform_renderer_context->context)) {
    logger_critical_format("<renderer:%p> eglMakeCurrent failed", renderer);

    return false;
  }

  return true;
}

/* TODO: imporve for multiple windows support */
b8 renderer_opengl_context_swap_buffers(renderer_t *renderer) {
  platform_renderer_context_t *platform_renderer_context = renderer->context->platform_context;

  if (!eglSwapBuffers(platform_renderer_context->display, platform_renderer_context->surface)) {
    logger_critical_format("<renderer:%p> eglSwapBuffers failed", renderer);

    return false;
  }

  return true;
}
