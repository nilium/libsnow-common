/*
 * Copyright Noel Cower 2014.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef __SNOW_COMMON__ALLOCATOR_HH__
#define __SNOW_COMMON__ALLOCATOR_HH__


#include <cstdlib>
#include <functional>
#include "align.hh"


namespace snow
{


/*

struct allocator
{
  // May be a struct or typedef, but must declare a deallocator function type.
  // The deallocator need only implement an operator () (void *). It may be
  // noexcept.
  typedef deallocator_type ...;

  // Must return a deallocator function type.
  deallocator_type deallocator();

  // Either a copy-constructor or a move-constructor must be supported by an
  // allocator.
  allocator(allocator const &) = required unless move is supported;
  allocator(allocator &&) = required unless copy is supported;

  void *allocate(size_t bytes);
  void deallocate(void *ptr) noexcept;
};

*/



template <typename Allocator>
struct simple_deallocator
{
  using allocator = Allocator;

  void operator () (void *const ptr) noexcept
  {
    Allocator{}.deallocate(ptr);
  }
};



template <typename Allocator>
struct bound_deallocator
{
  using allocator = Allocator;

  allocator &_alloc;

  void operator () (void *const ptr) noexcept
  {
    _alloc.deallocate(ptr);
  }
};



struct mallocator
{
  void *allocate(size_t const bytes) { return std::malloc(bytes); }
  void deallocate(void *const ptr) noexcept { std::free(ptr); }

  using deallocator_type = simple_deallocator<mallocator>;

  deallocator_type deallocator()
  {
    return deallocator_type {};
  }
};



template <size_t Align = 16>
struct aligned_mallocator
{
  static size_t const alignment = Align;

  void *allocate(size_t const bytes);
  void deallocate(void *const ptr) noexcept;

  using deallocator_type = simple_deallocator<aligned_mallocator>;

  deallocator_type deallocator();
};



template <size_t Align>
void aligned_mallocator<Align>::deallocate(void *const p) noexcept
{
  void *const actual = ((void **)p)[-1];
  std::free(actual);
}



template <size_t Align>
auto aligned_mallocator<Align>::deallocator() -> deallocator_type
{
  return deallocator_type {};
}



template <size_t Align>
void *aligned_mallocator<Align>::allocate(size_t const size_bytes)
{
  size_t const size_with_ptr = sizeof(void *) + size_bytes;
  size_t const aligned_size  = align(size_with_ptr, Align);

  void *const  block   = std::malloc(aligned_size);
  void **const wrapper = (void **)align(uintptr_t(block) + sizeof(void *), Align);

  wrapper[-1] = block;

  return wrapper;
}



template <>
struct aligned_mallocator<1> : public mallocator
{
  using mallocator::mallocator;
  static size_t const alignment = 1;
};



template <>
struct aligned_mallocator<2> : public mallocator
{
  using mallocator::mallocator;
  static size_t const alignment = 2;
};



template <>
struct aligned_mallocator<4> : public mallocator
{
  using mallocator::mallocator;
  static size_t const alignment = 4;
};



template <>
struct aligned_mallocator<8> : public mallocator
{
  using mallocator::mallocator;
  static size_t const alignment = 8;
};



template <>
struct aligned_mallocator<16> : public mallocator
{
  using mallocator::mallocator;
  static size_t const alignment = 16;
};


} // namespace snow


#endif /* end __ALLOCATOR_HH__ include guard */
