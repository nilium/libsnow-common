/*
 * Copyright Noel Cower 2014.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#include <snow/config.hh>
#include <snow/string/utf8.hh>


namespace snow
{


namespace utf8
{


int octets_for_code(uint32_t code)
{
  struct {
    uint32_t const begin;
    uint32_t const end;
    int const octets;
  } const unicode_ranges[] = {
    { 0x000000u, 0x00007Fu, 1 },
    { 0x000080u, 0x0007FFu, 2 },
    { 0x000800u, 0x000FFFu, 3 },
    { 0x001000u, 0x00CFFFu, 3 },
    { 0x00D000u, 0x00D7FFu, 3 },
    { 0x00E000u, 0x00FFFFu, 3 },
    { 0x010000u, 0x03FFFFu, 4 },
    { 0x040000u, 0x0FFFFFu, 4 },
    { 0x100000u, 0x10FFFFu, 4 }
  };

  for (auto const &range : unicode_ranges) {
    if (range.begin <= code && code <= range.end) {
      return range.octets;
    }
  }

  return 0;
}


} // namespace utf8


} // namespace snow
