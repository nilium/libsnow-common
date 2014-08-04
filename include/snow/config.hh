/*
 * Copyright Noel Cower 2013 - 2014.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#pragma once

#include "build-config.hh"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundef"

#if defined(__APPLE__)
# include <TargetConditionals.h>
#endif

#ifndef S_BREAKPOINT
# if NDEBUG
#   if defined(S_BREAKPOINT) && defined(S_NO_BREAKPOINTS)
      // should result in a compiler error if S_NO_BREAKPOINTS is enabled
#     define S_BREAKPOINT "BREAKPOINT IN NON-DEBUG CODE"
#   else
      // otherwise do nothing
#     define S_BREAKPOINT
#   endif // !defined(S_BREAKPOINT)
# else
#   define S_BREAKPOINT
# endif
#endif

/* define NULL ifndef */
#if !defined(NULL)
# if defined(__cplusplus)
#   define NULL nullptr
# else /* defined(__cplusplus) */
#   define NULL ((void *)0)
# endif /* !__cplusplus */
#endif /* !defined(NULL) */

#ifndef __has_feature
# define __has_feature(X) (1)
#endif

#ifndef __has_extension
# define __has_extension(X) (0)
#endif

/* Define YES/NO constants to map 1 and 0 respectively.
   These are an Apple-ism, and I like that they can contribute to code
   readability in some odd ways.  On the off chance that Cocoa.h is included
   before config.h in a Mac OS build, I just assume YES/NO are 1 and 0 or at
   least mapped similarly.
*/
#ifndef __cplusplus
#if !defined(YES)
# define YES (1)
#endif

#if !defined(NO)
# define NO (0)
#endif
#else
#if !defined(YES)
# define YES (true)
#endif

#if !defined(NO)
# define NO (false)
#endif
#endif

/* set up some macros for platforms */

#define S_PLATFORM_UNIX     (defined(unix) || defined(__unix) || defined(__unix__))
#define S_PLATFORM_APPLE    (defined(__APPLE__))
#define S_PLATFORM_WINDOWS  (defined(_WIN32) || defined(__MINGW32__))
#define S_PLATFORM_LINUX    (defined(__linux__) || defined(linux) || defined(__linux))
#define S_PLATFORM_MAC      (S_PLATFORM_APPLE && TARGET_OS_MAC && !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR))
#define S_PLATFORM_IOS      (S_PLATFORM_APPLE && TARGET_OS_IPHONE)
#define S_PLATFORM_IOS_SIM  (S_PLATFORM_APPLE && TARGET_IPHONE_SIMULATOR)
#define S_PLATFORM_QNX      (defined(__QNX__))
#define S_PLATFORM_ANDROID  (defined(__ANDROID__))

/* architectures */
#define S_ARCH_ARM        (__arm__)
#define S_ARCH_ARM_NEON   (S_ARCH_ARM && __ARM_NEON__)
#define S_ARCH_ARM_7      (S_ARCH_ARM && __ARM_ARCH_7A__)
#define S_ARCH_x86_64     (__x86_64__ || __x86_64 || __amd64__ || __amd64 || _M_X64)
#define S_ARCH_x86        (__i386 || __i386__ || i386 || _M_IX86 || _X86_ || __i486__ || __i586 || __i686__)
#define S_ARCH_PPC        (__powerpc || __powerpc__ || __POWERPC__ || __ppc__ || _M_PPC)

#if (S_PLATFORM_UNIX || S_PLATFORM_APPLE) && !defined(__USE_UNIX98)
# define __USE_UNIX98 1
#endif

/* specify the use of pthreads on supported platforms */
#if S_PLATFORM_APPLE || S_PLATFORM_UNIX || S_PLATFORM_LINUX || S_PLATFORM_QNX || S_PLATFORM_ANDROID
# define S_USE_PTHREADS (1)
#else
# define S_USE_PTHREADS (0)
#endif

#if S_USE_PTHREADS
# include <pthread.h>
#else
# error "No threading API available"
#endif /* USE_PTHREADS */

#include "snow/logging/log.hh"

#ifndef S_EXPORT
# if S_PLATFORM_WINDOWS && !defined(__GNUC__)
#   if defined(S_SHARED_LIBRARY) && defined(S_BUILD_SHARED_LIB)
#     define S_EXPORT __declspec(dllexport)
#   elif defined(S_SHARED_LIBRARY)
#     define S_EXPORT __declspec(dllimport)
#   else
#     define S_EXPORT
#   endif
# elif S_PLATFORM_WINDOWS
#   if defined(S_SHARED_LIBRARY) && defined(S_BUILD_SHARED_LIB)
#     define S_EXPORT __attribute__((dllexport))
#   elif defined(S_SHARED_LIBRARY)
#     define S_EXPORT __attribute__((dllimport))
#   else
#     define S_EXPORT
#   endif
# else
#   define S_EXPORT __attribute__((visibility("default")))
# endif
#endif

#ifndef S_HIDDEN
# if S_PLATFORM_WINDOWS && !defined(__GNUC__)
#   define S_HIDDEN
# else
#   define S_HIDDEN __attribute__((visibility("hidden")))
# endif
#endif

#ifndef S_DEPRECATED
# if __cplusplus > 201103L
#   define S_DEPRECATED(REASON) [[deprecated( REASON )]]
# elif __has_extension(attribute_deprecated_with_message)
#   define S_DEPRECATED(REASON) __attribute__((deprecated( REASON )))
# elif S_PLATFORM_WINDOWS && !defined(__GNUC__)
#   define S_DEPRECATED(REASON) __declspec(deprecated( REASON ))
# elif defined(__GNUC__)
#   define S_DEPRECATED(REASON) __attribute__((deprecated))
# else
#   pragma message "No deprecated attribute available, take caution"
#   define S_DEPRECATED(REASON)
# endif
#endif

#include <stdexcept>
#include <utility>
#include <type_traits>
#include <cstddef>
#include <cstdint>
#include <cfloat>
#include <cassert>

#pragma GCC diagnostic pop

// Define string type
#include <snow/string/string.hh>
