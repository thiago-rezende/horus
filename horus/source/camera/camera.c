/* horus camera layer */
#include <horus/camera/camera.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus logger layer */
#include <horus/logger/logger.h>

b8 __camera_update_view_matrix(camera_t *camera);

camera_t *camera_create(vector3f32_t position, quaternionf32_t rotation) {
  camera_t *camera = platform_memory_allocate(sizeof(camera_t));

  *camera = (camera_t){
      .view_matrix = matrix4f32_identity(),

      .position = position,
      .rotation = rotation,

      .should_update_view_matrix = true,
  };

  __camera_update_view_matrix(camera);

  return camera;
}

b8 camera_destroy(camera_t *camera) {
  (void)camera; /* unused */

  platform_memory_deallocate(camera);

  return false;
}

b8 camera_get_position(camera_t *camera, vector3f32_t *position) {
  if (position == NULL) {
    logger_error_format("<camera:%p> <position:%p> invalid output parameter", (void *)camera, (void *)position);

    return false;
  }

  *position = camera->position;

  return true;
}

b8 camera_set_position(camera_t *camera, vector3f32_t position) {
  camera->position = position;

  camera->should_update_view_matrix = true;

  return true;
}

b8 camera_get_rotation(camera_t *camera, quaternionf32_t *rotation) {
  if (rotation == NULL) {
    logger_error_format("<camera:%p> <rotation:%p> invalid output parameter", (void *)camera, (void *)rotation);

    return false;
  }

  *rotation = camera->rotation;

  return true;
}

b8 camera_set_rotation(camera_t *camera, quaternionf32_t rotation) {
  camera->rotation = rotation;

  camera->should_update_view_matrix = true;

  return true;
}

b8 camera_look_at(camera_t *camera, vector3f32_t target, vector3f32_t up) {
  vector3f32_t z_axis = vector3f32_subtract(camera->position, target);
  z_axis = vector3f32_normalize(z_axis);

  vector3f32_t x_axis = vector3f32_cross(up, z_axis);
  x_axis = vector3f32_normalize(x_axis);

  vector3f32_t y_axis = vector3f32_cross(z_axis, x_axis);

  camera->rotation = quaternionf32_from_axes(x_axis, y_axis, z_axis);

  camera->should_update_view_matrix = true;

  return true;
}

b8 camera_rotate_euler(camera_t *camera, vector3f32_t angles) {
  camera->rotation = quaternionf32_rotate_euler(camera->rotation, angles);

  camera->should_update_view_matrix = true;

  return true;
}

matrix4f32_t camera_view_matrix(camera_t *camera) {
  __camera_update_view_matrix(camera);

  return camera->view_matrix;
}

vector3f32_t camera_up_vector(camera_t *camera) {
  __camera_update_view_matrix(camera);

  return (vector3f32_t){{
      camera->view_matrix.column0[1],
      camera->view_matrix.column1[1],
      camera->view_matrix.column2[1],
  }};
}

vector3f32_t camera_down_vector(camera_t *camera) {
  __camera_update_view_matrix(camera);

  return (vector3f32_t){{
      -(camera->view_matrix.column0[1]),
      -(camera->view_matrix.column1[1]),
      -(camera->view_matrix.column2[1]),
  }};
}

vector3f32_t camera_left_vector(camera_t *camera) {
  __camera_update_view_matrix(camera);

  return (vector3f32_t){{
      -(camera->view_matrix.column0[0]),
      -(camera->view_matrix.column1[0]),
      -(camera->view_matrix.column2[0]),
  }};
}

vector3f32_t camera_right_vector(camera_t *camera) {
  __camera_update_view_matrix(camera);

  return (vector3f32_t){{
      camera->view_matrix.column0[0],
      camera->view_matrix.column1[0],
      camera->view_matrix.column2[0],
  }};
}

vector3f32_t camera_forward_vector(camera_t *camera) {
  __camera_update_view_matrix(camera);

  return (vector3f32_t){{
      -(camera->view_matrix.column0[2]),
      -(camera->view_matrix.column1[2]),
      -(camera->view_matrix.column2[2]),
  }};
}

vector3f32_t camera_backward_vector(camera_t *camera) {
  __camera_update_view_matrix(camera);

  return (vector3f32_t){{
      camera->view_matrix.column0[2],
      camera->view_matrix.column1[2],
      camera->view_matrix.column2[2],
  }};
}

b8 camera_move_up(camera_t *camera, f32 amount) {
  vector3f32_t diriction = vector3f32_up();
  vector3f32_t direction_scaled = vector3f32_scalar(diriction, amount);

  camera->position = vector3f32_add(camera->position, direction_scaled);
  camera->should_update_view_matrix = true;

  return true;
}

b8 camera_move_down(camera_t *camera, f32 amount) {
  vector3f32_t diriction = vector3f32_down();
  vector3f32_t direction_scaled = vector3f32_scalar(diriction, amount);

  camera->position = vector3f32_add(camera->position, direction_scaled);
  camera->should_update_view_matrix = true;

  return true;
}

b8 camera_move_left(camera_t *camera, f32 amount) {
  vector3f32_t diriction = camera_left_vector(camera);
  vector3f32_t direction_scaled = vector3f32_scalar(diriction, amount);

  camera->position = vector3f32_add(camera->position, direction_scaled);
  camera->should_update_view_matrix = true;

  return true;
}

b8 camera_move_right(camera_t *camera, f32 amount) {
  vector3f32_t diriction = camera_right_vector(camera);
  vector3f32_t direction_scaled = vector3f32_scalar(diriction, amount);

  camera->position = vector3f32_add(camera->position, direction_scaled);
  camera->should_update_view_matrix = true;

  return true;
}

b8 camera_move_forward(camera_t *camera, f32 amount) {
  vector3f32_t diriction = camera_forward_vector(camera);
  vector3f32_t direction_scaled = vector3f32_scalar(diriction, amount);

  camera->position = vector3f32_add(camera->position, direction_scaled);
  camera->should_update_view_matrix = true;

  return true;
}

b8 camera_move_backward(camera_t *camera, f32 amount) {
  vector3f32_t diriction = camera_backward_vector(camera);
  vector3f32_t direction_scaled = vector3f32_scalar(diriction, amount);

  camera->position = vector3f32_add(camera->position, direction_scaled);
  camera->should_update_view_matrix = true;

  return true;
}

b8 __camera_update_view_matrix(camera_t *camera) {
  if (camera->should_update_view_matrix) {
    quaternionf32_t rotation_conjugate = quaternionf32_to_conjugate(camera->rotation);

    camera->view_matrix = quaternionf32_to_matrix(rotation_conjugate);

    vector3f32_t negative_position = vector3f32_scalar(camera->position, -1.0f);

    vector3f32_t x_row = {{
        camera->view_matrix.column0[0],
        camera->view_matrix.column1[0],
        camera->view_matrix.column2[0],
    }};

    vector3f32_t y_row = {{
        camera->view_matrix.column0[1],
        camera->view_matrix.column1[1],
        camera->view_matrix.column2[1],
    }};

    vector3f32_t z_row = {{
        camera->view_matrix.column0[2],
        camera->view_matrix.column1[2],
        camera->view_matrix.column2[2],
    }};

    f32 dot_x = vector3f32_dot(x_row, negative_position);
    f32 dot_y = vector3f32_dot(y_row, negative_position);
    f32 dot_z = vector3f32_dot(z_row, negative_position);

    camera->view_matrix.column3 = (__v4f32){dot_x, dot_y, dot_z, 1.0f};

    camera->should_update_view_matrix = false;
  }

  return true;
}
