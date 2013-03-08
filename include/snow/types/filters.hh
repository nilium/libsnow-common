// filters.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__FILTERS_HH__
#define __SNOW_COMMON__FILTERS_HH__

#include <stdexcept>

namespace snow {

template <typename C, typename R, typename F, typename... ARGS>
R &map_values(const C &input, R &output, F &&func, ARGS&&... args)
{
  if ((void *)&input == (void *)&output)
    throw std::invalid_argument("input and output parameters must be distinct");

  for (auto value : input)
    output.insert(output.end(), func(value, args...));
  return output;
}

template <typename C, typename F, typename... ARGS>
C &select_values(const C &input, C &output, F &&func, ARGS&&... args)
{
  if ((void *)&input == (void *)&output)
    throw std::invalid_argument("input and output parameters must be distinct");

  for (auto value : input)
    if (func(value, args...))
      output.insert(output.end(), value);

  return output;
}

template <typename C, typename F, typename... ARGS>
C &reject_values(const C &input, C &output, F &&func, ARGS&&... args)
{
  if ((void *)&input == (void *)&output)
    throw std::invalid_argument("input and output parameters must be distinct");

  for (auto value : input)
    if (!func(value, args...))
      output.insert(output.end(), value);

  return output;
}

template <typename C, typename F, typename... ARGS>
C selected_values(const C &input, F &&func, ARGS&&... args)
{
  C output;
  for (auto value : input)
    if (func(value, args...))
      output.insert(output.end(), value);
  return output;
}

template <typename C, typename F, typename... ARGS>
C rejected_values(const C &input, F &&func, ARGS&&... args)
{
  C output;
  for (auto value : input)
    if (!func(value, args...))
      output.insert(output.end(), value);
  return output;
}

} // namespace snow

#endif /* end __SNOW_COMMON__FILTERS_HH__ include guard */
