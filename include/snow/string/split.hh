/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef __SNOW_COMMON__SPLIT_HH__
#define __SNOW_COMMON__SPLIT_HH__

#include <snow/config.hh>
#include <string>


namespace snow {


/**
  @brief Splits a string by a delimiter and writes each split string out to a
  given `result` iterator.

  Splits a string with a delimiter and writes each token to the result
  iterator. Returns the final iterator position. The delimiter cannot be
  escaped within the string.

  Recommended you pass an std::back_insert_iterator for the result iterator.

  @param  str    The string to split along `delim`.
  @param  delim  The delimiting character (or some value if not a char).
  @param  result The result iterator to write split strings to. Ideally, this
                 is an `std::push_back_iterator`.
  @result The given `result` iterator.
*/
template <typename out_iter, typename T, typename C>
out_iter split_string(const T &str, const C &delim, out_iter result)
{
  using in_iter = typename T::const_iterator;

  in_iter iter  = std::begin(str);
  in_iter last  = iter;
  in_iter end   = std::end(str);
  size_t len    = 0;

  for (; iter != end; ++iter, ++len) {
    if (*iter == delim && len) {
      len = 0;
      *(result++) = T(last, iter);
      last = iter;
      ++last;
    }
  }

  if (len) {
    *(result++) = T(last, iter);
  }

  return result;
}



/**
  @brief Splits a quoted string. Ignores instances of `delim` inside double
  quotes. Each resulting string is written out to the given `result` iterator.

  Splits a quoted string. This one requires its own buffer, so it's less
  friendly than the above function. However, it allows the strings inside to
  be enclosed within quotes (meaning no splits on the delimiter).

  Quotes within strings can be escaped with a backslash. Backslashes can also
  escape themselves. They escape nothing else. Recommended you use raw string
  literals to initialize strings passed to this.

  Recommended you pass an std::back_insert_iterator for the result iterator.

  @param  str    The string to split along delim.
  @param  delim  A character to split on.
  @param  result The iterator to write results to. Ideally, this is an
                 `std::push_back_iterator`.
  @return        The `result` iterator.
*/
template <typename out_iter, typename T, typename C>
out_iter split_string_quoted(const T &str, const C &delim, out_iter result)
{
  using in_iter = typename T::const_iterator;

  constexpr C quote   = C('"');
  constexpr C esc     = C('\\');
  in_iter iter        = std::begin(str);
  in_iter last        = iter;
  in_iter end         = std::end(str);
  bool in_escape      = false;
  bool in_quote       = false;
  T buffer;

  for (; iter != end; ++iter) {
    const auto cur = *iter;
    if (cur == esc && !in_escape) {
      in_escape = true;
    } else if (!in_escape && cur == quote) {
      if (in_quote || !buffer.empty()) {
        *(result++) = std::move(buffer);
      }
      last = iter;
      ++last;
      in_quote = !in_quote;
    } else if (!in_escape && !in_quote && cur == delim) {
      if (!buffer.empty()) {
        *(result++) = std::move(buffer);
      }
      last = iter;
      ++last;
    } else {
      buffer.push_back(cur);
      in_escape = false;
    }
  }

  if (in_quote) {
    s_throw(std::invalid_argument, "Unclosed quote");
  } else if (!buffer.empty()) {
    *(result++) = T(last, iter);
  }

  return result;
}


} // namespace snow

#endif /* end __SNOW_COMMON__SPLIT_HH__ include guard */
