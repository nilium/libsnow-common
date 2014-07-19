/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#pragma once

#include <snow/config.hh>


namespace snow {

/**
  @brief Returns a scored comparison of two strings.

  Does a scored comparison of two strings. Perfect matches return SIZE_MAX.
  Compares the left-hand string to the right-hand string. Their positions are
  swapped if the left (source) string is longer than the right string (other).

  Note that this should only really be used for short-ish strings. Though it
  will likely never have an overflow on the score, it is possible with a long
  enough string, but it also won't attempt to do complex scoring. This is very
  far from fuzzy string matching.

  This function should only be used for doing one-to-many string comparisons
  when not testing for equality.

  @param  source The string being compared to.
  @param  other  The string being compared to source.
  @return        A size_t that grows depending on `other`'s similarity to
                `source`. Perfectly equal strings yield `SIZE_MAX`.
*/
S_EXPORT size_t score_strings(const string &source, const string &other);


/**
  @brief Determines whether a string matches a glob pattern.

  Tries to match a string against a pattern using * and ? for wildcards, like
  the shell. Unlike the shell, * can consume as much as it needs in order to
  successfully match the string against the pattern. So, trying to match
  "foo*bar" against "foo/bar/baz/quux/bar" will result in a match, as it will
  fall back to the last * wilcard when it fails to match the string and consume
  the first instance of "bar".

  Keep in mind that a ? can never follow a * and a * following a * is an
  automatic failure (these patterns are invalid).

  Both * and ? must consume at least one character. They are essentially
  equivalent to .+? and . in a regex.

  @param  pattern A glob pattern string.
  @param  other   The string to test the pattern against.
  @return         True if `other` matches `pattern`, otherwise false.
*/
S_EXPORT bool pattern_match(const string &pattern, const string &other);

} // namespace snow
