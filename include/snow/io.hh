/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#pragma once

#include <snow/endian.hh>
// For SEEK_SET/CUR/END
#include <cstdio>
#include <type_traits>
#include <algorithm>


namespace snow {


namespace io {


// Basic IO functions -- does not care about anything other than bytes.

// Writes num_bytes from input_buffer to stream.
// Returns < 0 on failure.
template <class STREAM>
int write(STREAM &stream, int num_bytes, void const *input_buffer);

// Reads num_bytes from stream to the output_buffer.
// Returns < 0 on failure.
template <class STREAM>
int read(STREAM &stream, int num_bytes, void *output_buffer);

// Returns the current absolute position of read/write ops in the stream.
// Returns < 0 on failure.
template <class STREAM>
int tell(STREAM const &stream);

// Seeks to the given offset in the stream relative to origin.
// If successful, returns the current absolute position of read/write ops in
// the stream.
// Returns < 0 on failure.
template <class STREAM>
int seek(STREAM &stream, int offset, int origin);

// Returns whether the stream is at its EOF.
// If the stream does not implement eof(), this is always false.
template <class STREAM>
bool eof(STREAM const &stream);



template <class STREAM>
int write(STREAM &stream, int num_bytes, void const *input_buffer)
{
  static_assert(std::is_same<decltype(stream.write(0, nullptr)), int>::value,
    "stream.write must return an int to be compatible with io:: ops");

  if (num_bytes < 0) {
    return -1;
  } else if (num_bytes == 0) {
    return 0;
  }

  return stream.write(num_bytes, input_buffer);
}



template <class STREAM>
int read(STREAM &stream, int num_bytes, void *output_buffer)
{
  static_assert(std::is_same<decltype(stream.read(0, nullptr)), int>::value,
    "stream.read must return an int to be compatible with io:: ops");

  if (num_bytes < 0) {
    return -1;
  } else if (num_bytes == 0) {
    return 0;
  }

  return stream.read(num_bytes, output_buffer);
}



template <class STREAM>
int tell(STREAM const &stream)
{
  return stream.tell();
}



template <class STREAM>
int seek(STREAM &stream, int offset, int origin)
{
  return stream.seek(offset, origin);
}



template <class STREAM>
bool eof(STREAM const &stream)
{
  return stream.eof();
}



/*==============================================================================

  Endianness-friendly object read/write utility functions.

==============================================================================*/

/*
  When specializing write and read for non-trivial types, the endianness
  must be forwarded on to inner calls to read/write.
*/

template <class T, class STREAM>
auto write(STREAM &stream, T const &t_inst, endian_t order = endian_t::network)
  -> typename std::enable_if<std::is_pod<T>::value, int>::type;

// Returns sizeof(T) on success, 0 on failure.
template <class T, class STREAM>
auto read(STREAM &stream, T &t_inst, endian_t order = endian_t::network)
  -> typename std::enable_if<std::is_pod<T>::value, int>::type;


// Write fixed-size strings.
// A nulstring is simply a string of N bytes. The string will always contain a
// terminating null character. Strings shorter than the size written will be
// padded with null character. Strings that are longer than size-1 bytes will
// have their last byte replaced with a null character.
//
// If the length given is -1, write_nulstring will write only the bytes of the
// string up to and including the null character.
template <class STREAM>
int write_nulstring(STREAM &stream, const char *str, int length = -1, int cstrlen = -1);



template <class T, class STREAM>
auto write(STREAM &stream, T const &t_inst, endian_t order)
  -> typename std::enable_if<std::is_pod<T>::value, int>::type
{
  static_assert(std::is_pod<T>::value,
    "write default implementation only accepts POD types.");

  if (sizeof(T) <= 1 || endian_t::network == order) {
    return io::write(stream, int(sizeof(T)), &t_inst);
  } else {
    uint8_t const *ptr = reinterpret_cast<uint8_t const *>(&t_inst);
    for (int index = sizeof(T) - 1; index >= 0; index -= 1) {
      if (write(stream, 1, &ptr[index]) != 1) {
        return int(sizeof(T)) - (index + 1);
      }
    }
    return int(sizeof(T));
  }
}



template <class T, class STREAM>
auto read(STREAM &stream, T &t_inst, endian_t order)
  -> typename std::enable_if<std::is_pod<T>::value, int>::type
{
  static_assert(std::is_pod<T>::value,
    "read default implementation only accepts POD types.");

  if (sizeof(T) <= 1 || endian_t::network == order) {
    return read(stream, int(sizeof(T)), &t_inst);
  } else {
    uint8_t *ptr = reinterpret_cast<uint8_t *>(&t_inst);
    for (int index = sizeof(T) - 1; index >= 0; index -= 1) {
      if (read(stream, 1, &ptr[index]) != 1) {
        return int(sizeof(T)) - (index + 1);
      }
    }
    return int(sizeof(T));
  }
}



template <class STREAM>
int write_nulstring(STREAM &stream, const char *str, int length, int cstrlen)
{
  int_fast64_t zero = 0;

  if (cstrlen < 0) {
    cstrlen = int(strlen(str));
  }

  if (length < 0) {
    length = cstrlen + 1;
  } else if (length == 0) {
    return 0;
  }

  if (cstrlen > length) {
    cstrlen = length - 1;
  }

  int written = write(stream, cstrlen, str);

  // write padding null characters
  while (written < length) {
    int const zeroes_needed = std::min(
      length - written,
      static_cast<int>(sizeof(zero))
      );

    int const zeroes_written = write(stream, zeroes_needed, &zero);

    if (zeroes_written > 0) {
      written += zeroes_needed;
    }

    if (zeroes_written != zeroes_needed) {
      // stream didn't accept the full write (or at all), so return the number
      // of bytes successfully written and end early.
      return written;
    }
  }

  return length;
}


} // namespace io
} // namespace snow
