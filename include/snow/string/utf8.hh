/*
 * Copyright Noel Cower 2014.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef __SNOW_COMMON__UTF8_HH__
#define __SNOW_COMMON__UTF8_HH__


#include <cstdint>


namespace snow
{


namespace utf8
{


enum : uint32_t
{
  /* Returned by utf8::peek_code and utf8::next_code if either fails. */
  UTF8_INVALID_CODE            = 0xFFFFFFFFu,

  /* Alternatives you might use in peek/next for invalid codes */
  UTF8_REPLACEMENT_CHAR        = 0xFFFDu,
  UTF8_NONCHARACTER1           = 0xFFFEu,
  UTF8_NONCHARACTER2           = 0xFFFFu,
  UTF8_OBJECT_REPLACEMENT_CHAR = 0xFFFCu,
};


int const UTF8_BITS_INTERMEDIATE = 6;


enum : uint32_t {
  UTF8_MASK_INTERMEDIATE      = 0xC0u,
  UTF8_NAME_INTERMEDIATE      = (UTF8_MASK_INTERMEDIATE << 1) & 0xFF,
  UTF8_VAL_MASK_INTERMEDIATE  = (~UTF8_MASK_INTERMEDIATE) & 0xFF,

  UTF8_MASK_0     = 0x80u,
  UTF8_NAME_0     = (UTF8_MASK_0 << 1) & 0xFF,
  UTF8_VAL_MASK_0 = (~UTF8_MASK_0) & 0xFFu,

  UTF8_MASK_1     = 0xE0u,
  UTF8_NAME_1     = (UTF8_MASK_1 << 1) & 0xFF,
  UTF8_VAL_MASK_1 = (~UTF8_MASK_1) & 0xFFu,

  UTF8_MASK_2     = 0xF0u,
  UTF8_NAME_2     = (UTF8_MASK_2 << 1) & 0xFF,
  UTF8_VAL_MASK_2 = (~UTF8_MASK_2) & 0xFFu,

  UTF8_MASK_3     = 0xF8u,
  UTF8_NAME_3     = (UTF8_MASK_3 << 1) & 0xFF,
  UTF8_VAL_MASK_3 = (~UTF8_MASK_3) & 0xFFu,
};


/*==============================================================================
  utf8::value_mask__

    Returns the value mask for a UTF-8 bitmask.
==============================================================================*/
constexpr uint32_t value_mask__(uint32_t mask)
{
  return (~mask) & 0xFFu;
}


/*==============================================================================
  utf8::mask_name__

    Returns the name for a UTF-8 bitmask.
==============================================================================*/
constexpr uint32_t mask_name__(uint32_t mask)
{
  return (mask << 1) & 0xFFu;
}


/*==============================================================================
  utf8::intermediate_byte__

    Returns the intermediate byte for the given code and fourth of the
    first 24 bits of the code.
==============================================================================*/
constexpr uint32_t intermediate_byte__(uint32_t code, int fourth)
{
  return {
    ((code >> (fourth * UTF8_BITS_INTERMEDIATE)) &
      UTF8_VAL_MASK_INTERMEDIATE) |
    UTF8_NAME_INTERMEDIATE
  };
}


/*==============================================================================
  utf8::octets_for_code

    Returns the number of octets required for the given code, or zero if the
    code can't be represented as a well-formed UTF-8 byte sequence.
==============================================================================*/
int octets_for_code(uint32_t code);


/*==============================================================================
  utf8::next_octet__

    Gets the next octet from the iterator as a uint32_t. Does not check whether
    the iterator is valid. Internal use only.
==============================================================================*/
template <typename IT>
uint32_t next_octet__(IT const &start)
{
  return static_cast<uint32_t>(*start) & 0xFF;
}


/*==============================================================================
  utf8::read_BOM

    Reads the byte-order marker of octets 0xEF 0xBB 0xBF and returns true if
    the BOM was recognized, otherwise false. The iterator is advanced past the
    BOM only if it's present, otherwise it remains unchanged.
==============================================================================*/
template <typename IT>
bool read_BOM(IT &start, IT const &end)
{
  static uint32_t const BOM[3] { 0xEF, 0xBB, 0xBF };

  if (start == end) {
    return false;
  }

  IT iter { start };
  int index = 0;

  for (; index < 3 && iter != end; ++iter, ++index) {
    uint32_t byte = next_octet__(iter);
    if (byte != BOM[index]) {
      return false;
    }
  }

  start = iter;

  return true;
}


/*==============================================================================
  utf8::next_code

    Reads a code given a character iterator -- the characters do not necessarily
    have to be uint8_t though it helps since they're going to get cast to
    unsigned and truncated otherwise. If the code is invalid, the iterator is
    advanced to the next usable byte, then returns UTF8_INVALID_CODE.
==============================================================================*/
template <bool stop_at_invalid = true, typename IT>
uint32_t next_code(IT &iter, IT const &end, uint32_t invalid = UTF8_INVALID_CODE)
{
  static uint32_t const markers[4] = {
    UTF8_MASK_0, UTF8_MASK_1, UTF8_MASK_2, UTF8_MASK_3,
  };

  if (iter == end) {
    return invalid;
  }

  uint32_t code = next_octet__(iter);
  int count = 0;
  bool mask_found = false;

  for (; count < 4; ++count) {
    uint32_t const mask = markers[count];
    uint32_t const name = (mask << 1) & 0xFF;
    uint32_t const vmask = (~mask) & 0xFF;

    if ((code & mask) == name) {
      code &= vmask;
      mask_found = true;
      break;
    }
  }

  if (!mask_found) {
    // Invalid initial octet, skip all intermediate octets until a non-intermediate
    // octet is found.
    if (!stop_at_invalid) {
      do {
        ++iter;
      } while (iter != end && (next_octet__(iter) & UTF8_MASK_INTERMEDIATE) == UTF8_NAME_INTERMEDIATE);
    }

    return invalid;
  }

  ++iter;

  for (; count; ++iter, --count) {
    if (iter == end) {
      return invalid;
    }

    uint32_t const next_code = next_octet__(iter);

    if ((next_code & UTF8_MASK_INTERMEDIATE) != UTF8_NAME_INTERMEDIATE) {
      return invalid;
    }

    code = (code << UTF8_BITS_INTERMEDIATE) | (next_code & UTF8_VAL_MASK_INTERMEDIATE);
  }

  if (octets_for_code(code) == 0) {
    return invalid;
  }

  return code;
}


/*==============================================================================
  utf8::find_invalid

    Finds and returns an iterator pointing to the first invalid code found. If
    no invalid code is found, the end iterator will be returned.
==============================================================================*/
template <typename IT>
IT find_invalid(IT iter, IT const &end)
{
  while (iter != end) {
    if (next_code<true>(iter, end, UTF8_INVALID_CODE) == UTF8_INVALID_CODE) {
      break;
    }
  }
  return iter;
}


/*==============================================================================
  utf8::peek_code

    Peeks the next code from the given iterator and returns it if it's valid,
    otherwise returns UTF8_INVALID_CODE. Does not advance the iterator.
==============================================================================*/
template <typename IT>
uint32_t peek_code(IT const &iter, IT const &end, uint32_t invalid = UTF8_INVALID_CODE)
{
  IT dry { iter };
  return next_code<true, IT>(dry, end, invalid);
}


/*==============================================================================
  utf8::next_is_valid

    Returns true if the result of peek_code is valid, otherwise false.
==============================================================================*/
template <typename IT>
bool next_is_valid(IT const &iter, IT const &end)
{
  return peek_code(iter, end, UTF8_INVALID_CODE) != UTF8_INVALID_CODE;
}


/*==============================================================================
  utf::put_code

    Writes a given code to the iterator as a sequence of UTF-8 octets.
    Returns 0 if the code is not representable as a well-formed UTF-8 octet
    sequence. The output iterator is assumed to take octets of some kind.
==============================================================================*/
template <typename IT>
int put_code(IT &iter, uint32_t code)
{
  static constexpr uint32_t masks[4] {
    UTF8_MASK_0, UTF8_MASK_1, UTF8_MASK_2, UTF8_MASK_3
  };

  int const num_octets = octets_for_code(code);
  if (num_octets == 0) {
    return 0;
  }

  int const intermediate_bytes = num_octets - 1;
  uint32_t const mask = masks[intermediate_bytes];

  *(iter++) = mask_name__(mask) |
    (value_mask__(mask) & (code >> (UTF8_BITS_INTERMEDIATE * intermediate_bytes)));

  for (int rep = 1; rep < num_octets; ++rep, ++iter) {
    *iter = intermediate_byte__(code, intermediate_bytes - rep);
  }

  return num_octets;
}


} // namespace utf8


} // namespace snow

#endif /* end __SCOLEX_UTF8_HH__ include guard */
