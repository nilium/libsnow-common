/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#include <snow/string/string.hh>

#include <cassert>
#include <cstring>
#include <stdexcept>


namespace snow {


static int int_from_pointer_diff(char const *lhs, char const *rhs)
{
  return static_cast<int>(lhs - rhs);
}



static void check_range(int index, string_t::size_type length)
{
  if (index < 0 || index >= length) {
    s_throw(
      std::out_of_range,
      "Attempt to access out of range element of string (index = %d; length = %d)",
      index,
      length
      );
  }
}



// End is inclusive
static void check_range_iter_inclusive(char const *const ptr, char const *begin, char const *end)
{
  if (ptr < begin) {
    s_throw(
      std::out_of_range,
      "Pointer (%p) occurs before string data (%p - %p)",
      ptr,
      begin,
      end
      );
  } else if (ptr > end) {
    s_throw(
      std::out_of_range,
      "Pointer (%p) occurs on or after end of string data (%p - %p)",
      ptr,
      end,
      begin
      );
  }
}



string_t::string_t() :
  rep_({{0x0, 0x0}})
{
  /* nop */
}



string_t::string_t(const_iterator const from, const_iterator const to) :
  string_t(from, to <= from ? 0 : size_type(to - from))
{
  /* nop */
}



string_t::string_t(const char *zstr) :
  string_t(zstr, std::strlen(zstr))
{
  /* nop */
}



string_t::string_t(const char *zstr, size_type length) :
  string_t()
{
  assert(zstr);
  if (length > 0) {
    assign(zstr, length);
  }
}



string_t::string_t(string_t &&other) :
  data_(other.data_),
  rep_(other.rep_)
{
  if (other.is_short()) {
    data_ = rep_.short_.short_data_;
  }

  other.rep_.long_.length_ = 0;
  other.rep_.long_.capacity_ = 0;
  other.data_ = other.rep_.short_.short_data_;
}



string_t::string_t(const string_t &other) :
  string_t()
{
  const size_type other_len = other.size();
  resize(other_len);
  std::memcpy(data_, other.data_, other_len);
}







string_t::string_t(const std::string &other) :
string_t()
{
  const size_type other_len = other.size();
  resize(other_len);
  std::memcpy(data_, other.data(), other_len);
}



string_t::string_t(std::initializer_list<char> init) :
  string_t()
{
  const size_type len = init.size();
  resize(len);
  auto iter = init.begin();
  auto init_end = init.end();
  size_type index = 0;
  for (; iter != init_end && index < len; ++iter, ++index) {
    data_[index] = *iter;
  }
}



string_t::string_t(double value) :
  string_t()
{
  int formatted_length = snprintf(nullptr, 0, "%f", value);
  if (formatted_length == 0) {
    return;
  }
  resize(formatted_length);
  snprintf(data(), formatted_length + 1, "%f", value);
}



string_t::string_t(int value) :
  string_t()
{
  int formatted_length = snprintf(nullptr, 0, "%d", value);
  if (formatted_length == 0) {
    return;
  }
  resize(formatted_length);
  snprintf(data(), formatted_length + 1, "%d", value);
}



string_t::string_t(unsigned value) :
  string_t()
{
  int formatted_length = snprintf(nullptr, 0, "%u", value);
  if (formatted_length == 0) {
    return;
  }
  resize(formatted_length);
  snprintf(data(), formatted_length + 1, "%u", value);
}



string_t::string_t(char value) :
  string_t()
{
  resize(1);
  data()[0] = value;
}



string_t::~string_t()
{
  if (can_free()) {
    free(data_);
  }
}



string_t string_t::format(const char *format_string, ...)
{
  string_t result;
  va_list arguments;
  va_start(arguments, format_string);
  int formatted_length = vsnprintf(nullptr, 0, format_string, arguments);
  va_end(arguments);
  if (formatted_length == 0) {
    return result;
  }
  result.resize(formatted_length);
  va_start(arguments, format_string);
  vsnprintf(result.data(), formatted_length + 1, format_string, arguments);
  va_end(arguments);
  return result;
}



string_t &string_t::operator = (string_t &&other)
{
  if (&other == this) {
    return *this;
  }

  if (can_free()) {
    delete [] data_;
  }

  data_ = other.data_;
  rep_ = other.rep_;

  if (other.is_short()) {
    data_ = rep_.short_.short_data_;
  }

  other.data_ = other.rep_.short_.short_data_;
  other.rep_.long_.length_ = 0;
  other.rep_.long_.capacity_ = 0;

  return *this;
}



string_t &string_t::operator = (const std::string &other)
{
  const size_type len = other.size();
  resize(len);
  if (len) {
    std::memcpy(data_, other.data(), len);
  }
  return *this;
}



string_t &string_t::operator = (const char *zstr)
{
  assert(zstr);
  assert(zstr < data_ || zstr > data_ + size());

  const size_type len = std::strlen(zstr);
  resize(len);
  if (len) {
    std::memcpy(data_, zstr, len);
  }
  return *this;
}



string_t &string_t::operator = (const string_t &other)
{
  if (this != &other) {
    const size_type len = other.size();
    resize(len);
    if (len) {
      std::memcpy(data_, other.data_, len);
    }
  }
  return *this;
}



string_t &string_t::assign(const char *zstr, size_type length)
{
  assert(zstr < data_ || zstr > data_ + size());
  assert(zstr);

  resize(length);
  if (length) {
    std::memcpy(data_, zstr, length);
  }
  return *this;
}



int string_t::compare(const string_t &other) const
{
  if (this == &other) {
    return 0;
  }

  const size_type len = size();
  const size_type other_len = other.size();
  if (other_len == len) {
    return len ? std::memcmp(data_, other.data_, len) : 0;
  } else {
    return len < other_len ? -1 : 1;
  }
}



string_t &string_t::append(char ch)
{
  const size_type len = size();
  reserve_for_growth(len + 1);
  resize(len + 1);
  data_[len] = ch;
  return *this;
}



string_t &string_t::append(const char *zstr)
{
  assert(zstr);
  return append(zstr, std::strlen(zstr));
}



string_t &string_t::append(const char *zstr, size_type length)
{
  assert(zstr);
  assert(zstr < data_ || zstr > data_ + size());

  const size_type old_len = size();
  if (length) {
    reserve_for_growth(old_len + length);
    resize(old_len + length);
    std::memcpy(data_ + old_len, zstr, length);
  }
  return *this;
}



string_t &string_t::append(const string_t &str)
{
  const size_type old_len = size();
  const size_type other_len = str.size();
  if (other_len) {
    reserve_for_growth(old_len + other_len);
    resize(old_len + other_len);
    std::memcpy(data_ + old_len, str.data(), other_len);
  }
  return *this;
}



string_t &string_t::append(const_iterator const from, const_iterator const to)
{
  if (from == to || from < to) {
    return *this;
  }

  return append(from, size_type(to - from));
}



string_t &string_t::insert(const_iterator pos, char ch)
{
  return insert(index_of(pos), ch);
}



string_t &string_t::insert(const_iterator pos, const char *zstr)
{
  assert(zstr);
  assert(zstr < data_ || zstr > data_ + size());
  return insert(index_of(pos), zstr, std::strlen(zstr));
}



string_t &string_t::insert(const_iterator pos, const char *zstr, size_type length)
{
  assert(zstr);
  assert(zstr < data_ || zstr > data_ + size());
  return insert(index_of(pos), zstr, length);
}



string_t &string_t::insert(const_iterator pos, const string_t &str)
{
  assert(this != &str);
  return insert(index_of(pos), str.data(), str.size());
}



string_t &string_t::insert(size_type pos, char ch)
{
  const size_type len = size();

  assert(pos >= 0);
  assert(pos <= len);

  if (pos == len) {
    return append(ch);
  } else {
    const size_type new_length = len + 1;
    reserve_for_growth(new_length);
    resize(new_length);
    std::memmove(data_ + pos, data_ + pos + 1, (new_length) - pos);
    data_[pos] = ch;
  }
  return *this;
}



string_t &string_t::insert(size_type pos, const char *zstr)
{
  assert(zstr);
  assert(zstr < data_ || zstr > data_ + size());
  return insert(pos, zstr, std::strlen(zstr));
}



string_t &string_t::insert(size_type pos, const char *zstr, size_type length)
{
  assert(zstr);
  assert(zstr < data_ || zstr > data_ + size());

  switch (length) {
  case 1: return insert(pos, *zstr);
  default: {
      const size_type this_length = size();

      assert(pos >= 0);
      assert(pos <= this_length);

      if (pos == this_length) {
        return append(zstr, length);
      } else {
        const size_type new_length = this_length + length;
        reserve_for_growth(new_length);
        resize(new_length);
        std::memmove(data_ + pos, data_ + pos + length, (new_length) - pos);
        std::memcpy(data_ + pos, zstr, length);
      }
    }
  case 0: return *this;
  }
}



string_t &string_t::insert(size_type pos, const string_t &str)
{
  assert(this != &str);
  return insert(pos, str.data_, str.size());
}



string_t &string_t::push_back(char ch)
{
  return append(ch);
}



string_t &string_t::pop_back()
{
  const size_type len = size();
  assert(len > 0);
  if (len) {
    resize(len - 1);
  }
  return *this;
}



string_t &string_t::erase(size_type from, size_type count)
{
  const size_type len = size();
  size_type to;
  if (from < 0) {
    from = 0;
  } else if (count < 0) {
    s_throw(
      std::out_of_range,
      "count (%d) must be >= 0",
      count
      );
  }

  switch (count) {
  case 0: return *this;
  case npos: to = len - from; break;
  default: to = from + count; break;
  }

  assert(from <= len);
  assert(from + count <= len);

  if (from == len) {
    return *this;
  } else if (from == 0 && to == len) {
    resize(0);
    return *this;
  } else if (to == len) {
    resize(from);
    return *this;
  } else if (from == 0) {
    const size_type new_len = len - to;
    std::memmove(data_, data_ + to, new_len);
    resize(new_len);
    return *this;
  }

  const size_type remainder = len - to;
  std::memmove(data_ + from, data_ + to, remainder);
  return resize(from + remainder);
}



string_t &string_t::erase(const_iterator const pos)
{
  check_range_iter_inclusive(pos, cbegin(), cend());
  return erase(int_from_pointer_diff(pos, data_), 1);
}



string_t &string_t::erase(const_iterator const from, const_iterator const to)
{
  check_range_iter_inclusive(from, cbegin(), cend());
  check_range_iter_inclusive(to, cbegin(), cend());

  return erase(int_from_pointer_diff(from, data_), int_from_pointer_diff(to, from));
}



string_t &string_t::clear()
{
  return resize(0);
}



string_t &string_t::resize(size_type const new_length)
{
  const size_type old_len = size();

  if (new_length < 0) {
    s_throw(
      std::out_of_range,
      "resize(new_length) requires new_length (%d) >= 0",
      new_length
      );
  } else if (old_len == new_length) {
    return *this;
  } if (new_length > old_len) {
    reserve(new_length + 1);
  }

  data_[new_length] = '\0';
  if (is_short()) {
    rep_.short_.length_ = uint8_t(new_length);
  } else {
    rep_.long_.length_ = new_length;
  }

  return *this;
}



auto string_t::size() const -> size_type
{
  return is_short() ? size_type(rep_.short_.length_) : rep_.long_.length_;
}



bool string_t::empty() const
{
  return size() == 0;
}



string_t &string_t::shrink_to_fit()
{
  if (is_short()) {
    return *this;
  } else if (!can_free()) {
    return *this;
  }

  const size_type len = size();
  if (len < short_data_len_) {
    const size_type old_cap = rep_.long_.capacity_;

    rep_.long_.length_ = 0;
    rep_.long_.capacity_ = 0;
    std::memcpy(rep_.short_.short_data_, data_, len);

    if (old_cap) {
      delete [] data_;
    }

    rep_.short_.length_ = len;
    data_ = rep_.short_.short_data_;
    data_[len] = '\0';
  } else {
    rep_.long_.capacity_ = short_data_len_;
    reserve(len);
  }
  return *this;
}



string_t &string_t::reserve(size_type const requested_capacity)
{
  const size_type old_len = size();
  const bool is_short_cache = is_short();

  if ((!is_short_cache && requested_capacity <= capacity()) || (requested_capacity <= short_data_len_)) {
    return *this;
  }

  // Initial allocations get an alignment of 16 bytes.
  const size_type old_capacity = capacity();

  if (can_free()) {
    char *new_buffer = (char *)std::realloc(data_, requested_capacity);
    assert(new_buffer != nullptr);

    if (!new_buffer) {
      return *this;
    }

    data_ = new_buffer;
  } else {
    char *new_buffer = (char *)std::malloc(requested_capacity);
    // How the hell should you handle this, anyway?
    assert(new_buffer != nullptr);

    if (!new_buffer) {
      return *this;
    }

    std::memcpy(new_buffer, data_, old_capacity);

    data_ = new_buffer;
    rep_.long_.length_ = old_len;
    rep_.long_.capacity_ = requested_capacity;
  }

  return *this;
}



void string_t::reserve_for_growth(size_type const needed_cap)
{
  static size_type const CAPACITY_GROWTH = 14;
  static size_type const CAPACITY_FILL_RESIZE = 85;

  size_type const current_cap = capacity();

  if (needed_cap < current_cap) {
    size_type const current_size = size();
    size_type const current_fill = (current_size * 100) / current_cap;

    if (current_fill < 85) {
      return;
    }
  }

  size_type const spec_cap = (current_cap * CAPACITY_GROWTH) / 10;
  if (spec_cap >= needed_cap) {
    reserve(spec_cap);
  } else {
    reserve(needed_cap);
  }
}



auto string_t::capacity() const -> size_type
{
  return is_short() ? short_data_len_ : rep_.long_.capacity_;
}



char &string_t::operator [] (int index)
{
  /* bounds checking in debug mode only */
  assert(index >= 0);
  assert(index < size());

  return data_[index];
}



char string_t::operator [] (int index) const
{
  assert(index >= 0);
  assert(index < size());

  return data_[index];
}



char &string_t::at(int index)
{
  check_range(index, size());
  return data_[index];
}



char string_t::at(int index) const
{
  check_range(index, size());
  return data_[index];
}



char &string_t::front()
{
  assert(size());
  return data_[0];
}



char string_t::front() const
{
  assert(size());
  return data_[0];
}



char &string_t::back()
{
  assert(size());
  return data_[size() - 1];
}



char string_t::back() const
{
  assert(size());
  return data_[size() - 1];
}



auto string_t::index_of(const_iterator const iter) const -> size_type
{
  check_range_iter_inclusive(iter, cbegin(), cend());
  return int_from_pointer_diff(iter, data_);
}



auto string_t::index_of(const_reverse_iterator const iter) const -> size_type
{
  return index_of(iter.base());
}



string_t string_t::substr(size_type pos, size_type count) const
{
  assert(pos <= size());

  if (count == npos) {
    count = size() - pos;
  }

  assert(pos + count <= size());

  if (count == 0) {
    return string_t();
  }

  return string_t(data_ + pos, count);
}



string_t string_t::substr(const_iterator const from) const
{
  return string_t(from, cend());
}



string_t string_t::substr(const_iterator const from, const_iterator const to) const
{
  return string_t(from, to);
}



char *string_t::c_str()
{
  return data_;
}



const char *string_t::c_str() const
{
  return data_;
}



char *string_t::data()
{
  return data_;
}



const char *string_t::data() const
{
  return data_;
}



#define DEF_BEGIN_ITER(NAME, RTYPE, args...)                                  \
auto string_t:: NAME () args -> RTYPE                                         \
{                                                                             \
  return data_;                                                               \
}

#define DEF_END_ITER(NAME, RTYPE, args...)                                    \
auto string_t:: NAME () args -> RTYPE                                         \
{                                                                             \
  return data_ + size();                                                      \
}

DEF_BEGIN_ITER(cbegin, const_iterator, const)
DEF_BEGIN_ITER(begin, const_iterator, const)
DEF_BEGIN_ITER(begin, iterator)
DEF_END_ITER(cend, const_iterator, const)
DEF_END_ITER(end, const_iterator, const)
DEF_END_ITER(end, iterator)

#undef DEF_BEGIN_ITER
#undef DEF_END_ITER



#define DEF_RBEGIN_ITER(NAME, RTYPE, args...)                                 \
auto string_t:: NAME () args -> RTYPE                                         \
{                                                                             \
  return RTYPE { end() };                                                     \
}

#define DEF_REND_ITER(NAME, RTYPE, args...)                                   \
auto string_t:: NAME () args -> RTYPE                                         \
{                                                                             \
  return RTYPE { begin() };                                                   \
}

DEF_RBEGIN_ITER(crbegin, const_reverse_iterator, const)
DEF_RBEGIN_ITER(rbegin, const_reverse_iterator, const)
DEF_RBEGIN_ITER(rbegin, reverse_iterator)
DEF_REND_ITER(crend, const_reverse_iterator, const)
DEF_REND_ITER(rend, const_reverse_iterator, const)
DEF_REND_ITER(rend, reverse_iterator)

#undef DEF_RBEGIN_ITER
#undef DEF_REND_ITER



#define DEF_OFFSET_ITER(NAME, RTYPE, args...)                                 \
auto string_t:: NAME (size_type index) args -> RTYPE                          \
{                                                                             \
  assert(index >= 0);                                                         \
  assert(index <= size());                                                    \
  return RTYPE (data_ + index);                                               \
}

#define DEF_ROFFSET_ITER(NAME, RTYPE, args...)                                \
auto string_t:: NAME (size_type index) args -> RTYPE                          \
{                                                                             \
  assert(index >= 0);                                                         \
  assert(index <= size());                                                    \
  return RTYPE (data_ + size() - (1 + index));                                \
}

DEF_OFFSET_ITER(offset, iterator)
DEF_OFFSET_ITER(offset, const_iterator, const)
DEF_OFFSET_ITER(coffset, const_iterator, const)
DEF_ROFFSET_ITER(roffset, reverse_iterator)
DEF_ROFFSET_ITER(roffset, const_reverse_iterator, const)
DEF_ROFFSET_ITER(croffset, const_reverse_iterator, const)

#undef DEF_OFFSET_ITER
#undef DEF_ROFFSET_ITER



auto string_t::find(char ch, size_type from) -> iterator
{
  return iterator(data_ + find_char(ch, from));
}



auto string_t::find(const string_t &other, size_type from) -> iterator
{
  return iterator(data_ + find_substring(other.data_, from, other.size()));
}



auto string_t::find(const char *str, size_type from) -> iterator
{
  return iterator(data_ + find_substring(str, from, std::strlen(str)));
}



auto string_t::find(const char *str, size_type from, size_type length) -> iterator
{
  return iterator(data_ + find_substring(str, from, length));
}



auto string_t::find(char ch, size_type from) const -> const_iterator
{
  return const_iterator(data_ + find_char(ch, from));
}



auto string_t::find(const string_t &other, size_type from) const -> const_iterator
{
  return const_iterator(data_ + find_substring(other.data_, from, other.size()));
}



auto string_t::find(const char *str, size_type from) const -> const_iterator
{
  return const_iterator(data_ + find_substring(str, from, std::strlen(str)));
}



auto string_t::find(const char *str, size_type from, size_type length) const -> const_iterator
{
  return const_iterator(data_ + find_substring(str, from, length));
}



auto string_t::find(char ch, const_iterator const from) -> iterator
{
  return iterator(data_ + find_char(ch, index_of(from)));
}



auto string_t::find(const string_t &other, const_iterator const from) -> iterator
{
  return iterator(data_ + find_substring(other.data_, index_of(from), other.size()));
}



auto string_t::find(const char *str, const_iterator const from) -> iterator
{
  return iterator(data_ + find_substring(str, index_of(from), std::strlen(str)));
}



auto string_t::find(const char *str, const_iterator const from, size_type length) -> iterator
{
  return iterator(data_ + find_substring(str, index_of(from), length));
}



auto string_t::find(char ch, const_iterator const from) const -> const_iterator
{
  return const_iterator(data_ + find_char(ch, index_of(from)));
}



auto string_t::find(const string_t &other, const_iterator const from) const -> const_iterator
{
  return const_iterator(data_ + find_substring(other.data_, index_of(from), other.size()));
}



auto string_t::find(const char *str, const_iterator const from) const -> const_iterator
{
  return const_iterator(data_ + find_substring(str, index_of(from), std::strlen(str)));
}



auto string_t::find(const char *str, const_iterator const from, size_type length) const -> const_iterator
{
  return const_iterator(data_ + find_substring(str, index_of(from), length));
}



auto string_t::find_index(char ch, size_type from) const -> size_type
{
  const size_type result = find_char(ch, from);
  return result == size() ? npos : result;
}



auto string_t::find_index(const string_t &other, size_type from) const -> size_type
{
  const size_type result = find_substring(other.data_, from, other.size());
  return result == size() ? npos : result;
}



auto string_t::find_index(const char *str, size_type from) const -> size_type
{
  const size_type result = find_substring(str, from, std::strlen(str));
  return result == size() ? npos : result;
}



auto string_t::find_index(const char *str, size_type from, size_type length) const -> size_type
{
  const size_type result = find_substring(str, from, length);
  return result == size() ? npos : result;
}



auto string_t::find_index(char ch, const_iterator const from) const -> size_type
{
  const size_type result = find_char(ch, index_of(from));
  return result == size() ? npos : result;
}



auto string_t::find_index(const string_t &other, const_iterator const from) const -> size_type
{
  const size_type result = find_substring(other.data_, index_of(from), other.size());
  return result == size() ? npos : result;
}



auto string_t::find_index(const char *str, const_iterator const from) const -> size_type
{
  const size_type result = find_substring(str, index_of(from), std::strlen(str));
  return result == size() ? npos : result;
}



auto string_t::find_index(const char *str, const_iterator const from, size_type length) const -> size_type
{
  const size_type result = find_substring(str, index_of(from), length);
  return result == size() ? npos : result;
}



bool string_t::has_suffix(const string_t &str) const
{
  return has_suffix(str.data_, str.size());
}



bool string_t::has_suffix(const char *zstr) const
{
  return has_suffix(zstr, std::strlen(zstr));
}



bool string_t::has_suffix(const char *zstr, size_type length) const
{
  assert(zstr);

  if (length == 0) {
    return true;
  } else if (length > size()) {
    return false;
  }
  return std::memcmp(data_ + size() - length, zstr, length) == 0;
}



bool string_t::has_prefix(const string_t &str) const
{
  return has_prefix(str.data_, str.size());
}



bool string_t::has_prefix(const char *zstr) const
{
  return has_prefix(zstr, std::strlen(zstr));
}



bool string_t::has_prefix(const char *zstr, size_type length) const
{
  assert(zstr);

  if (length == 0) {
    return true;
  } else if (length > size()) {
    return false;
  }
  return std::memcmp(data_, zstr, length) == 0;
}



char *string_t::operator * ()
{
  return data_;
}



const char *string_t::operator * () const
{
  return data_;
}



string_t::operator char * ()
{
  return data_;
}



string_t::operator const char * () const
{
  return data_;
}



std::ostream &operator << (std::ostream &out, const string_t &in)
{
  const string_t::size_type len = in.size();
  if (len == 0) {
    return out;
  }

  return out.write(in.data(), len);
}



bool string_t::operator == (const char *zstr) const
{
  const size_type slen = size();
  const size_type zlen = std::strlen(zstr);
  if (zlen != slen) {
    return false;
  }
  return std::memcmp(data_, zstr, slen) == 0;
}



bool string_t::operator == (const string_t &other) const
{
  return compare(other) == 0;
}



bool string_t::operator != (const string_t &other) const
{
  return compare(other) != 0;
}



bool string_t::operator != (const char *zstr) const
{
  return !(*this == zstr);
}



bool string_t::operator >  (const string_t &other) const
{
  return compare(other) > 0;
}



bool string_t::operator <  (const string_t &other) const
{
  return compare(other) < 0;
}



bool string_t::operator >= (const string_t &other) const
{
  return compare(other) >= 0;
}



bool string_t::operator <= (const string_t &other) const
{
  return compare(other) <= 0;
}



string_t string_t::operator + (const string_t &rhs) const
{
  string_t result;
  result.reserve(size() + rhs.size());
  result.append(*this);
  result.append(rhs);
  return result;
}



auto string_t::find_char(char ch, size_type from) const -> size_type
{
  // Handled by the conditional below, but try to catch bad behavior in debug
  const size_type len = size();
  assert(from <= len);

  if (len == 0 || from >= len) {
    return len;
  } else if (from < 0) {
    return len;
  }

  const uint32_t *data_ptr = reinterpret_cast<const uint32_t *>(data_ + from);
  size_type count = len - from;
  while (count >= sizeof(*data_ptr)) {
    uint32_t ch_read = *data_ptr;
    if ((ch_read & 0xFF) == ch) return (len - count);
    if (((ch_read >> 8) & 0xFF) == ch) return (len - count) + 1;
    if (((ch_read >> 16) & 0xFF) == ch) return (len - count) + 2;
    if (((ch_read >> 24) & 0xFF) == ch) return (len - count) + 3;
    count -= sizeof(*data_ptr);
    ++data_ptr;
  }
  const char *remainder = reinterpret_cast<const char *>(data_ptr);
  switch (count) {
  case 3: if (remainder[0] == ch) { return len - 3; } // fall-through
  case 2: if (remainder[1] == ch) { return len - 2; } // fall-through
  case 1: if (remainder[2] == ch) { return len - 1; } // fall-through
  case 0: // fall-through
  default: return len;
  }
}



auto string_t::find_substring(const char *str, size_type from, size_type length) const -> size_type
{
  size_type const data_length = size();
  if (data_length < length) {
    return data_length;
  } else if (length <= 0) {
    return data_length;
  } else if (from < 0) {
    return data_length;
  }

  size_type const searchable_length = data_length - length;
  char const *ptr = data_;

  for (; from <= searchable_length; ++from) {
    if (std::memcmp(ptr + from, str, length) == 0) {
      return from;
    }
  }

  return data_length;
}



bool string_t::is_short() const
{
  return data_ == rep_.short_.short_data_;
}



bool string_t::can_free() const
{
  return !is_short();
}


} // namespace snow
