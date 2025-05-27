#include <horus/horus.h>

#include <sandbox/sandbox.h>

application_t *application_create(void) {
  application_t *application = platform_memory_allocate(sizeof(application_t));
  platform_memory_clear(application, sizeof(application_t));

  application->name = "Sandbox Application";

  application->configuration = (configuration_t){.fullscreen = false,
                                                 .resolution = (resolution_t){
                                                     .width = 1280,
                                                     .height = 720,
                                                 }};

  application->on_event = on_event;
  application->on_update = on_update;
  application->on_render = on_render;

  return application;
}

b8 application_destroy(application_t *application) {
  platform_memory_deallocate(application);

  return true;
}

b8 on_event(event_t *event) {
  if (event->type == EVENT_TYPE_MOUSE_MOVE) {
    mouse_move_event_t *mouse_move_event = (mouse_move_event_t *)event;

    logger_debug("<event:%p> <type:%s> <x:%u> <y:%u>", mouse_move_event, events_type_string(event->type),
                 mouse_move_event->position.x, mouse_move_event->position.y);
  }

  if (event->type == EVENT_TYPE_MOUSE_SCROLL) {
    mouse_scroll_event_t *mouse_scroll_event = (mouse_scroll_event_t *)event;

    logger_debug("<event:%p> <type:%s> <direction:%s> <x:%u> <y:%u> ", mouse_scroll_event,
                 events_type_string(event->type), input_mouse_scroll_direction_string(mouse_scroll_event->direction),
                 mouse_scroll_event->position.x, mouse_scroll_event->position.y);
  }

  if (event->type == EVENT_TYPE_MOUSE_BUTTON_PRESS) {
    mouse_button_press_event_t *mouse_button_press_event = (mouse_button_press_event_t *)event;

    logger_debug("<event:%p> <type:%s> <button:%s> <x:%u> <y:%u>", mouse_button_press_event,
                 events_type_string(event->type), input_mouse_button_string(mouse_button_press_event->button),
                 mouse_button_press_event->position.x, mouse_button_press_event->position.y);
  }

  if (event->type == EVENT_TYPE_MOUSE_BUTTON_RELEASE) {
    mouse_button_release_event_t *mouse_button_release_event = (mouse_button_release_event_t *)event;

    logger_debug("<event:%p> <type:%s> <button:%s> <x:%u> <y:%u>", mouse_button_release_event,
                 events_type_string(event->type), input_mouse_button_string(mouse_button_release_event->button),
                 mouse_button_release_event->position.x, mouse_button_release_event->position.y);
  }

  if (event->type == EVENT_TYPE_KEYBOARD_PRESS) {
    keyboard_press_event_t *keyboard_press_event = (keyboard_press_event_t *)event;

    logger_debug("<event:%p> <type:%s> <keycode:%s>", keyboard_press_event, events_type_string(event->type),
                 input_keyboard_keycode_string(keyboard_press_event->keycode));
  }

  if (event->type == EVENT_TYPE_KEYBOARD_RELEASE) {
    keyboard_release_event_t *keyboard_release_event = (keyboard_release_event_t *)event;

    logger_debug("<event:%p> <type:%s> <keycode:%s>", keyboard_release_event, events_type_string(event->type),
                 input_keyboard_keycode_string(keyboard_release_event->keycode));
  }

  if (event->type == EVENT_TYPE_KEYBOARD_HOLD) {
    keyboard_hold_event_t *keyboard_hold_event = (keyboard_hold_event_t *)event;

    logger_debug("<event:%p> <type:%s> <keycode:%s>", keyboard_hold_event, events_type_string(event->type),
                 keyboard_hold_event->keycode, input_keyboard_keycode_string(keyboard_hold_event->keycode));
  }

  return true;
}

b8 on_update(f64 timestep) {
  platform_window_t *window = platform_window();

  if (platform_window_has_resized(window)) {
    platform_window_size_t size = platform_window_size(window);

    logger_debug("<on_update> <timestep:%f> <width:%u> <height:%u> window resized", size.width, size.height);
  }

  if (input_mouse_has_moved()) {
    mouse_position_t position = input_mouse_position();

    logger_debug("<on_update> <timestep:%f> <x:%u> <y:%u> mouse moved", timestep, position.x, position.y);
  }

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_ESCAPE)) {
    logger_debug("<on_update> <timestep:%f> <keycode:%s> is pressed", timestep,
                 input_keyboard_keycode_string(KEYBOARD_KEYCODE_ESCAPE));
  }

  if (input_keyboard_keycode_is_released(KEYBOARD_KEYCODE_ESCAPE)) {
    logger_debug("<on_update> <timestep:%f> <keycode:%s> is released", timestep,
                 input_keyboard_keycode_string(KEYBOARD_KEYCODE_ESCAPE));
  }

  if (input_keyboard_keycode_is_held(KEYBOARD_KEYCODE_ESCAPE)) {
    logger_debug("<on_update> <timestep:%f> <keycode:%s> is held", timestep,
                 input_keyboard_keycode_string(KEYBOARD_KEYCODE_ESCAPE));
  }

  if (input_mouse_button_is_pressed(MOUSE_BUTTON_LEFT)) {
    logger_debug("<on_update> <timestep:%f> <mouse_button:%s> is pressed", timestep,
                 input_mouse_button_string(MOUSE_BUTTON_LEFT));
  }

  if (input_mouse_button_is_released(MOUSE_BUTTON_LEFT)) {
    logger_debug("<on_update> <timestep:%f> <mouse_button:%s> is released", timestep,
                 input_mouse_button_string(MOUSE_BUTTON_LEFT));
  }

  if (input_mouse_button_is_pressed(MOUSE_BUTTON_MIDDLE)) {
    logger_debug("<on_update> <timestep:%f> <mouse_button:%s> is pressed", timestep,
                 input_mouse_button_string(MOUSE_BUTTON_MIDDLE));
  }

  if (input_mouse_button_is_released(MOUSE_BUTTON_MIDDLE)) {
    logger_debug("<on_update> <timestep:%f> <mouse_button:%s> is released", timestep,
                 input_mouse_button_string(MOUSE_BUTTON_MIDDLE));
  }

  if (input_mouse_button_is_pressed(MOUSE_BUTTON_RIGHT)) {
    logger_debug("<on_update> <timestep:%f> <mouse_button:%s> is pressed", timestep,
                 input_mouse_button_string(MOUSE_BUTTON_RIGHT));
  }

  if (input_mouse_button_is_released(MOUSE_BUTTON_RIGHT)) {
    logger_debug("<on_update> <timestep:%f> <mouse_button:%s> is released", timestep,
                 input_mouse_button_string(MOUSE_BUTTON_RIGHT));
  }

  if (input_mouse_scroll_is_up()) {
    logger_debug("<on_update> <timestep:%f> <scroll:%s> is up", timestep,
                 input_mouse_scroll_direction_string(MOUSE_SCROLL_DIRECTION_UP));
  }

  if (input_mouse_scroll_is_down()) {
    logger_debug("<on_update> <timestep:%f> <scroll:%s> is down", timestep,
                 input_mouse_scroll_direction_string(MOUSE_SCROLL_DIRECTION_DOWN));
  }

  return true;
}

b8 on_render(void) {
  return true;
}
