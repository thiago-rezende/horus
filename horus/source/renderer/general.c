/* horus renderer layer */
#include <horus/renderer/general.h>

const char *renderer_implementation_strings[RENDERER_IMPLEMENTATION_COUNT] = {
    [RENDERER_IMPLEMENTATION_NONE] = "none",
    [RENDERER_IMPLEMENTATION_VULKAN] = "vulkan",
};

const char *__renderer_implementation_string(renderer_implementation_t renderer_implementation) {
  if (renderer_implementation < RENDERER_IMPLEMENTATION_COUNT &&
      renderer_implementation >= RENDERER_IMPLEMENTATION_NONE) {
    return renderer_implementation_strings[renderer_implementation];
  }

  return "unknown";
}
