// iterator_iterable.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__ITERATOR_ITERABLE_HH__
#define __SNOW_COMMON__ITERATOR_ITERABLE_HH__

#include <iterator>

namespace snow {

template <typename T>
class S_EXPORT iterable_iterator_t :
  public std::iterator<std::forward_iterator_tag, T>
{
public:
  typedef T iterator;

private:
  iterator iterator_;

public:
  iterable_iterator_t(const iterator &iter) :
    iterator_(iter)
  {}

  auto operator * () -> iterator&
  {
    return iterator_;
  }

  auto operator * () const -> const iterator&
  {
    return iterator_;
  }

  auto operator -> () -> iterator&
  {
    return iterator_;
  }

  auto operator -> () const -> const iterator&
  {
    return iterator_;
  }

  auto operator ++ () -> iterable_iterator_t&
  {
    ++iterator_;
    return *this;
  }

  auto operator ++ (int dummy) -> iterable_iterator_t
  {
    iterable_iterator_t temp = *this;
    ++iterator_;
    return temp;
  }

  auto equal(const iterable_iterator_t &rhs) const -> bool
  {
    return iterator_ == rhs.iterator_;
  }
};

template <typename T>
bool operator == (const iterable_iterator_t<T> &lhs,
                  const iterable_iterator_t<T> &rhs)
{
  return lhs.equal(rhs);
}

template <typename T>
bool operator != (const iterable_iterator_t<T> &lhs,
                  const iterable_iterator_t<T> &rhs)
{
  return ! lhs.equal(rhs);
}

template <typename T>
class S_EXPORT iterator_iterable_t
{
public:
  typedef T inner_iterator;
  typedef iterable_iterator_t<inner_iterator> iterator;

private:
  inner_iterator begin_, end_;

public:
  iterator_iterable_t(const inner_iterator &begin, const inner_iterator &end) :
    begin_(begin), end_(end)
  {}

  auto begin() const -> iterator
  {
    return iterator(begin_);
  }

  auto end() const -> iterator
  {
    return iterator(end_);
  }
};

template <typename T>
auto iterate(T &container) -> iterator_iterable_t<decltype(container.begin())>
{
  return iterator_iterable_t<decltype(container.begin())>(container.begin(), container.end());
}

template <typename T>
auto iterate_const(const T &container) -> iterator_iterable_t<decltype(container.cbegin())>
{
  return iterator_iterable_t<decltype(container.cbegin())>(container.cbegin(), container.cend());
}

template <typename T>
auto iterate_reverse(T &container) -> iterator_iterable_t<decltype(container.rbegin())>
{
  return iterator_iterable_t<decltype(container.rbegin())>(container.rbegin(), container.rend());
}

template <typename T>
auto iterate_reverse_const(const T &container) -> iterator_iterable_t<decltype(container.crbegin())>
{
  return iterator_iterable_t<decltype(container.crbegin())>(container.crbegin(), container.crend());
}


}

#endif /* end __SNOW_COMMON__ITERATOR_ITERABLE_HH__ include guard */
