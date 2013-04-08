// object_pool.hh -- Noel Cower -- Public Domain
#ifndef __SNOW_COMMON__OBJECT_POOL_HH__
#define __SNOW_COMMON__OBJECT_POOL_HH__

#include <new>
#include <set>
#include <vector>
#include <functional>
#include <type_traits>
#include <mutex>


namespace snow {

/*
  OC requires emplace(iter, T), end(), size(), clear(), and operator[](index)
  IC requires insert(iter, T), end(), begin(), erase(iter), clear(), and empty()

  Basically, OC, the object container, could reasonably be a deque or vector.
  IC, the index container, could be a set, deque, list, or vector. Both default
  to vector because there's not much point in changing either.
*/
template <typename T, typename IT = size_t, bool THREADSAFE = true,
          template <class COT, class COA = std::allocator<COT>> class OC = std::vector,
          template <class CIT, class CIA = std::allocator<CIT>> class IC = std::vector>
struct object_pool_t
{
  struct store_t
  {
    char data[sizeof(T)];
  };

  using object_t            = T;
  using index_t             = IT;
  using objects_t           = OC<store_t>;
  using indices_t           = IC<index_t>;
  using object_iter_t       = std::function<void(object_t &, const index_t &)>;
  using const_object_iter_t = std::function<void(const object_t &, const index_t &)>;



  ~object_pool_t()
  {
    clear();
  }



  template <typename... ARGS>
  index_t reserve(ARGS&&... args)
  {
    index_t index = 0;
    lock_.lock();
    if (unused_indices_.empty()) {
      index = objects_.size();
      objects_.emplace(objects_.end());
    } else {
      auto index_begin = unused_indices_.begin();
      index = *index_begin;
      unused_indices_.erase(index_begin);
    }
    new(ptr_for_index(index)) object_t(args...);
    lock_.unlock();
    return index;
  }



  void collect(const index_t index)
  {
    lock_.lock();
    destroy(index);
    unused_indices_.insert(unused_indices_.end(), index);
    lock_.unlock();
  }



  std::pair<bool, index_t> index_of(const object_t &obj) const
  {
    std::pair<bool, index_t> result { false, 0 };
    lock_.lock();
    const object_t *addr = &obj;
    index_t index = 0;
    const auto length = objects_.size();
    for (; index < length; ++index) {
      if (ptr_for_index(index) == addr) {
        result = { true, index };
        break;
      }
    }
    lock_.unlock();
    return result;
  }



  object_t &at(const index_t index)
  {
    lock_.lock();
    object_t &ret = *ptr_for_index(index);
    lock_.unlock();
    return ret;
  }



  const object_t &at(const index_t index) const
  {
    lock_.lock();
    object_t &ret = *ptr_for_index(index);
    lock_.unlock();
    return ret;
  }



  object_t &operator [](const index_t index)
  {
    lock_.lock();
    object_t &ret = *ptr_for_index(index);
    lock_.unlock();
    return ret;
  }



  const object_t &operator [](const index_t index) const {
    lock_.lock();
    object_t &ret = *ptr_for_index(index);
    lock_.unlock();
    return ret;
  }



  void each_object(object_iter_t &iter)
  {
    lock_.lock();
    std::set<index_t> index_set(unused_indices_.begin(), unused_indices_.end());
    const auto index_term = index_set.cend();
    index_t index = 0;
    const auto size = objects_.size();
    for (; index < size; ++index) {
      if (index_set.find(index) != index_term) {
        iter(*ptr_for_index(index), index);
      }
    }
    lock_.unlock();
  }



  void each_object_const(const const_object_iter_t &iter) const
  {
    lock_.lock();
    std::set<index_t> index_set(unused_indices_.begin(), unused_indices_.end());
    const auto index_term = index_set.cend();
    index_t index = 0;
    const auto size = objects_.size();
    for (; index < size; ++index) {
      if (index_set.find(index) != index_term) {
        iter(*ptr_for_index(index), index);
      }
    }
    lock_.unlock();
  }



  void clear()
  {
    lock_.lock();
    destroy_all();
    objects_.clear();
    unused_indices_.clear();
    lock_.unlock();
  }



private:

  void destroy(index_t index)
  {
    ptr_for_index(index)->~object_t();
  }



  void destroy_all()
  {
    std::set<index_t> indices(unused_indices_.cbegin(), unused_indices_.cend());
    const auto length = objects_.size();
    const auto index_term = indices.cend();
    for (int index = 0; index < length; ++index) {
      if (indices.find(index) == index_term) {
        destroy(index);
      }
    }
  }



  object_t *ptr_for_index(index_t index)
  {
    return (object_t *)&objects_.at(index);
  }



  const object_t *ptr_for_index(index_t index) const
  {
    return (const object_t *)&objects_.at(index);
  }



  struct dummylock_t {
    void lock() {}
    void unlock() {}
  };


  using lock_t = typename std::conditional<THREADSAFE, std::mutex, dummylock_t>::type;


  objects_t   objects_;
  indices_t   unused_indices_;
  mutable lock_t lock_;

}; // struct object_pool_t

} // namespace snow

#endif /* end __SNOW_COMMON__OBJECT_POOL_HH__ include guard */
