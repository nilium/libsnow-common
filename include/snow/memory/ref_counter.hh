// ref_counter.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__REF_COUNTER_HH__
#define __SNOW_COMMON__REF_COUNTER_HH__

#include <snow/config.hh>
#include <cassert>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>


namespace snow {


struct S_EXPORT ref_counter_t
{
    template <typename T>
    using finalizer_t = void (*)(T *object);
    using retain_map_t = std::map<const void *, uint_fast32_t>;

    template <typename T>
    static void default_finalizer(T *object)
    {
      if (object != nullptr)
        delete object;
      else
        s_throw(std::invalid_argument, "Attempt to default-finalize nullptr");
    }

    template <typename T>
    void  retain(const T *object);

    template <typename T>
    void  release(T *object, finalizer_t<T> finalize = default_finalizer<T>);

    template <typename T>
    auto  retain_count(const T *object) const -> uint_fast32_t;

private:
  retain_map_t                  retained_;
  mutable std::recursive_mutex  lock_;

  void  retain_object_locked(const void *obj);
  void  release_object_locked(void *obj, finalizer_t<void> finalize);
  int   object_retain_count_locked(const void *obj) const;

}; // struct ref_counter_t


template <typename T>
void  ref_counter_t::retain(const T *object)
{
  if (object == NULL)
    s_throw(std::invalid_argument, "Attempt to retain nullptr");

  retain_object_locked((void *)object);
}



template <typename T>
void  ref_counter_t::release(T *object, finalizer_t<T> finalize)
{
  if (object == nullptr)
    s_throw(std::invalid_argument, "Attempt to release nullptr");

  release_object_locked(object, (finalizer_t<void>)finalize);
}



template <typename T>
auto  ref_counter_t::retain_count(const T *object) const -> uint_fast32_t
{
  if (object == nullptr)
    return 0;

  return object_retain_count_locked(object);
}


} // namespace snow

#endif /* end __SNOW_COMMON__REF_COUNTER_HH__ include guard */
