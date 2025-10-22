#include <horus/horus.h>

#include <sandbox/sandbox.h>

#define QUAD_INDICES_COUNT 6
#define QUAD_VERTICES_COUNT 4
#define QUAD_INSTANCES_COUNT 2

const char *default_shader_module_path = "assets/shaders/build/default.spv";

shader_module_t *default_shader_module = NULL;

graphics_pipeline_t *default_graphics_pipeline = NULL;

f32 start_absolute_time = 0.0f;
uniform_buffer_t *uniform_buffer = NULL;
uniform_buffer_object_t uniform_buffer_object = {0};

index_buffer_t *quad_index_buffer = NULL;
vertex_buffer_t *quad_vertex_buffer = NULL;

instance_buffer_t *quad_instance_buffer = NULL;
instance_buffer_object_t quad_instance_buffer_objects[QUAD_INSTANCES_COUNT] = {0};

u32 quad_indices[QUAD_INDICES_COUNT] = {0, 1, 2, 2, 3, 0};

vertex_t quad_vertices[QUAD_VERTICES_COUNT] = {
    (vertex_t){.position = {{-0.5f, -0.5f, 0.0f}}, .color = {{1.0f, 1.0f, 0.0f, 1.0f}}},
    (vertex_t){.position = {{0.5f, -0.5f, 0.0f}}, .color = {{1.0f, 1.0f, 0.0f, 1.0f}}},
    (vertex_t){.position = {{0.5f, 0.5f, 0.0f}}, .color = {{0.0f, 1.0f, 1.0f, 1.0f}}},
    (vertex_t){.position = {{-0.5f, 0.5f, 0.0f}}, .color = {{0.0f, 1.0f, 1.0f, 1.0f}}},
};

f32 quad_scale_speed = 2.0f;
f32 quad_position_speed = 1.0f;
f32 quad_rotation_angle = 1.0f;

vector3f32_t quad_scale = {{1.0f, 1.0f, 1.0f}};
vector3f32_t quad_position = {{-0.4f, -0.3f, 0}};
vector3f32_t quad_rotation = {{0.0f, 0.0f, 0.0f}};

application_t *application_create(void) {
  application_t *application = platform_memory_allocate(sizeof(application_t));
  platform_memory_clear(application, sizeof(application_t));

  application->name = "Sandbox Application";

  application->version = (version_t){
      .major = 1,
      .minor = 0,
      .patch = 0,
  };

  application->configuration = (configuration_t){
      .fullscreen = false,
      .framerate = 60,
      .resolution =
          (resolution_t){
              .width = 1280,
              .height = 720,
          },
  };

  application->on_create = on_create;
  application->on_destroy = on_destroy;

  application->on_event = on_event;
  application->on_update = on_update;
  application->on_render = on_render;

  start_absolute_time = (f32)platform_absolute_time();

  return application;
}

b8 application_destroy(application_t *application) {
  platform_memory_deallocate(application);

  return true;
}

b8 on_create(application_t *application, renderer_t *renderer) {
  (void)application; /* unused */

  default_shader_module = shader_module_create_from_binary(renderer, SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT,
                                                           (char *)default_shader_module_path);

  logger_info_format("<renderer:%p> <module:%p> <path:%s> created", (void *)renderer, (void *)default_shader_module,
                     default_shader_module_path);

  default_graphics_pipeline = graphics_pipeline_create(renderer, default_shader_module);

  logger_info_format("<renderer:%p> <pipeline:%p> created", (void *)renderer, (void *)default_graphics_pipeline);

  uniform_buffer = uniform_buffer_create(renderer, &uniform_buffer_object);

  logger_info_format("<renderer:%p> <uniform_buffer:%p> created", (void *)renderer, (void *)uniform_buffer);

  quad_index_buffer = index_buffer_create(renderer, quad_indices, QUAD_INDICES_COUNT);

  logger_info_format("<renderer:%p> <index_buffer:%p> created", (void *)renderer, (void *)quad_index_buffer);

  quad_vertex_buffer = vertex_buffer_create(renderer, quad_vertices, QUAD_VERTICES_COUNT);

  logger_info_format("<renderer:%p> <vertex_buffer:%p> created", (void *)renderer, (void *)quad_vertex_buffer);

  quad_instance_buffer = instance_buffer_create(renderer, quad_instance_buffer_objects, QUAD_INSTANCES_COUNT);

  logger_info_format("<renderer:%p> <instance_buffer:%p> created", (void *)renderer, (void *)quad_instance_buffer);

  return true;
}

b8 on_destroy(application_t *application, renderer_t *renderer) {
  (void)application; /* unused */

  instance_buffer_destroy(quad_instance_buffer);

  logger_info_format("<renderer:%p> <instance_buffer:%p> destroyed", (void *)renderer, (void *)quad_instance_buffer);

  vertex_buffer_destroy(quad_vertex_buffer);

  logger_info_format("<renderer:%p> <vertex_buffer:%p> destroyed", (void *)renderer, (void *)quad_vertex_buffer);

  index_buffer_destroy(quad_index_buffer);

  logger_info_format("<renderer:%p> <index_buffer:%p> destroyed", (void *)renderer, (void *)quad_index_buffer);

  uniform_buffer_destroy(uniform_buffer);

  logger_info_format("<renderer:%p> <uniform_buffer:%p> destroyed", (void *)renderer, (void *)uniform_buffer);

  graphics_pipeline_destroy(default_graphics_pipeline);

  logger_info_format("<renderer:%p> <pipeline:%p> destroyed", (void *)renderer, (void *)default_graphics_pipeline);

  shader_module_destroy(default_shader_module);

  logger_info_format("<renderer:%p> <module:%p> destroyed", (void *)renderer, (void *)default_shader_module);

  return true;
}

b8 on_event(event_t *event) {
  if (event->type == EVENT_TYPE_MOUSE_MOVE) {
    mouse_move_event_t *mouse_move_event = (mouse_move_event_t *)event;

    logger_debug_format("<event:%p> <type:%s> <x:%u> <y:%u>", (void *)mouse_move_event, events_type_string(event->type),
                        mouse_move_event->position.x, mouse_move_event->position.y);
  }

  if (event->type == EVENT_TYPE_MOUSE_SCROLL) {
    mouse_scroll_event_t *mouse_scroll_event = (mouse_scroll_event_t *)event;

    logger_debug_format("<event:%p> <type:%s> <direction:%s> <x:%u> <y:%u> ", (void *)mouse_scroll_event,
                        events_type_string(event->type),
                        input_mouse_scroll_direction_string(mouse_scroll_event->direction),
                        mouse_scroll_event->position.x, mouse_scroll_event->position.y);
  }

  if (event->type == EVENT_TYPE_MOUSE_BUTTON_PRESS) {
    mouse_button_press_event_t *mouse_button_press_event = (mouse_button_press_event_t *)event;

    logger_debug_format("<event:%p> <type:%s> <button:%s> <x:%u> <y:%u>", (void *)mouse_button_press_event,
                        events_type_string(event->type), input_mouse_button_string(mouse_button_press_event->button),
                        mouse_button_press_event->position.x, mouse_button_press_event->position.y);
  }

  if (event->type == EVENT_TYPE_MOUSE_BUTTON_RELEASE) {
    mouse_button_release_event_t *mouse_button_release_event = (mouse_button_release_event_t *)event;

    logger_debug_format("<event:%p> <type:%s> <button:%s> <x:%u> <y:%u>", (void *)mouse_button_release_event,
                        events_type_string(event->type), input_mouse_button_string(mouse_button_release_event->button),
                        mouse_button_release_event->position.x, mouse_button_release_event->position.y);
  }

  if (event->type == EVENT_TYPE_KEYBOARD_PRESS) {
    keyboard_press_event_t *keyboard_press_event = (keyboard_press_event_t *)event;

    logger_debug_format("<event:%p> <type:%s> <keycode:%s> <scancode:%s>", (void *)keyboard_press_event,
                        events_type_string(event->type), input_keyboard_keycode_string(keyboard_press_event->keycode),
                        input_keyboard_keycode_string(keyboard_press_event->scancode));
  }

  if (event->type == EVENT_TYPE_KEYBOARD_RELEASE) {
    keyboard_release_event_t *keyboard_release_event = (keyboard_release_event_t *)event;

    logger_debug_format("<event:%p> <type:%s> <keycode:%s> <scancode:%s>", (void *)keyboard_release_event,
                        events_type_string(event->type), input_keyboard_keycode_string(keyboard_release_event->keycode),
                        input_keyboard_keycode_string(keyboard_release_event->scancode));
  }

  if (event->type == EVENT_TYPE_KEYBOARD_HOLD) {
    keyboard_hold_event_t *keyboard_hold_event = (keyboard_hold_event_t *)event;

    logger_debug_format("<event:%p> <type:%s> <keycode:%s> <scancode:%s>", (void *)keyboard_hold_event,
                        events_type_string(event->type), input_keyboard_keycode_string(keyboard_hold_event->keycode),
                        input_keyboard_keycode_string(keyboard_hold_event->scancode));
  }

  return true;
}

b8 on_update(f64 timestep) {
  platform_window_t *window = platform_window();

  if (platform_window_has_resized(window)) {
    platform_window_size_t size = platform_window_size(window);

    logger_debug_format("<on_update> <timestep:%f> <width:%u> <height:%u> window resized", timestep, size.width,
                        size.height);
  }

  if (input_mouse_has_moved()) {
    mouse_position_t position = input_mouse_position();

    logger_debug_format("<on_update> <timestep:%f> <x:%u> <y:%u> mouse moved", timestep, position.x, position.y);
  }

  if (input_keyboard_keycode_is_released(KEYBOARD_KEYCODE_ESCAPE)) {
    platform_window_set_should_close(window, true);
  }

  if (input_keyboard_keycode_is_released(KEYBOARD_KEYCODE_ENTER)) {
    platform_window_set_fullscreen(window, !platform_window_is_fullscreen(window));
  }

  if (input_keyboard_keycode_is_held(KEYBOARD_KEYCODE_ESCAPE)) {
    logger_debug_format("<on_update> <timestep:%f> <keycode:%s> is held", timestep,
                        input_keyboard_keycode_string(KEYBOARD_KEYCODE_ESCAPE));
  }

  if (input_mouse_button_is_pressed(MOUSE_BUTTON_LEFT)) {
    logger_debug_format("<on_update> <timestep:%f> <mouse_button:%s> is pressed", timestep,
                        input_mouse_button_string(MOUSE_BUTTON_LEFT));

    quad_rotation.z += quad_rotation_angle;
  }

  if (input_mouse_button_is_released(MOUSE_BUTTON_LEFT)) {
    logger_debug_format("<on_update> <timestep:%f> <mouse_button:%s> is released", timestep,
                        input_mouse_button_string(MOUSE_BUTTON_LEFT));
  }

  if (input_mouse_button_is_pressed(MOUSE_BUTTON_MIDDLE)) {
    logger_debug_format("<on_update> <timestep:%f> <mouse_button:%s> is pressed", timestep,
                        input_mouse_button_string(MOUSE_BUTTON_MIDDLE));
  }

  if (input_mouse_button_is_released(MOUSE_BUTTON_MIDDLE)) {
    logger_debug_format("<on_update> <timestep:%f> <mouse_button:%s> is released", timestep,
                        input_mouse_button_string(MOUSE_BUTTON_MIDDLE));
  }

  if (input_mouse_button_is_pressed(MOUSE_BUTTON_RIGHT)) {
    logger_debug_format("<on_update> <timestep:%f> <mouse_button:%s> is pressed", timestep,
                        input_mouse_button_string(MOUSE_BUTTON_RIGHT));

    quad_rotation.z -= quad_rotation_angle;
  }

  if (input_mouse_button_is_released(MOUSE_BUTTON_RIGHT)) {
    logger_debug_format("<on_update> <timestep:%f> <mouse_button:%s> is released", timestep,
                        input_mouse_button_string(MOUSE_BUTTON_RIGHT));
  }

  if (input_mouse_scroll_is_up()) {
    logger_debug_format("<on_update> <timestep:%f> <scroll:%s> is up", timestep,
                        input_mouse_scroll_direction_string(MOUSE_SCROLL_DIRECTION_UP));

    quad_scale.x += quad_scale_speed * timestep;
    quad_scale.y += quad_scale_speed * timestep;
  }

  if (input_mouse_scroll_is_down()) {
    logger_debug_format("<on_update> <timestep:%f> <scroll:%s> is down", timestep,
                        input_mouse_scroll_direction_string(MOUSE_SCROLL_DIRECTION_DOWN));

    quad_scale.x -= quad_scale_speed * timestep;
    quad_scale.y -= quad_scale_speed * timestep;
  }

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_RIGHT)) {
    quad_position.x += quad_position_speed * timestep;
  }

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_LEFT)) {
    quad_position.x -= quad_position_speed * timestep;
  }

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_UP)) {
    quad_position.y -= quad_position_speed * timestep;
  }

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_DOWN)) {
    quad_position.y += quad_position_speed * timestep;
  }

  return true;
}

b8 on_render(renderer_t *renderer) {
  /* uniform buffer object update */
  f32 current_absolute_time = platform_absolute_time();
  f32 elapsed_time = start_absolute_time - current_absolute_time;

  uniform_buffer_object = (uniform_buffer_object_t){
      .time = elapsed_time,
      .view = matrix4f32_identity(),
      .projection = matrix4f32_identity(),
  };

  uniform_buffer_update(uniform_buffer, &uniform_buffer_object);

  /* instance buffer object update */
  matrix4f32_t quad_model_matrix = matrix4f32_identity();
  quad_model_matrix = matrix4f32_scale(quad_model_matrix, quad_scale);
  quad_model_matrix = matrix4f32_translate(quad_model_matrix, quad_position);
  quad_model_matrix = matrix4f32_rotate_euler(quad_model_matrix, quad_rotation);

  quad_instance_buffer_objects[0] = (instance_buffer_object_t){
      .model = quad_model_matrix,
  };

  quad_instance_buffer_objects[1] = (instance_buffer_object_t){
      .model = matrix4f32_translate(matrix4f32_identity(), (vector3f32_t){{0.4f, 0.3f, 0}}),
  };

  instance_buffer_update(quad_instance_buffer, quad_instance_buffer_objects, QUAD_INSTANCES_COUNT);

  /* graphics pipeline setup */
  if (!graphics_pipeline_bind(default_graphics_pipeline, renderer)) {
    logger_critical_format("<renderer:%p> <pipeline:%p> pipeline binding failed", renderer, default_graphics_pipeline);

    return false;
  }

  /* uniform buffer object setup */
  if (!uniform_buffer_bind(uniform_buffer, default_graphics_pipeline, renderer)) {
    logger_critical_format("<renderer:%p> <pipeline:%p> <uniform_buffer:%p> index buffer binding failed", renderer,
                           default_graphics_pipeline, uniform_buffer);

    return false;
  }

  /* geometry indices setup */
  if (!index_buffer_bind(quad_index_buffer, renderer)) {
    logger_critical_format("<renderer:%p> <pipeline:%p> <index_buffer:%p> index buffer binding failed", renderer,
                           default_graphics_pipeline, quad_index_buffer);

    return false;
  }

  /* geometry vertices setup */
  if (!vertex_buffer_bind(quad_vertex_buffer, renderer)) {
    logger_critical_format("<renderer:%p> <pipeline:%p> <vertex_buffer:%p> vertex buffer binding failed", renderer,
                           default_graphics_pipeline, quad_vertex_buffer);

    return false;
  }

  /* geometry instances setup */
  if (!instance_buffer_bind(quad_instance_buffer, default_graphics_pipeline, renderer)) {
    logger_critical_format("<renderer:%p> <pipeline:%p> <instance_buffer:%p> instance buffer binding failed", renderer,
                           default_graphics_pipeline, quad_instance_buffer);

    return false;
  }

  /* geometry draw call */
  if (!renderer_draw_indexed(renderer, QUAD_INDICES_COUNT, QUAD_INSTANCES_COUNT)) {
    logger_error_format("<renderer:%p> draw indexed command failed", renderer);

    return false;
  }

  return true;
}
