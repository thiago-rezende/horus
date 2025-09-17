#pragma once

/* horus platform layer [ windows ] */
#include <horus/platform/windows/windows.h>

/* horus platform layer */
#include <horus/platform/window.h>

struct __platform_window_context {
  HINSTANCE hinstance;

  HWND window;

  WNDCLASSEX window_class;
  LPCSTR window_class_name;
};
