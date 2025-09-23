#pragma once

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/debugger.h>

#define assert(expression)                                                                                             \
  if (!(expression)) {                                                                                                 \
    logger_critical_format("<assertion:failed> <file:%s> <line:%d> <expression:%s>", __FILE__, __LINE__, #expression); \
    platform_debugger_break();                                                                                         \
  }

#define assert_message(expression, message)                                                                       \
  if (!(expression)) {                                                                                            \
    logger_critical_format("<assertion:failed> <file:%s> <line:%d> <expression:%s> " message, __FILE__, __LINE__, \
                           #expression);                                                                          \
    platform_debugger_break();                                                                                    \
  }

#define assert_message_format(expression, message, ...)                                                           \
  if (!(expression)) {                                                                                            \
    logger_critical_format("<assertion:failed> <file:%s> <line:%d> <expression:%s> " message, __FILE__, __LINE__, \
                           #expression, __VA_ARGS__);                                                             \
    platform_debugger_break();                                                                                    \
  }
