#ifndef __SNOW_COMMON__FILTERS_HH__
#define __SNOW_COMMON__FILTERS_HH__

namespace snow {

template <typename R, typename C, typename F, typename... ARGS>
R map_values(const C &input, F &&func, ARGS&&... args)
{
  R out;
  for (auto value : input)
    out.insert(out.end(), func(value, args...));
  return out;
}

} // namespace snow

#endif /* end __SNOW_COMMON__FILTERS_HH__ include guard */
