#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus platform layer */
#include <horus/platform/detection.h>

#if defined(HORUS_PLATFORM_LINUX)

#include <signal.h>

#define platform_debugger_break() raise(SIGTRAP)

#elif defined(HORUS_PLATFORM_WINDOWS)

#define platform_debugger_break() __debugbreak()

#else

#define platform_debugger_break()

#endif
