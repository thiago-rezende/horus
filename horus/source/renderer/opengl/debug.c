/* horus logger layer */
#include <horus/logger/logger.h>

/* horus renderer layer [ opengl ] */
#include <horus/renderer/opengl/debug.h>

const char *gl_debug_source_api_string = "api";
const char *gl_debug_source_other_string = "other";
const char *gl_debug_source_unknown_string = "unknown";
const char *gl_debug_source_application_string = "application";
const char *gl_debug_source_third_party_string = "third_party";
const char *gl_debug_source_window_system_string = "window_system";
const char *gl_debug_source_shader_compiler_string = "shader_compiler";

const char *gl_debug_type_error_string = "error";
const char *gl_debug_type_other_string = "other";
const char *gl_debug_type_marker_string = "marker";
const char *gl_debug_type_unknown_string = "unknown";
const char *gl_debug_type_pop_group_string = "pop_group";
const char *gl_debug_type_push_group_string = "push_group";
const char *gl_debug_type_performance_string = "performance";
const char *gl_debug_type_portability_string = "portability";
const char *gl_debug_type_undefined_behavior_string = "undefined_behavior";
const char *gl_debug_type_deprecated_behavior_string = "deprecated_behavior";

const char *gl_debug_severity_low_string = "low";
const char *gl_debug_severity_high_string = "high";
const char *gl_debug_severity_medium_string = "medium";
const char *gl_debug_severity_unknown_string = "unknown";
const char *gl_debug_severity_notification_string = "notification";

void renderer_opengl_debug_callback(GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const GLchar *message,
                                    const void *user_parameter) {
  char *type_string;
  char *source_string;
  char *severity_string;

  logger_level_t logger_level = LOGGER_LEVEL_DEBUG;

  switch (source) {
    case GL_DEBUG_SOURCE_API: {
      source_string = (char *)gl_debug_source_api_string;

      break;
    }

    case GL_DEBUG_SOURCE_OTHER: {
      source_string = (char *)gl_debug_source_other_string;

      break;
    }

    case GL_DEBUG_SOURCE_APPLICATION: {
      source_string = (char *)gl_debug_source_application_string;

      break;
    }

    case GL_DEBUG_SOURCE_THIRD_PARTY: {
      source_string = (char *)gl_debug_source_third_party_string;

      break;
    }

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
      source_string = (char *)gl_debug_source_window_system_string;

      break;
    }

    case GL_DEBUG_SOURCE_SHADER_COMPILER: {
      source_string = (char *)gl_debug_source_shader_compiler_string;

      break;
    }

    default: {
      source_string = (char *)gl_debug_source_unknown_string;
    }
  }

  switch (type) {
    case GL_DEBUG_TYPE_ERROR: {
      type_string = (char *)gl_debug_type_error_string;

      break;
    }

    case GL_DEBUG_TYPE_OTHER: {
      type_string = (char *)gl_debug_type_other_string;

      break;
    }

    case GL_DEBUG_TYPE_MARKER: {
      type_string = (char *)gl_debug_type_marker_string;

      break;
    }

    case GL_DEBUG_TYPE_POP_GROUP: {
      type_string = (char *)gl_debug_type_pop_group_string;

      break;
    }

    case GL_DEBUG_TYPE_PUSH_GROUP: {
      type_string = (char *)gl_debug_type_push_group_string;

      break;
    }

    case GL_DEBUG_TYPE_PERFORMANCE: {
      type_string = (char *)gl_debug_type_performance_string;

      break;
    }

    case GL_DEBUG_TYPE_PORTABILITY: {
      type_string = (char *)gl_debug_type_portability_string;

      break;
    }

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
      type_string = (char *)gl_debug_type_undefined_behavior_string;

      break;
    }

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
      type_string = (char *)gl_debug_type_deprecated_behavior_string;

      break;
    }

    default: {
      type_string = (char *)gl_debug_type_unknown_string;
    }
  }

  switch (severity) {
    case GL_DEBUG_SEVERITY_LOW: {
      severity_string = (char *)gl_debug_severity_low_string;

      break;
    }

    case GL_DEBUG_SEVERITY_HIGH: {
      logger_level = LOGGER_LEVEL_ERROR;

      severity_string = (char *)gl_debug_severity_high_string;

      break;
    }

    case GL_DEBUG_SEVERITY_MEDIUM: {
      logger_level = LOGGER_LEVEL_WARNING;

      severity_string = (char *)gl_debug_severity_medium_string;

      break;
    }

    case GL_DEBUG_SEVERITY_NOTIFICATION: {
      logger_level = LOGGER_LEVEL_TRACE;

      severity_string = (char *)gl_debug_severity_notification_string;

      break;
    }

    default: {
      severity_string = (char *)gl_debug_severity_unknown_string;
    }
  }

  __logger_general_format(logger_level, "<opengl> <id:%u> <source:%s> <type:%s> <severity:%s>\n|- [ message ] %s", id,
                          source_string, type_string, severity_string, message);
}
