#pragma once

// Source: https://github.com/SFML/SFML/blob/master/include/SFML/Config.hpp

// Identify the operating system
#if defined(_WIN32)
#define AW_OS_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
// iOS
#define AW_OS_IOS
#elif TARGET_OS_MAC
// MacOS
#define AW_OS_MACOS
#else
// Unsupported Apple system
#error This Apple operating system is not supported
#endif
#elif defined(__unix__)
// UNIX system, see which one it is
#if defined(__ANDROID__)
// Android
#define AW_OS_ANDROID
#elif defined(__linux__)
// Linux
#define AW_OS_LINUX
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
// FreeBSD
#define AW_OS_FREEBSD
#elif defined(__OpenBSD__)
// OpenBSD
#define AW_OS_OPENBSD
#else
// Unsupported UNIX system
#error This UNIX operating system is not supported by awEngine
#endif
#else
// Unsupported system
#error This operating system is not supported by awEngine
#endif

// Defines to export symbols
#if !defined(AW_STATIC)

#if defined(AW_OS_WINDOWS)

// Windows compilers need specific (and different) keywords for export and import
#define AW_API_EXPORT __declspec(dllexport)
#define AW_API_IMPORT __declspec(dllimport)

// For Visual C++ compilers, we also need to turn off this annoying C4251 warning
#ifdef _MSC_VER

#pragma warning(disable : 4251)

#endif

#else // Linux, FreeBSD, Mac OS X

#if __GNUC__ >= 4

// GCC 4 has special keywords for showing/hidding symbols,
// the same keyword is used for both importing and exporting
#define AW_API_EXPORT __attribute__((__visibility__("default")))
#define AW_API_IMPORT __attribute__((__visibility__("default")))

#else

// GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
#define AW_API_EXPORT
#define AW_API_IMPORT

#endif

#endif

#else

// Static build doesn't need import/export macros
#define AW_API_EXPORT
#define AW_API_IMPORT

#endif
