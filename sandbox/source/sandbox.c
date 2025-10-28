#include <horus/horus.h>

#include <sandbox/sandbox.h>

#define QUAD_INDICES_COUNT 6
#define QUAD_VERTICES_COUNT 4
#define QUAD_INSTANCES_COUNT 2

/* shader modules global variables */
const char *default_shader_module_path = "assets/shaders/build/default.spv";

shader_module_t *default_shader_module = NULL;

/* graphics pipelines global variables */
graphics_pipeline_t *default_graphics_pipeline = NULL;

/* uniform buffer global variables */
f32 start_absolute_time = 0.0f;
uniform_buffer_t *uniform_buffer = NULL;
uniform_buffer_object_t uniform_buffer_object = {0};

/* quad buffers global variables */
index_buffer_t *quad_index_buffer = NULL;
vertex_buffer_t *quad_vertex_buffer = NULL;

instance_buffer_t *quad_instance_buffer = NULL;
instance_buffer_object_t quad_instance_buffer_objects[QUAD_INSTANCES_COUNT] = {0};

u32 quad_indices[QUAD_INDICES_COUNT] = {0, 1, 2, 2, 3, 0};

vertex_t quad_vertices[QUAD_VERTICES_COUNT] = {
    (vertex_t){.position = {{-0.5f, -0.5f, 0.0f}}, .color = {{255, 255, 0, 255}}},
    (vertex_t){.position = {{0.5f, -0.5f, 0.0f}}, .color = {{255, 255, 0, 255}}},
    (vertex_t){.position = {{0.5f, 0.5f, 0.0f}}, .color = {{0, 255, 255, 255}}},
    (vertex_t){.position = {{-0.5f, 0.5f, 0.0f}}, .color = {{0, 255, 255, 255}}},
};

f32 quad_scale_speed = 2.0f;
f32 quad_position_speed = 1.0f;
f32 quad_rotation_angle = 45.0f;

vector3f32_t quad_scale = {{2.0f, 1.0f, 0.0f}};
vector3f32_t quad_position = {{-0.4f, -0.3f, 0}};
quaternionf32_t quad_rotation = {{0.0f, 0.0f, 0.0f, 1.0f}};

/* camera global variables */
camera_t *camera = NULL;
camera_type_t camera_type = CAMERA_TYPE_FIXED;
camera_projection_t camera_projection = CAMERA_PROJECTION_PERSPECTIVE;

f32 camera_speed = 1.0f;
f32 camera_rotation_angle = 45.0f;

vector3f32_t camera_up = {{0.0f, 1.0f, 0.0f}};
vector3f32_t camera_target = {0};
vector3f32_t camera_position = {{0.0f, 0.0f, 2.0f}};

quaternionf32_t camera_rotation = {{0.0f, 0.0f, 0.0f, 1.0f}};

f32 camera_zoom = 2.0f;
f32 camera_far_plane = 100.0f;
f32 camera_near_plane = 0.1f;
f32 camera_field_of_view = 60.0f;

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

b8 on_create(application_t *application, platform_window_t *window, renderer_t *renderer) {
  (void)window;      /* unused */
  (void)application; /* unused */

  /* TODO: proper error handling */
  default_shader_module = shader_module_create_from_binary(renderer, SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT,
                                                           (char *)default_shader_module_path);

  logger_info_format("<renderer:%p> <module:%p> <path:%s> created", (void *)renderer, (void *)default_shader_module,
                     default_shader_module_path);

  /* TODO: proper error handling */
  default_graphics_pipeline = graphics_pipeline_create(renderer, default_shader_module);

  logger_info_format("<renderer:%p> <pipeline:%p> created", (void *)renderer, (void *)default_graphics_pipeline);

  /* TODO: proper error handling */
  uniform_buffer = uniform_buffer_create(renderer, &uniform_buffer_object);

  logger_info_format("<renderer:%p> <uniform_buffer:%p> created", (void *)renderer, (void *)uniform_buffer);

  /* TODO: proper error handling */
  quad_index_buffer = index_buffer_create(renderer, quad_indices, QUAD_INDICES_COUNT);

  logger_info_format("<renderer:%p> <index_buffer:%p> created", (void *)renderer, (void *)quad_index_buffer);

  /* TODO: proper error handling */
  quad_vertex_buffer = vertex_buffer_create(renderer, quad_vertices, QUAD_VERTICES_COUNT);

  logger_info_format("<renderer:%p> <vertex_buffer:%p> created", (void *)renderer, (void *)quad_vertex_buffer);

  /* TODO: proper error handling */
  quad_instance_buffer = instance_buffer_create(renderer, quad_instance_buffer_objects, QUAD_INSTANCES_COUNT);

  logger_info_format("<renderer:%p> <instance_buffer:%p> created", (void *)renderer, (void *)quad_instance_buffer);

  /* TODO: proper error handling */
  platform_window_size_t window_size = platform_window_size(window);

  camera_create_info_t camera_create_info = (camera_create_info_t){
      /* general info */
      .type = camera_type,
      .projection = camera_projection,

      /* movement info */
      .speed = camera_speed,

      /* position info */
      .up = camera_up,
      .target = camera_target,
      .position = camera_position,

      /* rotation info */
      .rotation = camera_rotation,

      /* frustum info */
      .zoom = camera_zoom,
      .far_plane = camera_far_plane,
      .near_plane = camera_near_plane,
      .field_of_view = camera_field_of_view,

      /* projection info */
      .width = window_size.width,
      .height = window_size.height,
  };

  camera = camera_create(camera_create_info);

  logger_info_format("<camera:%p> <type:%s> <projection:%s> created", (void *)camera, camera_type_string(camera->type),
                     camera_projection_string(camera->projection));

  return true;
}

b8 on_destroy(application_t *application, platform_window_t *window, renderer_t *renderer) {
  (void)window;      /* unused */
  (void)application; /* unused */

  camera_destroy(camera);

  logger_info_format("<camera:%p> destroyed", (void *)camera);

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

    quad_rotation =
        quaternionf32_rotate_euler(quad_rotation, (vector3f32_t){{0.0f, 0.0f, quad_rotation_angle * timestep}});
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

    quad_rotation =
        quaternionf32_rotate_euler(quad_rotation, (vector3f32_t){{0.0f, 0.0f, -1.0f * quad_rotation_angle * timestep}});
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

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_SPACE)) {
    quad_position.z += quad_position_speed * timestep;
  }

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_LEFT_CONTROL)) {
    quad_position.z -= quad_position_speed * timestep;
  }

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_P)) {
    camera->projection = CAMERA_PROJECTION_PERSPECTIVE;
  }

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_O)) {
    camera->projection = CAMERA_PROJECTION_ORTHOGRAPHIC;
  }

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_A)) {
    camera->position.x -= camera_speed * timestep;
  }

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_D)) {
    camera->position.x += camera_speed * timestep;
  }

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_S)) {
    camera->position.y += camera_speed * timestep;
  }

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_W)) {
    camera->position.y -= camera_speed * timestep;
  }

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_E)) {
    camera->rotation =
        quaternionf32_rotate_euler(camera->rotation, (vector3f32_t){{0.0f, 0.0f, camera_rotation_angle * timestep}});
  }

  if (input_keyboard_keycode_is_pressed(KEYBOARD_KEYCODE_Q)) {
    camera->rotation = quaternionf32_rotate_euler(
        camera->rotation, (vector3f32_t){{0.0f, 0.0f, -1.0f * camera_rotation_angle * timestep}});
  }

  platform_window_size_t size = platform_window_size(window);

  camera_update_info_t camera_update_info = (camera_update_info_t){
      /* general info */
      .timestep = timestep,

      /* position info */
      .target = camera_target,

      /* projection info */
      .width = size.width,
      .height = size.height,
  };

  camera_update(camera, camera_update_info);

  return true;
}

b8 on_render(renderer_t *renderer) {
  /* uniform buffer object update */
  f32 current_absolute_time = platform_absolute_time();
  f32 elapsed_time = start_absolute_time - current_absolute_time;

  uniform_buffer_object = (uniform_buffer_object_t){
      .time = elapsed_time,
      .view = camera->view_matrix,
      .projection = camera->projection_matrix,
  };

  uniform_buffer_update(uniform_buffer, &uniform_buffer_object);

  /* instance buffer object update */
  matrix4f32_t quad_model_matrix = matrix4f32_identity();
  quad_model_matrix = matrix4f32_multiply(quad_model_matrix, quaternionf32_to_matrix(quad_rotation));
  quad_model_matrix = matrix4f32_scale(quad_model_matrix, quad_scale);
  quad_model_matrix = matrix4f32_translate(quad_model_matrix, quad_position);

  quad_instance_buffer_objects[0] = (instance_buffer_object_t){
      .model = quad_model_matrix,
  };

  quad_instance_buffer_objects[1] = (instance_buffer_object_t){
      .model = matrix4f32_translate(matrix4f32_scale(matrix4f32_identity(), (vector3f32_t){{2.0f, 1.0f, 1.0f}}),
                                    (vector3f32_t){{0.4f, 0.3f, 0}}),
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
