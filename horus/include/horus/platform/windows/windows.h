#pragma once

/* Target Windows 10 or later */
#if defined(WINVER)
#undef WINVER
#endif

#if defined(_WIN32_WINNT)
#undef _WIN32_WINNT
#endif

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>

/**
 * The following definitions disable a bunch of unused windows stuff
 * If you get weird errors when trying to do some windows stuff, try
 * removing some (or all) of these defines (it will increase build time though).
 *
 */
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
/* #define NOSYSMETRICS */
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE

/**
 * This will prevent the definition of the min and max Windows-specific
 * preprocessor macros.
 *
 */
#define NOMINMAX

/**
 * When you define the STRICT symbol, you enable features that require more care
 * in declaring and using types. This helps you write more portable code. This
 * extra care will also reduce your debugging time.
 *
 */
#define STRICT

/**
 * The plain versions without the underscore affect the character set the
 * Windows header files treat as default. So if you define UNICODE, then
 * GetWindowText will map to GetWindowTextW instead of GetWindowTextA, for
 * example. Similarly, the TEXT macro will map to L"..." instead of "...".
 *
 * The versions with the underscore affect the character set the C runtime
 * header files treat as default. So if you define _UNICODE, then _tcslen will
 * map to wcslen instead of strlen, for example. Similarly, the _TEXT macro will
 * map to L"..." instead of "...".
 *
 */
/* #define UNICODE */
/* #define _UNICODE */

/* Windows header file */
#include <windows.h>
#include <windowsx.h>
