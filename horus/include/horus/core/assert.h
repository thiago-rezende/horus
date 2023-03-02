#pragma once

#include <horus/logger/logger.h>
#include <horus/platform/detection.h>

#if defined(HORUS_PLATFORM_LINUX)

#include <signal.h>

#define DEBUGGER_BREAK() raise(SIGTRAP)

#else

#define DEBUGGER_BREAK()

#endif

#define ASSERT(expression)                                                                                \
  if (!(expression)) {                                                                                    \
    HCRITICAL("<assertion:failed> <file:%s> <line:%d> <expression:%s>", __FILE__, __LINE__, #expression); \
    DEBUGGER_BREAK();                                                                                     \
  }

#define ASSERT_MESSAGE(expression, message, ...)                                                                  \
  if (!(expression)) {                                                                                            \
    HCRITICAL("<assertion:failed> <file:%s> <line:%d> <expression:%s> " message, __FILE__, __LINE__, #expression, \
              __VA_ARGS__);                                                                                       \
    DEBUGGER_BREAK();                                                                                             \
  }
