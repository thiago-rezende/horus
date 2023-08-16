#pragma once

#include <horus/core/types.h>
#include <horus/platform/detection.h>

#if defined(HORUS_PLATFORM_LINUX)

#include <signal.h>

#define platform_debugger_break() raise(SIGTRAP)

#else

#define platform_debugger_break()

#endif
