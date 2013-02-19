#ifndef __SNOW_VEC4_HH__
#define __SNOW_VEC4_HH__

#include "math.hh"

#include <cmath>
#include <iostream>
#include <type_traits>

/*
vec4_t<value_type>
{
  ////// Member variables //////

  value_type  x
  value_type  y
  value_type  z
  value_type  w


  /////// Static Members ///////

  vec4_t zero       -> Zero vector -> { 0, 0, 0 }
  vec4_t one        -> One vector  -> { 1, 1, 1 }


  ////// Instance Methods //////

  vec4_t &      normalize()
    Get the result of normalizing this vector.
  ------------------------------------------------------------------------------
  vec4_t        normalized() const
    Normalize this vector.
  ------------------------------------------------------------------------------
  value_type    magnitude() const
    Get the magnitude of this vector.
  ------------------------------------------------------------------------------
  value_type    length() const
    Get the squared length of this vector.
  ------------------------------------------------------------------------------
  vec4_t        difference(const vec4_t &other) const
    Get the difference of this vector and another vector.
  ------------------------------------------------------------------------------
  vec4_t &      subtract(const vec4_t &other)
    Subtract another vector from this vector.
  ------------------------------------------------------------------------------
  vec4_t        sum(const vec4_t &other) const
    Get the sum of this vector and another vector.
  ------------------------------------------------------------------------------
  vec4_t &      add(const vec4_t &other)
    Add another vector to this vector.
  ------------------------------------------------------------------------------
  vec4_t        scaled(const vec4_t &other) const
    Get the result of scaling this vector by another vector.
  ------------------------------------------------------------------------------
  vec4_t &      scale(const vec4_t &other)
    Scale this vector by another vector.
  ------------------------------------------------------------------------------
  vec4_t        scaled(value_type scalar) const
    Get the result of scaling this vector by a scalar.
  ------------------------------------------------------------------------------
  vec4_t &      scale(value_type scalar)
    Scale this vector by a scalar.
  ------------------------------------------------------------------------------
  vec4_t        negated() const
    Get the negation of this vector.
  ------------------------------------------------------------------------------
  vec4_t &      negate()
    Negate this vector.
    ------------------------------------------------------------------------------
  vec4_t        inverse() const
    Returns the inverse of this vector. In other words, 1 / component for each
    component of the vector.
  ------------------------------------------------------------------------------
  vec4_t &      invert()
    Inverts this vector.
  ------------------------------------------------------------------------------
  value_type    dot_product(const vec4_t &other) const
    Get the dot product of this vector and another vector.
  ------------------------------------------------------------------------------
  vec4_t &      operator += (const vec4_t &other)
    Adds another vector to this vector.
  ------------------------------------------------------------------------------
  vec4_t &      operator -= (const vec4_t &other)
    Subtracts another vector from this vector.
  ------------------------------------------------------------------------------
  vec4_t &      operator *= (const vec4_t &other)
    Scales this vector by another vector.
  ------------------------------------------------------------------------------
  vec4_t &      operator *= (value_type scalar)
    Scales this vector by a scalar.
  ------------------------------------------------------------------------------
  vec4_t &      operator /= (const vec4_t &other)
    Scales this vector by the inverse of another vector.
  ------------------------------------------------------------------------------
  vec4_t &      operator /= (value_type scalar)
    Scales this vector by one over the scalar provided. In the event that the
    scalar is zero, something bad might happen. Chances are it'll blow your
    foot off or something.
  ------------------------------------------------------------------------------
  vec4_t        operator - () const
    Get the negation of this vector.
  ------------------------------------------------------------------------------
  vec4_t        operator ~ () const
    Get the inverse of this vector.
  ------------------------------------------------------------------------------
  vec4_t<T>     operator - (const vec4_t<T> &lhs, const vec4_t<Q> &rhs)
    Difference operator.
  ------------------------------------------------------------------------------
  vec4_t<T>     operator + (const vec4_t<T> &lhs, const vec4_t<Q> &rhs)
    Sum operator.
  ------------------------------------------------------------------------------
  vec4_t<T>     operator * (const vec4_t<T> &lhs, const vec4_t<Q> &rhs)
    Scale-by-vector operator.
  ------------------------------------------------------------------------------
  vec4_t<T>     operator * (const vec4_t<T> &lhs, Q rhs)
    Scale-by-scalar operator.
  ------------------------------------------------------------------------------
  vec4_t<T>     operator / (const vec4_t<T> &lhs, const vec4_t<Q> &rhs)
    Returns the product of scaling this vector by the inverse of another vector.
  ------------------------------------------------------------------------------
  vec4_t<T>     operator / (const vec4_t<T> &lhs, Q rhs)
    Division operator - returns the result of scaling by one over rhs.
  ------------------------------------------------------------------------------
  value_type    operator % (const vec4_t<T> &lhs, const vec4_t<Q> &rhs)
    Dot product operator.
  ------------------------------------------------------------------------------
  bool          operator == (const vec4_t<T> &lhs, const vec4_t<Q> &rhs)
  bool          operator != (const vec4_t<T> &lhs, const vec4_t<Q> &rhs)
    Equality operators.
  ------------------------------------------------------------------------------
  std::ostream &operator << (std::ostream &out, const vec4_t<T> &in)
    Write the vector to an ostream (as text).
  ------------------------------------------------------------------------------
  value_type &  operator[] (int index)
  value_type    operator[] (int index) const
    Get reference to member at index (e.g., 0 = x, 1 = y, 2 = z, 3 = w, and
    any other index is an exception [happy birthday]).
  ------------------------------------------------------------------------------
                operator value_type* ()
                operator const value_type* () const
    Convert to value_type pointer (same as getting &x).
  ------------------------------------------------------------------------------
                operator vec4_t<Q> () const
    Cast this vector to a vector of value_type Q.
}
*/

namespace snow
{

#include "inline/vec4.thh"

typedef vec4_t<float> vec4f_t;
typedef vec4_t<double> vec4d_t;

}

#endif /* end __SNOW_VEC4_HH__ include guard */
