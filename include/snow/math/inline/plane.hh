/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#pragma once


namespace snow {


/** @cond IGNORE */
template <typename T> struct vec3_t;
template <typename T> struct line_t;
/** @endcond */


/** @addtogroup Math3D */
///@{


enum plane_side_t : int
{
  PLANE_BACK=-1,
  PLANE_INSIDE=0,
  PLANE_FRONT=1
};

template <typename T>
struct S_EXPORT alignas(T) plane_t
{
  typedef T value_type;
  typedef vec3_t<T> vec3_t;
  typedef line_t<T> line_t;

  vec3_t normal;
  value_type d;

  static plane_t   make(vec3_t normal, float d);
  // A plane specifically for three points forming a triangle. Uses CCW winding order.
  static plane_t   for_triangle(vec3_t v0, vec3_t v1, vec3_t v2);
  // A plane for four or more points. Uses CCW winding order.
  template <typename C> // where C provides cbegin/cend and a forward const_iterator
  static plane_t   for_points(const C &points);

  value_type       distance(vec3_t p) const;
  vec3_t           nearest_to(vec3_t p) const;
  plane_side_t     side(vec3_t p) const;
  vec3_t           intersection(line_t line) const;

  operator value_type * ();
  operator const value_type * () const;
};

template <typename T>
bool operator != (plane_t<T> lhs, plane_t<T> rhs);

template <typename T>
bool operator == (plane_t<T> lhs, plane_t<T> rhs);


using planef_t = plane_t<float>;
using planed_t = plane_t<double>;


///@}


} // namespace snow

#include "plane.cc"
