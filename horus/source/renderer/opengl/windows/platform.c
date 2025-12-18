
/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer [ windows ] */
#include <horus/platform/windows/window.h>
#include <horus/platform/windows/windows.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/platform.h>

b8 renderer_opengl_context_create(renderer_t *renderer, platform_window_t *window) {
  (void)window;   /* unused */
  (void)renderer; /* unused */

  return true;
}
b8 renderer_opengl_context_destroy(renderer_t *renderer) {
  (void)renderer; /* unused */

  return true;
}
