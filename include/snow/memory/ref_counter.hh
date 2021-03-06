/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#pragma once

#include <snow/config.hh>
#include <cassert>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>


namespace snow {


struct
  S_EXPORT
  S_DEPRECATED("ref_counter_t is unsafe and will be removed shortly")
  ref_counter_t
{
    template <typename T>
    using finalizer_t = void (*)(T *object);
    using retain_map_t = std::map<const void *, uint_fast32_t>;

    template <typename T>
    auto  retain(T *object) -> T *;

    template <typename T>
    bool  release(T *object, finalizer_t<T> finalize);

    template <typename T>
    bool  release(T *object);

    template <typename T>
    auto  retain_count(const T *object) const -> uint_fast32_t;

    // Clears any retained objects from the ref counter. Does not attempt to
    // finalize them in any way -- assumes this has already been forced on the
    // objects in the counter.
    void clear();

private:
  retain_map_t                  retained_;
  mutable std::recursive_mutex  lock_;

  void  retain_object_locked(const void *obj);
  bool  release_object_locked(void *obj, finalizer_t<void> finalize);
  bool  release_object_locked(void *obj, std::nullptr_t);
  auto  object_retain_count_locked(const void *obj) const -> uint_fast32_t;

}; // struct ref_counter_t


template <typename T>
auto  ref_counter_t::retain(T *object) -> T *
{
  if (object == NULL) {
    s_throw(std::invalid_argument, "Attempt to retain nullptr");
  }

  retain_object_locked(reinterpret_cast<void *>(object));

  return object;
}



template <typename T>
bool  ref_counter_t::release(T *object, finalizer_t<T> finalize)
{
  if (object == nullptr) {
    s_throw(std::invalid_argument, "Attempt to release nullptr");
  }

  return release_object_locked(object, reinterpret_cast<finalizer_t<void>>(finalize));
}



template <typename T>
bool  ref_counter_t::release(T *object)
{
  if (object == nullptr) {
    s_throw(std::invalid_argument, "Attempt to release nullptr");
  }

  return release_object_locked(object, nullptr);
}



template <typename T>
auto  ref_counter_t::retain_count(const T *object) const -> uint_fast32_t
{
  if (object == nullptr) {
    return 0;
  }

  return object_retain_count_locked(object);
}


} // namespace snow
