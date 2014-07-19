/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#pragma once


namespace snow {


template <typename T>
auto plane_t<T>::make(vec3_t normal, float d) -> plane_t
{
  return { normal.normalized(), d };
}



template <typename T>
auto plane_t<T>::for_triangle(vec3_t v0, vec3_t v1, vec3_t v2) -> plane_t
{
  vec3_t n = (v1 - v0).cross_product(v2 - v1).normalize();
  return {
    n,
    n.dot_product(v0)
  };
}



template <typename T>
template <typename C>
auto plane_t<T>::for_points(const C &points) -> plane_t
{
  typename C::const_iterator iter, end;
  iter = points.cbegin();
  end = points.cend();

  value_type count;
  if ((count = std::distance(iter, end)) < value_type(3.5))
    s_throw(std::invalid_argument, "Points container must have four or more points.");

  vec3_t normal, last;
  vec3_t first = *iter;
  value_type d = 0;
  for (;;) {
    typename C::const_iterator cur = iter++;
    last = *cur;

    if (iter == end)
      break;

    normal.x += (last.z + iter->z) * (last.y - iter->y);
    normal.y += (last.x + iter->x) * (last.z - iter->z);
    normal.z += (last.y + iter->y) * (last.x - iter->x);
  }
  normal.x += (last.z + first.z) * (last.y - first.y);
  normal.y += (last.x + first.x) * (last.z - first.z);
  normal.z += (last.y + first.y) * (last.x - first.x);

  normal.normalize();

  for (iter = points.cbegin(); iter != end; ++iter)
    d += iter->dot_product(normal);

  d /= count;

  return {
    normal,
    d
  };
}



template <typename T>
auto plane_t<T>::distance(vec3_t p) const -> value_type
{
  return normal.dot_product(p) - d;
}



template <typename T>
auto plane_t<T>::nearest_to(vec3_t p) const -> vec3_t
{
  return p + normal * (d - p.dot_product(normal));
}



template <typename T>
auto plane_t<T>::side(vec3_t p) const -> plane_side_t
{
  const value_type dist = distance(p);
  if (is_zero(dist))
    return PLANE_INSIDE;
  else if (dist > 0)
    return PLANE_FRONT;
  else
    return PLANE_BACK;
}



template <typename T>
auto plane_t<T>::intersection(line_t line) const -> vec3_t
{
  value_type t = (d - line.origin.dot_product(normal)) / (line.dist.dot_product(normal));
  return line.origin + line.dist * t;
}



template <typename T>
plane_t<T>::operator value_type * ()
{
  return (value_type *)normal;
}



template <typename T>
plane_t<T>::operator const value_type * () const
{
  return (const value_type *)normal;
}



template <typename T>
bool operator == (plane_t<T> lhs, plane_t<T> rhs)
{
  return are_equiv(lhs.d, rhs.d) && lhs.normal == rhs.normal;
}



template <typename T>
bool operator != (plane_t<T> lhs, plane_t<T> rhs)
{
  return !(lhs == rhs);
}


} // namespace snow
