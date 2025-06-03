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

/* horus platform layer [ windows ] */
#include <horus/platform/windows/windows.h>

/* horus input layer [ windows ] */
#include <horus/platform/windows/input/mouse.h>
#include <horus/platform/windows/input/keyboard.h>

#define WINDOW_TITLE_MAX_LENGTH 255

struct __platform_window {
  HINSTANCE hinstance;

  HWND window;
  WNDCLASSEX window_class;
  LPCSTR window_class_name;

  platform_window_size_t size;

  b8 fullscreen;
  b8 has_focus;
  b8 has_resized;
  b8 should_close;

  platform_window_event_callback_t on_event;
};

platform_window_t *__window = NULL;

b8 __window_resized_state = false;

static LRESULT CALLBACK windows_handle_event(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK windows_handle_event_setup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

platform_window_t *platform_window_create(char *title, platform_window_size_t size, b8 fullscreen) {
  platform_window_t *window = platform_memory_allocate(sizeof(platform_window_t));

  window->hinstance = GetModuleHandle(NULL);
  window->window_class_name = "horus_window_class";

  RECT rectangle = {0};
  rectangle.top = 0;
  rectangle.left = 0;
  rectangle.right = size.width;
  rectangle.bottom = size.height;

  AdjustWindowRectEx(&rectangle, WS_OVERLAPPEDWINDOW, 0, WS_EX_OVERLAPPEDWINDOW | WS_EX_ACCEPTFILES);

  window->window_class.cbSize = sizeof(window->window_class);
  window->window_class.style = CS_HREDRAW | CS_VREDRAW;
  window->window_class.lpfnWndProc = windows_handle_event_setup;
  window->window_class.cbClsExtra = 0;
  window->window_class.cbWndExtra = 0;
  window->window_class.hInstance = window->hinstance;
  window->window_class.hIcon = NULL;
  window->window_class.hCursor = NULL;
  window->window_class.hbrBackground = GetStockObject(BLACK_BRUSH);
  window->window_class.lpszMenuName = NULL;
  window->window_class.lpszClassName = window->window_class_name;
  window->window_class.hIconSm = NULL;

  RegisterClassEx(&window->window_class);

  window->window = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW | WS_EX_ACCEPTFILES, /* dwExStyle */
                                  window->window_class_name,                  /* lpClassName */
                                  title,                                      /* lpWindowName */
                                  WS_OVERLAPPEDWINDOW,                        /* dwStyle */
                                  CW_USEDEFAULT,                              /* X */
                                  CW_USEDEFAULT,                              /* Y */
                                  rectangle.right - rectangle.left,           /* nWidth */
                                  rectangle.bottom - rectangle.top,           /* nHeight */
                                  NULL,                                       /* hWndParent */
                                  NULL,                                       /* hMenu */
                                  window->hinstance,                          /* hInstance */
                                  window                                      /* lpParam */
  );

  ShowWindow(window->window, SW_SHOWDEFAULT);

  window->size = size;
  window->fullscreen = fullscreen;
  window->has_focus = false;
  window->has_resized = false;
  window->should_close = false;

  logger_debug("<window:%p> <win32_window:%p> created", window, window->window);

  return window;
}

b8 platform_window_destroy(platform_window_t *window) {
  UnregisterClass(window->window_class_name, window->hinstance);

  DestroyWindow(window->window);

  logger_debug("<window:%p> <win32_window:%p> destroyed", window, window->window);

  platform_memory_deallocate(window);

  return true;
}

b8 platform_window_process_events(platform_window_t *window) {
  __platform_window_resized_clear_state(window);

  __platform_input_mouse_button_clear_state();
  __platform_input_mouse_scroll_clear_state();
  __platform_input_mouse_position_clear_state();
  __platform_input_keyboard_keycode_clear_state();

  MSG event;

  while (PeekMessage(&event, NULL, 0, 0, PM_REMOVE)) {
    if (event.message == WM_QUIT) {
      window->should_close = true;
    }

    TranslateMessage(&event);
    DispatchMessage(&event);
  }

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
  RECT rectangle = {0};
  rectangle.top = 0;
  rectangle.left = 0;
  rectangle.right = size.width;
  rectangle.bottom = size.height;

  AdjustWindowRectEx(&rectangle, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW | WS_EX_ACCEPTFILES);

  if (!SetWindowPos(window->window, 0, 0, 0, rectangle.right - rectangle.left, rectangle.bottom - rectangle.top,
                    SWP_NOMOVE | SWP_NOZORDER)) {
    logger_error("<window:%p> <win32_window:%p> platform_window_set_size failed", window, window->window);

    return false;
  }

  window->size = size;

  window->has_resized = true;

  return true;
}

b8 platform_window_set_title(platform_window_t *window, char *title) {
  if (!SetWindowText(window->window, title)) {
    logger_error("<window:%p> <win32_window:%p> platform_window_set_title failed", window, window->window);

    return false;
  }

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

static LRESULT CALLBACK windows_handle_event_setup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if (msg == WM_NCCREATE) {
    const CREATESTRUCT *const pCreate = (CREATESTRUCT *)lParam;

    platform_window_t *window = (platform_window_t *)pCreate->lpCreateParams;

    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);

    SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)&windows_handle_event);

    return windows_handle_event(hWnd, msg, wParam, lParam);
  }

  return DefWindowProc(hWnd, msg, wParam, lParam);
}

static LRESULT CALLBACK windows_handle_event(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  platform_window_t *window = (platform_window_t *)GetWindowLongPtr(hWnd, GWLP_USERDATA);

  switch (msg) {
    case WM_CLOSE: {
      PostQuitMessage(0);

      break;
    }

    case WM_PAINT: {
      break;
    }

    case WM_SIZE: {
      u16 width = (u16)LOWORD(lParam);
      u16 height = (u16)HIWORD(lParam);

      window->size.width = width;
      window->size.height = height;

      window->has_resized = true;

      break;
    }

    case WM_SETFOCUS: {
      window->has_focus = true;

      break;
    }

    case WM_KILLFOCUS: {
      window->has_focus = false;

      break;
    }
  }

  return DefWindowProc(hWnd, msg, wParam, lParam);
}
