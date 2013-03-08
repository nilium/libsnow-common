#ifndef __SNOW_COMMON__FILTERS_HH__
#define __SNOW_COMMON__FILTERS_HH__

namespace snow {

template <typename R, typename C, typename F, typename... ARGS>
R mapped_values(const C &input, F &&func, ARGS&&... args)
{
  R out;
  for (auto value : input)
    out.insert(out.end(), func(value, args...));
  return out;
}

template <typename C, typename F, typename... ARGS>
C selected_values(const C &input, F &&func, ARGS&&... args)
{
  C out;
  for (auto value : input)
    if (func(value, args...))
      out.insert(out.end(), value);

  return out;
}

template <typename C, typename F, typename... ARGS>
C rejected_values(const C &input, F &&func, ARGS&&... args)
{
  C out;
  for (auto value : input)
    if (!func(value, args...))
      out.insert(out.end(), value);

  return out;
}

} // namespace snow

#endif /* end __SNOW_COMMON__FILTERS_HH__ include guard */
