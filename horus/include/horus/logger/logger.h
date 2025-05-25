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
  LOGGER_LEVEL_COUNT
} logger_level_t;

b8 __logger_general(logger_level_t level, const char *message, ...);

#define logger_critical(message, ...) __logger_general(LOGGER_LEVEL_CRITICAL, message __VA_OPT__(, ) __VA_ARGS__)

#define logger_error(message, ...) __logger_general(LOGGER_LEVEL_ERROR, message __VA_OPT__(, ) __VA_ARGS__)

#define logger_warning(message, ...) __logger_general(LOGGER_LEVEL_WARNING, message __VA_OPT__(, ) __VA_ARGS__)

#define logger_info(message, ...) __logger_general(LOGGER_LEVEL_INFO, message __VA_OPT__(, ) __VA_ARGS__)

#define logger_debug(message, ...) __logger_general(LOGGER_LEVEL_DEBUG, message __VA_OPT__(, ) __VA_ARGS__)

#define logger_trace(message, ...) __logger_general(LOGGER_LEVEL_TRACE, message __VA_OPT__(, ) __VA_ARGS__)
