#ifndef __SNOW__BUFFER_STREAM_HH__
#define __SNOW__BUFFER_STREAM_HH__

#include <snow/config.hh>
#include <stdexcept>


namespace snow {


/**
  A class to read raw data from a block of memory. Includes methods to read
  null-terminated strings as well.
*/
struct buffer_stream_t
{
  /**
    Constant to indicate the bounds for a stream should be unchecked. Pass as
    the stream's length.
  */
  static const size_t unchecked = ~size_t(0);

  buffer_stream_t() = delete;

  /**
    Constructs a buffer stream starting at the given data pointer that spans
    length bytes.
  */
  buffer_stream_t(void *data, size_t length);

  /**
    Reads an object of type T from the stream. Recommended that specializations
    be written for non-POD/standard layout data.

    The default implementation simply dereferences the current position in the
    buffer as though it points to an object of type T.

    @param result Where to store the result of the read.
  */
  template <typename T>
  size_t read(T &result)
  {
    const T *T_ptr = (const T *)offset_;
    seek(tell() + sizeof(T));
    result = *T_ptr;
    return sizeof(T);
  }

  /**
    Writes an object of type T to the stream. Recommended that specializations
    be written for non-POD/standard layout data (see string_t specialization).

    Default implementation simply does a memmove to copy the object to the
    buffer.

    @param data The object to write to the stream.
    @return The number of bytes written. This may be less than or equal to the
    number of bytes needed to fully write the object.
  */
  template <typename T>
  size_t write(const T& data)
  {
    return write(&data, sizeof(data));
  }

  /**
    Returns whether or not there's more data in the buffer.
    @return False if at the end of the stream, true otherwise.
  */
  inline bool more() const
  {
    return offset_ < end_;
  }

  /**
    Returns the size of the buffer, as passed to the length argument of the
    constructor.
    @return The size of the stream. For unchecked streams, the result is
    undefined.
  */
  inline size_t size() const {
    return static_cast<size_t>(end_ - base_);
  }

  /**
    Gets the remaining number of bytes in the buffer.
    @return The remaining bytes in the buffer.
  */
  inline size_t remainder() const {
    return more() ? static_cast<size_t>(end_ - offset_) : 0;
  }

  /**
    Gets the current position in the buffer relative to its start.

    A result of 0 is the start, where size() is the end if a clear length was
    provided.

    @return
  */
  inline ptrdiff_t tell() const
  {
    return offset_ - base_;
  }

  /**
    Moves the buffer stream's current read/write position to the given offset
    relative to the start of the buffer.

    If the new offset is out of range or at the end of the buffer, read and
    write operations have undefined behavior.

    @param  offset An offset relative to the start of the buffer. Must be
    within 0 and size().
    @return The buffer offset after seeking.
  */
  ptrdiff_t seek(ptrdiff_t offset);

  /**
    Writes length bytes from the provided data to the buffer stream.
    @param   data The data to copy to the stream.
    @param length The number of bytes to copy from the data to the stream.
    @return The number of bytes actually written. May be less than or equal to
    the requested number of bytes.
  */
  size_t write(const void *data, size_t length);

  /**
    Reads length bytes from the stream into the data buffer.
    @param   data The buffer to read into.
    @param length The number of bytes to read from the buffer stream.
    @return The number of bytes actually copied to the data buffer. May be less
    than or equal to the requested number of bytes.
  */
  size_t read(void *data, size_t length);

  /**
    Skips a given number of bytes in the buffer stream.
    @param  length The number of bytes to skip in the buffer.
    @return The number of bytes actually skipped. May be less than or equal to
    length.
  */
  inline size_t skip(size_t length)
  {
    return read(nullptr, length);
  }

  /** Gets the base pointer. */
  inline char *base() { return base_; }
  inline const char *base() const { return base_; }

  /** Gets the current pointer into the buffer. */
  inline char *pointer() { return offset_; }
  inline const char *pointer() const { return offset_; }

  /**
    Gets a pointer to the end of the buffer. Dereferencing this pointer has
    undefined behavior.
  */
  inline char *end() { return end_; }
  inline const char *end() const { return end_; }

private:
  char *base_;      // Base pointer. The start of the buffer.
  char *end_;       // End pointer -- may point to invalid memory in unchecked buffers.
  char *offset_;    // Current offset in the stream. Should be between base_ and end_.
};


/**
  Writes a null-terminated string to the buffer. Returns the number of bytes
  written, including the null character.

  If the return is smaller than string.size() then the string will be truncated
  to write some of the string and the null character. If only the null
  character can be written, nothing will be written and the function will
  return 0.

  @param string The string to write to the stream.
  @return The number of bytes writen to the stream, null character included.
  May be less than the length of the string, though if nonzero, the string
  written contains a null character.
*/
template <>
size_t buffer_stream_t::write(const string_t &string);

/**
  Reads a string from the buffer stream to the result string.

  @param result The string to store the read string in.
  @return The number of bytes read from the buffer, including any null character
  if one was found. If the end of the stream was reached before finding a null
  character, the remainder of the stream is stored in the result string.
*/
template <>
size_t buffer_stream_t::read(string_t &result);


} // namespace snow

#endif /* end __SNOW__BUFFER_STREAM_HH__ include guard */
