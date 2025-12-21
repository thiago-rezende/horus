/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/window.h>

/* horus platform layer [ windows ] */
#include <horus/platform/windows/window.h>
#include <horus/platform/windows/windows.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/renderer.h>
#include <horus/renderer/opengl/platform.h>

/* horus renderer loader layer [ opengl ] */
#include <horus/renderer/opengl/glad/gl.h>
#include <horus/renderer/opengl/glad/wgl.h>

struct __platform_renderer_context {
  HDC device_context_handle;

  HGLRC opengl_rendering_context_handle;
  HGLRC opengl_rendering_context_handle_modern;
};

/* TODO: imporve for multiple windows support */
b8 renderer_opengl_context_create(renderer_t *renderer, platform_window_t *window) {
  platform_window_context_t *window_context = platform_window_context(window);

  HDC device_context_handle = GetDC(window_context->window);

  PIXELFORMATDESCRIPTOR pixel_format_descriptor = {
      .nSize = sizeof(PIXELFORMATDESCRIPTOR),
      .nVersion = 1,
      .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
      .iPixelType = PFD_TYPE_RGBA,
      .cColorBits = 32,
      .cRedBits = 8,
      .cRedShift = 0,
      .cGreenBits = 8,
      .cGreenShift = 0,
      .cBlueBits = 8,
      .cBlueShift = 0,
      .cAlphaBits = 8,
      .cAlphaShift = 0,
      .cAccumBits = 0,
      .cAccumRedBits = 0,
      .cAccumGreenBits = 0,
      .cAccumBlueBits = 0,
      .cAccumAlphaBits = 0,
      .cDepthBits = 24,
      .cStencilBits = 8,
      .cAuxBuffers = 0,
      .iLayerType = PFD_MAIN_PLANE,
      .bReserved = 0,
      .dwLayerMask = 0,
      .dwVisibleMask = 0,
      .dwDamageMask = 0,
  };

  int pixel_format = ChoosePixelFormat(device_context_handle, &pixel_format_descriptor);

  if (pixel_format == 0) {
    logger_critical_format("<renderer:%p> <window:%p> ChoosePixelFormat failed", renderer, window);

    ReleaseDC(window_context->window, device_context_handle);

    return false;
  }

  if (SetPixelFormat(device_context_handle, pixel_format, &pixel_format_descriptor) == FALSE) {
    logger_critical_format("<renderer:%p> <window:%p> <format:%d> SetPixelFormat failed", renderer, window,
                           pixel_format);

    ReleaseDC(window_context->window, device_context_handle);

    return false;
  }

  HGLRC opengl_rendering_context_handle = wglCreateContext(device_context_handle);

  if (opengl_rendering_context_handle == NULL) {
    logger_critical_format("<renderer:%p> <window:%p> wglCreateContext failed", renderer, window);

    ReleaseDC(window_context->window, device_context_handle);

    return false;
  }

  wglMakeCurrent(device_context_handle, opengl_rendering_context_handle);

  if (!gladLoaderLoadWGL(device_context_handle)) {
    logger_critical_format("<renderer:%p> <window:%p> gladLoaderLoadWGL failed", renderer, window);

    wglDeleteContext(opengl_rendering_context_handle);

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
  /* clang-format off */

  HGLRC opengl_rendering_context_handle_modern =
      wglCreateContextAttribsARB(device_context_handle, 0, opengl_context_attrubutes);

  if (opengl_rendering_context_handle_modern == NULL) {
    logger_critical_format("<renderer:%p> <window:%p> wglCreateContext failed", renderer, window);

    ReleaseDC(window_context->window, device_context_handle);

    return false;
  }

  wglMakeCurrent(device_context_handle, opengl_rendering_context_handle_modern);

  if (!gladLoaderLoadGL()) {
    logger_critical_format("<renderer:%p> <window:%p> gladLoaderLoadGL failed", renderer, window);

    wglDeleteContext(opengl_rendering_context_handle);
    wglDeleteContext(opengl_rendering_context_handle_modern);

    ReleaseDC(window_context->window, device_context_handle);

    return false;
  }

  /* TODO: setup debug message callback */

  glEnable(GL_DEPTH_TEST);

  wglSwapIntervalEXT(true);

  renderer->context->platform_context = platform_memory_allocate(sizeof(platform_renderer_context_t));

  *renderer->context->platform_context = (platform_renderer_context_t){
      .device_context_handle = device_context_handle,
      .opengl_rendering_context_handle = opengl_rendering_context_handle,
      .opengl_rendering_context_handle_modern = opengl_rendering_context_handle_modern,
  };

  return true;
}

/* TODO: imporve for multiple windows support */
b8 renderer_opengl_context_destroy(renderer_t *renderer) {
  platform_window_t *window = platform_window();
  platform_window_context_t *window_context = platform_window_context(window);

  platform_renderer_context_t *platform_renderer_context = renderer->context->platform_context;

  /* TODO: investigate unresolved external symbol */
  /* gladUnloadWGL(); */

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(platform_renderer_context->opengl_rendering_context_handle);
  wglDeleteContext(platform_renderer_context->opengl_rendering_context_handle_modern);

  ReleaseDC(window_context->window, platform_renderer_context->device_context_handle);

  platform_memory_deallocate(renderer->context->platform_context);

  return true;
}

b8 renderer_opengl_context_make_current(renderer_t *renderer) {
  platform_renderer_context_t *platform_renderer_context = renderer->context->platform_context;

  wglMakeCurrent(platform_renderer_context->device_context_handle,
                 platform_renderer_context->opengl_rendering_context_handle_modern);

  return true;
}
b8 renderer_opengl_context_swap_buffers(renderer_t *renderer) {
  platform_renderer_context_t *platform_renderer_context = renderer->context->platform_context;

  SwapBuffers(platform_renderer_context->device_context_handle);

  return true;
}
