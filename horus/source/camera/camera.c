/* horus camera layer */
#include <horus/camera/camera.h>

/* horus platform layer */
#include <horus/platform/memory.h>

/* horus logger layer */
#include <horus/logger/logger.h>

static const char *camera_type_strings[CAMERA_TYPE_COUNT] = {
    [CAMERA_TYPE_NONE] = "none",
    [CAMERA_TYPE_FIXED] = "fixed",
    [CAMERA_TYPE_SPLINE] = "spline",
    [CAMERA_TYPE_FREE_LOOK] = "free_look",
    [CAMERA_TYPE_ORTHOGONAL] = "orthogonal",
    [CAMERA_TYPE_FIRST_PERSON] = "first_person",
    [CAMERA_TYPE_THIRD_PERSON] = "third_person",
};

static const char *camera_projection_strings[CAMERA_PROJECTION_COUNT] = {
    [CAMERA_PROJECTION_NONE] = "none",
    [CAMERA_PROJECTION_PERSPECTIVE] = "perspective",
    [CAMERA_PROJECTION_ORTHOGRAPHIC] = "orthographic",
};

b8 __camera_update_fixed(camera_t *camera, camera_update_info_t info);
b8 __camera_update_spline(camera_t *camera, camera_update_info_t info);
b8 __camera_update_free_look(camera_t *camera, camera_update_info_t info);
b8 __camera_update_orthogonal(camera_t *camera, camera_update_info_t info);
b8 __camera_update_first_person(camera_t *camera, camera_update_info_t info);
b8 __camera_update_third_person(camera_t *camera, camera_update_info_t info);

b8 __camera_update_perspective(camera_t *camera, camera_update_info_t info);
b8 __camera_update_orthographic(camera_t *camera, camera_update_info_t info);

typedef b8 (*camera_update_fn_t)(camera_t *camera, camera_update_info_t info);
typedef b8 (*camera_update_projection_fn_t)(camera_t *camera, camera_update_info_t info);

static camera_update_fn_t camera_update_functions[CAMERA_TYPE_COUNT] = {
    [CAMERA_TYPE_NONE] = NULL,
    [CAMERA_TYPE_FIXED] = __camera_update_fixed,
    [CAMERA_TYPE_SPLINE] = __camera_update_spline,
    [CAMERA_TYPE_FREE_LOOK] = __camera_update_free_look,
    [CAMERA_TYPE_ORTHOGONAL] = __camera_update_orthogonal,
    [CAMERA_TYPE_FIRST_PERSON] = __camera_update_first_person,
    [CAMERA_TYPE_THIRD_PERSON] = __camera_update_third_person,
};

static camera_update_projection_fn_t camera_update_projection_functions[CAMERA_TYPE_COUNT] = {
    [CAMERA_TYPE_NONE] = NULL,
    [CAMERA_PROJECTION_PERSPECTIVE] = __camera_update_perspective,
    [CAMERA_PROJECTION_ORTHOGRAPHIC] = __camera_update_orthographic,
};

camera_t *camera_create(camera_create_info_t info) {
  camera_t *camera = platform_memory_allocate(sizeof(camera_t));

  *camera = (camera_t){
      .type = info.type,
      .projection = info.projection,

      .speed = info.speed,

      .view_matrix = matrix4f32_identity(),
      .projection_matrix = matrix4f32_identity(),

      .up = (vector3f32_t){0}, /* TODO: build the up vector from rotation angles */
      .target = info.target,
      .position = info.position,

      .rotation = info.rotation,

      .far_plane = info.far_plane,
      .near_plane = info.near_plane,
      .field_of_view = info.field_of_view,

      .width = info.width,
      .height = info.height,
  };

  return camera;
}

b8 camera_destroy(camera_t *camera) {
  (void)camera; /* unused */

  platform_memory_deallocate(camera);

  return false;
}

b8 camera_update(camera_t *camera, camera_update_info_t info) {
  (void)info;   /* unused */
  (void)camera; /* unused */

  if (camera->type < CAMERA_TYPE_NONE || camera->type >= CAMERA_TYPE_COUNT) {
    logger_critical_format("<camera:%p> <type:%d> unknown camera type", camera, camera->type);

    return false;
  }

  if (camera->projection < CAMERA_PROJECTION_NONE || camera->projection >= CAMERA_PROJECTION_COUNT) {
    logger_critical_format("<camera:%p> <type:%s> <projection:%d> unknown camera projection", camera,
                           camera_type_string(camera->type), camera->projection);

    return false;
  }

  camera_update_fn_t camera_update_function = camera_update_functions[camera->type];
  camera_update_projection_fn_t camera_update_projection_function =
      camera_update_projection_functions[camera->projection];

  if (camera_update_function && !camera_update_function(camera, info)) {
    logger_critical_format("<camera:%p> <type:%s> <projection:%s> camera update function failed", camera,
                           camera_type_string(camera->type), camera_projection_string(camera->projection));

    return false;
  }

  if (camera_update_projection_function && !camera_update_projection_function(camera, info)) {
    logger_critical_format("<camera:%p> <type:%s> <projection:%s> camera update projection function failed", camera,
                           camera_type_string(camera->type), camera_projection_string(camera->projection));

    return false;
  }

  return true;
}

const char *camera_type_string(camera_type_t type) {
  if (type < CAMERA_TYPE_COUNT && type >= CAMERA_TYPE_NONE) {
    return camera_type_strings[type];
  }

  return "unknown";
}

const char *camera_projection_string(camera_projection_t projection) {
  if (projection < CAMERA_PROJECTION_COUNT && projection >= CAMERA_PROJECTION_NONE) {
    return camera_projection_strings[projection];
  }

  return "unknown";
}

b8 __camera_update_fixed(camera_t *camera, camera_update_info_t info) {
  (void)info;   /* unused */
  (void)camera; /* unused */

  logger_critical("<__camera_update_fixed> not implemented");

  return false;
}

b8 __camera_update_spline(camera_t *camera, camera_update_info_t info) {
  (void)info;   /* unused */
  (void)camera; /* unused */

  logger_critical("<__camera_update_spline> not implemented");

  return false;
}

b8 __camera_update_free_look(camera_t *camera, camera_update_info_t info) {
  (void)info;   /* unused */
  (void)camera; /* unused */

  logger_critical("<__camera_update_free_look> not implemented");

  return false;
}

b8 __camera_update_orthogonal(camera_t *camera, camera_update_info_t info) {
  (void)info;   /* unused */
  (void)camera; /* unused */

  logger_critical("<__camera_update_orthogonal> not implemented");

  return false;
}

b8 __camera_update_first_person(camera_t *camera, camera_update_info_t info) {
  (void)info;   /* unused */
  (void)camera; /* unused */

  logger_critical("<__camera_update_first_person> not implemented");

  return false;
}

b8 __camera_update_third_person(camera_t *camera, camera_update_info_t info) {
  (void)info;   /* unused */
  (void)camera; /* unused */

  logger_critical("<__camera_update_third_person> not implemented");

  return false;
}

b8 __camera_update_perspective(camera_t *camera, camera_update_info_t info) {
  (void)info;   /* unused */
  (void)camera; /* unused */

  logger_critical("<__camera_update_perspective> not implemented");

  return false;
}

b8 __camera_update_orthographic(camera_t *camera, camera_update_info_t info) {
  (void)info;   /* unused */
  (void)camera; /* unused */

  logger_critical("<__camera_update_orthographic> not implemented");

  return false;
}
