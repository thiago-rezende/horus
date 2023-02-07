#pragma once

/* check for emscripten */
#if defined(__EMSCRIPTEN__)

#define HORUS_PLATFORM_EMSCRIPTEN
#error "[ platform ]: emscripten platform is not supported!"

/* check for windows platform */
#elif defined(_WIN32)

#define HORUS_PLATFORM_WINDOWS
#error "[ platform ]: windows platform is not supported!"

/* Windows x64/x86 */
#if defined(_WIN64)

/* Windows x64  */
#define HORUS_PLATFORM_WINDOWS_64
#error "[ platform ]: windows x64 builds are not supported!"

#else  // _WIN64

/* Windows x86 */
#define HORUS_PLATFORM_WINDOWS_86
#error "[ platform ]: windows x86 builds are not supported!"

#endif  // _WIN32

/* check for apple flatofrms */
#elif defined(__APPLE__) || defined(__MACH__)

#define HORUS_PLATFORM_APPLE
#error "[ platform ]: apple platforms are not supported!"

#include <TargetConditionals.h>

/* TARGET_OS_MAC exists on all Apple platforms
 * so we must check all of them (in this order)
 * to ensure that we're running on MAC
 * and not some other Apple platform
 */
#if TARGET_IPHONE_SIMULATOR == 1

#define HORUS_PLATFORM_IOS_SIMULATOR
#error "[ platform ]: ios simulator is not supported!"

#elif TARGET_OS_IPHONE == 1

#define HORUS_PLATFORM_IOS
#error "[ platform ]: ios is not supported!"

#elif TARGET_OS_MAC == 1

#define HORUS_PLATFORM_MACOS
#error "[ platform ]: macos is not supported!"

#else

#error "[ platform ]: unknown apple platform!"

#endif  // __APPLE__ || __MACH__

/* check for __android__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined
 */
#elif defined(__ANDROID__)

#define HORUS_PLATFORM_ANDROID
#error "[ platform ]: android is not supported!"

/* check for linux */
#elif defined(__linux__)

#define HORUS_PLATFORM_LINUX
/* #error "[ platform ]: linux is not supported!" */

/* check for freebsd */
#elif defined(__FreeBSD__)
#define HORUS_PLATFORM_FREEBSD
#error "[ platform ]: freebsd is not supported!"

#else  // __FreeBSD__

/* Unknown compiler/platform */
#error "[ platform ]: unknown platform!"

#endif
