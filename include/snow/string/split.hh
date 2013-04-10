// split.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__SPLIT_HH__
#define __SNOW_COMMON__SPLIT_HH__

#include <string>


namespace snow {


/*==============================================================================
  split_string

    Splits a string with a delimiter and writes each token to the result
    iterator. Returns the final iterator position. The delimiter cannot be
    escaped within the string.

    Recommended you pass a back_insert_iterator for the result iterator.
==============================================================================*/
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



/*==============================================================================
  split_string_quoted

    Splits a quoted string. This one requires its own buffer, so it's less
    friendly than the above function. However, it allows the strings inside to
    be enclosed within quotes (meaning no splits on the delimiter).

    Quotes within strings can be escaped with a backslash. Backslashes can also
    escape themselves. They escape nothing else. Recommended you use raw string
    literals to initialize strings passed to this.

    Recommended you pass a back_insert_iterator for the result iterator.
==============================================================================*/
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
    throw std::invalid_argument("Unclosed quote");
  } else if (!buffer.empty()) {
    *(result++) = T(last, iter);
  }

  return result;
}


} // namespace snow

#endif /* end __SNOW_COMMON__SPLIT_HH__ include guard */
