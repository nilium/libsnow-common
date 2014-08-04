/*
 * Copyright Noel Cower 2014.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#pragma once

#ifndef S_DISABLE_TERM_COLORS
  #define S_TERM_RED     "[31;m"
  #define S_TERM_YELLOW  "[33;m"
  #define S_TERM_GREEN   "[32;m"
  #define S_TERM_DEFAULT "[0;m"
#else
  #define S_TERM_RED     ""
  #define S_TERM_YELLOW  ""
  #define S_TERM_GREEN   ""
  #define S_TERM_DEFAULT ""
#endif

#if !defined(NDEBUG) && !defined(S_DISABLE_ASSERTIONS)

  #include <cstdio>
  #include <cstdlib>

  #ifndef S_SHOULD_LOG_ASSERT_PASSES
    #define S_SHOULD_LOG_ASSERT_PASSES (0)
  #else
    // ensure this has an actual value.
    #undef S_SHOULD_LOG_ASSERT_PASSES
    #define S_SHOULD_LOG_ASSERT_PASSES (1)
  #endif

  #define S_LOG_ASSERT_FAIL__(EXPR, TYPE, FORMAT, ARGS...) do {          \
    ::std::fprintf(stderr,                                               \
      S_TERM_YELLOW "--- %s:%d in %s at %s %s: ---\n"                    \
      S_TERM_RED    #TYPE "(" S_TERM_DEFAULT " " #EXPR S_TERM_RED " )\n" \
                       " => " S_TERM_DEFAULT,                            \
      __FILE__, __LINE__, __FUNCTION__, __DATE__, __TIME__               \
      );                                                                 \
    ::std::fprintf(stderr, FORMAT "\n", ##ARGS );                        \
  } while (0)

  #define S_LOG_ASSERT_PASS__(EXPR, TYPE, FORMAT, ARGS...) do {                    \
    ::std::fprintf(stderr,                                                         \
      S_TERM_YELLOW  "--- %s:%d in %s at %s %s: ---\n"                             \
      S_TERM_GREEN   #TYPE "(" S_TERM_DEFAULT " " #EXPR S_TERM_GREEN " )\n"        \
                        " => passed( " S_TERM_DEFAULT,                             \
      __FILE__, __LINE__, __FUNCTION__, __DATE__, __TIME__                         \
      );                                                                           \
    ::std::fprintf(stderr, FORMAT S_TERM_GREEN " )" S_TERM_DEFAULT "\n", ##ARGS ); \
  } while (0)

  #define S_ASSERT_WITHOUT_PASS(EXPR, FORMAT, ARGS...) do { \
    if (!(EXPR)) {                                          \
      S_LOG_ASSERT_FAIL__(EXPR, ASSERT, FORMAT, ##ARGS);    \
      ::std::abort();                                       \
    }                                                       \
  } while(0)

  #define S_ASSERT(EXPR, FORMAT, ARGS...) do {             \
    if (!(EXPR)) {                                         \
      S_LOG_ASSERT_FAIL__(EXPR, ASSERT, FORMAT, ##ARGS);   \
      ::std::abort();                                      \
    } else if (S_SHOULD_LOG_ASSERT_PASSES) {               \
      S_LOG_ASSERT_PASS__(EXPR, ASSERT, FORMAT, ##ARGS);   \
    }                                                      \
  } while(0)

#else

#define S_LOG_ASSERT_FAIL__(ARGS...)
#define S_LOG_ASSERT_PASS__(ARGS...)
#define S_ASSERT_WITHOUT_PASS(EXPR, FORMAT, ARGS...)
#define S_ASSERT(ARGS...)

#endif

#define S_ASSERT_NOT_NULL(EXPR) \
  S_ASSERT((EXPR) != nullptr, #EXPR " must not be null")
