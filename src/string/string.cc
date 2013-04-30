// string.cc -- Noel Cower -- Public Domain

#include <snow/string/string.hh>

#include <cassert>
#include <cstring>


namespace snow {


string_t::string_t()
{
  /* nop */
}



string_t::string_t(const const_iterator &from, const const_iterator &to) :
  string_t(from.ptr, to <= from ? 0 : size_type(to - from))
{
  /* nop */
}



string_t::string_t(const iterator &from, const iterator &to, bool nofree) :
  string_t(from.ptr, to <= from ? 0 : size_type(to - from), nofree)
{
  /* nop */
}



string_t::string_t(const char *zstr) :
  string_t(zstr, std::strlen(zstr))
{
  /* nop */
}



string_t::string_t(const char *zstr, size_type length)
{
  assert(zstr);
  if (length > 0) {
    assign(zstr, length);
  }
}



string_t::string_t(string_t &&other) :
  length_(other.length_),
  capacity_(other.capacity_),
  data_(other.data_)
{
  // if (data_ == other.short_data_ && length_) {
    // std::memcpy(short_data_, other.short_data_, short_data_len_);
    // data_ = short_data_;
  if (data_ == (char *)&other.capacity_) {
    data_ = (char *)&capacity_;
  }

  other.length_ = 0;
  other.capacity_ = 0;
  other.data_ = (char *)&other.capacity_;
  // other.capacity_ = short_data_len_;
  // other.data_ = other.short_data_;
  // *(void **)other.short_data_ = 0;
}



string_t::string_t(const string_t &other)
{
  resize(other.length_);
  std::memcpy(data_, other.data_, other.length_);
}



string_t::string_t(std::initializer_list<char> init)
{
  resize(init.size());
  auto iter = init.begin();
  auto init_end = init.end();
  size_type index = 0;
  for (; iter != init_end; ++iter, ++index) {
    data_[index] = *iter;
  }
}



string_t::string_t(char *zstr, size_type length, bool nofree)
{
  assert(zstr);
  if (nofree) {
    length_ = length;
    data_ = zstr;
    capacity_ = 0;
  } else {
    resize(length_);
    if (length_) {
      std::memcpy(data_, zstr, length_);
    }
    data_[length_] = '\0';
  }
}



string_t::~string_t()
{
  // if (data_ != short_data_ && capacity_ > 0) {
  if (data_ != (char *)&capacity_ && capacity_ > 0) {
    delete [] data_;
  }
}



string_t &string_t::operator = (string_t &&other)
{
  if (&other == this) {
    return *this;
  }

  // if (data_ != short_data_ && capacity_ > 0) {
  if (data_ != (char *)&capacity_ && capacity_ > 0) {
    delete [] data_;
  }

  data_ = other.data_;
  length_ = other.length_;
  capacity_ = other.capacity_;

  // if (data_ == other.short_data_) {
    // std::memcpy(short_data_, other.short_data_, short_data_len_);
    // data_ = short_data_;
  if (data_ == (char *)&other.capacity_) {
    data_ = (char *)&capacity_;
  }

  other.data_ = (char *)&other.capacity_;
  other.length_ = 0;
  other.capacity_ = 0;
  // other.capacity_ = short_data_len_;
  // other.data_ = other.short_data_;
  // *(void **)other.short_data_ = 0;

  return *this;
}



string_t &string_t::operator = (const std::string &other)
{
  resize(other.size());
  if (length_) {
    std::memcpy(data_, other.data(), length_);
  }
  return *this;
}



string_t &string_t::operator = (const char *zstr)
{
  assert(zstr);
  const size_type len = std::strlen(zstr);
  resize(len);
  if (len) {
    std::memcpy(data_, zstr, length_);
  }
  return *this;
}



string_t &string_t::operator = (const string_t &other)
{
  if (this != &other) {
    resize(other.length_);
    if (length_) {
      std::memcpy(data_, other.data_, length_);
    }
  }
  return *this;
}



string_t &string_t::assign(const char *zstr, size_type length)
{
  assert(zstr);
  resize(length);
  if (length_) {
    std::memcpy(data_, zstr, length);
  }
  return *this;
}



int string_t::compare(const string_t &other) const
{
  if (this == &other) {
    return 0;
  } else if (other.length_ == length_) {
    return length_ ? std::memcmp(data_, other.data_, length_) : 0;
  } else {
    return length_ < other.length_ ? -1 : 1;
  }
}



void string_t::append(char ch)
{
  const size_type len = length_;
  resize(len + 1);
  data_[len] = ch;
}



void string_t::append(const char *zstr)
{
  assert(zstr);
  append(zstr, std::strlen(zstr));
}



void string_t::append(const char *zstr, size_type length)
{
  assert(zstr);
  const size_type old_length = length_;
  if (length) {
    resize(length_ + length);
    std::memcpy(data_ + old_length, zstr, length);
  }
}



void string_t::append(const string_t &str)
{
  const size_type old_length = length_;
  if (str.length_) {
    resize(length_ + str.length_);
    std::memcpy(data_ + old_length, str.data(), str.size());
  }
}



void string_t::append(const const_iterator &from, const const_iterator &to)
{
  if (from == to || from < to) {
    return;
  }

  append(from.ptr, to.ptr - from.ptr);
}



void string_t::push_back(char ch)
{
  append(ch);
}



void string_t::pop_back()
{
  assert(length_ > 0);
  if (length_) {
    resize(length_ - 1);
  }
}



void string_t::erase(size_type from, size_type count)
{
  size_type to;
  switch (count) {
  case 0: return;
  case npos: to = length_ - from; break;
  default: to = from + count; break;
  }

  assert(from <= length_);
  assert(from + count <= length_);

  if (from == length_) {
    return;
  } else if (from == 0 && to == length_) {
    resize(0);
    return;
  } else if (to == length_) {
    resize(from);
    return;
  } else if (from == 0) {
    const size_type new_len = length_ - to;
    std::memmove(data_, data_ + to, new_len);
    resize(new_len);
    return;
  }

  const size_type remainder = length_ - to;
  std::memmove(data_ + from, data_ + to, remainder);
  length_ = from + remainder;
}



void string_t::erase(const const_iterator &pos)
{
  if (pos.ptr == data_ + length_) {
    return;
  }

  erase(size_type(data_ - pos.ptr), 1);
}



void string_t::erase(const const_iterator &from, const const_iterator &to)
{
  if (from >= to) {
    return;
  }
  const size_type from_sz = size_type(from.ptr - data_);
  erase(from_sz, size_type(to.ptr - data_) - from_sz);
}



void string_t::clear()
{
  resize(0);
}



void string_t::resize(size_type len)
{
  if (length_ == len) {
    return;
  } if (len > length_) {
    reserve(len + 1);
  }
  data_[len] = '\0';
  length_ = len;
}



auto string_t::size() const -> size_type
{
  return length_;
}



bool string_t::empty() const
{
  return length_ == 0;
}



void string_t::shrink_to_fit()
{
  if (data_ == (char *)&capacity_) {
    return;
  }
  if (length_ < short_data_len_) {
    capacity_ = 0;
    std::memcpy((char *)&capacity_, data_, length_);
  } else {
    capacity_ = short_data_len_;
    reserve(length_);
  }
}



void string_t::reserve(size_type cap)
{
  static size_type fixed_alignments[128] = {
    /* 0 - 7: 16 bytes */
    16, 16, 16, 16, 16, 16, 16, 16,
    /* 8-15: 32 bytes */
    32, 32, 32, 32, 32, 32, 32, 32,
    /* 16 - 23: 64 bytes */
    64, 64, 64, 64, 64, 64, 64, 64,
    /* 24 - 31: 128 bytes */
    128, 128, 128, 128, 128, 128, 128, 128,
    /* 32 - 63: 256 bytes */
    256, 256, 256, 256, 256, 256, 256, 256,
    256, 256, 256, 256, 256, 256, 256, 256,
    256, 256, 256, 256, 256, 256, 256, 256,
    256, 256, 256, 256, 256, 256, 256, 256,
    /* 64 - 95: 512 bytes */
    512, 512, 512, 512, 512, 512, 512, 512,
    512, 512, 512, 512, 512, 512, 512, 512,
    512, 512, 512, 512, 512, 512, 512, 512,
    512, 512, 512, 512, 512, 512, 512, 512,
    /* 96 - 128 and above: 1kb */
    1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
    1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
    1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
    1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
  };

  const size_type is_short = (data_ == (char *)&capacity_);

  if ((!is_short && cap <= capacity_) || (is_short && cap <= short_data_len_)) {
    return;
  }

  size_type cap_align_diff;
  cap_align_diff = (cap - capacity_ * !is_short) / 16;

  if (cap_align_diff > 127) {
    cap_align_diff = 127;
  }

  const size_type alignment =
    (!is_short && capacity_ != short_data_len_) ? fixed_alignments[cap_align_diff] : 16;

  cap = (cap + alignment) & ~(alignment - 1);
  const size_type cap_diff = cap - capacity_ * !is_short;
  char *newbuf = new char[cap];

  // How the hell should you handle this, anyway?
  assert(newbuf);

  if (length_) {
    std::memcpy(newbuf, data_, length_);
    std::memset(newbuf + length_, 0, cap_diff);
  } else {
    std::memset(newbuf, 0, capacity_);
  }

  // if (data_ != short_data_ && capacity_ > 0) {
  if (data_ != (char *)&capacity_ && capacity_ > 0) {
    delete [] data_;
  }

  data_ = newbuf;
  capacity_ = cap;
}



auto string_t::capacity() const -> size_type
{
  return capacity_;
}



char &string_t::operator [] (ptrdiff_t index)
{
  if (index < 0) {
    index = length_ + index;
  }

  /* bounds checking in debug mode only */
  assert(index >= 0);
  assert(index < length_);

  return data_[index];
}



char string_t::operator [] (ptrdiff_t index) const
{
  if (index < 0) {
    index = length_ + index;
  }

  assert(index >= 0);
  assert(index < length_);

  return data_[index];
}



char &string_t::at(ptrdiff_t index)
{
  if (index < 0) {
    index = length_ + index;
  }

  // check bounds - this fails regardless of NDEBUG since ::at must fail if the
  // index is out of bounds (bearing in mind that the above negative indices
  // still work)
  if (!(index >= 0 || index < length_)) {
    std::abort();
  }

  return data_[index];
}



char string_t::at(ptrdiff_t index) const
{
  if (index < 0) {
    index = length_ + index;
  }

  if (!(index >= 0 || index < length_)) {
    std::abort();
  }

  return data_[index];
}



char &string_t::front()
{
  assert(length_);
  return data_[0];
}



char string_t::front() const
{
  assert(length_);
  return data_[0];
}



char &string_t::back()
{
  assert(length_);
  return data_[length_ - 1];
}



char string_t::back() const
{
  assert(length_);
  return data_[length_ - 1];
}



auto string_t::index_of(const iterator &iter) const -> size_type
{
  if (iter.ptr < data_ || iter.ptr > data_ + length_) {
    return npos;
  } else {
    return size_type(iter.ptr - data_);
  }
}



auto string_t::index_of(const const_iterator &iter) const -> size_type
{
  if (iter.ptr < data_ || iter.ptr > data_ + length_) {
    return npos;
  } else {
    return size_type(iter.ptr - data_);
  }
}



auto string_t::index_of(const reverse_iterator &iter) const -> size_type
{
  if (iter.ptr < data_ || iter.ptr > data_ + length_) {
    return npos;
  } else {
    return size_type(iter.ptr - data_);
  }
}



auto string_t::index_of(const const_reverse_iterator &iter) const -> size_type
{
  if (iter.ptr < data_ || iter.ptr > data_ + length_) {
    return npos;
  } else {
    return size_type(iter.ptr - data_);
  }
}



string_t string_t::substr(size_type pos, size_type count) const
{
  assert(pos <= length_);

  if (count == npos) {
    count = length_ - pos;
  }

  assert(pos + count <= length_);

  if (count == 0) {
    return string_t();
  }

  return string_t(data_ + pos, count);
}



string_t string_t::substr(const const_iterator &from) const
{
  return string_t(from, cend());
}



string_t string_t::substr(const const_iterator &from, const const_iterator &to) const
{
  return string_t(from, to);
}



string_t string_t::window(size_type pos, size_type count)
{
  assert(pos <= length_);
  assert(pos + count <= length_);
  if (count == 0) {
    return string_t();
  }

  return string_t(data_ + pos, count);
}



string_t string_t::window(const iterator &from)
{
  return string_t(from, end(), true);
}



string_t string_t::window(const iterator &from, const iterator &to)
{
  return string_t(from, to, true);
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



auto string_t::cbegin() const -> const_iterator
{
  return const_iterator(data_);
}



auto string_t::cend() const -> const_iterator
{
  return const_iterator(data_ + length_);
}



auto string_t::begin() -> iterator
{
  return iterator(data_);
}



auto string_t::end() -> iterator
{
  return iterator(data_ + length_);
}



auto string_t::begin() const -> const_iterator
{
  return const_iterator(data_);
}



auto string_t::end() const -> const_iterator
{
  return const_iterator(data_ + length_);
}



auto string_t::crbegin() const -> const_reverse_iterator
{
  return const_reverse_iterator(data_ + length_ - 1);
}



auto string_t::crend() const -> const_reverse_iterator
{
  return const_reverse_iterator(data_ - 1);
}



auto string_t::rbegin() -> reverse_iterator
{
  return reverse_iterator(data_ + length_ - 1);
}



auto string_t::rend() -> reverse_iterator
{
  return reverse_iterator(data_ - 1);
}



auto string_t::rbegin() const -> const_reverse_iterator
{
  return const_reverse_iterator(data_ + length_ - 1);
}



auto string_t::rend() const -> const_reverse_iterator
{
  return const_reverse_iterator(data_ - 1);
}



auto string_t::find(char ch, size_type from) -> iterator
{
  return iterator(data_ + find_char(ch, from));
}



auto string_t::find(const string_t &other, size_type from) -> iterator
{
  return iterator(data_ + find_substring(other.data_, from, other.length_));
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
  return const_iterator(data_ + find_substring(other.data_, from, other.length_));
}



auto string_t::find(const char *str, size_type from) const -> const_iterator
{
  return const_iterator(data_ + find_substring(str, from, std::strlen(str)));
}



auto string_t::find(const char *str, size_type from, size_type length) const -> const_iterator
{
  return const_iterator(data_ + find_substring(str, from, length));
}



auto string_t::find(char ch, const const_iterator &from) -> iterator
{
  return iterator(data_ + find_char(ch, size_type(from.ptr - data_)));
}



auto string_t::find(const string_t &other, const const_iterator &from) -> iterator
{
  return iterator(data_ + find_substring(other.data_, size_type(from.ptr - data_), other.length_));
}



auto string_t::find(const char *str, const const_iterator &from) -> iterator
{
  return iterator(data_ + find_substring(str, size_type(from.ptr - data_), std::strlen(str)));
}



auto string_t::find(const char *str, const const_iterator &from, size_type length) -> iterator
{
  return iterator(data_ + find_substring(str, size_type(from.ptr - data_), length));
}



auto string_t::find(char ch, const const_iterator &from) const -> const_iterator
{
  return const_iterator(data_ + find_char(ch, size_type(from.ptr - data_)));
}



auto string_t::find(const string_t &other, const const_iterator &from) const -> const_iterator
{
  return const_iterator(data_ + find_substring(other.data_, size_type(from.ptr - data_), other.length_));
}



auto string_t::find(const char *str, const const_iterator &from) const -> const_iterator
{
  return const_iterator(data_ + find_substring(str, size_type(from.ptr - data_), std::strlen(str)));
}



auto string_t::find(const char *str, const const_iterator &from, size_type length) const -> const_iterator
{
  return const_iterator(data_ + find_substring(str, size_type(from.ptr - data_), length));
}



auto string_t::find_index(char ch, size_type from) const -> size_type
{
  const size_type result = find_char(ch, from);
  return result == length_ ? npos : result;
}



auto string_t::find_index(const string_t &other, size_type from) const -> size_type
{
  const size_type result = find_substring(other.data_, from, other.length_);
  return result == length_ ? npos : result;
}



auto string_t::find_index(const char *str, size_type from) const -> size_type
{
  const size_type result = find_substring(str, from, std::strlen(str));
  return result == length_ ? npos : result;
}



auto string_t::find_index(const char *str, size_type from, size_type length) const -> size_type
{
  const size_type result = find_substring(str, from, length);
  return result == length_ ? npos : result;
}



auto string_t::find_index(char ch, const const_iterator &from) const -> size_type
{
  const size_type result = find_char(ch, size_type(from.ptr - data_));
  return result == length_ ? npos : result;
}



auto string_t::find_index(const string_t &other, const const_iterator &from) const -> size_type
{
  const size_type result = find_substring(other.data_, size_type(from.ptr - data_), other.length_);
  return result == length_ ? npos : result;
}



auto string_t::find_index(const char *str, const const_iterator &from) const -> size_type
{
  const size_type result = find_substring(str, size_type(from.ptr - data_), std::strlen(str));
  return result == length_ ? npos : result;
}



auto string_t::find_index(const char *str, const const_iterator &from, size_type length) const -> size_type
{
  const size_type result = find_substring(str, size_type(from.ptr - data_), length);
  return result == length_ ? npos : result;
}



bool string_t::has_suffix(const string_t &str) const
{
  return has_suffix(str.data_, str.length_);
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
  } else if (length > length_) {
    return false;
  }
  return std::memcmp(data_ + length_ - length, zstr, length) == 0;
}



bool string_t::has_prefix(const string_t &str) const
{
  return has_prefix(str.data_, str.length_);
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
  } else if (length > length_) {
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
  if (in.length_ == 0) {
    return out;
  }

  return out.write(in.data_, in.length_);
}



bool string_t::operator == (const char *zstr) const
{
  const size_type zlen = std::strlen(zstr);
  if (zlen != length_) {
    return false;
  }
  return std::memcmp(data_, zstr, length_) == 0;
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
  result.reserve(length_ + rhs.length_);
  result.append(*this);
  result.append(rhs);
  return result;
}



auto string_t::find_char(char ch, size_type from) const -> size_type
{
  // Handled by the conditional below, but try to catch bad behavior in debug
  assert(from <= length_);

  if (length_ == 0 || from >= length_) {
    return length_;
  }

  const uint32_t *data_ptr = (const uint32_t *)(data_ + from);
  size_type count = length_ - from;
  while (count >= sizeof(*data_ptr)) {
    uint32_t ch_read = *data_ptr;
    if ((ch_read & 0xFF) == ch) return (length_ - count);
    if (((ch_read >> 8) & 0xFF) == ch) return (length_ - count) + 1;
    if (((ch_read >> 16) & 0xFF) == ch) return (length_ - count) + 2;
    if (((ch_read >> 24) & 0xFF) == ch) return (length_ - count) + 3;
    count -= sizeof(*data_ptr);
    ++data_ptr;
  }
  const char *remainder = (const char *)data_ptr;
  switch (count) {
  case 3: if (remainder[0] == ch) { return length_ - 3; } // fall-through
  case 2: if (remainder[1] == ch) { return length_ - 2; } // fall-through
  case 1: if (remainder[2] == ch) { return length_ - 1; } // fall-through
  case 0: // fall-through
  default: return length_;
  }
}



static void gen_str_search_table(const char *str, string_t::size_type length, ptrdiff_t jumps[])
{
  ptrdiff_t str_index = 0;
  ptrdiff_t jmp_index = jumps[0] = -1;
  while (str_index < length) {
    while (jmp_index > -1 && str[str_index] != str[jmp_index]) {
      jmp_index = jumps[jmp_index];
    }

    jmp_index += 1;
    str_index += 1;

    if (str[str_index] == str[jmp_index]) {
      jumps[str_index] = jumps[jmp_index];
    } else {
      jumps[str_index] = jmp_index;
    }
  }
}



auto string_t::find_substring(const char *str, size_type from, size_type length) const -> size_type
{
  assert(str);
  assert(from <= length_);

  if (from >= length_ || from + length >= length_) {
    return length_;
  } else if (length == 0) {
    return from;
  }

  std::vector<ptrdiff_t> jumps(length);
  gen_str_search_table(str, length, jumps.data());

  ptrdiff_t data_index = from;
  ptrdiff_t str_index = 0;
  const size_type data_length = length_;
  const char *data_str = data_;

  while (data_index < data_length) {
    while (str_index > -1 && str[str_index] != data_str[data_index]) {
      str_index = jumps[str_index];
    }

    data_index += 1;
    str_index += 1;

    if (str_index >= length) {
      return size_type(data_index - str_index);
    }
  }

  return data_length;
}


} // namespace snow
