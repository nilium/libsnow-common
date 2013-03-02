// config.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__CONFIG_HH__
#define __SNOW_COMMON__CONFIG_HH__

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

/* Define YES/NO constants to map 1 and 0 respectively.
   These are an Apple-ism, and I like that they can contribute to code
   readability in some odd ways.  On the off chance that Cocoa.h is included
   before config.h in a Mac OS build, we can just assume YES/NO are 1 and 0 or
   at least mapped similarly.
*/
#if !defined(YES)
# define YES (1)
#endif

#if !defined(NO)
# define NO (0)
#endif

/* set up some macros for platforms */
#define S_PLATFORM_UNIX     (defined(unix) || defined(__unix) || defined(__unix__))
#define S_PLATFORM_APPLE    (defined(__APPLE__))
#define S_PLATFORM_WINDOWS  (defined(_WIN32) || defined(__MINGW32__))
#define S_PLATFORM_LINUX    (defined(__linux__) || defined(linux) || defined(__linux))
#define S_PLATFORM_IOS      (S_PLATFORM_APPLE && TARGET_OS_IPHONE)
#define S_PLATFORM_MAC      (S_PLATFORM_APPLE && TARGET_OS_MAC)
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

#if S_PLATFORM_TOUCHPAD
# include <PDL.h>
#endif

// Define string type
#include <string>

namespace snow {

typedef std::string string;

} // namespace snow

#endif /* end __SNOW_COMMON__CONFIG_HH__ include guard */
