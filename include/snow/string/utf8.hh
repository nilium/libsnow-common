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
#include <iterator>


namespace snow
{


namespace utf8
{


namespace unsafe
{

template <typename T>
class iterator;

} // namespace unsafe


template <typename T>
class iterator;


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
  utf8::value_mask_

    Returns the value mask for a UTF-8 bitmask.
==============================================================================*/
constexpr uint32_t value_mask_(uint32_t mask)
{
  return (~mask) & 0xFFu;
}


/*==============================================================================
  utf8::mask_name_

    Returns the name for a UTF-8 bitmask.
==============================================================================*/
constexpr uint32_t mask_name_(uint32_t mask)
{
  return (mask << 1) & 0xFFu;
}



/*==============================================================================
  utf8::is_intermediate_

    Returns whether the octet (granted, it's passed as a uint32_t) is
    considered an intermediate octet (i.e., not the start of a sequence of
    UTF8 octets).
==============================================================================*/
constexpr bool is_intermediate_(uint32_t code)
{
  return (code & UTF8_MASK_INTERMEDIATE) == UTF8_NAME_INTERMEDIATE;
}



/*==============================================================================
  utf8::intermediate_byte_

    Returns the intermediate byte for the given code and fourth of the
    first 24 bits of the code.
==============================================================================*/
constexpr uint32_t intermediate_byte_(uint32_t code, int fourth)
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
  utf8::next_octet_

    Gets the next octet from the iterator as a uint32_t. Does not check whether
    the iterator is valid. Internal use only.
==============================================================================*/
template <typename IT>
uint32_t next_octet_(IT const &start)
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
    uint32_t byte = next_octet_(iter);
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

  uint32_t code = next_octet_(iter);
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
      } while (iter != end && (next_octet_(iter) & UTF8_MASK_INTERMEDIATE) == UTF8_NAME_INTERMEDIATE);
    }

    return invalid;
  }

  ++iter;

  for (; count; ++iter, --count) {
    if (iter == end) {
      return invalid;
    }

    uint32_t const next_code = next_octet_(iter);

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
  utf8::put_code

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

  *(iter++) = mask_name_(mask) |
    (value_mask_(mask) & (code >> (UTF8_BITS_INTERMEDIATE * intermediate_bytes)));

  for (int rep = 1; rep < num_octets; ++rep, ++iter) {
    *iter = intermediate_byte_(code, intermediate_bytes - rep);
  }

  return num_octets;
}



/*==============================================================================
  utf8::put_code

    Const-friendly put_code. Does not modify iter.
==============================================================================*/
template <typename IT>
int put_code(IT &&iter, uint32_t code)
{
  return put_code(iter, code);
}



/*==============================================================================
  utf8::distance

    Returns the distance in (valid) codepoints between iter and end.
==============================================================================*/
template <typename IT>
int distance(IT iter, IT const &end)
{
  int count = 0;
  for (; iter != end; ++count) {
    if (next_code(iter, end, UTF8_INVALID_CODE) == UTF8_INVALID_CODE) {
      break;
    }
  }
  return count;
}



/*==============================================================================
  utf8::advance

    Advances iter by count codepoints. Advancement will stop if an invalid
    codepoint is encountered. Returns the number of codepoints iter was
    advanced by.

    If stop_at_invalid is false, advance will not stop upon encountering an
    invalid codepoint.
==============================================================================*/
template <bool stop_at_invalid = true, typename IT>
int advance(IT &iter, IT const &end, int count)
{
  int result_count = 0;
  while (result_count < count && iter != end) {
    if (next_code<stop_at_invalid>(iter, end, UTF8_INVALID_CODE) == UTF8_INVALID_CODE && stop_at_invalid) {
      break;
    }
    ++result_count;
  }
  return result_count;
}



/*==============================================================================

  utf8::iterator<T>

  UTF8 code iterator -- dereferencing the iterator yields the code currently
  pointed to be the iterator, or UTF8_INVALID_CODE if the code is invalid.
  Incrementing it advances it to the next codepoint. If the iterator points to
  an invalid codepoint, it will skip all bytes in the underlying iterator until
  it finds a byte that does not match UTF8_MASK/NAME_INTERMEDIATE.

  iterator<T> is a bounded iterator -- it requires both an iterator pointing
  to the current location and an iterator pointing to a location just after the
  valid range of the iterator.

  It is read-only.

==============================================================================*/
template <typename T>
class iterator
{
  T underlying_;
  T end_;

public:

  using underlying_traits = std::iterator_traits<T>;
  using value_type = typename underlying_traits::value_type;
  using difference_type = typename underlying_traits::difference_type;
  using pointer = typename underlying_traits::pointer;
  using reference = typename underlying_traits::reference;
  using iterator_category = std::forward_iterator_tag;

  iterator() = delete;
  iterator(T const iter_, T const end_)
  : underlying_(iter_)
  , end_(end_)
  {
    /* nop */
  }

  // Constructs a sentinel iterator<T> -- both the underlying iterator and its
  // ending iterator are equal.
  explicit iterator(T const end_)
  : underlying_(end_)
  , end_(end_)
  {
    /* nop */
  }

  iterator(iterator const &other)
  : underlying_(other.underlying_)
  , end_(other.end_)
  {
    /* nop */
  }

  iterator(iterator &&) = delete;

  iterator &operator = (iterator const &other)
  {
    underlying_ = other.underlying_;
    end_ = other.end_;
    return *this;
  }

  iterator &operator = (iterator &&) = delete;

  auto operator * () const -> uint32_t
  {
    // Could cache this, not sure if I will.
    return peek_code(underlying_, end_);
  }

  iterator &operator ++ ()
  {
    next_code<false>(underlying_, end_);
    return *this;
  }

  iterator operator ++ (int _)
  {
    iterator copy { underlying_, end_ };
    next_code<false>(underlying_, end_);
    return copy;
  }

  // Convenient at-end test.
  operator bool () const { return underlying_ != end_; }
  operator T () const { return underlying_; }

  T underlying() const { return underlying_; }
  T underlying_end() const { return end_; }

  void set(T const it, T const end)
  {
    underlying_ = it;
    end_ = end;
  }

  void set(T const end)
  {
    underlying_ = end;
    end_ = end;
  }

  // Convenient begin/end methods. Basically allows for-range looping
  // using a utf8::iterator.
  iterator const &begin() const { return *this; }
  iterator end() const { return iterator { end_ }; }

  unsafe::iterator<T> unsafe() const { return unsafe::iterator<T> { underlying_ }; }
  operator unsafe::iterator<T> () const { return unsafe(); }
};



// Comparison operators
template <typename T, typename Q>
bool operator == (iterator<T> const &lhs, iterator<Q> const &rhs)
{
  return lhs.underlying() == rhs.underlying();
}



template <typename T, typename Q>
bool operator != (iterator<T> const &lhs, iterator<Q> const &rhs)
{
  return lhs.underlying() != rhs.underlying();
}



template <typename T>
bool operator == (T const &lhs, iterator<T> const &rhs)
{
  return lhs == rhs.underlying();
}



template <typename T>
bool operator != (T const &lhs, iterator<T> const &rhs)
{
  return lhs != rhs.underlying();
}



template <typename T>
bool operator == (iterator<T> const &lhs, T const &rhs)
{
  return lhs.underlying() == rhs;
}



template <typename T>
bool operator != (iterator<T> const &lhs, T const &rhs)
{
  return lhs.underlying() != rhs;
}



/*==============================================================================

  unsafe -- unsafe variants of utf8:: functions. Typically, this just means the
  functions are not bounds-checked.

==============================================================================*/
namespace unsafe
{


/*==============================================================================
  utf8::unsafe::read_BOM

    Reads the byte-order marker of octets 0xEF 0xBB 0xBF and returns true if
    the BOM was recognized, otherwise false. The iterator is advanced past the
    BOM only if it's present, otherwise it remains unchanged.
==============================================================================*/
template <typename IT>
bool read_BOM(IT &start)
{
  static uint32_t const BOM[3] { 0xEF, 0xBB, 0xBF };

  IT iter { start };
  int index = 0;

  for (; index < 3; ++iter, ++index) {
    uint32_t byte = next_octet_(iter);
    if (byte != BOM[index]) {
      return false;
    }
  }

  start = iter;

  return true;
}



template <bool stop_at_invalid = true, typename IT>
uint32_t next_code(IT &iter, uint32_t invalid = UTF8_INVALID_CODE)
{
  static uint32_t const markers[4] = {
    UTF8_MASK_0, UTF8_MASK_1, UTF8_MASK_2, UTF8_MASK_3,
  };

  IT temp_iter ((iter));
  uint32_t code = next_octet_(temp_iter);
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
    if (!stop_at_invalid) {
      do {
        ++iter;
      } while ((next_octet_(iter) & UTF8_MASK_INTERMEDIATE) == UTF8_NAME_INTERMEDIATE);
    }

    return invalid;
  }

  ++temp_iter;

  for (; count; ++temp_iter, --count) {
    uint32_t const next_code = next_octet_(temp_iter);

    if ((next_code & UTF8_MASK_INTERMEDIATE) != UTF8_NAME_INTERMEDIATE) {
      return invalid;
    }

    code = (code << UTF8_BITS_INTERMEDIATE) | (next_code & UTF8_VAL_MASK_INTERMEDIATE);
  }

  if (octets_for_code(code) == 0) {
    return invalid;
  }

  iter = temp_iter;

  return code;
}



/*==============================================================================
  utf8::unsafe::peek_code

    Peeks the next code from the given iterator and returns it if it's valid,
    otherwise returns UTF8_INVALID_CODE. Does not advance the iterator.
==============================================================================*/
template <typename IT>
uint32_t peek_code(IT const &iter, uint32_t invalid = UTF8_INVALID_CODE)
{
  IT dry { iter };
  return next_code<true, IT>(dry, invalid);
}



/*==============================================================================
  utf8::unsafe::next_is_valid

    Returns true if the result of peek_code is valid, otherwise false.
==============================================================================*/
template <typename IT>
bool next_is_valid(IT const &iter)
{
  return peek_code(iter, UTF8_INVALID_CODE) != UTF8_INVALID_CODE;
}



/*==============================================================================
  utf8::unsafe::advance

    Advances iter by count codepoints. Encountering an invalid codepoint stops
    the advance.

    If stop_at_invalid is false, encountering an invalid codepoint does not
    stop the advance.
==============================================================================*/
template <bool stop_at_invalid = true, typename IT>
int advance(IT &iter, int count)
{
  int result_count = 0;
  while (result_count < count) {
    if (next_code<stop_at_invalid>(iter, UTF8_INVALID_CODE) == UTF8_INVALID_CODE) {
      break;
    }
    ++result_count;
  }
  return result_count;
}



/*==============================================================================

  utf8::unsafe::iterator<T>

  UTF8 code iterator -- dereferencing the iterator yields the code currently
  pointed to be the iterator, or UTF8_INVALID_CODE if the code is invalid.
  Incrementing it advances it to the next codepoint. If the iterator points to
  an invalid codepoint, it will skip all bytes in the underlying iterator until
  it finds a byte that does not match UTF8_MASK/NAME_INTERMEDIATE.

  unsafe::iterator<T> is an unbounded iterator.

  It is read-only.

==============================================================================*/
template <typename T>
class iterator
{
  T underlying_;

public:

  using underlying_traits = std::iterator_traits<T>;
  using value_type = typename underlying_traits::value_type;
  using difference_type = typename underlying_traits::difference_type;
  using pointer = typename underlying_traits::pointer;
  using reference = typename underlying_traits::reference;
  using iterator_category = std::forward_iterator_tag;

  iterator() = delete;
  explicit iterator(T const &iter_)
  : underlying_(iter_)
  {
    /* nop */
  }

  explicit iterator(T &&iter_)
  : underlying_(std::forward<T &&>(iter_))
  {
    /* nop */
  }

  iterator(iterator const &other)
  : underlying_(other.underlying_)
  {
    /* nop */
  }

  iterator(iterator &&) = delete;

  iterator &operator = (iterator const &other)
  {
    underlying_ = other.underlying_;
    return *this;
  }

  iterator &operator = (T const &iter_)
  {
    underlying_ = iter_;
    return *this;
  }

  iterator &operator = (T &&iter_)
  {
    underlying_ = std::forward<T &&>(iter_);
    return *this;
  }

  iterator &operator = (iterator &&) = delete;

  auto operator * () const -> uint32_t
  {
    // Could cache this, not sure if I will.
    return peek_code(underlying_);
  }

  iterator &operator ++ ()
  {
    next_code<false>(underlying_);
    return *this;
  }

  iterator operator ++ (int _)
  {
    iterator copy { underlying_ };
    next_code<false>(underlying_);
    return copy;
  }

  // Convenient at-end test.
  operator T () const { return underlying_; }

  T underlying() const { return underlying_; }

  void set(T const it)
  {
    underlying_ = it;
  }
};



// Comparison operators
template <typename T, typename Q>
bool operator == (iterator<T> const &lhs, iterator<Q> const &rhs)
{
  return lhs.underlying() == rhs.underlying();
}



template <typename T, typename Q>
bool operator != (iterator<T> const &lhs, iterator<Q> const &rhs)
{
  return lhs.underlying() != rhs.underlying();
}



template <typename T>
bool operator == (T const &lhs, iterator<T> const &rhs)
{
  return lhs == rhs.underlying();
}



template <typename T>
bool operator != (T const &lhs, iterator<T> const &rhs)
{
  return lhs != rhs.underlying();
}



template <typename T>
bool operator == (iterator<T> const &lhs, T const &rhs)
{
  return lhs.underlying() == rhs;
}



template <typename T>
bool operator != (iterator<T> const &lhs, T const &rhs)
{
  return lhs.underlying() != rhs;
}


} // namespace unsafe


} // namespace utf8


} // namespace snow

#endif /* end __SNOW_COMMON__UTF8_HH__ include guard */
