#pragma once

#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>

/* horus platform layer */
#include <horus/platform/window.h>

struct __platform_window_context {
  xcb_connection_t *connection;
  xcb_screen_t *screen;
  xcb_window_t window;

  xcb_key_symbols_t *keysyms;
};
