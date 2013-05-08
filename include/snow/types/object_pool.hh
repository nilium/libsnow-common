// object_pool.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__OBJECT_POOL_HH__
#define __SNOW_COMMON__OBJECT_POOL_HH__

#include <snow/config.hh>
#include <cstdint>
#include <functional>
#include <iterator>
#include <mutex>
#include <new>
#include <type_traits>
#include <vector>


namespace snow {


// Note: because this is backed by a vector, you _should not_ store pointers or
// references to objects in the pool. Use the indices to access the objects when
// you need them. In addition, you can iterate over all allocated objects if
// you need to, so there's that.
template <typename T, typename IT = size_t, bool THREADSAFE = true>
struct object_pool_t
{
  struct store_t {
    using data_t = typename std::aligned_storage<sizeof(T)>::type;

    uint8_t     used = 0;
    data_t      data;
  };

  using object_t            = T;
  using index_t             = IT;
  using objects_t           = std::vector<store_t>;
  using object_iter_t       = std::function<void(object_t &, const index_t &)>;
  using const_object_iter_t = std::function<void(const object_t &, const index_t &)>;


  struct const_iterator : public std::iterator<object_t, std::input_iterator_tag>
  {
    const_iterator() = default;

    const object_t *operator -> () const
    {
      return (object_t *)&iter_->data;
    }

    const object_t &operator * () const
    {
      return *(object_t *)&iter_->data;
    }

    const_iterator &operator ++ ()
    {
      do {
        ++iter_;
      } while (iter_ != end_ && !iter_->used);
      return *this;
    }

    const_iterator operator ++ (int dummy)
    {
      const_iterator cur = *this;
      ++*this;
      return cur;
    }

    bool operator == (const const_iterator &other) const
    {
      return iter_ == other.iter_;
    }

    bool operator != (const const_iterator &other) const
    {
      return iter_ != other.iter_;
    }

  protected:
    friend struct object_pool_t;
    using base_iter = typename objects_t::iterator;

    const_iterator(base_iter iter, base_iter end) :
      iter_(iter),
      end_(end)
    {
      /* nop */
    }

    base_iter iter_;
    base_iter end_;
  };



  struct iterator : public const_iterator
  {
    iterator() = default;

    object_t *operator -> ()
    {
      return (object_t *)&(const_iterator::iter_->data);
    }

    object_t &operator * ()
    {
      return *(object_t *)&(const_iterator::iter_->data);
    }

    iterator &operator ++ ()
    {
      const_iterator::operator++();
      return *this;
    }

    iterator operator ++ (int)
    {
      iterator cur = *this;
      const_iterator::operator++();
      return cur;
    }

  protected:
    friend struct object_pool_t;
    using base_iter = typename const_iterator::base_iter;

    iterator(base_iter iter, base_iter end) :
      const_iterator(iter, end)
    {
      /* nop */
    }
  };



  iterator begin()
  {
    auto iter = objects_.begin();
    auto iter_end = objects_.end();
    while (!iter->used && iter != iter_end) {
      ++iter;
    }
    return iterator(iter, iter_end);
  }



  const_iterator begin() const
  {
    auto iter = objects_.begin();
    auto iter_end = objects_.end();
    while (!iter->used && iter != iter_end) {
      ++iter;
    }
    return iterator(iter, iter_end);
  }



  const_iterator cbegin() const
  {
    auto iter = objects_.begin();
    auto iter_end = objects_.end();
    while (!iter->used && iter != iter_end) {
      ++iter;
    }
    return iterator(iter, iter_end);
  }



  iterator end()
  {
    return iterator(objects_.end(), objects_.end());
  }



  const_iterator end() const
  {
    return iterator(objects_.end(), objects_.end());
  }



  const_iterator cend() const
  {
    return iterator(objects_.end(), objects_.end());
  }



  object_pool_t()
  {
    /* nop */
  }



  object_pool_t(size_t reserved)
  {
    reserve(reserved);
  }



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
  allocate

    Reserves an object in the pool and constructs it. Arguments correspond to
    whatever constructor of the pool's object type they match. Returns the
    object's index.
==============================================================================*/
  template <typename... ARGS>
  index_t allocate(ARGS&&... args)
  {
    const index_t index = make_storage();
    new(&objects_[index].data) object_t(std::forward<ARGS>(args)...);
    return index;
  }



/*==============================================================================
  make_storage

    Reserves an object in the pool. Does not construct the object at all,
    simply returns its index.
==============================================================================*/
  index_t make_storage()
  {
    index_t index;
    std::lock_guard<lock_t> lock(lock_);
    index = next_unused_;
    const size_t length = objects_.size();
    for (; index < length; ++index) {
      if (!objects_[index].used) {
        goto construct_object_at_index;
      }
    }
    next_unused_ = (index = index_t(length)) + 1;
    objects_.emplace_back();
construct_object_at_index:
    objects_[index].used = 1;
    return index;
  }



/*==============================================================================
  destroy

    Collects an object's index, calls its dtor, and prepares it to be reused
    later.
==============================================================================*/
  void destroy(const index_t index)
  {
    std::lock_guard<lock_t> lock(lock_);
    throw_unallocated(index);

    store_t &store = objects_[index];
    store.used = 0;

    if (!std::is_pod<object_t>::value) {
      ((object_t *)&store.data)->~object_t();
    }

    if (next_unused_ > index) {
      next_unused_ = index;
    }
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
    index_t index = 0;
    const size_t size = objects_.size();
    for (; index < size; ++index) {
      store_t &store = objects_[index];
      if (store.used) {
        iter(*(object_t *)&store.data, index);
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
    index_t index = 0;
    const size_t size = objects_.size();
    for (; index < size; ++index) {
      store_t &store = objects_[index];
      if (store.used) {
        iter(*(object_t *)&store.data, index);
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
    if (!std::is_pod<object_t>::value) {
      destroy_all_nolock();
    }
    objects_.clear();
    next_unused_ = 0;
  }



/*==============================================================================
  size

    Returns the total number of allocated objects in the pool.
==============================================================================*/
  size_t size() const
  {
    std::lock_guard<lock_t> lock(lock_);
    size_t count = 0;
    index_t index = 0;
    const size_t size = objects_.size();
    for (; index < size; ++index) {
      store_t &store = objects_[index];
      if (store.used) {
        ++count;
      }
    }
    return count;
  }



private:

/*==============================================================================
  destroy

    Destroys an object at the given index. Performs basic bounds-checking, but
    does not check to see if the object has been allocated.
==============================================================================*/
  void destroy_nolock(index_t index)
  {
    ptr_for_index(index)->~object_t();
  }



/*==============================================================================
  destroy_all_nolock

    Destroys all allocated objects in the pool.
==============================================================================*/
  void destroy_all_nolock()
  {
    const auto length = objects_.size();
    for (int index = 0; index < length; ++index) {
      store_t &store = objects_[index];
      if (store.used) {
        store.used = 0;
        ((object_t *)&store.data)->~object_t();
      }
    }
    next_unused_ = 0;
  }



/*==============================================================================
  throw_unallocated

    Throws an out_of_range exception if the given index isn't already allocated.
==============================================================================*/
  void throw_unallocated(index_t index) const
  {
    if (!objects_.at(index).used) {
      s_throw(std::out_of_range, "Index is out of range");
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
    return (object_t *)&objects_.at(index).data;
  }



/*==============================================================================
  ptr_for_index

    Const version of above ptr_for_index.
==============================================================================*/
  const object_t *ptr_for_index(index_t index) const
  {
    return (const object_t *)&objects_.at(index).data;
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
  index_t           next_unused_ = ~0;
  mutable lock_t    lock_;

}; // struct object_pool_t

} // namespace snow

#endif /* end __SNOW_COMMON__OBJECT_POOL_HH__ include guard */
