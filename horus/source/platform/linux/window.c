#include <stddef.h>
#include <stdlib.h>

#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>

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

/* horus platform layer [ linux ] */
#include <horus/platform/linux/window.h>

/* horus input layer [ linux ] */
#include <horus/platform/linux/input/mouse.h>
#include <horus/platform/linux/input/keyboard.h>

#define XCB_ATOM_MAX_LENGTH 255

#define WINDOW_TITLE_MAX_LENGTH 255

#define PLATFORM_MOUSE_SCROLL_UP_BUTTON 4
#define PLATFORM_MOUSE_SCROLL_DOWN_BUTTON 5
#define PLATFORM_MOUSE_SCROLL_LEFT_BUTTON 6
#define PLATFORM_MOUSE_SCROLL_RIGHT_BUTTON 7

typedef struct __platform_window_atoms {
  xcb_atom_t WM_PROTOCOLS;
  xcb_atom_t WM_DELETE_WINDOW;
  xcb_atom_t _NET_WM_STATE;
  xcb_atom_t _NET_WM_STATE_ADD;
  xcb_atom_t _NET_WM_STATE_REMOVE;
  xcb_atom_t _NET_WM_STATE_FULLSCREEN;
} platform_window_atoms_t;

static platform_window_atoms_t global_platform_window_atoms = {0};

struct __platform_window {
  platform_window_context_t context;

  platform_window_size_t size;

  b8 fullscreen;
  b8 has_focus;
  b8 has_resized;
  b8 should_close;

  platform_window_event_callback_t on_event;
};

platform_window_t *__window = NULL;

b8 __window_resized_state = false;

b8 __platform_window_fetch_atoms(xcb_connection_t *connection);

b8 __platform_window_setup_xkb_extension(xcb_connection_t *connection);

b8 __platform_window_is_mouse_button_scroll(xcb_button_t button);

platform_window_t *platform_window_create(char *title, platform_window_size_t size, b8 fullscreen) {
  platform_window_t *window = platform_memory_allocate(sizeof(platform_window_t));

  window->on_event = NULL;

  window->context.connection = xcb_connect(NULL, NULL);

  logger_debug_format("<window:%p> <xcb_connection:%p> connected", (void *)window, (void *)window->context.connection);

  const xcb_setup_t *screen_setup = xcb_get_setup(window->context.connection);
  xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(screen_setup);

  window->context.screen = screen_iterator.data;

  window->context.window = xcb_generate_id(window->context.connection);

  uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

  int value_list[2];

  value_list[0] = window->context.screen->black_pixel;
  value_list[1] = XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS |
                  XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW |
                  XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
                  XCB_EVENT_MASK_FOCUS_CHANGE;

  xcb_create_window(window->context.connection,          /* connection */
                    XCB_COPY_FROM_PARENT,                /* depth */
                    window->context.window,              /* window identifier */
                    window->context.screen->root,        /* parent window */
                    0, 0,                                /* x, y */
                    size.width, size.height,             /* width, height */
                    0,                                   /* border width */
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,       /* window class */
                    window->context.screen->root_visual, /* window visual */
                    value_mask,                          /* masks */
                    value_list                           /* masks */
  );

  __platform_window_fetch_atoms(window->context.connection);

  xcb_change_property(window->context.connection, XCB_PROP_MODE_REPLACE, window->context.window,
                      global_platform_window_atoms.WM_PROTOCOLS, 4, 32, 1,
                      &global_platform_window_atoms.WM_DELETE_WINDOW);

  xcb_change_property(window->context.connection, XCB_PROP_MODE_REPLACE, window->context.window, XCB_ATOM_WM_NAME,
                      XCB_ATOM_STRING, 8, string_length_secure(title, WINDOW_TITLE_MAX_LENGTH), title);

  if (fullscreen) {
    xcb_change_property(window->context.connection, XCB_PROP_MODE_REPLACE, window->context.window,
                        global_platform_window_atoms._NET_WM_STATE, XCB_ATOM_ATOM, 32, 1,
                        &global_platform_window_atoms._NET_WM_STATE_FULLSCREEN);
  }

  xcb_map_window(window->context.connection, window->context.window);

  window->context.keysyms = xcb_key_symbols_alloc(window->context.connection);

  xcb_flush(window->context.connection);

  window->size = size;
  window->fullscreen = fullscreen;
  window->has_focus = false;
  window->has_resized = false;
  window->should_close = false;

  logger_debug_format("<window:%p> <xcb_window:%u> created", (void *)window, window->context.window);

  return window;
}

b8 platform_window_destroy(platform_window_t *window) {
  xcb_destroy_window(window->context.connection, window->context.window);

  logger_debug_format("<window:%p> <xcb_window:%u> destroyed", (void *)window, window->context.window);

  xcb_disconnect(window->context.connection);

  logger_debug_format("<window:%p> <xcb_connection:%p> disconnected", (void *)window,
                      (void *)window->context.connection);

  xcb_key_symbols_free(window->context.keysyms);

  logger_debug_format("<window:%p> <xcb_key_symbols:%p> destroyed", (void *)window, (void *)window->context.keysyms);

  platform_memory_deallocate(window);

  return true;
}

b8 platform_window_process_events(platform_window_t *window) {
  xcb_generic_event_t *event = NULL;

  __platform_window_resized_clear_state(window);

  __platform_input_mouse_button_clear_state();
  __platform_input_mouse_scroll_clear_state();
  __platform_input_mouse_position_clear_state();
  __platform_input_keyboard_keycode_clear_state();

  while ((event = xcb_poll_for_event(window->context.connection))) {
    switch (event->response_type & ~0x80) {
      case XCB_EXPOSE: {
        xcb_expose_event_t *expose_event = (xcb_expose_event_t *)event;
        (void)expose_event;

        break;
      }

      case XCB_BUTTON_PRESS: {
        xcb_button_press_event_t *button_press_event = (xcb_button_press_event_t *)event;
        (void)button_press_event;

        /* ignore press event for left and right mouse scroll */
        if (button_press_event->detail == PLATFORM_MOUSE_SCROLL_LEFT_BUTTON ||
            button_press_event->detail == PLATFORM_MOUSE_SCROLL_RIGHT_BUTTON) {
          break;
        }

        if (__platform_window_is_mouse_button_scroll(button_press_event->detail)) {
          mouse_scroll_event_t mouse_scroll_event = {0};

          mouse_scroll_event.base = (event_t){
              .type = EVENT_TYPE_MOUSE_SCROLL,
          };

          mouse_scroll_event.direction = __platform_input_mouse_scroll_direction(button_press_event->detail);

          mouse_scroll_event.position.x = button_press_event->event_x;
          mouse_scroll_event.position.y = button_press_event->event_y;

          mouse_scroll_state_t mouse_scroll_state =
              __platform_input_mouse_scroll_direction_to_state(mouse_scroll_event.direction);

          if (!__platform_input_mouse_scroll_set_state(mouse_scroll_state)) {
            logger_error_format("<window:%p> <state:%s> __platform_input_mouse_scroll_set_state failed", (void *)window,
                                input_mouse_scroll_state_string(mouse_scroll_state));
          }

          if (window->on_event) {
            if (!window->on_event((event_t *)&mouse_scroll_event)) {
              logger_error_format("<window:%p> <on_event:%p> <type:%s> failed", (void *)window, window->on_event,
                                  events_type_string(mouse_scroll_event.base.type));
            }
          }

          break;
        }

        mouse_button_press_event_t mouse_button_press_event = {0};

        mouse_button_press_event.base = (event_t){
            .type = EVENT_TYPE_MOUSE_BUTTON_PRESS,
        };

        mouse_button_press_event.button = __platform_input_mouse_button(button_press_event->detail);

        mouse_button_press_event.position.x = button_press_event->event_x;
        mouse_button_press_event.position.y = button_press_event->event_y;

        if (!__platform_input_mouse_button_set_state(mouse_button_press_event.button, MOUSE_BUTTON_STATE_PRESSED)) {
          logger_error_format("<window:%p> <state:%s> __platform_input_mouse_button_set_state failed", (void *)window,
                              input_mouse_button_state_string(MOUSE_BUTTON_STATE_PRESSED));
        }

        if (window->on_event) {
          if (!window->on_event((event_t *)&mouse_button_press_event)) {
            logger_error_format("<window:%p> <on_event:%p> <type:%s> failed", (void *)window, window->on_event,
                                events_type_string(mouse_button_press_event.base.type));
          }
        }

        break;
      }

      case XCB_BUTTON_RELEASE: {
        xcb_button_release_event_t *button_release_event = (xcb_button_release_event_t *)event;
        (void)button_release_event;

        if (__platform_window_is_mouse_button_scroll(button_release_event->detail)) {
          if (!__platform_input_mouse_scroll_set_state(MOUSE_SCROLL_STATE_NONE)) {
            logger_error_format("<window:%p> <state:%s> __platform_input_mouse_scroll_set_state failed", (void *)window,
                                input_mouse_scroll_state_string(MOUSE_SCROLL_STATE_NONE));
          }

          break;
        }

        mouse_button_release_event_t mouse_button_release_event = {0};

        mouse_button_release_event.base = (event_t){
            .type = EVENT_TYPE_MOUSE_BUTTON_RELEASE,
        };

        mouse_button_release_event.button = __platform_input_mouse_button(button_release_event->detail);

        mouse_button_release_event.position.x = button_release_event->event_x;
        mouse_button_release_event.position.y = button_release_event->event_y;

        if (!__platform_input_mouse_button_set_state(mouse_button_release_event.button, MOUSE_BUTTON_STATE_RELEASED)) {
          logger_error_format("<window:%p> <state:%s> __platform_input_mouse_button_set_state failed", (void *)window,
                              input_mouse_button_state_string(MOUSE_BUTTON_STATE_RELEASED));
        }

        if (window->on_event) {
          if (!window->on_event((event_t *)&mouse_button_release_event)) {
            logger_error_format("<window:%p> <on_event:%p> <type:%s> failed", (void *)window, window->on_event,
                                events_type_string(mouse_button_release_event.base.type));
          }
        }

        break;
      }

      case XCB_KEY_PRESS: {
        xcb_key_press_event_t *key_press_event = (xcb_key_press_event_t *)event;
        (void)key_press_event;

        keyboard_hold_event_t keyboard_hold_event = {0};
        keyboard_press_event_t keyboard_press_event = {0};

        keyboard_hold_event.base = (event_t){
            .type = EVENT_TYPE_KEYBOARD_HOLD,
        };

        keyboard_press_event.base = (event_t){
            .type = EVENT_TYPE_KEYBOARD_PRESS,
        };

        xcb_keycode_t xcb_keycode = key_press_event->detail;
        xcb_keysym_t xcb_keysymbol = xcb_key_symbols_get_keysym(window->context.keysyms, xcb_keycode, 0);

        keyboard_keycode_t keycode = __platform_input_keyboard_keycode(xcb_keysymbol);
        keyboard_keycode_t scancode = __platform_input_keyboard_scancode(xcb_keycode);

        keyboard_hold_event.keycode = keycode;
        keyboard_hold_event.scancode = scancode;
        keyboard_press_event.keycode = keycode;
        keyboard_press_event.scancode = scancode;

        keyboard_keycode_state_t keyboard_keycode_state = __platform_input_keyboard_keycode_pressed_state(keycode);

        if (!__platform_input_keyboard_keycode_set_state(keycode, keyboard_keycode_state)) {
          logger_error_format("<window:%p> <state:%s> __platform_input_keyboard_keycode_set_state failed",
                              (void *)window, input_keyboard_keycode_state_string(keyboard_keycode_state));
        }

        if (window->on_event) {
          event_t *event = keyboard_keycode_state == KEYBOARD_KEYCODE_STATE_HELD ? (event_t *)&keyboard_hold_event
                                                                                 : (event_t *)&keyboard_press_event;

          if (!window->on_event(event)) {
            logger_error_format("<window:%p> <on_event:%p> <type:%s> failed", (void *)window, window->on_event,
                                events_type_string(event->type));
          }
        }

        break;
      }

      case XCB_KEY_RELEASE: {
        xcb_key_release_event_t *key_release_event = (xcb_key_release_event_t *)event;
        (void)key_release_event;

        keyboard_release_event_t keyboard_release_event = {0};

        keyboard_release_event.base = (event_t){
            .type = EVENT_TYPE_KEYBOARD_RELEASE,
        };

        xcb_keycode_t xcb_keycode = key_release_event->detail;
        xcb_keysym_t xcb_keysymbol = xcb_key_symbols_get_keysym(window->context.keysyms, xcb_keycode, 0);

        keyboard_keycode_t keycode = __platform_input_keyboard_keycode(xcb_keysymbol);
        keyboard_keycode_t scancode = __platform_input_keyboard_scancode(xcb_keycode);

        keyboard_release_event.keycode = keycode;
        keyboard_release_event.scancode = scancode;

        if (!__platform_input_keyboard_keycode_set_state(keycode, KEYBOARD_KEYCODE_STATE_RELEASED)) {
          logger_error_format("<window:%p> <state:%s> __platform_input_keyboard_keycode_set_state failed",
                              (void *)window, input_keyboard_keycode_state_string(KEYBOARD_KEYCODE_STATE_RELEASED));
        }

        if (window->on_event) {
          if (!window->on_event((event_t *)&keyboard_release_event)) {
            logger_error_format("<window:%p> <on_event:%p> <type:%s> failed", (void *)window, window->on_event,
                                events_type_string(keyboard_release_event.base.type));
          }
        }

        break;
      }

      case XCB_ENTER_NOTIFY: {
        xcb_enter_notify_event_t *enter_notify_event = (xcb_enter_notify_event_t *)event;
        (void)enter_notify_event;

        break;
      }

      case XCB_LEAVE_NOTIFY: {
        xcb_leave_notify_event_t *leave_notify_event = (xcb_leave_notify_event_t *)event;
        (void)leave_notify_event;

        break;
      }

      case XCB_MOTION_NOTIFY: {
        xcb_motion_notify_event_t *motion_notify_event = (xcb_motion_notify_event_t *)event;
        (void)motion_notify_event;

        mouse_move_event_t mouse_move_event = {0};

        mouse_move_event.base = (event_t){
            .type = EVENT_TYPE_MOUSE_MOVE,
        };

        mouse_move_event.position.x = motion_notify_event->event_x;
        mouse_move_event.position.y = motion_notify_event->event_y;

        mouse_position_t current_position = __platform_input_mouse_current_position();

        if (!__platform_input_mouse_set_previous_position(current_position)) {
          logger_error_format("<window:%p> <position:(%u, %u)> __platform_input_mouse_set_previous_position failed",
                              (void *)window, mouse_move_event.position.x, mouse_move_event.position.y);
        }

        if (!__platform_input_mouse_set_current_position(mouse_move_event.position)) {
          logger_error_format("<window:%p> <position:(%u, %u)> __platform_input_mouse_set_current_position failed",
                              (void *)window, mouse_move_event.position.x, mouse_move_event.position.y);
        }

        if (window->on_event) {
          if (!window->on_event((event_t *)&mouse_move_event)) {
            logger_error_format("<window:%p> <on_event:%p> <type:%s> failed", (void *)window, window->on_event,
                                events_type_string(mouse_move_event.base.type));
          }
        }

        break;
      }

      case XCB_FOCUS_IN: {
        xcb_focus_in_event_t *focus_in_event = (xcb_focus_in_event_t *)event;
        (void)focus_in_event;

        window->has_focus = true;

        break;
      }

      case XCB_FOCUS_OUT: {
        xcb_focus_out_event_t *focus_out_event = (xcb_focus_out_event_t *)event;
        (void)focus_out_event;

        window->has_focus = false;

        break;
      }

      case XCB_CONFIGURE_NOTIFY: {
        xcb_configure_notify_event_t *configure_notify_event = (xcb_configure_notify_event_t *)event;

        if (configure_notify_event->width != window->size.width ||
            configure_notify_event->height != window->size.height) {
          window->size.width = configure_notify_event->width;
          window->size.height = configure_notify_event->height;

          window->has_resized = true;
        }

        break;
      }

      case XCB_CLIENT_MESSAGE: {
        xcb_client_message_event_t *client_message_event = (xcb_client_message_event_t *)event;

        if (client_message_event->data.data32[0] == global_platform_window_atoms.WM_DELETE_WINDOW) {
          window->should_close = true;
        }

        break;
      }
    }

    free(event);
  }

  return true;
}

b8 platform_window_should_close(platform_window_t *window) {
  return window->should_close;
}

b8 platform_window_set_should_close(platform_window_t *window, b8 should_close) {
  window->should_close = should_close;

  return true;
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
  u16 mask = 0;

  mask |= XCB_CONFIG_WINDOW_WIDTH;
  mask |= XCB_CONFIG_WINDOW_HEIGHT;

  u32 values[] = {size.width, size.height};

  xcb_configure_window(window->context.connection, window->context.window, mask, values);
  xcb_flush(window->context.connection);

  return true;
}

b8 platform_window_set_title(platform_window_t *window, char *title) {
  xcb_change_property(window->context.connection, XCB_PROP_MODE_REPLACE, window->context.window, XCB_ATOM_WM_NAME,
                      XCB_ATOM_STRING, 8, string_length_secure(title, WINDOW_TITLE_MAX_LENGTH), title);

  xcb_flush(window->context.connection);

  return true;
}

b8 platform_window_is_fullscreen(platform_window_t *window) {
  return window->fullscreen;
}

b8 platform_window_set_fullscreen(platform_window_t *window, b8 fullscreen) {
  xcb_client_message_event_t event = {0};

  event.response_type = XCB_CLIENT_MESSAGE;
  event.window = window->context.window;
  event.type = global_platform_window_atoms._NET_WM_STATE;
  event.format = 32;

  event.data.data32[0] =
      fullscreen ? global_platform_window_atoms._NET_WM_STATE_ADD : global_platform_window_atoms._NET_WM_STATE_REMOVE;
  event.data.data32[1] = global_platform_window_atoms._NET_WM_STATE_FULLSCREEN;
  event.data.data32[2] = XCB_ATOM_NONE;

  xcb_send_event(window->context.connection, 1, window->context.screen->root,
                 XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY, (const char *)&event);

  xcb_flush(window->context.connection);

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

platform_window_context_t *platform_window_context(platform_window_t *window) {
  return &window->context;
}

b8 __platform_window_fetch_atoms(xcb_connection_t *connection) {
  xcb_intern_atom_cookie_t wm_protocols_cookie;
  xcb_intern_atom_cookie_t wm_delete_window_cookie;
  xcb_intern_atom_cookie_t net_wm_state_cookie;
  xcb_intern_atom_cookie_t net_wm_state_fullscreen_cookie;

  xcb_intern_atom_reply_t *wm_protocols_cookie_reply = NULL;
  xcb_intern_atom_reply_t *wm_delete_window_cookie_reply = NULL;
  xcb_intern_atom_reply_t *net_wm_state_cookie_reply = NULL;
  xcb_intern_atom_reply_t *net_wm_state_fullscreen_cookie_reply = NULL;

  const char *wm_protocols_name = "WM_PROTOCOLS";
  const char *wm_delete_window_name = "WM_DELETE_WINDOW";
  const char *net_wm_state_name = "_NET_WM_STATE";
  const char *net_wm_state_fullscreen_name = "_NET_WM_STATE_FULLSCREEN";

  u64 wm_protocols_name_length = string_length_secure((char *)wm_protocols_name, XCB_ATOM_MAX_LENGTH);
  u64 wm_delete_window_name_length = string_length_secure((char *)wm_delete_window_name, XCB_ATOM_MAX_LENGTH);
  u64 net_wm_state_name_length = string_length_secure((char *)net_wm_state_name, XCB_ATOM_MAX_LENGTH);
  u64 net_wm_state_fullscreen_name_length =
      string_length_secure((char *)net_wm_state_fullscreen_name, XCB_ATOM_MAX_LENGTH);

  wm_protocols_cookie = xcb_intern_atom(connection, 0, wm_protocols_name_length, wm_protocols_name);
  wm_delete_window_cookie = xcb_intern_atom(connection, 0, wm_delete_window_name_length, wm_delete_window_name);
  net_wm_state_cookie = xcb_intern_atom(connection, 0, net_wm_state_name_length, net_wm_state_name);
  net_wm_state_fullscreen_cookie =
      xcb_intern_atom(connection, 0, net_wm_state_fullscreen_name_length, net_wm_state_fullscreen_name);

  wm_protocols_cookie_reply = xcb_intern_atom_reply(connection, wm_protocols_cookie, NULL);
  wm_delete_window_cookie_reply = xcb_intern_atom_reply(connection, wm_delete_window_cookie, NULL);
  net_wm_state_cookie_reply = xcb_intern_atom_reply(connection, net_wm_state_cookie, NULL);
  net_wm_state_fullscreen_cookie_reply = xcb_intern_atom_reply(connection, net_wm_state_fullscreen_cookie, NULL);

  global_platform_window_atoms = (platform_window_atoms_t){
      .WM_PROTOCOLS = wm_protocols_cookie_reply->atom,
      .WM_DELETE_WINDOW = wm_delete_window_cookie_reply->atom,
      ._NET_WM_STATE = net_wm_state_cookie_reply->atom,
      ._NET_WM_STATE_ADD = 1,
      ._NET_WM_STATE_REMOVE = 0,
      ._NET_WM_STATE_FULLSCREEN = net_wm_state_fullscreen_cookie_reply->atom,
  };

  free(wm_protocols_cookie_reply);
  free(wm_delete_window_cookie_reply);
  free(net_wm_state_cookie_reply);
  free(net_wm_state_fullscreen_cookie_reply);

  return true;
}

b8 __platform_window_is_mouse_button_scroll(xcb_button_t button) {
  return button >= PLATFORM_MOUSE_SCROLL_UP_BUTTON && button <= PLATFORM_MOUSE_SCROLL_RIGHT_BUTTON;
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
