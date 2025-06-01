#include <stddef.h>
#include <stdlib.h>

/* horus base layer */
#include <horus/definitions.h>

/* horus core layer */
#include <horus/core/assert.h>
#include <horus/core/strings.h>

/* horus events layer */
#include <horus/events/base.h>
#include <horus/events/mouse.h>
#include <horus/events/keyboard.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/window.h>

/* horus input layer [ windows ] */
#include <horus/platform/windows/input/mouse.h>
#include <horus/platform/windows/input/keyboard.h>

struct __platform_window {
  platform_window_size_t size;

  b8 fullscreen;
  b8 has_focus;
  b8 has_resized;
  b8 should_close;

  platform_window_event_callback_t on_event;
};

platform_window_t *__window = NULL;

b8 __window_resized_state = false;

platform_window_t *platform_window_create(char *title, platform_window_size_t size, b8 fullscreen) {
  platform_window_t *window = platform_memory_allocate(sizeof(platform_window_t));

  logger_debug("<window:%p> created", window);

  return window;
}

b8 platform_window_destroy(platform_window_t *window) {
  platform_memory_deallocate(window);

  logger_debug("<window:%p> destroyed", window);

  return true;
}

b8 platform_window_process_events(platform_window_t *window) {
  return true;
}

b8 platform_window_should_close(platform_window_t *window) {
  return window->should_close;
}

b8 platform_window_has_focus(platform_window_t *window) {
  return window->has_focus;
}

b8 platform_window_has_resized(platform_window_t *window) {
  return window->has_resized;
}

platform_window_size_t platform_window_size(platform_window_t *window) {
  return window->size;
}

b8 platform_window_set_size(platform_window_t *window, platform_window_size_t size) {
  return true;
}

b8 platform_window_set_title(platform_window_t *window, char *title) {
  return true;
}

b8 platform_window_is_fullscreen(platform_window_t *window) {
  return window->fullscreen;
}

b8 platform_window_set_fullscreen(platform_window_t *window, b8 fullscreen) {
  return true;
}

b8 platform_window_set_event_callback(platform_window_t *window, platform_window_event_callback_t callback) {
  window->on_event = callback;

  return true;
}

platform_window_t *platform_window(void) {
  return __window;
}

b8 __platform_window_set_global_instance(platform_window_t *window) {
  if (!__window && window) {
    __window = window;

    return true;
  }

  return false;
}

b8 __platform_window_resized_clear_state(platform_window_t *window) {
  window->has_resized = false;

  return true;
}
