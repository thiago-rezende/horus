#include <ktx.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/window.h>

/* horus platform layer [ windows ] */
#include <horus/platform/windows/window.h>
#include <horus/platform/windows/windows.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/debug.h>
#include <horus/renderer/opengl/renderer.h>
#include <horus/renderer/opengl/platform.h>

/* horus renderer loader layer [ opengl ] */
#include <horus/renderer/opengl/glad/gl.h>
#include <horus/renderer/opengl/glad/wgl.h>
#include <wingdi.h>

b8 __renderer_opengl_create_placeholder_context_and_load_wgl(renderer_t *renderer);

struct __platform_renderer_context {
  HDC device_context_handle;

  HGLRC opengl_rendering_context_handle;
};

/* TODO: imporve for multiple windows support */
b8 renderer_opengl_context_create(renderer_t *renderer, platform_window_t *window) {
  if (!__renderer_opengl_create_placeholder_context_and_load_wgl(renderer)) {
    return false;
  }

  platform_window_context_t *window_context = platform_window_context(window);

  HDC device_context_handle = GetDC(window_context->window);

  /* clang-format off */
  int wgl_pixel_format_attributes[] = {
    WGL_DRAW_TO_WINDOW_ARB,    GL_TRUE,
    WGL_SUPPORT_OPENGL_ARB,    GL_TRUE,
    WGL_DOUBLE_BUFFER_ARB,     GL_TRUE,
    /* multisample                      */
    /* WGL_SAMPLE_BUFFERS_ARB, GL_TRUE, */
    /* WGL_SAMPLES_ARB,        4,       */
    WGL_ACCELERATION_ARB,      WGL_FULL_ACCELERATION_ARB,
    WGL_PIXEL_TYPE_ARB,        WGL_TYPE_RGBA_ARB,
    WGL_COLOR_BITS_ARB,        32,
    WGL_RED_BITS_ARB,          8,
    WGL_GREEN_BITS_ARB,        8,
    WGL_BLUE_BITS_ARB,         8,
    WGL_DEPTH_BITS_ARB,        24,
    WGL_STENCIL_BITS_ARB,      8,
    0,
  };
  /* clang-format on */

  int wgl_pixel_format = 0;

  unsigned int wgl_pixel_format_count = 0;

  /* clang-format off */
  if (!wglChoosePixelFormatARB(device_context_handle, wgl_pixel_format_attributes, NULL, 1, &wgl_pixel_format, &wgl_pixel_format_count) || wgl_pixel_format_count != 1) {
    logger_critical_format("<renderer:%p> <window:%p> wglChoosePixelFormatARB failed", renderer, window);

    ReleaseDC(window_context->window, device_context_handle);
  }
  /* clang-format on */

  if (!SetPixelFormat(device_context_handle, wgl_pixel_format, NULL)) {
    logger_critical_format("<renderer:%p> <window:%p> <format:%d> SetPixelFormat failed", renderer, window,
                           wgl_pixel_format);

    ReleaseDC(window_context->window, device_context_handle);

    return false;
  }

  /* clang-format off */
  int opengl_context_attrubutes[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
    0,
  };
  /* clang-format on */

  HGLRC opengl_rendering_context_handle =
      wglCreateContextAttribsARB(device_context_handle, 0, opengl_context_attrubutes);

  if (opengl_rendering_context_handle == NULL) {
    logger_critical_format("<renderer:%p> <window:%p> wglCreateContext failed", renderer, window);

    ReleaseDC(window_context->window, device_context_handle);

    return false;
  }

  wglMakeCurrent(device_context_handle, opengl_rendering_context_handle);

  if (!gladLoaderLoadGL()) {
    logger_critical_format("<renderer:%p> <window:%p> gladLoaderLoadGL failed", renderer, window);

    wglDeleteContext(opengl_rendering_context_handle);

    ReleaseDC(window_context->window, device_context_handle);

    return false;
  }

  glEnable(GL_FRAMEBUFFER_SRGB);

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(renderer_opengl_debug_callback, renderer);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

  glEnable(GL_DEPTH_TEST);

  wglSwapIntervalEXT(true);

  ktx_error_code_e ktx_load_opengl_result = ktxLoadOpenGL((PFNGLGETPROCADDRESS)wglGetProcAddress);

  if (ktx_load_opengl_result != KTX_SUCCESS) {
    logger_warning_format("<ktx> <wglGetProcAddress:%p> OpenGL functions loading failed", wglGetProcAddress);
  }

  renderer->context->platform_context = platform_memory_allocate(sizeof(platform_renderer_context_t));

  *renderer->context->platform_context = (platform_renderer_context_t){
      .device_context_handle = device_context_handle,
      .opengl_rendering_context_handle = opengl_rendering_context_handle,
  };

  return true;
}

/* TODO: imporve for multiple windows support */
b8 renderer_opengl_context_destroy(renderer_t *renderer) {
  platform_window_t *window = platform_window_get_global_instance();
  platform_window_context_t *window_context = platform_window_context(window);

  platform_renderer_context_t *platform_renderer_context = renderer->context->platform_context;

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(platform_renderer_context->opengl_rendering_context_handle);

  gladLoaderUnloadGL();

  ReleaseDC(window_context->window, platform_renderer_context->device_context_handle);

  platform_memory_deallocate(renderer->context->platform_context);

  return true;
}

b8 renderer_opengl_context_make_current(renderer_t *renderer) {
  platform_renderer_context_t *platform_renderer_context = renderer->context->platform_context;

  wglMakeCurrent(platform_renderer_context->device_context_handle,
                 platform_renderer_context->opengl_rendering_context_handle);

  return true;
}
b8 renderer_opengl_context_swap_buffers(renderer_t *renderer) {
  platform_renderer_context_t *platform_renderer_context = renderer->context->platform_context;

  SwapBuffers(platform_renderer_context->device_context_handle);

  return true;
}

b8 __renderer_opengl_create_placeholder_context_and_load_wgl(renderer_t *renderer) {
  HWND placeholder_window = CreateWindowExA(0, "horus_placeholder_window_class", "placeholder_window",
                                            WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);

  HDC placeholder_device_context_handle = GetDC(placeholder_window);

  PIXELFORMATDESCRIPTOR placeholder_pixel_format_descriptor = {
      .nSize = sizeof(PIXELFORMATDESCRIPTOR),
      .nVersion = 1,
      .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
      .iPixelType = PFD_TYPE_RGBA,
      .cColorBits = 32,
  };

  int placeholder_pixel_format =
      ChoosePixelFormat(placeholder_device_context_handle, &placeholder_pixel_format_descriptor);

  /* clang-format off */
  if (!SetPixelFormat(placeholder_device_context_handle, placeholder_pixel_format, &placeholder_pixel_format_descriptor)) {
    logger_critical_format("<renderer:%p> <placeholder_window:%p> <format:%d> SetPixelFormat failed", renderer,
                           placeholder_window, placeholder_pixel_format);

    ReleaseDC(placeholder_window, placeholder_device_context_handle);

    DestroyWindow(placeholder_window);

    return false;
  }
  /* clang-format no */

  HGLRC placeholder_context = wglCreateContext(placeholder_device_context_handle);

  wglMakeCurrent(placeholder_device_context_handle, placeholder_context);

  if (!gladLoaderLoadWGL(placeholder_device_context_handle)) {
    logger_critical_format("<renderer:%p> <window:%p> gladLoaderLoadWGL failed", renderer);

    ReleaseDC(placeholder_window, placeholder_device_context_handle);

    DestroyWindow(placeholder_window);

    return false;
  }

  ReleaseDC(placeholder_window, placeholder_device_context_handle);

  DestroyWindow(placeholder_window);

  return true;
}
