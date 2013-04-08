// ref_counter.cc -- Noel Cower -- Public Domain

#include <snow/memory/ref_counter.hh>


namespace snow {


void ref_counter_t::retain_object_locked(const void *obj)
{
  lock_.lock();
  auto iter = retained_.find(obj);
  if (iter != retained_.end()) {
    iter->second += 1;
  } else {
    retained_.emplace(obj, 1);
  }
  lock_.unlock();
}



void ref_counter_t::release_object_locked(void *obj, finalizer_t<void> finalize)
{
  assert(finalize != nullptr);

  lock_.lock();
  auto iter = retained_.find(obj);
  if (iter == retained_.end()) {
    finalize(obj);
    lock_.unlock();
  } else {
    iter->second -= 1;
    if (iter->second == 0) {
      retained_.erase(iter);
    }
    lock_.unlock();
  }
}



int ref_counter_t::object_retain_count_locked(const void *obj) const
{
  uint_fast32_t count = 1;
  lock_.lock();
  auto iter = retained_.find(obj);
  if (iter != retained_.end())
    count += iter->second;
  lock_.unlock();
  return count;
}


} // namespace snow
