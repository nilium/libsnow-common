// log.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__COMMON__LOG_HH__
#define __SNOW_COMMON__COMMON__LOG_HH__

#include <snow/build-config.hh>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>

#if USE_EXCEPTIONS
#include <stdexcept>
#endif


#ifndef S_EXPORT
#define S_EXPORT __attribute__((visibility("default")))
#endif


#define SNOW_LOG_NOTHING          (0)
#define SNOW_LOG_ERRORS           (1)
#define SNOW_LOG_WARNINGS_ERRORS  (2)
#define SNOW_LOG_EVERYTHING       (3)

#ifndef S_LOG_ERROR_PREFIX
#define S_LOG_ERROR_PREFIX "Error "
#endif /* S_LOG_ERROR_PREFIX */

#ifndef S_LOG_WARNING_PREFIX
#define S_LOG_WARNING_PREFIX "Warning "
#endif /* S_LOG_WARNING_PREFIX */

#ifndef S_LOG_NOTE_PREFIX
#define S_LOG_NOTE_PREFIX "Note "
#endif /* S_LOG_NOTE_PREFIX */

/*! Force the inclusion of logging macros regardless of release/debug mode. */
#if !defined(FORCE_LOGGING)
#define FORCE_LOGGING 0
#endif


#if !defined(SNOW_LOGGING_LEVEL)
#define SNOW_LOGGING_LEVEL SNOW_LOG_EVERYTHING
#endif


namespace snow {


using s_log_fn_t = void (*)(const char *msg, size_t length, void *ctx);
// Log callback functions -- not thread safe
void s_log_callback(const char *msg, size_t length);
void s_set_log_callback(s_log_fn_t cb, void *ctx);

void s_log_lock();
void s_log_unlock();
void s_log_impl(const char *format, ...);


#if !defined(s_fatal_error)
/*! Writes an error message to clog then either throws an exception of type EX_T
 *  or calls std::abort(). The former only happens if USE_EXCEPTIONS is defined
 *  as nonzero. The remaining arguments to the function are those passed to
 *  the string formatting function as inputs. Does not call the log callback.
 *
 *  @param[in] format The format for the error message.
 *  @param[in] args   Inputs for the string formatting code.
 */
template <typename EX_T>
void s_fatal_error_impl(const char *format, ...)
{
  va_list arguments_null;
  va_list arguments_real;

  va_start(arguments_null, format);
  va_copy(arguments_real, arguments_null);

  int length = vsnprintf(NULL, 0, format, arguments_null);
  va_end(arguments_null);

  if (length == 0) {
    return;
  }
  std::vector<char> strbuf;
  strbuf.resize(length + 1);
  vsnprintf(strbuf.data(), length + 1, format, arguments_real);
  va_end(arguments_real);

  s_log_lock();
  std::cout << strbuf.data();
  std::cout.flush();

  if (length > 0 && strbuf[length - 1] == '\n') {
    strbuf.pop_back();
    strbuf[--length] = '\0';
  }
  s_log_unlock();

#if USE_EXCEPTIONS
  if (length) {
    throw EX_T(strbuf.data());
  } else {
    throw EX_T("Fatal error");
  }
#else
  abort();
#endif
} /* log_fatal */


} // namespace snow


#define s_throw(EXCEPTION, FORMAT, args...) ::snow::s_fatal_error_impl<EXCEPTION>(#EXCEPTION " [%s:%s:%d]:\n    " FORMAT "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
/*! \brief Macro around ::log_fatal_ to pass in additional file, line number,
  and callee information to the format string.  This is never a no-op.
*/
#define s_fatal_error(FORMAT, args...) ::snow::s_fatal_error_impl<std::runtime_error>("Fatal Error [%s:%s:%d]:\n    " FORMAT "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
#endif


#define s_log(STR, args...) ::snow::s_log_impl((STR), ##args)


/* if debugging or logging is forcibly enabled, then provide additional logging macros */
#if !defined(NDEBUG) || FORCE_LOGGING
#if !defined(s_log_error) && SNOW_LOGGING_LEVEL >= SNOW_LOG_ERRORS
#define s_log_error(FORMAT, args...)   s_log(S_LOG_ERROR_PREFIX "[%s:%s:%d] "   FORMAT "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
#endif
#if !defined(s_log_warning) && SNOW_LOGGING_LEVEL >= SNOW_LOG_WARNINGS_ERRORS
#define s_log_warning(FORMAT, args...) s_log(S_LOG_WARNING_PREFIX  "[%s:%s:%d] " FORMAT "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
#endif
#if !defined(s_log_note) && SNOW_LOGGING_LEVEL >= SNOW_LOG_EVERYTHING
#define s_log_note(FORMAT, args...)    s_log(S_LOG_NOTE_PREFIX "[%s:%s:%d] "    FORMAT "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
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

#endif /* end of include guard: __SNOW_COMMON__COMMON__LOG_HH__ */
