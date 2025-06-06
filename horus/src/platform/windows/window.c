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

RECT __window_rect_before_fullscreen = {0};
DWORD __window_style_before_fullscreen = 0;
DWORD __window_exstyle_before_fullscreen = 0;

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

  AdjustWindowRectEx(&rectangle, WS_OVERLAPPEDWINDOW, 0, WS_EX_APPWINDOW);

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

  window->window = CreateWindowEx(WS_EX_APPWINDOW,                  /* dwExStyle */
                                  window->window_class_name,        /* lpClassName */
                                  title,                            /* lpWindowName */
                                  WS_OVERLAPPEDWINDOW,              /* dwStyle */
                                  CW_USEDEFAULT,                    /* X */
                                  CW_USEDEFAULT,                    /* Y */
                                  rectangle.right - rectangle.left, /* nWidth */
                                  rectangle.bottom - rectangle.top, /* nHeight */
                                  NULL,                             /* hWndParent */
                                  NULL,                             /* hMenu */
                                  window->hinstance,                /* hInstance */
                                  window                            /* lpParam */
  );

  ShowWindow(window->window, SW_SHOWDEFAULT);

  window->size = size;
  window->has_focus = false;
  window->has_resized = false;
  window->should_close = false;

  if (fullscreen) {
    platform_window_set_fullscreen(window, fullscreen);
  }

  window->fullscreen = fullscreen;

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

    if (__platform_input_mouse_button_pressed(event.message)) {
      mouse_button_press_event_t mouse_button_press_event = {
          .base = (event_t){.type = EVENT_TYPE_MOUSE_BUTTON_PRESS},
          .button = __platform_input_mouse_button(event.message, event.wParam),
          .position =
              (mouse_position_t){
                  .x = (u16)GET_X_LPARAM(event.lParam),
                  .y = (u16)GET_Y_LPARAM(event.lParam),
              },
      };

      if (!__platform_input_mouse_button_set_state(mouse_button_press_event.button, MOUSE_BUTTON_STATE_PRESSED)) {
        logger_error("<window:%p> <state:%s> __platform_input_mouse_button_set_state failed", window,
                     input_mouse_button_state_string(MOUSE_BUTTON_STATE_PRESSED));
      }

      if (window->on_event) {
        if (!window->on_event((event_t *)&mouse_button_press_event)) {
          logger_error("<window:%p> <on_event:%p> <type:%s> failed", window, window->on_event,
                       events_type_string(mouse_button_press_event.base.type));
        }
      }
    }

    if (__platform_input_mouse_button_released(event.message)) {
      mouse_button_release_event_t mouse_button_release_event = {
          .base = (event_t){.type = EVENT_TYPE_MOUSE_BUTTON_RELEASE},
          .button = __platform_input_mouse_button(event.message, event.wParam),
          .position =
              (mouse_position_t){
                  .x = (u16)GET_X_LPARAM(event.lParam),
                  .y = (u16)GET_Y_LPARAM(event.lParam),
              },
      };

      if (!__platform_input_mouse_button_set_state(mouse_button_release_event.button, MOUSE_BUTTON_STATE_RELEASED)) {
        logger_error("<window:%p> <state:%s> __platform_input_mouse_button_set_state failed", window,
                     input_mouse_button_state_string(MOUSE_BUTTON_STATE_RELEASED));
      }

      if (window->on_event) {
        if (!window->on_event((event_t *)&mouse_button_release_event)) {
          logger_error("<window:%p> <on_event:%p> <type:%s> failed", window, window->on_event,
                       events_type_string(mouse_button_release_event.base.type));
        }
      }
    }

    if (event.message == WM_MOUSEMOVE) {
      mouse_move_event_t mouse_move_event = {0};

      mouse_move_event.base = (event_t){
          .type = EVENT_TYPE_MOUSE_MOVE,
      };

      mouse_move_event.position.x = (u16)GET_X_LPARAM(event.lParam);
      mouse_move_event.position.y = (u16)GET_Y_LPARAM(event.lParam);

      mouse_position_t current_position = __platform_input_mouse_current_position();

      if (!__platform_input_mouse_set_previous_position(current_position)) {
        logger_error("<window:%p> <position:(%u, %u)> __platform_input_mouse_set_previous_position failed", window,
                     mouse_move_event.position.x, mouse_move_event.position.y);
      }

      if (!__platform_input_mouse_set_current_position(mouse_move_event.position)) {
        logger_error("<window:%p> <position:(%u, %u)> __platform_input_mouse_set_current_position failed", window,
                     mouse_move_event.position.x, mouse_move_event.position.y);
      }

      if (window->on_event) {
        if (!window->on_event((event_t *)&mouse_move_event)) {
          logger_error("<window:%p> <on_event:%p> <type:%s> failed", window, window->on_event,
                       events_type_string(mouse_move_event.base.type));
        }
      }
    }

    if (event.message == WM_MOUSEWHEEL) {
      mouse_scroll_event_t mouse_scroll_event = {0};

      mouse_scroll_event.base = (event_t){
          .type = EVENT_TYPE_MOUSE_SCROLL,
      };

      mouse_scroll_event.direction = __platform_input_mouse_scroll_direction(event.wParam);

      mouse_scroll_event.position.x = (u16)GET_X_LPARAM(event.lParam);
      mouse_scroll_event.position.y = (u16)GET_Y_LPARAM(event.lParam);

      mouse_scroll_state_t mouse_scroll_state =
          __platform_input_mouse_scroll_direction_to_state(mouse_scroll_event.direction);

      if (!__platform_input_mouse_scroll_set_state(mouse_scroll_state)) {
        logger_error("<window:%p> <state:%s> __platform_input_mouse_scroll_set_state failed", window,
                     input_mouse_scroll_state_string(mouse_scroll_state));
      }

      if (window->on_event) {
        if (!window->on_event((event_t *)&mouse_scroll_event)) {
          logger_error("<window:%p> <on_event:%p> <type:%s> failed", window, window->on_event,
                       events_type_string(mouse_scroll_event.base.type));
        }
      }
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
  if (window->fullscreen) {
    return true;
  }

  RECT rectangle = {0};
  rectangle.top = 0;
  rectangle.left = 0;
  rectangle.right = size.width;
  rectangle.bottom = size.height;

  AdjustWindowRectEx(&rectangle, WS_OVERLAPPEDWINDOW, false, WS_EX_APPWINDOW);

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
  if (window->fullscreen == fullscreen) {
    return true;
  }

  if (fullscreen) {
    GetWindowRect(window->window, &__window_rect_before_fullscreen);
    __window_style_before_fullscreen = GetWindowLong(window->window, GWL_STYLE);
    __window_exstyle_before_fullscreen = GetWindowLong(window->window, GWL_EXSTYLE);

    MONITORINFO monitor_info = {sizeof(MONITORINFO)};

    GetMonitorInfo(MonitorFromWindow(window->window, MONITOR_DEFAULTTOPRIMARY), &monitor_info);

    SetWindowLong(window->window, GWL_STYLE, GetWindowLong(window->window, GWL_STYLE) & ~(WS_OVERLAPPEDWINDOW));
    SetWindowLong(window->window, GWL_EXSTYLE, GetWindowLong(window->window, GWL_EXSTYLE) & ~(WS_EX_OVERLAPPEDWINDOW));

    SetWindowPos(window->window, HWND_TOP, monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
                 monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
                 monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
  } else {
    SetWindowLong(window->window, GWL_STYLE, __window_style_before_fullscreen);
    SetWindowLong(window->window, GWL_EXSTYLE, __window_exstyle_before_fullscreen);

    SetWindowPos(window->window, HWND_NOTOPMOST, __window_rect_before_fullscreen.left,
                 __window_rect_before_fullscreen.top,
                 __window_rect_before_fullscreen.right - __window_rect_before_fullscreen.left,
                 __window_rect_before_fullscreen.bottom - __window_rect_before_fullscreen.top,
                 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
  }

  window->fullscreen = fullscreen;

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
