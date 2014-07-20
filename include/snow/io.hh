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

/**
  @brief Writes num_bytes from input_buffer to stream.
  @param  stream       The stream to write to.
  @param  num_bytes    The number of bytes being written from input_buffer to
    the stream. Values < 0 will return an error.
  @param  input_buffer The input buffer to read bytes from. May be null if the
    stream's write member function supports it or this function has been
    specialized to support null input buffers.
  @return The number of bytes written. Returns < 0 on failure and < num_bytes
    for partial writes.

  @note The input_buffer argument is not checked for whether it's null in the
    default implementation of write, as this is left up to the stream's write
    member function to check. Specializations for other stream types may check
    for whether the buffer is null and return a value < 0 accordingly.
*/
template <class Stream>
int write(Stream &stream, int num_bytes, void const *input_buffer);

/**
  @brief Reads num_bytes from stream to the output_buffer.
  @param stream        The stream to read from.
  @param num_bytes     The number of bytes to read from the stream into
    output_buffer. Values < 0 will return an error.
  @param output_buffer The buffer to store read bytes in. May be null if the
    stream's read method supports it or this function has been specialized to
    support null output buffers.
  @return The number of bytes read into output_buffer. Returns < 0 on failure
    and < num_bytes for partial writes.

  @note The output_buffer argument is not checked for whether it's null in the
    default implementation of read, as this is left up to the stream's read
    member function to check. Specializations for other stream types may check
    for whether the buffer is null and return a value < 0 accordingly.
*/
template <class Stream>
int read(Stream &stream, int num_bytes, void *output_buffer);

/**
  @brief Gets the current absolute position of read/write ops in the stream.
  @return The current absolute position or offset of read/write ops in the
    stream. Returns < 0 on failure.
*/
template <class Stream>
int tell(Stream const &stream);

/**
  @brief Seeks to the given offset in the stream relative to origin.

  If successful, returns the current absolute position of read/write ops in
  the stream (i.e., seek(stream, 0, SEEK_CUR) should be equivalent to tell).

  @param stream The stream to seek in.
  @param offset The offset, relative to the origin, to seek to.
  @param origin The origin point of the seek. Must be one of the standard
    SEEK_SET, SEEK_CUR, or SEEK_END.
  @return The new absolute position or offset of read/write ops in the stream.
    Returns < 0 on failure.

  @note Not all streams may support seeking and some may only support seeking
    from a given origin. Ideally, all streams should support at least seeking
    with positive offsets and SEEK_CUR, but this isn't required.
*/
template <class Stream>
int seek(Stream &stream, int offset, int origin);

/**
  @brief Returns whether the stream is at its EOF.

  Returns whether the stream is at its EOF. This isn't necessarily an error
  condition, and depends on the type of stream.

  The default implementation simply calls a stream's eof() member function.
  Streams that do not support an eof check should specialize for this and
  return an appropriate value.

  @param  stream The stream the check for EOF status on.
  @return True if the stream is at EOF (or has the EOF flag set), else false.
*/
template <class Stream>
bool eof(Stream const &stream);



template <class Stream>
int write(Stream &stream, int num_bytes, void const *input_buffer)
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



template <class Stream>
int read(Stream &stream, int num_bytes, void *output_buffer)
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



template <class Stream>
int tell(Stream const &stream)
{
  return stream.tell();
}



template <class Stream>
int seek(Stream &stream, int offset, int origin)
{
  return stream.seek(offset, origin);
}



template <class Stream>
bool eof(Stream const &stream)
{
  return stream.eof();
}



/*==============================================================================

  Endianness-friendly object read/write utility functions.

==============================================================================*/

/*
  When specializing/overloading write and read for non-trivial types, the
  endianness must be forwarded on to inner calls to read/write where
  appropriate.
*/

/**
  @brief Writes an object of type T to the given stream in a given endianness.

  The default implementation reverses the byte order of objects if the
  endianness specified differs from the host endianness (usually little
  endian).

  Can only write POD-type objects to streams by default. This is checked at
  compile-time.

  @param  stream The stream to write the object to.
  @param  t_inst An instance of T to write to the stream.
  @param  order  The byte-order (endianness) to write to the stream in.
    By default, this is network endianness, which in snow-common often also
    defaults to little endian.

  @return The number of bytes written to the stream (i.e., sizeof(T) for
    successful writes). Returns < 0 on failure and < sizeof(T) for partial
    writes.
*/
template <class T, class Stream>
auto write(Stream &stream, T const &t_inst, endian_t order = endian_t::network)
  -> typename std::enable_if<std::is_pod<T>::value, int>::type;

/**
  @brief Reads an object of type T from the given stream in a given endianness.

  The default implementation reverses the byte order of objects if the
  endianness specified differs from the host endianness (usually little
  endian).

  Can only read POD-type objects to streams by default. This is checked at
  compile-time.

  @param  stream The stream to read the object from.
  @param  t_inst A reference to an instance of T to write the resulting
    object to. If the result < sizeof(T), this will not be modified.
  @param  order  The byte-order (endianness) to read from the stream in.
    By default, this is network endianness, which in snow-common often also
    defaults to little endian.

  @return The number of bytes read from the stream (i.e., sizeof(T) on
    success). Returns < 0 on failure and < sizeof(T) for partial reads.
*/
template <class T, class Stream>
auto read(Stream &stream, T &t_inst, endian_t order = endian_t::network)
  -> typename std::enable_if<std::is_pod<T>::value, int>::type;


/**
  @brief Writes a fixed-size string to a stream.

  A nulstring is simply a string of N bytes. The string will always contain a
  terminating null character. Strings shorter than the size written will be
  padded with null character. Strings that are longer than length-1 bytes will
  have their last byte replaced with a null character.

  If the length given is -1, write_nulstring will write only the bytes of the
  string up to and including the null character (that is, length is defaulted
  to cstrlen + 1).

  @param  stream  The stream to write the nulstring to.
  @param  str     The string to write. Must have either a terminating NUL
    character or be only as long as cstrlen if cstrlen >= 0.
  @param  length  The length of the string to write in bytes. If < 0, length is
    the same as cstrlen + 1 (after determining cstrlen if it's also < 0).
  @param  cstrlen The length of str in bytes. If < 0, str's length is measured
    using std::strlen.
  @return The number of bytes written to the stream. Returns < 0 on failure and
    < length for partial writes.
*/
template <class Stream>
int write_nulstring(Stream &stream, const char *str, int length = -1, int cstrlen = -1);



template <class T, class Stream>
auto write(Stream &stream, T const &t_inst, endian_t order)
  -> typename std::enable_if<std::is_pod<T>::value, int>::type
{
  static_assert(std::is_pod<T>::value,
    "write default implementation only accepts POD types.");


  if (sizeof(T) <= 1 || endian_t::host == order) {
    return write(stream, int(sizeof(T)), &t_inst);
  } else {
    uint8_t buffer[sizeof(T)] { 0 };
    uint8_t const *ptr = reinterpret_cast<uint8_t const *>(&t_inst);
    std::reverse_copy(ptr, ptr + sizeof(T), std::begin(buffer));
    return write(stream, static_cast<int>(sizeof(T)), buffer);
  }
}



template <class T, class Stream>
auto read(Stream &stream, T &t_inst, endian_t order)
  -> typename std::enable_if<std::is_pod<T>::value, int>::type
{
  static_assert(std::is_pod<T>::value,
    "read default implementation only accepts POD types.");

  int const num_bytes = static_cast<int>(sizeof(T));
  uint8_t buffer[sizeof(T)];
  int const result = read(stream, num_bytes, &buffer[0]);

  if (result != num_bytes) {
    return num_bytes;
  }

  if (sizeof(T) > 1 && endian_t::host != order) {
    std::reverse(std::begin(buffer), std::end(buffer));
  }

  t_inst = std::move(*reinterpret_cast<T *>(&buffer[0]));

  return result;
}



template <class Stream>
int write_nulstring(Stream &stream, const char *str, int length, int cstrlen)
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
