#include <horus/horus.h>

#include <sandbox/sandbox.h>

application_t *application_create() {
  application_t *application = platform_memory_allocate(sizeof(application_t));
  platform_memory_clear(application, sizeof(application_t));

  application->name = "Sandbox Application";

  application->on_event = on_event;
  application->on_update = on_update;
  application->on_render = on_render;

  application->configuration = (configuration_t){.fullscreen = false,
                                                 .resolution = (resolution_t){
                                                     .width = 1280,
                                                     .height = 720,
                                                 }};

  return application;
}

void application_destroy(application_t *application) {
  platform_memory_deallocate(application);
}

b8 on_event(event_t *event) {
  if (event->type == EVENT_TYPE_MOUSE_MOVE) {
    mouse_move_event_t *mouse_move_event = (mouse_move_event_t *)event;

    logger_debug("<event:%p> <type:%s> <x:%u> <y:%u>", mouse_move_event, event_type_string(event->type),
                 mouse_move_event->position.x, mouse_move_event->position.y);
  }

  if (event->type == EVENT_TYPE_MOUSE_SCROLL) {
    mouse_scroll_event_t *mouse_scroll_event = (mouse_scroll_event_t *)event;

    logger_debug("<event:%p> <type:%s> <direction:%s> <x:%u> <y:%u> ", mouse_scroll_event,
                 event_type_string(event->type), mouse_scroll_direction_string(mouse_scroll_event->direction),
                 mouse_scroll_event->position.x, mouse_scroll_event->position.y);
  }

  if (event->type == EVENT_TYPE_MOUSE_BUTTON_PRESS) {
    mouse_button_press_event_t *mouse_button_press_event = (mouse_button_press_event_t *)event;

    logger_debug("<event:%p> <type:%s> <button:%s> <x:%u> <y:%u>", mouse_button_press_event,
                 event_type_string(event->type), mouse_button_string(mouse_button_press_event->button),
                 mouse_button_press_event->position.x, mouse_button_press_event->position.y);
  }

  if (event->type == EVENT_TYPE_MOUSE_BUTTON_RELEASE) {
    mouse_button_release_event_t *mouse_button_release_event = (mouse_button_release_event_t *)event;

    logger_debug("<event:%p> <type:%s> <button:%s> <x:%u> <y:%u>", mouse_button_release_event,
                 event_type_string(event->type), mouse_button_string(mouse_button_release_event->button),
                 mouse_button_release_event->position.x, mouse_button_release_event->position.y);
  }

  if (event->type == EVENT_TYPE_KEYBOARD_PRESS) {
    keyboard_press_event_t *keyboard_press_event = (keyboard_press_event_t *)event;

    logger_debug("<event:%p> <type:%s> <keycode:%s> <keysymbol:%s>", keyboard_press_event,
                 event_type_string(event->type), keyboard_keycode_string(keyboard_press_event->keycode),
                 keyboard_keysymbol_string(keyboard_press_event->keysymbol));
  }

  if (event->type == EVENT_TYPE_KEYBOARD_RELEASE) {
    keyboard_release_event_t *keyboard_release_event = (keyboard_release_event_t *)event;

    logger_debug("<event:%p> <type:%s> <keycode:%s> <keysymbol:%s>", keyboard_release_event,
                 event_type_string(event->type), keyboard_keycode_string(keyboard_release_event->keycode),
                 keyboard_keysymbol_string(keyboard_release_event->keysymbol));
  }

  if (event->type == EVENT_TYPE_KEYBOARD_HOLD) {
    keyboard_hold_event_t *keyboard_hold_event = (keyboard_hold_event_t *)event;

    logger_debug("<event:%p> <type:%s> <keycode:%s> <keysymbol:%s>", keyboard_hold_event,
                 event_type_string(event->type), keyboard_hold_event->keycode,
                 keyboard_keycode_string(keyboard_hold_event->keycode),
                 keyboard_keysymbol_string(keyboard_hold_event->keysymbol));
  }

  return true;
}

b8 on_render(void) {
  return true;
}

b8 on_update(f64 timestep) {
  return true;
}
