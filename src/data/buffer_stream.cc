/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#include <snow/data/buffer_stream.hh>


namespace snow {


buffer_stream_t::buffer_stream_t(void *data, size_t length) :
  base_(static_cast<char *>(data)),
  end_(length == unchecked ? (char *)INTPTR_MAX : base_ + length),
  offset_(base_)
{
  /* nop */
}



ptrdiff_t buffer_stream_t::seek(ptrdiff_t offset) const
{
  char *next_offset = base_ + offset;
  if (next_offset < base_ || (next_offset > end_)) {
    s_throw(std::out_of_range, "Attempt to seek to out of range offset %td", offset);
    return tell();
  }
  offset_ = next_offset;
  return tell();
}



size_t buffer_stream_t::write(const void *buffer, size_t length)
{
  assert(length > 0);
  assert(buffer != nullptr);
  length = std::min(length, remainder());
  if (length) {
    std::memmove(offset_, buffer, length);
  }
  return length;
}



template <>
size_t buffer_stream_t::write(const string &string)
{
  const size_t rem = remainder();
  if (rem <= 1) {
    return 0;
  }

  size_t length = std::min(static_cast<size_t>(string.size()), rem - 1);
  std::memmove(offset_, string.data(), length);
  offset_[length] = '\0';
  seek(tell() + length + 1);

  return length + 1;
}



size_t buffer_stream_t::read(void *buffer, size_t length) const
{
  assert(length > 0);
  length = std::min(length, remainder());
  if (buffer && length) {
    std::memmove(buffer, offset_, length);
  }
  seek(tell() + length);
  return length;
}



template <>
size_t buffer_stream_t::read(string &result) const
{
  char *lenptr = offset_;
  for (; lenptr < end_ && *lenptr; ++lenptr) ;
  size_t length = static_cast<size_t>(lenptr - offset_);
  result.resize(length);
  std::memmove(result.data(), offset_, length);
  return length + 1;
}


} // namespace snow
