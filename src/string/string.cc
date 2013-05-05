// string.cc -- Noel Cower -- Public Domain

#include <snow/string/string.hh>

#include <cassert>
#include <cstring>


namespace snow {


static void gen_str_search_table(
  const char *str,
  string_t::size_type length,
  ptrdiff_t jumps[]
  );



string_t::string_t() :
  rep_({{0x0, 0x0}})
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
  if (data_ == other.rep_.short_.short_data_) {
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



string_t::string_t(std::initializer_list<char> init) :
  string_t()
{
  resize(init.size());
  auto iter = init.begin();
  auto init_end = init.end();
  size_type index = 0;
  for (; iter != init_end; ++iter, ++index) {
    data_[index] = *iter;
  }
}



string_t::string_t(char *zstr, size_type length, bool nofree) :
  string_t()
{
  assert(zstr);
  if (nofree) {
    data_ = zstr;
    rep_.long_.length_ = length;
    rep_.long_.capacity_ = 0;
  } else {
    resize(length);
    if (length) {
      std::memcpy(data_, zstr, length);
    }
  }
}



string_t::~string_t()
{
  if (data_ != rep_.short_.short_data_ && rep_.long_.capacity_ > 0) {
    delete [] data_;
  }
}



string_t &string_t::operator = (string_t &&other)
{
  if (&other == this) {
    return *this;
  }

  if (data_ != rep_.short_.short_data_ && rep_.long_.capacity_ > 0) {
    delete [] data_;
  }

  data_ = other.data_;
  rep_ = other.rep_;

  if (data_ == other.rep_.short_.short_data_) {
    data_ = rep_.short_.short_data_;
  }

  other.data_ = other.rep_.short_.short_data_;
  other.rep_.long_.length_ = 0;
  other.rep_.long_.capacity_ = 0;

  return *this;
}



string_t &string_t::operator = (const std::string &other)
{
  resize(other.size());
  const size_type len = size();
  if (len) {
    std::memcpy(data_, other.data(), len);
  }
  return *this;
}



string_t &string_t::operator = (const char *zstr)
{
  assert(zstr);
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



void string_t::append(char ch)
{
  const size_type len = size();
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
  const size_type old_len = size();
  if (length) {
    resize(old_len + length);
    std::memcpy(data_ + old_len, zstr, length);
  }
}



void string_t::append(const string_t &str)
{
  const size_type old_len = size();
  const size_type other_len = str.size();
  if (other_len) {
    resize(old_len + other_len);
    std::memcpy(data_ + old_len, str.data(), other_len);
  }
}



void string_t::append(const const_iterator &from, const const_iterator &to)
{
  if (from == to || from < to) {
    return;
  }

  append(from.ptr, size_type(to.ptr - from.ptr));
}



void string_t::push_back(char ch)
{
  append(ch);
}



void string_t::pop_back()
{
  const size_type len = size();
  assert(len > 0);
  if (len) {
    resize(len - 1);
  }
}



void string_t::erase(size_type from, size_type count)
{
  const size_type len = size();
  size_type to;
  switch (count) {
  case 0: return;
  case npos: to = len - from; break;
  default: to = from + count; break;
  }

  assert(from <= len);
  assert(from + count <= len);

  if (from == len) {
    return;
  } else if (from == 0 && to == len) {
    resize(0);
    return;
  } else if (to == len) {
    resize(from);
    return;
  } else if (from == 0) {
    const size_type new_len = len - to;
    std::memmove(data_, data_ + to, new_len);
    resize(new_len);
    return;
  }

  const size_type remainder = len - to;
  std::memmove(data_ + from, data_ + to, remainder);
  resize(from + remainder);
}



void string_t::erase(const const_iterator &pos)
{
  if (pos.ptr == data_ + size()) {
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
  const size_type old_len = size();
  if (old_len == len) {
    return;
  } if (len > old_len) {
    reserve(len + 1);
  }
  data_[len] = '\0';
  if (data_ == rep_.short_.short_data_) {
    rep_.short_.length_ = uint8_t(len);
  } else {
    rep_.long_.length_ = len;
  }
}



auto string_t::size() const -> size_type
{
  return data_ == rep_.short_.short_data_ ? size_type(rep_.short_.length_) : rep_.long_.length_;
}



bool string_t::empty() const
{
  return size() == 0;
}



void string_t::shrink_to_fit()
{
  if (data_ == rep_.short_.short_data_) {
    return;
  } else if (rep_.long_.capacity_ == 0) {
    return;
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

  const size_type old_len = size();
  const size_type is_short = (data_ == rep_.short_.short_data_);

  if ((!is_short && cap <= rep_.long_.capacity_) || (is_short && cap <= short_data_len_)) {
    return;
  }

  size_type cap_align_diff;
  cap_align_diff = (cap - rep_.long_.capacity_ * !is_short) / 16;

  if (cap_align_diff > 127) {
    cap_align_diff = 127;
  }

  const size_type alignment =
    (!is_short && rep_.long_.capacity_ != short_data_len_) ? fixed_alignments[cap_align_diff] : 16;

  cap = (cap + alignment) & ~(alignment - 1);
  const size_type cap_diff = cap - rep_.long_.capacity_ * !is_short;
  char *newbuf = new char[cap];

  // How the hell should you handle this, anyway?
  assert(newbuf);

  const size_type len = size();
  if (len) {
    std::memcpy(newbuf, data_, len);
    std::memset(newbuf + len, 0, cap_diff);
  } else {
    std::memset(newbuf, 0, cap_diff);
  }

  if (!is_short && rep_.long_.capacity_ > 0) {
    delete [] data_;
  }

  data_ = newbuf;
  rep_.long_.length_ = old_len;
  rep_.long_.capacity_ = cap;
}



auto string_t::capacity() const -> size_type
{
  return data_ == rep_.short_.short_data_ ? short_data_len_ : rep_.long_.capacity_;
}



char &string_t::operator [] (ptrdiff_t index)
{
  if (index < 0) {
    index = size() + index;
  }

  /* bounds checking in debug mode only */
  assert(index >= 0);
  assert(index < size());

  return data_[index];
}



char string_t::operator [] (ptrdiff_t index) const
{
  if (index < 0) {
    index = size() + index;
  }

  assert(index >= 0);
  assert(index < size());

  return data_[index];
}



char &string_t::at(ptrdiff_t index)
{
  if (index < 0) {
    index = size() + index;
  }

  // check bounds - this fails regardless of NDEBUG since ::at must fail if the
  // index is out of bounds (bearing in mind that the above negative indices
  // still work)
  if (!(index >= 0 || index < size())) {
    std::abort();
  }

  return data_[index];
}



char string_t::at(ptrdiff_t index) const
{
  if (index < 0) {
    index = size() + index;
  }

  if (!(index >= 0 || index < size())) {
    std::abort();
  }

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



auto string_t::index_of(const iterator &iter) const -> size_type
{
  if (iter.ptr < data_ || iter.ptr > data_ + size()) {
    return npos;
  } else {
    return size_type(iter.ptr - data_);
  }
}



auto string_t::index_of(const const_iterator &iter) const -> size_type
{
  if (iter.ptr < data_ || iter.ptr > data_ + size()) {
    return npos;
  } else {
    return size_type(iter.ptr - data_);
  }
}



auto string_t::index_of(const reverse_iterator &iter) const -> size_type
{
  if (iter.ptr < data_ || iter.ptr > data_ + size()) {
    return npos;
  } else {
    return size_type(iter.ptr - data_);
  }
}



auto string_t::index_of(const const_reverse_iterator &iter) const -> size_type
{
  if (iter.ptr < data_ || iter.ptr > data_ + size()) {
    return npos;
  } else {
    return size_type(iter.ptr - data_);
  }
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
  assert(pos <= size());
  assert(pos + count <= size());
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



#define DEF_BEGIN_ITER(NAME, RTYPE, args...)                                  \
auto string_t:: NAME () args -> RTYPE                                         \
{                                                                             \
  return RTYPE (data_);                                                       \
}

#define DEF_END_ITER(NAME, RTYPE, args...)                                    \
auto string_t:: NAME () args -> RTYPE                                         \
{                                                                             \
  return RTYPE (data_ + size());                                              \
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
  return RTYPE (data_ + size() - 1);                                          \
}

#define DEF_REND_ITER(NAME, RTYPE, args...)                                   \
auto string_t:: NAME () args -> RTYPE                                         \
{                                                                             \
  return RTYPE (data_ - 1);                                                   \
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



auto string_t::find(char ch, const const_iterator &from) -> iterator
{
  return iterator(data_ + find_char(ch, size_type(from.ptr - data_)));
}



auto string_t::find(const string_t &other, const const_iterator &from) -> iterator
{
  return iterator(data_ + find_substring(other.data_, size_type(from.ptr - data_), other.size()));
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
  return const_iterator(data_ + find_substring(other.data_, size_type(from.ptr - data_), other.size()));
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



auto string_t::find_index(char ch, const const_iterator &from) const -> size_type
{
  const size_type result = find_char(ch, size_type(from.ptr - data_));
  return result == size() ? npos : result;
}



auto string_t::find_index(const string_t &other, const const_iterator &from) const -> size_type
{
  const size_type result = find_substring(other.data_, size_type(from.ptr - data_), other.size());
  return result == size() ? npos : result;
}



auto string_t::find_index(const char *str, const const_iterator &from) const -> size_type
{
  const size_type result = find_substring(str, size_type(from.ptr - data_), std::strlen(str));
  return result == size() ? npos : result;
}



auto string_t::find_index(const char *str, const const_iterator &from, size_type length) const -> size_type
{
  const size_type result = find_substring(str, size_type(from.ptr - data_), length);
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
  }

  const uint32_t *data_ptr = (const uint32_t *)(data_ + from);
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
  const char *remainder = (const char *)data_ptr;
  switch (count) {
  case 3: if (remainder[0] == ch) { return len - 3; } // fall-through
  case 2: if (remainder[1] == ch) { return len - 2; } // fall-through
  case 1: if (remainder[2] == ch) { return len - 1; } // fall-through
  case 0: // fall-through
  default: return len;
  }
}



static void gen_str_search_table(
  const char *str,
  string_t::size_type length,
  ptrdiff_t jumps[]
  )
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
  if (length == 1) {
    return find_char(*str, from);
  }

  const size_type len = size();

  assert(str);
  assert(from <= len);

  if (from >= len || from + length >= len) {
    return len;
  } else if (length == 0) {
    return from;
  }

  std::vector<ptrdiff_t> jumps(length);
  gen_str_search_table(str, length, jumps.data());

  ptrdiff_t data_index = from;
  ptrdiff_t str_index = 0;
  const size_type data_length = len;
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
