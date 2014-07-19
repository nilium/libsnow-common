#pragma once


#include <cstddef>


namespace snow
{


/**
  @brief Aligns num to the given alignment.
  @param  num       The number to align.
  @param  alignment An alignment that's a power of two. Inputs that aren't a
                    power of two yield undefined results.
  @return num aligned to alignment.
*/
constexpr size_t align(size_t const num, size_t const alignment)
{
  return (num + (alignment - 1)) & ~(alignment - 1);
}



/**
  @brief Aligns num to the given alignment. In this case, the alignment is the
  sole template parameter of the function.
  @param  num       The number to align.
  @return num aligned to alignment.
*/
template <size_t alignment>
constexpr size_t align(size_t const num)
{
  return (num + (alignment - 1)) & ~(alignment - 1);
}



/**
  @brief Specialization of align<size_t> for alignments of 1. This function is
  a no-op.
  @param  num The number to align (will be returned unchanged).
  @return num
*/
template <>
constexpr size_t align<1>(size_t const num)
{
  return num;
}


} // namespace snow
