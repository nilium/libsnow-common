#ifndef __SNOW_COMMON__ALIGN_HH__
#define __SNOW_COMMON__ALIGN_HH__


#include <cstddef>


namespace snow
{


constexpr size_t align(size_t const num, size_t const alignment)
{
  return (num + (alignment - 1)) & ~(alignment - 1);
}



template <size_t alignment>
constexpr size_t align(size_t const num)
{
  return (num + (alignment - 1)) & ~(alignment - 1);
}



template <>
constexpr size_t align<1>(size_t const num)
{
  return num;
}


} // namespace snow

#endif /* end __SNOW_COMMON__ALIGN_HH__ include guard */
