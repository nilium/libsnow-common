// vec3.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__VEC3_HH__
#define __SNOW_COMMON__VEC3_HH__

#include "math.hh"

#include <cmath>
#include <iostream>
#include <type_traits>

/*
vec3_t<value_type>
{
  ////// Member variables //////

  value_type  x
  value_type  y
  value_type  z


  /////// Static Members ///////

  vec3_t zero       -> Zero vector -> { 0, 0, 0 }
  vec3_t one        -> One vector  -> { 1, 1, 1 }


  ////// Instance Methods //////

  vec3_t &      normalize()
    Get the result of normalizing this vector.
  ------------------------------------------------------------------------------
  vec3_t        normalized() const
    Normalize this vector.
  ------------------------------------------------------------------------------
  value_type    magnitude() const
    Get the magnitude of this vector.
  ------------------------------------------------------------------------------
  value_type    length() const
    Get the squared length of this vector.
  ------------------------------------------------------------------------------
  vec3_t        difference(const vec3_t &other) const
    Get the difference of this vector and another vector.
  ------------------------------------------------------------------------------
  vec3_t &      subtract(const vec3_t &other)
    Subtract another vector from this vector.
  ------------------------------------------------------------------------------
  vec3_t        sum(const vec3_t &other) const
    Get the sum of this vector and another vector.
  ------------------------------------------------------------------------------
  vec3_t &      add(const vec3_t &other)
    Add another vector to this vector.
  ------------------------------------------------------------------------------
  vec3_t        scaled(const vec3_t &other) const
    Get the result of scaling this vector by another vector.
  ------------------------------------------------------------------------------
  vec3_t &      scale(const vec3_t &other)
    Scale this vector by another vector.
  ------------------------------------------------------------------------------
  vec3_t        scaled(value_type scalar) const
    Get the result of scaling this vector by a scalar.
  ------------------------------------------------------------------------------
  vec3_t &      scale(value_type scalar)
    Scale this vector by a scalar.
  ------------------------------------------------------------------------------
  vec3_t        negated() const
    Get the negation of this vector.
  ------------------------------------------------------------------------------
  vec3_t &      negate()
    Negate this vector.
  ------------------------------------------------------------------------------
  vec3_t        inverse() const
    Returns the inverse of this vector. In other words, 1 / component for each
    component of the vector.
  ------------------------------------------------------------------------------
  vec3_t &      invert()
    Inverts this vector.
  ------------------------------------------------------------------------------
  vec3_t        cross_product(const vec3_t &other) const
    Get the cross product of this vector and another vector.
  ------------------------------------------------------------------------------
  value_type    dot_product(const vec3_t &other) const
    Get the dot product of this vector and another vector.
  ------------------------------------------------------------------------------
  vec3_t &      operator += (const vec3_t &other)
    Adds another vector to this vector.
  ------------------------------------------------------------------------------
  vec3_t &      operator -= (const vec3_t &other)
    Subtracts another vector from this vector.
  ------------------------------------------------------------------------------
  vec3_t &      operator *= (const vec3_t &other)
    Scales this vector by another vector.
  ------------------------------------------------------------------------------
  vec3_t &      operator *= (value_type scalar)
    Scales this vector by a scalar.
  ------------------------------------------------------------------------------
  vec3_t &      operator /= (const vec3_t &other)
    Scales this vector by the inverse of another vector.
  ------------------------------------------------------------------------------
  vec3_t &      operator /= (value_type scalar)
    Scales this vector by one over the scalar provided. In the event that the
    scalar is zero, something bad might happen. Chances are it'll blow your
    foot off or something.
  ------------------------------------------------------------------------------
  vec3_t        operator - () const
    Get the negation of this vector.
  ------------------------------------------------------------------------------
  vec3_t        operator ~ () const
    Get the inverse of this vector.
  ------------------------------------------------------------------------------
  auto          operator - (const vec3_t<T> &lhs, const vec3_t<Q> &rhs) -> vec3_t<T>
    Difference operator.
  ------------------------------------------------------------------------------
  auto          operator + (const vec3_t<T> &lhs, const vec3_t<Q> &rhs) -> vec3_t<T>
    Sum operator.
  ------------------------------------------------------------------------------
  auto          operator * (const vec3_t<T> &lhs, const vec3_t<Q> &rhs) -> vec3_t<T>
    Scale-by-vector operator.
  ------------------------------------------------------------------------------
  auto          operator * (const vec3_t<T> &lhs, Q rhs) -> vec3_t<T>
    Scale-by-scalar operator.
  ------------------------------------------------------------------------------
  auto          operator / (const vec3_t<T> &lhs, const vec3_t<Q> &rhs) -> vec3_t<T>
    Returns the product of scaling this vector by the inverse of another vector.
  ------------------------------------------------------------------------------
  auto          operator / (const vec3_t<T> &lhs, Q rhs) -> vec3_t<T>
    Division operator - returns the result of scaling by one over rhs.
  ------------------------------------------------------------------------------
  value_type    operator % (const vec3_t<T> &lhs, const vec3_t<Q> &rhs)
    Dot product operator.
  ------------------------------------------------------------------------------
  bool          operator == (const vec3_t<T> &lhs, const vec3_t<Q> &rhs)
  bool          operator != (const vec3_t<T> &lhs, const vec3_t<Q> &rhs)
    Equality operators.
  ------------------------------------------------------------------------------
  std::ostream &operator << (std::ostream &out, const vec3_t<T> &in)
    Write the vector to an ostream (as text).
  ------------------------------------------------------------------------------
  value_type &  operator[] (int index)
  value_type    operator[] (int index) const
    Get reference to member at index (e.g., 0 = x, 1 = y, 2 = z, and any other
    index is an exception [happy birthday]).
  ------------------------------------------------------------------------------
                operator value_type* ()
                operator const value_type* () const
    Convert to value_type pointer (same as getting &x).
  ------------------------------------------------------------------------------
                operator vec3_t<Q> () const
    Cast this vector to a vector of value_type Q.
}
*/

namespace snow
{

template <typename T> struct vec2_t;
template <typename T> struct vec4_t;

#include "inline/vec3.hh"

typedef vec3_t<float> vec3f_t;
typedef vec3_t<double> vec3d_t;

}

#include "vec2.hh"
#include "vec4.hh"

#endif /* end __SNOW_COMMON__VEC3_HH__ include guard */
