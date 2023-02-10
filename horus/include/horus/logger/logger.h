#pragma once

typedef enum {
  LOGGER_LEVEL_CRITICAL = 0,
  LOGGER_LEVEL_ERROR,
  LOGGER_LEVEL_WARNING,
  LOGGER_LEVEL_INFO,
  LOGGER_LEVEL_DEBUG,
  LOGGER_LEVEL_TRACE,
  LOGGER_LEVEL_COUNT
} logger_level;

void logger(logger_level level, const char *message, ...);

#define HCRITICAL(message, ...) logger(LOGGER_LEVEL_CRITICAL, message __VA_OPT__(, ) __VA_ARGS__)

#define HERROR(message, ...) logger(LOGGER_LEVEL_ERROR, message __VA_OPT__(, ) __VA_ARGS__)

#define HWARNING(message, ...) logger(LOGGER_LEVEL_WARNING, message __VA_OPT__(, ) __VA_ARGS__)

#define HINFO(message, ...) logger(LOGGER_LEVEL_INFO, message __VA_OPT__(, ) __VA_ARGS__)

#define HDEBUG(message, ...) logger(LOGGER_LEVEL_DEBUG, message __VA_OPT__(, ) __VA_ARGS__)

#define HTRACE(message, ...) logger(LOGGER_LEVEL_TRACE, message __VA_OPT__(, ) __VA_ARGS__)
