/*
  Logging routines / macros
  Written by Noel Cower

  See LICENSE.md for license information
*/

#ifndef __SNOW_COMMON__COMMON__LOG_HH__
#define __SNOW_COMMON__COMMON__LOG_HH__

#if defined(__cplusplus)
#include <cstdio>
#else
#include <stdio.h>
#endif

#if S_PLATFORM_TOUCHPAD
#include <PDL.h>
#endif

#define SNOW_LOG_NOTHING          (0)
#define SNOW_LOG_ERRORS           (1)
#define SNOW_LOG_WARNINGS_ERRORS  (2)
#define SNOW_LOG_EVERYTHING       (3)

/*! Force the inclusion of logging macros regardless of release/debug mode. */
#if !defined(FORCE_LOGGING)
#define FORCE_LOGGING 0
#endif

#if !defined(SNOW_LOGGING_LEVEL)
#define SNOW_LOGGING_LEVEL SNOW_LOG_EVERYTHING
#endif

#if defined(__cplusplus)
extern "C"
{
#endif

#if !defined(s_fatal_error)
/*! Writes an error message to stderr then kills the program with the given
 *  error code.  This should be used only when an error absolutely cannot be
 *  recovered from.  It should be obvious, but this function does not return.
 *  Preferably, you should use the ::log_fatal macro to additionally pass file
 *  and line number information along with the error message.
 *
 *  @param[in] format The format for the error message.
 *  @param[in] error The error code to exit with.
 */
#define USE_FATAL_ERROR_IMPL 1
void s_fatal_error_impl(const char *format, ...);

/*! \brief Macro around ::log_fatal_ to pass in additional file, line number,
  and callee information to the format string.  This is never a no-op.
*/
#define s_fatal_error(FORMAT, args...) s_fatal_error_impl("Fatal Error [%s:%s:%d]:\n    " FORMAT "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
#endif

#if S_PLATFORM_TOUCHPAD
#define s_log(STR, args...) PDL_Log((STR), ##args)
#else
#define s_log(STR, args...) fprintf(stderr, (STR), ##args)
#endif

/* if debugging or logging is forcibly enabled, then provide additional logging macros */
#if !defined(NDEBUG) || FORCE_LOGGING
#if !defined(s_log_error) && SNOW_LOGGING_LEVEL >= SNOW_LOG_ERRORS
#define s_log_error(FORMAT, args...)   s_log("Error [%s:%s:%d]\n    "   FORMAT "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
#endif
#if !defined(s_log_warning) && SNOW_LOGGING_LEVEL >= SNOW_LOG_WARNINGS_ERRORS
#define s_log_warning(FORMAT, args...) s_log("Warning [%s:%s:%d]\n    " FORMAT "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
#endif
#if !defined(s_log_note) && SNOW_LOGGING_LEVEL >= SNOW_LOG_EVERYTHING
#define s_log_note(FORMAT, args...)    s_log("Note [%s:%s:%d]\n    "    FORMAT "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
#endif
#endif

/* unless previously defined, log macros do nothing in release builds */
#if !defined(s_log_error)
#define s_log_error(FORMAT, args...)
#endif
#if !defined(s_log_warning)
#define s_log_warning(FORMAT, args...)
#endif
#if !defined(s_log_note)
#define s_log_note(FORMAT, args...)
#endif

/*! \def log_error(FORMAT, args...)
  \brief Macro to write an error message with the given format and parameters
  to stderr.  Includes file, line number, and callee information in the format
  string automatically.

  In builds with NDEBUG defined, this will be a no-op.
*/

/*! \def log_warning(FORMAT, args...)
  \brief Macro to write an warning message with the given format and parameters
  to stderr.  Includes file, line number, and callee information in the format
  string automatically.

  In builds with NDEBUG defined, this will be a no-op.
*/

/*! \def log_note(FORMAT, args...)
  \brief Macro to write a general message with the given format and parameters
  to stderr.  Includes file, line number, and callee information in the format
  string automatically.

  In builds with NDEBUG defined, this will be a no-op.
*/

#if defined(__cplusplus)
}
#endif

#endif /* end of include guard: __SNOW_COMMON__COMMON__LOG_HH__ */
