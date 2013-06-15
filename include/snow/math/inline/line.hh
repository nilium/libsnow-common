// line.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__LINE_THH__
#define __SNOW_COMMON__LINE_THH__


namespace snow {


/** @cond IGNORE */
template <typename T> struct vec3_t;
/** @endcond */


/** Basic 3D line type. */
template <typename T>
struct S_EXPORT alignas(T) line_t
{
  /** @brief The line's component type. */
  using value_type = T;
  /** @brief The line's vec3_t type. */
  using vec3       = vec3_t<T>;

  /** @brief The line's origin. */
  vec3 origin;
  /** @brief The line's direction and distance. */
  vec3 dist;

  /** @brief Returns a vector for the end of the line. */
  auto end() const -> vec3;
  /** @brief Returns a copy of the line translated by a vector. */
  auto translated(vec3 d) const -> line_t;
  /** @brief Translates the line by a vector. */
  auto translate(vec3 d) -> line_t&;
  /** @brief Returns a copy of the line with its end scaled by a scalar. */
  auto scaled(value_type d) const -> line_t;
  /** @brief Scales the line's end point by a scalar. */
  auto scale(value_type d) -> line_t&;
  /** @brief Returns a vector along the line nearest to the given vector. */
  auto nearest_to(const vec3& p) const -> vec3;
};


using linef_t = line_t<float>;
using lined_t = line_t<double>;


} // namespace snow

#include "line.cc"

#endif /* end __SNOW_COMMON__LINE_THH__ include guard */
