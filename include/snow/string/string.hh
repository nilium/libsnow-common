// string.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__STRING_HH__
#define __SNOW_COMMON__STRING_HH__

#include <snow/config.hh>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <ostream>
#include <string>
#include <type_traits>


namespace snow {


struct string_t;


std::ostream &operator << (std::ostream &out, const string_t &in);


template <bool is_const, ptrdiff_t ptr_step>
struct string_iter_t
{
  using value_type = char;
  using pointer = typename std::conditional<
    is_const, const value_type *, value_type *>::type;
  using reference = typename std::conditional<
    is_const, value_type, value_type &>::type;
  using difference_type = ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;


  string_iter_t() = default;
  explicit string_iter_t(pointer ptr_) : ptr(ptr_) { /* nop */ }

  reference operator * ()
  {
    return *ptr;
  }

  pointer operator -> ()
  {
    return ptr;
  }

  string_iter_t &operator ++ ()
  {
    ptr += ptr_step;
    return *this;
  }

  string_iter_t  operator ++ (int dummy)
  {
    string_iter_t result = *this;
    ++ *this;
    return result;
  }

  string_iter_t &operator -- ()
  {
    ptr -= ptr_step;
    return *this;
  }

  string_iter_t  operator -- (int dummy)
  {
    string_iter_t result = *this;
    -- *this;
    return result;
  }

  string_iter_t  operator +  (difference_type delta) const
  {
    string_iter_t result = *this;
    result.ptr += delta;
    return result;
  }

  string_iter_t &operator += (difference_type delta)
  {
    ptr += delta;
    return *this;
  }

  string_iter_t  operator -  (difference_type delta) const
  {
    string_iter_t result = *this;
    result.ptr -= delta;
    return result;
  }

  string_iter_t &operator -= (difference_type delta)
  {
    ptr -= delta;
    return *this;
  }

  template <bool other_const, ptrdiff_t step>
  bool operator == (const string_iter_t<other_const, step> &rhs) const
  {
    return ptr == rhs.ptr;
  }

  template <bool other_const, ptrdiff_t step>
  bool operator != (const string_iter_t<other_const, step> &rhs) const
  {
    return ptr != rhs.ptr;
  }

  template <bool other_const, ptrdiff_t step>
  bool operator <  (const string_iter_t<other_const, step> &rhs) const
  {
    return ptr < rhs.ptr;
  }

  template <bool other_const, ptrdiff_t step>
  bool operator >  (const string_iter_t<other_const, step> &rhs) const
  {
    return ptr > rhs.ptr;
  }

  template <bool other_const, ptrdiff_t step>
  bool operator <= (const string_iter_t<other_const, step> &rhs) const
  {
    return ptr <= rhs.ptr;
  }

  template <bool other_const, ptrdiff_t step>
  bool operator >= (const string_iter_t<other_const, step> &rhs) const
  {
    return ptr >= rhs.ptr;
  }

  operator string_iter_t<true, ptr_step> () const
  {
    string_iter_t<true, ptr_step> result;
    result.ptr = ptr;
    return result;
  }

  operator pointer () { return ptr; }

  pointer ptr = nullptr;
};



template <bool lconst, ptrdiff_t ldiff, bool rconst, ptrdiff_t rdiff>
ptrdiff_t operator - (const string_iter_t<lconst, ldiff> &lc, const string_iter_t<rconst, rdiff> &rc)
{
  return lc.ptr - rc.ptr;
}



struct string_t
{
  using value_type = char; // May not change.
  using size_type = size_t;
  using iterator = string_iter_t<false, 1>;
  using const_iterator = string_iter_t<true, 1>;
  using reverse_iterator = string_iter_t<false, -1>;
  using const_reverse_iterator = string_iter_t<true, -1>;


  static const size_type npos = ~0;


  string_t();
  string_t(const std::string &other);
  string_t(const const_iterator &from, const const_iterator &to);
  string_t(const char *zstr);
  string_t(const char *zstr, size_type length);
  string_t(string_t &&other);
  string_t(const string_t &other);
  string_t(std::initializer_list<char> init);
  // Note: this constructor allows you to create a string with no null
  // terminating character, but only if nofree is true. If nofree is false,
  // these are equivalent to string_t(const char * [, size_type]);
  // In no case will the original zstr pointer be freed.
  // If a string with nofree is resized or reserve is called, it will copy the
  // string it was pointing to into a new buffer and perform the resize/reserve
  // with that. Afterward, it will be in its own memory on the heap.
  explicit string_t(char *zstr, size_type length, bool nofree);
  explicit string_t(const iterator &from, const iterator &to, bool nofree);

  ~string_t();

  string_t &operator = (string_t &&other);
  string_t &operator = (const string_t &other);
  string_t &operator = (const std::string &other);
  string_t &operator = (const char *zstr);

  string_t &assign(const char *zstr, size_type length);

  int compare(const string_t &other) const;

  void append(char ch);
  void append(const char *zstr);
  void append(const char *zstr, size_type length);
  void append(const string_t &str);
  void append(const const_iterator &from, const const_iterator &to);

  void push_back(char ch);
  void pop_back();

  void erase(size_type from = 0, size_type count = npos);
  void erase(const const_iterator &pos);
  void erase(const const_iterator &from, const const_iterator &to);

  // If shrinking the string, there is no guarantee that the capacity of the
  // string will change. If growing the array, the new characters will be
  // garbage data except for adding a null character at the end of the string.
  void clear();
  void resize(size_type);
  size_type size() const;
  bool empty() const;

  void shrink_to_fit();
  void reserve(size_type);
  size_type capacity() const;


  // Note: negative indices return characters relative to the back of the string.
  char &operator [] (ptrdiff_t index);
  char operator [] (ptrdiff_t index) const;

  char &at(ptrdiff_t index);
  char at(ptrdiff_t index) const;

  char &front();
  char front() const;

  char &back();
  char back() const;

  size_type index_of(const iterator &iter) const;
  size_type index_of(const const_iterator &iter) const;
  size_type index_of(const reverse_iterator &iter) const;
  size_type index_of(const const_reverse_iterator &iter) const;

  string_t substr(size_type pos, size_type count = npos) const;
  string_t substr(const const_iterator &from) const;
  string_t substr(const const_iterator &from, const const_iterator &to) const;

  // Window strings work the same as passing some portion of a string's data
  // to a nofree constructor (with nofree true). If the originating string is
  // modified is resized or its capacity or contents change, this string may
  // be invalid.
  string_t window(size_type pos, size_type count);
  string_t window(const iterator &from);
  string_t window(const iterator &from, const iterator &to);

  char *c_str();
  const char *c_str() const;

  char *data();
  const char *data() const;

  const_iterator cbegin() const;
  const_iterator cend() const;

  iterator begin();
  iterator end();

  const_iterator begin() const;
  const_iterator end() const;

  const_reverse_iterator crbegin() const;
  const_reverse_iterator crend() const;

  reverse_iterator rbegin();
  reverse_iterator rend();

  const_reverse_iterator rbegin() const;
  const_reverse_iterator rend() const;

  /** The many varieties of find **/
  iterator find(char ch, size_type from = 0);
  iterator find(const string_t &other, size_type from = 0);
  iterator find(const char *str, size_type from = 0);
  iterator find(const char *str, size_type from, size_type length);
  const_iterator find(char ch, size_type from = 0) const;
  const_iterator find(const string_t &other, size_type from = 0) const;
  const_iterator find(const char *str, size_type from = 0) const;
  const_iterator find(const char *str, size_type from, size_type length) const;

  iterator find(char ch, const const_iterator &from);
  iterator find(const string_t &other, const const_iterator &from);
  iterator find(const char *str, const const_iterator &from);
  iterator find(const char *str, const const_iterator &from, size_type length);
  const_iterator find(char ch, const const_iterator &from) const;
  const_iterator find(const string_t &other, const const_iterator &from) const;
  const_iterator find(const char *str, const const_iterator &from) const;
  const_iterator find(const char *str, const const_iterator &from, size_type length) const;

  size_type find_index(char ch, size_type from = 0) const;
  size_type find_index(const string_t &other, size_type from = 0) const;
  size_type find_index(const char *str, size_type from = 0) const;
  size_type find_index(const char *str, size_type from, size_type length) const;
  size_type find_index(char ch, const const_iterator &from) const;
  size_type find_index(const string_t &other, const const_iterator &from) const;
  size_type find_index(const char *str, const const_iterator &from) const;
  size_type find_index(const char *str, const const_iterator &from, size_type length) const;

  bool has_suffix(const string_t &str) const;
  bool has_suffix(const char *zstr) const;
  bool has_suffix(const char *zstr, size_type length) const;

  bool has_prefix(const string_t &str) const;
  bool has_prefix(const char *zstr) const;
  bool has_prefix(const char *zstr, size_type length) const;

  char *operator * ();
  const char *operator * () const;

  operator char * ();
  operator const char * () const;

  template <size_type N>
  bool operator == (const char str[N]) const;
  bool operator == (const char *zstr) const;
  bool operator == (const string_t &other) const;
  bool operator != (const string_t &other) const;
  template <size_type N>
  bool operator != (const char str[N]) const;
  bool operator != (const char *zstr) const;
  bool operator >  (const string_t &other) const;
  bool operator <  (const string_t &other) const;
  bool operator >= (const string_t &other) const;
  bool operator <= (const string_t &other) const;

  string_t operator + (const string_t &rhs) const;
  string_t &operator += (const string_t &rhs);

private:
  size_type find_char(char ch, size_type from) const;
  size_type find_substring(const char *str, size_type from, size_type length) const;

  struct long_data_t
  {
    size_type length_;
    size_type capacity_;
  };

  // hack
  struct short_data_dummy_t
  {
    uint8_t length_;
    char short_data_[1];
  };

  enum : size_type
  {
    short_data_len_ = sizeof(long_data_t) - offsetof(short_data_dummy_t, short_data_)
  };

  struct short_data_t
  {
    uint8_t length_;
    char short_data_[short_data_len_];
  };

  char *data_ = rep_.short_.short_data_;
  union {
    long_data_t long_;
    short_data_t short_;
  } rep_;

};



template <string_t::size_type N>
bool string_t::operator == (const char str[N]) const
{
  const size_type len = size();
  return std::strncmp(data_, str, N > len ? len : N) == 0;
}



template <string_t::size_type N>
bool string_t::operator != (const char str[N]) const
{
  const size_type len = size();
  return std::strncmp(data_, str, N > len ? len : N) != 0;
}


} // namespace snow


#endif /* end __SNOW_COMMON__STRING_HH__ include guard */
