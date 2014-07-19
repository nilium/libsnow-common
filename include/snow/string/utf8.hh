/*
 * Copyright Noel Cower 2014.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#pragma once


#include <cstdint>
#include <iterator>


namespace snow
{


/**
  Functions for working with UTF-8 byte sequences.
*/
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
  /**
   @brief Returned by utf8::peek_code and utf8::next_code by default if either
   fails.
  */
  UTF8_INVALID_CODE            = 0xFFFFFFFFu,

  /**
    @brief The replacement character.

    This is a good alternative to UTF8_INVALID_CODE for when you want a stream
    of well-defined codes even if you don't have one for input.
  */
  UTF8_REPLACEMENT_CHAR        = 0xFFFDu,
  /// @brief A non-character code.
  UTF8_NONCHARACTER1           = 0xFFFEu,
  /// @brief A non-character code.
  UTF8_NONCHARACTER2           = 0xFFFFu,
  /// @brief The object replacement character.
  UTF8_OBJECT_REPLACEMENT_CHAR = 0xFFFCu,
};


// The number of bits in an intermediate byte.
int const UTF8_BITS_INTERMEDIATE = 6;


// Internal enums. Masks, names, and value masks for UTF-8 bytes.
// An _NAME enum is the part of the most significant bits in a UTF-8 byte. The
// _MASK enums are for extracting just the _NAME bits of a byte. The _VAL_MASK
// enums are for extracting the remaining bits of a byte.
enum : uint32_t
{
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



/**
  @brief Returns the value mask for a given UTF-8 name mask.
  @param mask One of the UTF8_MASK_* enums.
  @return `mask`'s corresponding UTF8_VAL_MASK_* enum.
*/
constexpr uint32_t value_mask_(uint32_t mask)
{
  return (~mask) & 0xFFu;
}


/**
  @brief Returns the name for a given UTF-8 name mask.
  @param mask One of the UTF8_MASK_* enums.
  @return `mask`'s corresponding UTF8_NAME_* enum.
*/
constexpr uint32_t mask_name_(uint32_t mask)
{
  return (mask << 1) & 0xFFu;
}



/**
  @brief Returns whether the given octet (or the least significant byte of the
  given integer) can be considered an intermediate octet (i.e., the octet is
  not the start of a code).

  @param code An integer to test against -- only the least significant byte is
    tested.
  @return True if the LSB is an intermediate octet, otherwise false.
*/
constexpr bool is_intermediate_(uint32_t code)
{
  return (code & UTF8_MASK_INTERMEDIATE) == UTF8_NAME_INTERMEDIATE;
}



/**
  @brief Returns the intermediate byte for the given code and fourth of the
  first 24 bits of the code.

  @param code   The code to get an intermediate byte for.
  @param fourth Which fourth of the code to slice into an intermediate byte.
  @return An intermediate byte representing a fourth of the code, not counting
  the leading byte.
*/
constexpr uint32_t intermediate_byte_(uint32_t code, int fourth)
{
  return
    ((code >> (fourth * UTF8_BITS_INTERMEDIATE)) &
      UTF8_VAL_MASK_INTERMEDIATE) |
    UTF8_NAME_INTERMEDIATE;
}



/**
  @brief Returns the number of octets required for the given code, or zero if
  the code can't be represented as a well-formed UTF-8 byte sequence.
*/
int octets_for_code(uint32_t code);



/**
  @brief Gets the next octet from the iterator as a uint32_t. Does not check
  whether the iterator is valid. Internal use only.
  @param start The iterator to dereference.
*/
template <typename IT>
uint32_t next_octet_(IT const &start)
{
  return static_cast<uint32_t>(*start) & 0xFF;
}



/**
  @brief Attempts to read the byte-order marker from the given iterator
  sequence.

  Reads the byte-order marker of octets 0xEF 0xBB 0xBF and returns true if
  the BOM was recognized, otherwise false. The iterator is advanced past the
  BOM only if it's present, otherwise it remains unchanged.

  @param start The iterator to start with and advance if the BOM is present.
  @param end The ending iterator.
  @return True if the iterator was advanced and the BOM read, otherwise false.
*/
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



/**
  @brief Reads the next code from a UTF-8 byte sequence iterator and returns it.

  Reads a code given a character iterator -- the characters do not necessarily
  have to be uint8_t though it helps since they're going to get cast to
  unsigned and truncated otherwise. If the code is invalid, the iterator is
  advanced to the next usable byte, then returns UTF8_INVALID_CODE.

  @param iter            The iterator to read from.
  @param end             An iterator pointing to one past the end of the UTF-8
    byte sequence.
  @param invalid         The code to return when an invalid code is encountered.
  @param stop_at_invalid Whether to stop when an invalid code is encountered.
    If true, iteration ceases and `invalid` is returned. If false, next_code
    will read past the invalid code and return `invalid`.

  @return Upon reading a valid code, returns that and the iterator is advanced.
    If an invalid code is encountered and `stop_at_invalid` is true, the
    iterator is not advanced and `invalid` is returned. If `stop_at_invalid` is
    false, the iterator is advanced until next_code encounters the next viable
    starting point for a code and returns `invalid`.

    If at any point next_code reaches the `end` iterator before it can fully
    read a code, it returns `invalid` (the `stop_at_invalid` behavior applies).
*/
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
      } while (iter != end && is_intermediate_(next_octet_(iter)));
    }

    return invalid;
  }

  ++iter;

  for (; count; ++iter, --count) {
    if (iter == end) {
      return invalid;
    }

    uint32_t const next_code = next_octet_(iter);

    if (!is_intermediate_(next_code)) {
      return invalid;
    }

    code = (code << UTF8_BITS_INTERMEDIATE) | (next_code & UTF8_VAL_MASK_INTERMEDIATE);
  }

  if (octets_for_code(code) == 0) {
    return invalid;
  }

  return code;
}



/**
  @brief Returns an iterator pointing to the UTF-8 code before the given
  iterator.

  This returns an iterator pointing to the UTF-8 code before the one at the
  given iterator. This can only be safely used for valid UTF-8 sequences.
  before(IT) is not bounds-checked (i.e., doesn't test for whether it's before
  the start of a sequence), so using it is only valid when there is a valid
  code preceding the iterator.

  Essentially, all this does is walk backwards until it finds the first
  non-intermediate octet (i.e., it finds an octet that looks like it's the
  start of a code).

  @param iter The iterator to start at. Must point to a valid code.
  @return An iterator pointing to the first valid code before this iterator.
*/
template <typename IT>
IT before(IT iter)
{
  do {
    --iter;
  } while (is_intermediate_(next_octet_(iter)));
  return iter;
}



/**
  @brief Returns an iterator pointing to the UTF-8 code `count` codes before the
  given iterator.

  @param iter  The iterator to start at. Must point to a valid code.
  @param count The number of codes to rewind the iterator by before returning
    it. There must be at least `count` codes before iter, otherwise the result
    of this function is undefined.
  @return An iterator that points to the `count`-th code before the starting
    iterator.

  @see snow::utf8::before(iter)
*/
template <typename IT>
IT before(IT iter, int count)
{
  for (; count > 0; --count) {
    iter = before(iter);
  }
  return iter;
}



/**
  @brief Opposite of utf8::advance -- walks backwards in a sequence.

  Walks backward in a UTF-8 sequence. The iterator must have `count` valid
  UTF-8 codes before it, otherwise the result of this function is undefined
  (especially since it may attempt to access unallocated or unassociated
  memory if the iterator is a pointer and unchecked).

  @param iter  The iterator to rewind or backtrack from.
  @param count The number of times to walk backwards from the iterator.
*/
template <typename IT>
void backtrack(IT &iter, int count)
{
  iter = before(iter, count);
}



/**
  @brief Finds and returns an iterator pointing to the first invalid code or
  the end of the sequence.

  @param iter The iterator to start at.
  @param end  The end of the iterator sequence.
  @return If an invalid code is found, an iterator before `end` is returned,
    otherwise `end` is returned to indicate that the entire sequence is valid.
*/
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



/**
  @brief Attempts to peek the next code from the given iterator and return it.

  Peeks the next code from the given iterator and returns it if it's valid,
  otherwise returns UTF8_INVALID_CODE. Does not advance the iterator.

  @param iter    The iterator to try to peek from.
  @param end     The ending iterator.
  @param invalid The value to return if an invalid code was read or `iter`
    is at `end`.
  @return The next code that will be read from the iterator if using
    `utf8::next_code(iter, end, invalid)`.
*/
template <typename IT>
uint32_t peek_code(IT const &iter, IT const &end, uint32_t invalid = UTF8_INVALID_CODE)
{
  IT dry { iter };
  return next_code<true, IT>(dry, end, invalid);
}



/**
  @brief Returns true if the result of peek_code is valid, otherwise false.

  Convenience function for testing if the next code in a sequence is valid by
  using peek_code.
*/
template <typename IT>
bool next_is_valid(IT const &iter, IT const &end)
{
  return peek_code(iter, end, UTF8_INVALID_CODE) != UTF8_INVALID_CODE;
}



/**
  @brief Writes a code to the iterator as a sequence of UTF-8 octets.

  Writes a given code to the iterator as a sequence of UTF-8 octets.
  Returns 0 if the code is not representable as a well-formed UTF-8 octet
  sequence. The output iterator is assumed to take octets of some kind.

  @param iter The output iterator to write to. Is modified by put_code.
  @return The number of bytes written to `iter`, otherwise 0 if the code cannot
    be written as a UTF-8 sequence.
*/
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



/**
  @brief Const-friendly form of put_code. Does not modify iter.
*/
template <typename IT>
int put_code(IT const &iter, uint32_t code)
{
  IT temp { iter };
  return put_code(temp, code);
}



/**
  @brief Returns the distance in (valid) codepoints between `start` and `end`.

  Reads as many codes as possible between `start` and `end` and returns the
  number of codes successfully read. If an invalid code is encountered, only
  the number of codes up to that point is returned (i.e., the result is the
  same as if calling `distance(start, find_invalid(start, end))`).

  @param start The iterator to start at.
  @param end   The end of the sequence.
  @return The number of valid codes between `start` and `end`.
*/
template <typename IT>
int distance(IT start, IT const &end)
{
  int count = 0;
  for (; start != end; ++count) {
    if (next_code(start, end, UTF8_INVALID_CODE) == UTF8_INVALID_CODE) {
      break;
    }
  }
  return count;
}



/**
  @brief Advances iter by `count` codepoints.

  Advances iter by `count` codepoints. Advancement will stop if an invalid
  codepoint is encountered. Returns the number of codepoints iter was
  advanced by.

  If stop_at_invalid is false, advance will not stop upon encountering an
  invalid codepoint.

  @param start The iterator to start at.
  @param end   The end of the sequence.
  @param count The number of codes to advance by. A `count` <= 0 does nothing.
  @return The number of codes, valid or invalid, actually advanced by.
*/
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



/**
  @brief Read-only UTF-8 code iterator.

  UTF-8 code iterator -- dereferencing the iterator yields the code currently
  pointed to be the iterator, or UTF8_INVALID_CODE if the code is invalid.
  Incrementing it advances it to the next codepoint. If the iterator points to
  an invalid codepoint, it will skip all bytes in the underlying iterator until
  it finds a byte that does not match UTF8_MASK/NAME_INTERMEDIATE.

  iterator<T> is a bounded iterator -- it requires both an iterator pointing
  to the current location and an iterator pointing to a location just after the
  valid range of the iterator.

  It is read-only.

*/
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



/**
  @brief unsafe variants of snow::utf8 functions and iterators. Typically, this
  just means these functions/types are not bounds-checked.
*/
namespace unsafe
{


/**
  @brief Attempts to read the byte-order marker from the given iterator
  sequence.

  Reads the byte-order marker of octets 0xEF 0xBB 0xBF and returns true if
  the BOM was recognized, otherwise false. The iterator is advanced past the
  BOM only if it's present, otherwise it remains unchanged.

  This function does not ensure it won't read past the end of a sequence.

  @param start The iterator to start with and advance if the BOM is present.
  @return True if the iterator was advanced and the BOM read, otherwise false.
*/
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



/**
  @brief Reads the next code from a UTF-8 byte sequence iterator and returns it.

  @param  iter            The iterator to read from.
  @param  invalid         The code to return when an invalid code is
                          encountered.
  @param  stop_at_invalid Whether to stop when an invalid code is encountered.
                          If true, iteration ceases and `invalid` is returned.
                          If false, next_code will read past the invalid code
                          and return `invalid`.
  @return Upon reading a valid code, returns that and the iterator is advanced.
          If an invalid code is encountered and `stop_at_invalid` is true, the
          iterator is not advanced and `invalid` is returned.
          If `stop_at_invalid` is false, the iterator is advanced until
          next_code encounters the next viable starting point for a code and
          returns `invalid`.
*/
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
      } while (is_intermediate_(next_octet_(iter)));
    }

    return invalid;
  }

  ++temp_iter;

  for (; count; ++temp_iter, --count) {
    uint32_t const next_code = next_octet_(temp_iter);

    if (!is_intermediate_(next_code)) {
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



/**
  @brief Attempts to peek the next code from the given iterator and return it.

  Peeks the next code from the given iterator and returns it if it's valid,
  otherwise returns UTF8_INVALID_CODE. Does not advance the iterator.

  This form doesn't ensure it won't read past the end of the sequence.

  @param iter    The iterator to try to peek from.
  @param invalid The value to return if an invalid code was read or `iter`
    is at `end`.
  @return The next code that will be read from the iterator if using
    `utf8::next_code(iter, end, invalid)`.
*/
template <typename IT>
uint32_t peek_code(IT const &iter, uint32_t invalid = UTF8_INVALID_CODE)
{
  IT dry { iter };
  return next_code<true, IT>(dry, invalid);
}



/**
  @brief Returns true if the result of peek_code is valid, otherwise false.

  Convenience function for testing if the next code in a sequence is valid by
  using peek_code.

  This form doesn't ensure it won't read past the end of the sequence.
*/
template <typename IT>
bool next_is_valid(IT const &iter)
{
  return peek_code(iter, UTF8_INVALID_CODE) != UTF8_INVALID_CODE;
}



/**
  @brief Advances iter by `count` codepoints.

  Advances iter by `count` codepoints. Advancement will stop if an invalid
  codepoint is encountered. Returns the number of codepoints iter was
  advanced by.

  If stop_at_invalid is false, advance will not stop upon encountering an
  invalid codepoint.

  This form doesn't ensure it won't read past the end of the sequence.

  @param start The iterator to start at.
  @param count The number of codes to advance by. A `count` <= 0 does nothing.
  @return The number of codes, valid or invalid, actually advanced by.
*/
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



/**
  @brief Read-only UTF-8 code iterator. Uses utf8::unsafe functions. Unlike
  the utf8::iterator, this does not store the end of the sequence, so must be
  explicitly tested whether it's at the end of a sequence.

  UTF-8 code iterator -- dereferencing the iterator yields the code currently
  pointed to be the iterator, or UTF8_INVALID_CODE if the code is invalid.
  Incrementing it advances it to the next codepoint. If the iterator points to
  an invalid codepoint, it will skip all bytes in the underlying iterator until
  it finds a byte that does not match UTF8_MASK/NAME_INTERMEDIATE.

  unsafe::iterator<T> is an unbounded iterator.

  It is read-only.
*/
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
