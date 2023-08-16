#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <xcb/xcb.h>

#include <horus/definitions.h>
#include <horus/logger/logger.h>
#include <horus/platform/memory.h>
#include <horus/platform/window.h>

struct __platform_window {
  xcb_connection_t *connection;
  xcb_screen_t *screen;
  xcb_window_t window;

  xcb_intern_atom_reply_t *protocols_cookie_reply;

  xcb_intern_atom_reply_t *close_client_cookie_reply;

  xcb_intern_atom_reply_t *state_cookie_reply;

  xcb_intern_atom_reply_t *fullscreen_cookie_reply;

  u16 width;
  u16 height;

  b8 windowed;
  b8 has_focus;
  b8 should_close;
};

platform_window_t *platform_window_create(char *title, u16 width, u16 height, b8 windowed) {
  platform_window_t *window = platform_memory_allocate(sizeof(platform_window_t));

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

  xcb_intern_atom_cookie_t protocols_cookie = xcb_intern_atom(window->connection, 0, 12, "WM_PROTOCOLS");

  window->protocols_cookie_reply = xcb_intern_atom_reply(window->connection, protocols_cookie, 0);

  xcb_intern_atom_cookie_t close_client_cookie = xcb_intern_atom(window->connection, 0, 16, "WM_DELETE_WINDOW");

  window->close_client_cookie_reply = xcb_intern_atom_reply(window->connection, close_client_cookie, 0);

  xcb_intern_atom_cookie_t window_state_cookie = xcb_intern_atom(window->connection, 0, 13, "_NET_WM_STATE");
  window->state_cookie_reply = xcb_intern_atom_reply(window->connection, window_state_cookie, NULL);

  xcb_intern_atom_cookie_t window_fullscreen_cookie =
      xcb_intern_atom(window->connection, 0, 24, "_NET_WM_STATE_FULLSCREEN");
  window->fullscreen_cookie_reply = xcb_intern_atom_reply(window->connection, window_fullscreen_cookie, NULL);

  xcb_change_property(window->connection, XCB_PROP_MODE_REPLACE, window->window, (*window->protocols_cookie_reply).atom,
                      4, 32, 1, &(*window->close_client_cookie_reply).atom);

  xcb_change_property(window->connection, XCB_PROP_MODE_REPLACE, window->window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                      strlen(title), title);

  if (!windowed) {
    xcb_change_property(window->connection, XCB_PROP_MODE_REPLACE, window->window, window->state_cookie_reply->atom,
                        XCB_ATOM_ATOM, 32, 1, &(window->fullscreen_cookie_reply->atom));
  }

  xcb_map_window(window->connection, window->window);

  xcb_flush(window->connection);

  window->width = width;
  window->height = height;
  window->windowed = windowed;
  window->has_focus = false;
  window->should_close = false;

  logger_debug("<window:%p> <xcb_window:%lu> created", window, window->window);

  return window;
}

void platform_window_destroy(platform_window_t *window) {
  free(window->protocols_cookie_reply);

  free(window->close_client_cookie_reply);

  free(window->state_cookie_reply);

  free(window->fullscreen_cookie_reply);

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

        break;
      }

      case XCB_BUTTON_RELEASE: {
        xcb_button_release_event_t *button_release_event = (xcb_button_release_event_t *)event;
        (void)button_release_event;

        break;
      }

      case XCB_KEY_PRESS: {
        xcb_key_press_event_t *key_press_event = (xcb_key_press_event_t *)event;
        (void)key_press_event;

        break;
      }

      case XCB_KEY_RELEASE: {
        xcb_key_release_event_t *key_release_event = (xcb_key_release_event_t *)event;
        (void)key_release_event;

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

        if (client_message_event->data.data32[0] == (*window->close_client_cookie_reply).atom) {
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
                      strlen(title), title);

  xcb_flush(window->connection);

  return true;
}
