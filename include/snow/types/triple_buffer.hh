// triple_buffer.hh -- Noel Cower -- Public Domain
// Based on the macros by Remis at http://remis-thoughts.blogspot.com/2012/01/triple-buffering-as-concurrency_30.html

#ifndef __SNOW_COMMON__TRIPLEBUFFER_HH__
#define __SNOW_COMMON__TRIPLEBUFFER_HH__ 1

#include <array>
#include <atomic>
#include <cstdint>
#include <type_traits>

namespace snow
{

/*==============================================================================

  A triple buffer for synchronization purposes. Buffer swaps are lock-free.

==============================================================================*/
template <class T>
struct triple_buffer_t
{
  typedef T buffer_type;

  static_assert( ! std::is_reference<buffer_type>::value,
                "buffer_type to triple_buffer_t cannot be a reference type");
  static_assert((std::is_default_constructible<buffer_type>::value ||
                 std::is_copy_constructible<buffer_type>::value ||
                 std::is_copy_assignable<buffer_type>::value ||
                 std::is_pod<buffer_type>::value),
                "buffer_type must provide a default constructor, copy"
                " constructor, be copy-assignable, or a POD type");

private:

  typedef uint_fast8_t index_t;
  typedef std::atomic<index_t> indices_t;

  static const unsigned int INDEX_BITS = 2;
  static const unsigned int BACK_MASK = 0x3;
  static const unsigned int MIDDLE_MASK = BACK_MASK << INDEX_BITS;
  static const unsigned int FRONT_MASK = MIDDLE_MASK << INDEX_BITS;
  static const unsigned int BACK_OFFSET = 0;
  static const unsigned int MIDDLE_OFFSET = INDEX_BITS;
  static const unsigned int FRONT_OFFSET = INDEX_BITS << 1;
  static const unsigned int DEFAULT_INDICES = ((0x2 << INDEX_BITS) | 0x1) << INDEX_BITS;

  // Pointers to each buffer
  indices_t indices_;
  std::array<buffer_type, 3> buffers_;

public:
  /*============================================================================
    triple_buffer_t(void)

      Constructs a triple_buffer_t with default values for its backing buffers.
  ============================================================================*/
  triple_buffer_t(void) :
    indices_(DEFAULT_INDICES), buffers_()
  {
    static_assert((std::is_default_constructible<buffer_type>::value ||
                   std::is_pod<buffer_type>::value),
                  "buffer_type has no default constructor - provide an initial value");
  }

  /*============================================================================
    triple_buffer_t(init_value)

      Constructs a triple_buffer_t using init_value to initialize its backing
      buffers. This requires the buffer_type implement a copy constructor.
  ============================================================================*/
  explicit
  triple_buffer_t(const buffer_type &init_value) :
    indices_(DEFAULT_INDICES), buffers_({{init_value, init_value, init_value}})
  {
    static_assert((std::is_copy_constructible<buffer_type>::value ||
                   std::is_copy_assignable<buffer_type>::value ||
                   std::is_pod<buffer_type>::value),
                  "buffer_type must be copy-constructible to provide an initial value");
  }

  /*============================================================================
    swap_back

      Swaps the write-side buffers. The back buffer will be swapped with the
      intermediate buffer, allowing the reader side to acquire changes written
      to the back buffer in future reader swaps.
  ============================================================================*/
  void               swap_back()
  {
    index_t indices = indices_.load();
    index_t new_indices;
    do {
      new_indices = ((indices & FRONT_MASK) |
                     ((indices & BACK_MASK) << INDEX_BITS) |
                     ((indices & MIDDLE_MASK) >> INDEX_BITS));
    } while ( ! indices_.compare_exchange_weak(indices, new_indices));
  }

  /*============================================================================
    swap_front

      Does much the same thing as swap_back, except that it operates on the
      reader side. This swaps the intermediate buffer with the front buffer,
      allowing the reader side to work with the most recent changes.
  ============================================================================*/
  void               swap_front()
  {
    index_t indices;
    index_t new_indices;
    do {
      indices = indices_.load();
      new_indices = ((indices & BACK_MASK) |
                     ((indices & FRONT_MASK) >> INDEX_BITS) |
                     ((indices & MIDDLE_MASK) << INDEX_BITS));
    } while ( ! indices_.compare_exchange_weak(indices, new_indices));
  }

  /*============================================================================
    back_buffer

      Returns a non-const reference to the current back buffer. This reference
      is valid until the next call to swap_back.
  ============================================================================*/
  auto back_buffer() -> buffer_type& {
    return buffers_[indices_.load() & BACK_MASK];
  }

  /*============================================================================
    back_buffer const

      Returns a const reference to the current back buffer. This is recommended
      for copying the buffer if necessary. This reference is valid until the
      next call to swap_back.
  ============================================================================*/
  auto back_buffer(void) const -> const buffer_type&
  {
    return buffers_[indices_.load() & BACK_MASK];
  }

  /*==============================================================================
    front_buffer

      Returns a non-const reference to the current front buffer. This reference
      is only valid until the next swap_front call.
  ==============================================================================*/
  auto front_buffer(void) -> buffer_type&
  {
    return buffers_[(indices_.load() & FRONT_MASK) >> FRONT_OFFSET];
  }

  /*============================================================================
    front_buffer const

      Returns a const reference to the current front buffer. If you need to
      modify this data, it is recommended you copy it. Modifying the returned
      object in any way is undefined behavior. This reference is valid until the
      next call to swap_front.
  ============================================================================*/
  auto front_buffer(void) const -> const buffer_type&
  {
    return buffers_[(indices_.load() & FRONT_MASK) >> FRONT_OFFSET];
  }

};

} // namespace snow

#endif /* end __SNOW_COMMON__TRIPLEBUFFER_HH__ include guard */
