// object_pool.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__OBJECT_POOL_HH__
#define __SNOW_COMMON__OBJECT_POOL_HH__

#include <functional>
#include <mutex>
#include <new>
#include <type_traits>
#include <unordered_set>
#include <vector>


namespace snow {


// Note: because this is backed by a vector, you _should not_ store pointers or
// references to objects in the pool. Use the indices to access the objects when
// you need them.
template <typename T, typename IT = size_t, bool THREADSAFE = true>
struct object_pool_t
{
  using store_t             = typename std::aligned_storage<sizeof(T)>::type;
  using object_t            = T;
  using index_t             = IT;
  using objects_t           = std::vector<store_t>;
  using indices_t           = std::unordered_set<index_t>;
  using object_iter_t       = std::function<void(object_t &, const index_t &)>;
  using const_object_iter_t = std::function<void(const object_t &, const index_t &)>;



/*==============================================================================
  dtor

    Destroys the pool and calls the destructor for any uncollected objects in
    the pool.
==============================================================================*/
  ~object_pool_t()
  {
    clear();
  }



/*==============================================================================
  reserve

    Attempts to reserve enough memory for the given number of objects.
==============================================================================*/
  void reserve(size_t num_objects)
  {
    objects_.reserve(num_objects);
  }



/*==============================================================================
  reserve

    Reserves an object in the pool. Arguments correspond to whatever constructor
    of the pool's object type they match. Returns the object's index.
==============================================================================*/
  template <typename... ARGS>
  index_t reserve(ARGS&&... args)
  {
    index_t index = 0;
    std::lock_guard<lock_t> lock(lock_);
    if (unused_indices_.empty()) {
      index = objects_.size();
      objects_.emplace_back();
    } else {
      auto index_begin = unused_indices_.cbegin();
      index = *index_begin;
      unused_indices_.erase(index_begin);
    }
    object_t *store = ptr_for_index(index);
    new(store) object_t(std::forward<ARGS>(args)...);
    return index;
  }



/*==============================================================================
  collect

    Collects an object's index, calls its dtor, and prepares it to be reused
    later.
==============================================================================*/
  void collect(const index_t index)
  {
    std::lock_guard<lock_t> lock(lock_);
    throw_unallocated(index);
    destroy(index);
    unused_indices_.insert(index);
  }



/*==============================================================================
  index_of

    Gets the index of an object, if it's in the pool. Only tests for whether the
    object address matches that of one in the pool.
==============================================================================*/
  std::pair<bool, index_t> index_of(const object_t &obj) const
  {
    std::pair<bool, index_t> result { false, 0 };
    std::lock_guard<lock_t> lock(lock_);
    const object_t *addr = &obj;
    index_t index = 0;
    const auto length = objects_.size();
    for (; index < length; ++index) {
      if (ptr_for_index(index) == addr) {
        result = { true, index };
        break;
      }
    }
    return result;
  }



/*==============================================================================
  at

    Gets an object at the given index. Will throw an exception if the index
    doesn't match an allocated object.
==============================================================================*/
  object_t &at(const index_t index)
  {
    std::lock_guard<lock_t> lock(lock_);
    throw_unallocated(index);
    object_t &ret = *ptr_for_index(index);
    return ret;
  }



  const object_t &at(const index_t index) const
  {
    std::lock_guard<lock_t> lock(lock_);
    throw_unallocated(index);
    object_t &ret = *ptr_for_index(index);
    return ret;
  }



/*==============================================================================
  operator []

    See 'at'
==============================================================================*/
  object_t &operator [](const index_t index)
  {
    std::lock_guard<lock_t> lock(lock_);
    throw_unallocated(index);
    object_t &ret = *ptr_for_index(index);
    return ret;
  }



  const object_t &operator [](const index_t index) const
  {
    std::lock_guard<lock_t> lock(lock_);
    throw_unallocated(index);
    object_t &ret = *ptr_for_index(index);
    return ret;
  }



/*==============================================================================
  each_object

    Calls the object iter function for each allocated object in the pool. The
    iter function must not cause any objects other than the one currently being
    evaluated to be collected. If possible, just don't try to collect stuff in
    this loop.
==============================================================================*/
  void each_object(object_iter_t &iter)
  {
    std::lock_guard<lock_t> lock(lock_);
    const auto index_term = unused_indices_.cend();
    index_t index = 0;
    const auto size = objects_.size();
    for (; index < size; ++index) {
      if (unused_indices_.find(index) != index_term) {
        iter(*ptr_for_index(index), index);
      }
    }
  }



/*==============================================================================
  each_object_const

    Same as each_object. Again, objects should not be collected while the iter
    function is running unless you're very careful.
==============================================================================*/
  void each_object_const(const const_object_iter_t &iter) const
  {
    std::lock_guard<lock_t> lock(lock_);
    const auto index_term = unused_indices_.cend();
    index_t index = 0;
    const auto size = objects_.size();
    for (; index < size; ++index) {
      if (unused_indices_.find(index) != index_term) {
        iter(*ptr_for_index(index), index);
      }
    }
  }



/*==============================================================================
  clear

    Wipes out all objects in the collection. Does call the dtor on each object
    in the pool. This does not assure anything with respect to memory usage.
==============================================================================*/
  void clear()
  {
    std::lock_guard<lock_t> lock(lock_);
    destroy_all();
    objects_.clear();
    unused_indices_.clear();
  }



private:

/*==============================================================================
  destroy

    Destroys an object at the given index. Performs basic bounds-checking, but
    does not check to see if the object has been allocated.
==============================================================================*/
  void destroy(index_t index)
  {
    ptr_for_index(index)->~object_t();
  }



/*==============================================================================
  destroy_all

    Destroys all allocated objects in the pool.
==============================================================================*/
  void destroy_all()
  {
    const auto length = objects_.size();
    const auto index_term = unused_indices_.cend();
    for (int index = 0; index < length; ++index) {
      if (unused_indices_.find(index) == index_term) {
        destroy(index);
      }
    }
  }



/*==============================================================================
  throw_unallocated

    Throws an out_of_range exception if the given index isn't already allocated.
==============================================================================*/
  void throw_unallocated(index_t index) const
  {
    if (unused_indices_.find(index) != unused_indices_.cend()) {
      throw std::out_of_range("Index is out of range");
    }
  }



/*==============================================================================
  ptr_for_index

    Returns a pointer to the object at the given index. No guarantee it has
    been allocated, though the vector will throw an exception if the index is
    out of range.
==============================================================================*/
  object_t *ptr_for_index(index_t index)
  {
    return (object_t *)&objects_.at(index);
  }



/*==============================================================================
  ptr_for_index

    Const version of above ptr_for_index.
==============================================================================*/
  const object_t *ptr_for_index(index_t index) const
  {
    return (const object_t *)&objects_.at(index);
  }



/*==============================================================================

  Dummy lock type that implements the BasicLockable concept. lock and unlock are
  no-ops.

==============================================================================*/
  struct dummylock_t {
    void lock() {}
    void unlock() {}
  };


  using lock_t = typename std::conditional<THREADSAFE, std::recursive_mutex, dummylock_t>::type;


  objects_t         objects_;
  indices_t         unused_indices_;
  mutable lock_t    lock_;

}; // struct object_pool_t

} // namespace snow

#endif /* end __SNOW_COMMON__OBJECT_POOL_HH__ include guard */
