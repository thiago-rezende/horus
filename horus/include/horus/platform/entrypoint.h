#pragma once

/* horus platform layer */
#include <horus/platform/detection.h>

#if defined(HORUS_PLATFORM_LINUX)

/* horus platform layer [ linux ] */
#include <horus/platform/linux/entrypoint.h>

#endif

#if defined(HORUS_PLATFORM_WINDOWS)

/* horus platform layer [ windows ] */
#include <horus/platform/windows/entrypoint.h>

#endif
