#include <stddef.h>
#include <stdlib.h>

#include <xcb/xcb.h>

/* horus base layer */
#include <horus/definitions.h>

/* horus core layer */
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

/* horus input layer [ linux ] */
#include <horus/platform/linux/input/keyboard.h>

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
  xcb_connection_t *connection;
  xcb_screen_t *screen;
  xcb_window_t window;

  u16 width;
  u16 height;

  b8 fullscreen;
  b8 has_focus;
  b8 should_close;

  platform_window_event_callback_t on_event;
};

b8 __platform_window_fetch_atoms(xcb_connection_t *connection) {
  xcb_intern_atom_cookie_t wm_protocols_cookie;
  xcb_intern_atom_cookie_t wm_delete_window_cookie;
  xcb_intern_atom_cookie_t net_wm_state_cookie;
  xcb_intern_atom_cookie_t net_wm_state_fullscreen_cookie;

  xcb_intern_atom_reply_t *wm_protocols_cookie_reply;
  xcb_intern_atom_reply_t *wm_delete_window_cookie_reply;
  xcb_intern_atom_reply_t *net_wm_state_cookie_reply;
  xcb_intern_atom_reply_t *net_wm_state_fullscreen_cookie_reply;

  const char *wm_protocols_name = "WM_PROTOCOLS";
  const char *wm_delete_window_name = "WM_DELETE_WINDOW";
  const char *net_wm_state_name = "_NET_WM_STATE";
  const char *net_wm_state_fullscreen_name = "_NET_WM_STATE_FULLSCREEN";

  u64 wm_protocols_name_length = string_length((char *)wm_protocols_name);
  u64 wm_delete_window_name_length = string_length((char *)wm_delete_window_name);
  u64 net_wm_state_name_length = string_length((char *)net_wm_state_name);
  u64 net_wm_state_fullscreen_name_length = string_length((char *)net_wm_state_fullscreen_name);

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

platform_window_t *platform_window_create(char *title, u16 width, u16 height, b8 fullscreen) {
  platform_window_t *window = platform_memory_allocate(sizeof(platform_window_t));

  window->on_event = NULL;

  window->connection = xcb_connect(NULL, NULL);

  logger_debug("<window:%p> <xcb_connection:%p> connected", window, window->connection);

  const xcb_setup_t *screen_setup = xcb_get_setup(window->connection);
  xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(screen_setup);

  window->screen = screen_iterator.data;

  window->window = xcb_generate_id(window->connection);

  uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

  int value_list[2];

  value_list[0] = window->screen->black_pixel;
  value_list[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
                  XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW | XCB_EVENT_MASK_KEY_PRESS |
                  XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_FOCUS_CHANGE;

  xcb_create_window(window->connection,            /* connection */
                    XCB_COPY_FROM_PARENT,          /* depth */
                    window->window,                /* window identifier */
                    window->screen->root,          /* parent window */
                    0, 0,                          /* x, y */
                    width, height,                 /* width, height */
                    0,                             /* border width */
                    XCB_WINDOW_CLASS_INPUT_OUTPUT, /* window class */
                    window->screen->root_visual,   /* window visual */
                    value_mask,                    /* masks */
                    value_list                     /* masks */
  );

  __platform_window_fetch_atoms(window->connection);

  xcb_change_property(window->connection, XCB_PROP_MODE_REPLACE, window->window,
                      global_platform_window_atoms.WM_PROTOCOLS, 4, 32, 1,
                      &global_platform_window_atoms.WM_DELETE_WINDOW);

  xcb_change_property(window->connection, XCB_PROP_MODE_REPLACE, window->window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                      string_length(title), title);

  if (fullscreen) {
    xcb_change_property(window->connection, XCB_PROP_MODE_REPLACE, window->window,
                        global_platform_window_atoms._NET_WM_STATE, XCB_ATOM_ATOM, 32, 1,
                        &global_platform_window_atoms._NET_WM_STATE_FULLSCREEN);
  }

  xcb_map_window(window->connection, window->window);

  xcb_flush(window->connection);

  window->width = width;
  window->height = height;
  window->fullscreen = fullscreen;
  window->has_focus = false;
  window->should_close = false;

  logger_debug("<window:%p> <xcb_window:%lu> created", window, window->window);

  return window;
}

void platform_window_destroy(platform_window_t *window) {
  xcb_destroy_window(window->connection, window->window);

  logger_debug("<window:%p> <xcb_window:%lu> destroyed", window, window->window);

  xcb_disconnect(window->connection);

  logger_debug("<window:%p> <xcb_connection:%p> disconnected", window, window->connection);

  platform_memory_deallocate(window);
}

void platform_window_process_events(platform_window_t *window) {
  xcb_generic_event_t *event = NULL;

  while ((event = xcb_poll_for_event(window->connection))) {
    switch (event->response_type & ~0x80) {
      case XCB_EXPOSE: {
        xcb_expose_event_t *expose_event = (xcb_expose_event_t *)event;
        (void)expose_event;

        break;
      }

      case XCB_BUTTON_PRESS: {
        xcb_button_press_event_t *button_press_event = (xcb_button_press_event_t *)event;
        (void)button_press_event;

        if (window->on_event) {
          mouse_button_press_event_t mouse_button_press_event = {0};

          mouse_button_press_event.base = (event_t){
              .type = EVENT_TYPE_MOUSE_BUTTON_PRESS,
          };

          mouse_button_press_event.button = button_press_event->detail == 1   ? MOUSE_BUTTON_LEFT
                                            : button_press_event->detail == 2 ? MOUSE_BUTTON_MIDDLE
                                            : button_press_event->detail == 3 ? MOUSE_BUTTON_RIGHT
                                                                              : MOUSE_BUTTON_NONE;
          mouse_button_press_event.position.x = button_press_event->event_x;
          mouse_button_press_event.position.y = button_press_event->event_y;

          if (!window->on_event((event_t *)&mouse_button_press_event)) {
            logger_error("<window:%p> <on_event:%p> <type:%s> failed", window, window->on_event,
                         event_type_string(mouse_button_press_event.base.type));
          }
        }

        break;
      }

      case XCB_BUTTON_RELEASE: {
        xcb_button_release_event_t *button_release_event = (xcb_button_release_event_t *)event;
        (void)button_release_event;

        if (window->on_event) {
          mouse_button_release_event_t mouse_button_release_event = {0};

          mouse_button_release_event.base = (event_t){
              .type = EVENT_TYPE_MOUSE_BUTTON_RELEASE,
          };

          mouse_button_release_event.button = button_release_event->detail == 1   ? MOUSE_BUTTON_LEFT
                                              : button_release_event->detail == 2 ? MOUSE_BUTTON_MIDDLE
                                              : button_release_event->detail == 3 ? MOUSE_BUTTON_RIGHT
                                                                                  : MOUSE_BUTTON_NONE;

          mouse_button_release_event.position.x = button_release_event->event_x;
          mouse_button_release_event.position.y = button_release_event->event_y;

          if (!window->on_event((event_t *)&mouse_button_release_event)) {
            logger_error("<window:%p> <on_event:%p> <type:%s> failed", window, window->on_event,
                         event_type_string(mouse_button_release_event.base.type));
          }
        }

        break;
      }

      case XCB_KEY_PRESS: {
        xcb_key_press_event_t *key_press_event = (xcb_key_press_event_t *)event;
        (void)key_press_event;

        if (window->on_event) {
          keyboard_press_event_t keyboard_press_event = {0};

          keyboard_press_event.base = (event_t){
              .type = EVENT_TYPE_KEYBOARD_PRESS,
          };

          keyboard_press_event.keycode = platform_keyboard_keycode(key_press_event->detail);

          if (!window->on_event((event_t *)&keyboard_press_event)) {
            logger_error("<window:%p> <on_event:%p> <type:%s> failed", window, window->on_event,
                         event_type_string(keyboard_press_event.base.type));
          }
        }

        break;
      }

      case XCB_KEY_RELEASE: {
        xcb_key_release_event_t *key_release_event = (xcb_key_release_event_t *)event;
        (void)key_release_event;

        if (window->on_event) {
          keyboard_release_event_t keyboard_release_event = {0};

          keyboard_release_event.base = (event_t){
              .type = EVENT_TYPE_KEYBOARD_RELEASE,
          };

          keyboard_release_event.keycode = platform_keyboard_keycode(key_release_event->detail);

          if (!window->on_event((event_t *)&keyboard_release_event)) {
            logger_error("<window:%p> <on_event:%p> <type:%s> failed", window, window->on_event,
                         event_type_string(keyboard_release_event.base.type));
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

        if (window->on_event) {
          mouse_move_event_t mouse_move_event = {0};

          mouse_move_event.base = (event_t){
              .type = EVENT_TYPE_MOUSE_MOVE,
          };

          mouse_move_event.position.x = motion_notify_event->event_x;
          mouse_move_event.position.y = motion_notify_event->event_y;

          if (!window->on_event((event_t *)&mouse_move_event)) {
            logger_error("<window:%p> <on_event:%p> <type:%s> failed", window, window->on_event,
                         event_type_string(mouse_move_event.base.type));
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

        if (configure_notify_event->width != window->width || configure_notify_event->height != window->height) {
          window->width = configure_notify_event->width;
          window->height = configure_notify_event->height;
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
}

b8 platform_window_should_close(platform_window_t *window) {
  return window->should_close;
}

b8 platform_window_has_focus(platform_window_t *window) {
  return window->has_focus;
}

void platform_window_size(platform_window_t *window, u16 *width, u16 *height) {
  *width = window->width;
  *height = window->height;
}

b8 platform_window_set_size(platform_window_t *window, u16 width, u16 height) {
  u16 mask = 0;

  mask |= XCB_CONFIG_WINDOW_WIDTH;
  mask |= XCB_CONFIG_WINDOW_HEIGHT;

  u32 values[] = {width, height};

  xcb_configure_window(window->connection, window->window, mask, values);
  xcb_flush(window->connection);

  return true;
}

b8 platform_window_set_title(platform_window_t *window, char *title) {
  xcb_change_property(window->connection, XCB_PROP_MODE_REPLACE, window->window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                      string_length(title), title);

  xcb_flush(window->connection);

  return true;
}

b8 platform_window_is_fullscreen(platform_window_t *window) {
  return window->fullscreen;
}

b8 platform_window_set_fullscreen(platform_window_t *window, b8 fullscreen) {
  xcb_client_message_event_t event = {0};

  event.response_type = XCB_CLIENT_MESSAGE;
  event.window = window->window;
  event.type = global_platform_window_atoms._NET_WM_STATE;
  event.format = 32;

  event.data.data32[0] =
      fullscreen ? global_platform_window_atoms._NET_WM_STATE_ADD : global_platform_window_atoms._NET_WM_STATE_REMOVE;
  event.data.data32[1] = global_platform_window_atoms._NET_WM_STATE_FULLSCREEN;
  event.data.data32[2] = XCB_ATOM_NONE;

  xcb_send_event(window->connection, 1, window->screen->root,
                 XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY, (const char *)&event);

  xcb_flush(window->connection);

  window->fullscreen = fullscreen;

  return true;
}

b8 platform_window_set_event_callback(platform_window_t *window, platform_window_event_callback_t callback) {
  window->on_event = callback;

  return true;
}
