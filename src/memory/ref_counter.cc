/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#include <snow/memory/ref_counter.hh>


namespace snow {


namespace {

using lock_guard = std::lock_guard<std::recursive_mutex>;

}



void ref_counter_t::clear()
{
  lock_guard guard { lock_ };
  retained_.clear();
}



void ref_counter_t::retain_object_locked(const void *obj)
{
  lock_guard guard { lock_ };
  auto iter = retained_.find(obj);
  if (iter != retained_.end()) {
    iter->second += 1;
  } else {
    retained_.emplace(obj, 1);
  }
}



bool ref_counter_t::release_object_locked(void *obj, finalizer_t<void> finalize)
{
  if (finalize == nullptr) {
    return release_object_locked(obj, nullptr);
  } else {
    lock_guard guard { lock_ };
    auto iter = retained_.find(obj);
    if (iter == retained_.end()) {
      finalize(obj);
      return true;
    } else {
      iter->second -= 1;
      if (iter->second == 0) {
        retained_.erase(iter);
      }
      return false;
    }
  }
}



bool ref_counter_t::release_object_locked(void *obj, std::nullptr_t np)
{
  lock_guard guard { lock_ };
  auto iter = retained_.find(obj);
  if (iter == retained_.end()) {
    return true;
  } else {
    iter->second -= 1;
    if (iter->second == 0) {
      retained_.erase(iter);
    }
    return false;
  }
}



uint_fast32_t ref_counter_t::object_retain_count_locked(const void *obj) const
{
  lock_guard guard { lock_ };
  uint_fast32_t count = 1;
  auto iter = retained_.find(obj);
  if (iter != retained_.end()) {
    count += iter->second;
  }
  return count;
}


} // namespace snow
