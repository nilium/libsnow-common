/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef __SNOW_COMMON__LINE_TCC__
#define __SNOW_COMMON__LINE_TCC__


namespace snow {


template <typename T>
auto line_t<T>::end() const -> vec3
{
  return origin + dist;
}



template <typename T>
auto line_t<T>::translated(vec3 d) const -> line_t
{
  return {
    origin + d, dist
  };
}



template <typename T>
auto line_t<T>::translate(vec3 d) -> line_t&
{
  origin += d;
  return *this;
}



template <typename T>
auto line_t<T>::scaled(T d) const -> line_t
{
  return {
      origin, dist * d
  };
}



template <typename T>
auto line_t<T>::scale(T d) -> line_t&
{
  dist *= d;
  return *this;
}



template <typename T>
auto line_t<T>::nearest_to(const vec3& p) const -> vec3
{
  // This is borrowed from Mark Sibly's Blitz3D maths code since it's
  // really rather nice.
  return end().scale(dist.dot_product(p - origin) / dist.dot_product(dist));
}


} // namespace snow

#endif /* end __SNOW_COMMON__LINE_TCC__ include guard */
