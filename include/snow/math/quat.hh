// quat.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__QUAT_HH__
#define __SNOW_COMMON__QUAT_HH__

#include <snow/config.hh>
#include "math.hh"

#include <cmath>
#include <iostream>
#include <type_traits>

/*
quat_t<value_type>
{
  ////// Member variables //////

  vec3_t      xyz
  value_type  w


  /////// Static Members ///////

  quat_t zero       -> Zero quaternion     -> { 0, 0, 0, 0 }
  quat_t one        -> One quaternion      -> { 1, 1, 1, 0 }
  quat_t identity   -> Identity quaternion -> { 0, 0, 0, 1 }


  quat_t        from_mat4(const mat4_t& mat)
    Constructs a quaternion from a 4x4 matrix.
  ------------------------------------------------------------------------------
  quat_t        from_angle_axis(value_type angle, const vec3_t &axis)
    Constructs a quaternion representation rotation about the given axis.


  ////// Instance Methods //////

  value_type    length() const
    See vec4::length.
  ------------------------------------------------------------------------------
  value_type    magnitude() const
    See vec4::magnitude.
  ------------------------------------------------------------------------------
  quat_t        inverse() const
    Returns the inverse of this quaternion.
  ------------------------------------------------------------------------------
  quat_t &      invert()
    Inverts this quaternion.
  ------------------------------------------------------------------------------
  quat_t        negated() const
    Returns the negation of this quaternion (see vec4::negated).
  ------------------------------------------------------------------------------
  quat_t &      negate()
    Negates this quaternion (see vec4::negate).
  ------------------------------------------------------------------------------
  quat_t        product(const quat_t& other) const
    Gets the product of this and another quaternion.
  ------------------------------------------------------------------------------
  quat_t &      multiply(const quat_t& other)
    Multiplies this quaternion by another quaternion.
  ------------------------------------------------------------------------------
  quat_t        normalized() const
    See vec4::normalized.
  ------------------------------------------------------------------------------
  quat_t &      normalize()
    See vec4::normalize.
  ------------------------------------------------------------------------------
  quat_t        difference(const quat_t &other) const
    See vec4::difference.
  ------------------------------------------------------------------------------
  quat_t &      subtract(const quat_t &other)
    See vec4::subtract.
  ------------------------------------------------------------------------------
  quat_t        sum(const quat_t &other) const
    See vec4::sum.
  ------------------------------------------------------------------------------
  quat_t &      add(const quat_t &other)
    See vec4::add.
  ------------------------------------------------------------------------------
  quat_t        scaled(value_type scalar) const
    See vec4::scaled.
  ------------------------------------------------------------------------------
  quat_t        scaled(const quat_t &other) const
    See vec4::scaled.
  ------------------------------------------------------------------------------
  quat_t &      scale(value_type scalar)
    See vec4::scale.
  ------------------------------------------------------------------------------
  quat_t &      scale(const quat_t &other)
    See vec4::scale.
  ------------------------------------------------------------------------------
  value_type    dot_product(const quat_t &other) const
    See vec4::dot_product.
  ------------------------------------------------------------------------------
  quat_t        slerp(const quat_t &to, value_type delta) const
    Interpolates from this quaternion to another using spherical linear
    interpolation and returns the result. For sufficiently small rotations,
    it will revert to linear interpolation (see quat::lerp).
  ------------------------------------------------------------------------------
  quat_t        lerp(const quat_t &to, value_type delta) const
    Interpolates from this quaternion to another using linear interpolation.
    The result is always normalized.
  ------------------------------------------------------------------------------
  quat_t &      operator *= (const quat_t &other)
    Multiplies this quaternion by another.
  ------------------------------------------------------------------------------
  quat_t &      operator *= (value_type scalar)
    Scales this quaternion with a scalar.
  ------------------------------------------------------------------------------
  quat_t &      operator += (const quat_t &other)
    Adds another quaternion to this quaternion.
  ------------------------------------------------------------------------------
  quat_t &      operator -= (const quat_t &other)
    Subtracts another quaternion from this quaternion.
  ------------------------------------------------------------------------------
  quat_t        operator - () const
    Returns the negation - not the inverse - of this quaternion.
  ------------------------------------------------------------------------------
  quat_t        operator ~ () const
    Returns the inverse of this quaternion.
  ------------------------------------------------------------------------------
  quat_t<T>     operator * (const quat_t<T> &lhs, const quat_t<Q> &rhs)
    Returns the product of this quaternion and another quaternion.
  ------------------------------------------------------------------------------
  quat_t<T>     operator * (const quat_t<T> &lhs, Q rhs)
    Returns the product of this quaternion and a scalar.
  ------------------------------------------------------------------------------
  quat_t<T>     operator + (const quat_t<T> &lhs, const quat_t<Q> &rhs)
    Returns the sum of this quaternion and another quaternion.
  ------------------------------------------------------------------------------
  quat_t<T>     operator - (const quat_t<T> &lhs, const quat_t<Q> &rhs)
    Returns the difference of this quaternion and another quaternion.
  ------------------------------------------------------------------------------
  value_type    operator % (const quat_t<T> &lhs, const quat_t<Q> &rhs)
    Returns the dot product of this quaternion and another quaternion.
  ------------------------------------------------------------------------------
  bool          operator == (const quat_t<T> &lhs, const quat_t<Q> &rhs)
  bool          operator != (const quat_t<T> &lhs, const quat_t<Q> &rhs)
    Equality operators.
  ------------------------------------------------------------------------------
  value_type &  operator[] (int index)
  value_type    operator[] (int index) const
    Get reference to member at index (0 = x, 1 = y, 2 = z, 3 = w; any other
    index throws an exception).
  ------------------------------------------------------------------------------
                operator value_type* ()
                operator const value_type* () const
    Convert to value_type pointer (same as getting &xyz.x).
  ------------------------------------------------------------------------------
  std::ostream &operator << (std::ostream &out, const quat_t<T> &in)
    Writes this quaternion to an output stream (uses text).
  ------------------------------------------------------------------------------
                operator quat_t<Q> () const
    Cast this quaternion to a quaternion of value_type Q.
}
*/

#include "inline/quat.hh"

#include "vec3.hh"
#include "mat4.hh"
#include "mat3.hh"

#endif /* end __SNOW_COMMON__QUAT_HH__ include guard */
