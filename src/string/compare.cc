// compare.cc -- Noel Cower -- Public Domain

#include <snow/string/compare.hh>
#include <cstdint>
#include <stack>
#include <stdexcept>
#include <utility>

BEGIN_SNOW_NS

size_t score_strings(const string &lhs, const string &rhs)
{
  string::size_type src_length = lhs.length();
  string::size_type dst_length = rhs.length();

  if (src_length == dst_length && lhs == rhs)
    return SIZE_MAX;

  const char *src = lhs.c_str();
  const char *dst = rhs.c_str();

  // Always keep the shorter string on the source side of things.
  if (src_length < dst_length) {
    std::swap(src, dst);
    std::swap(src_length, dst_length);
  }

  string::size_type src_index = 0;
  string::size_type dst_index = 0;
  size_t score = 0;
  size_t score_increment = 1;

  for (; dst_index < dst_length && src_index < src_length; ++dst_index) {
    const char dst_char = dst[dst_index];
    if (dst_char == src[src_index]) {
      score_find:
      score += score_increment;
      score_increment += 2;
      ++src_index;
    } else if (src_index + 1 < src_length) {
      score_increment = 1;
      const auto next_index = lhs.find(dst_char, src_index);
      if (next_index != string::npos) {
        src_index = next_index;
        goto score_find;
      }
    }
  }

  return score;
}

bool pattern_match(const string &pattern, const string &other)
{
  const char *backup = nullptr;
  auto p_cstr = pattern.c_str();
  auto o_cstr = other.c_str();
  const auto p_end = p_cstr + pattern.length();
  const auto o_end = o_cstr + other.length();

  while (p_cstr < p_end && o_cstr < o_end)
    switch (*p_cstr) {
    case '*': {
        // automatic match if * is at the end of the pattern
        if (p_cstr + 1 == p_end)
          return true;

        backup = p_cstr;

        const char next = p_cstr[1];
        // validate pattern - throw an exception for ** and *?
        if (next == '*')
          throw std::runtime_error("Invalid pattern: contains '**'");
        else if (next == '?')
          throw std::runtime_error("Invalid pattern: contains '*?'");

        for (;o_cstr < o_end && *o_cstr != next; ++o_cstr) ;
        // We know the next character matches or something went awry, so skip it
        // for both strings.
        p_cstr += 2;
        o_cstr += 1;
      } break;

    case '?':
      ++p_cstr;
      ++o_cstr;
      break;

    default:
      if (*(p_cstr++) != *(o_cstr++) || (p_cstr == p_end && o_cstr != o_end)) {
        // If we reach the end of p_cstr too quickly or the strings stop
        // matching, try to back up to the last match. Repeat this as many times
        // as needed.

        if (backup) {
          p_cstr = backup;
          backup = nullptr;
        } else {
          return false;
        }
      }
    }

  return p_cstr == p_end && o_cstr == o_end;
}

END_SNOW_NS
