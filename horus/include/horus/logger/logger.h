#pragma once

/* horus core layer */
#include <horus/core/types.h>

typedef enum __logger_level {
  LOGGER_LEVEL_CRITICAL = 0,
  LOGGER_LEVEL_ERROR,
  LOGGER_LEVEL_WARNING,
  LOGGER_LEVEL_INFO,
  LOGGER_LEVEL_TRACE,
  LOGGER_LEVEL_DEBUG,
  LOGGER_LEVEL_COUNT,
} logger_level_t;

b8 __logger_general(logger_level_t level, const char *message);
b8 __logger_general_format(logger_level_t level, const char *message, ...);

#define logger_critical(message, ...) __logger_general(LOGGER_LEVEL_CRITICAL, message)

#define logger_error(message, ...) __logger_general(LOGGER_LEVEL_ERROR, message)

#define logger_warning(message, ...) __logger_general(LOGGER_LEVEL_WARNING, message)

#define logger_info(message, ...) __logger_general(LOGGER_LEVEL_INFO, message)

#define logger_debug(message, ...) __logger_general(LOGGER_LEVEL_DEBUG, message)

#define logger_trace(message, ...) __logger_general(LOGGER_LEVEL_TRACE, message)

#define logger_critical_format(message, ...) __logger_general_format(LOGGER_LEVEL_CRITICAL, message, __VA_ARGS__)

#define logger_error_format(message, ...) __logger_general_format(LOGGER_LEVEL_ERROR, message, __VA_ARGS__)

#define logger_warning_format(message, ...) __logger_general_format(LOGGER_LEVEL_WARNING, message, __VA_ARGS__)

#define logger_info_format(message, ...) __logger_general_format(LOGGER_LEVEL_INFO, message, __VA_ARGS__)

#define logger_debug_format(message, ...) __logger_general_format(LOGGER_LEVEL_DEBUG, message, __VA_ARGS__)

#define logger_trace_format(message, ...) __logger_general_format(LOGGER_LEVEL_TRACE, message, __VA_ARGS__)
