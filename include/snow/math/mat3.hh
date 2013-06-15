// mat3.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__MAT3_HH__
#define __SNOW_COMMON__MAT3_HH__

#include <snow/config.hh>
#include "math.hh"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <type_traits>

/*
mat3_t<value_type>
{
  ////// Member variables //////

  value_type  m00, m10, m20, m30
  value_type  m01, m11, m21, m31
  value_type  m02, m12, m22, m32
  value_type  m03, m13, m23, m33


  /////// Static Members ///////

  mat3_t zero       -> Zero matrix
  mat3_t identity   -> Identity matrix


  mat3_t      scaling(const vec3 &off)
    Creates a scale matrix. Named 'scaling' due to name overlap with
    mat3::scale(vec3) below.
  ------------------------------------------------------------------------------
  mat3_t      rotation(value_type angle, const vec3 &axis)
    Creates a rotation matrix.
  ------------------------------------------------------------------------------
  mat3_t      from_quat(const quat &in)
    Converts the given quaternion to a mat3.


  ////// Instance Methods //////

  mat3_t &    transpose()
  mat3_t      transposed() const
    Tranpose this matrix or get the transposed matrix.
  ------------------------------------------------------------------------------
  vec3        colvec3(int index) const
  mat3_t &    set_colvec3(int index, const vec3 &col)
    Get and set 3-component row and column vectors. To access row-vectors, just
    use the matrix's r, s, and t member vectors.
  ------------------------------------------------------------------------------
  mat3_t      inverse() const
  mat3_t &    invert()
    Get the inverse of the matrix and invert the matrix, respectively.
  ------------------------------------------------------------------------------
  mat3_t      negated() const
  mat3_t &    negate()
    Get the negation of this matrix. Simply put, this just negates all elements
    of the matrix.
  ------------------------------------------------------------------------------
  mat3_t      sum(const mat3_t &other) const
  mat3_t      sum(value_type scalar) const
    Get the sum of this matrix and another matrix or a scalar value.
  ------------------------------------------------------------------------------
  mat3_t &    add(const mat3_t &other)
  mat3_t &    add(value_type scalar)
    Add a matrix or a scalar value to this matrix.
  ------------------------------------------------------------------------------
  mat3_t      difference(const mat3_t &other) const
  mat3_t      difference(value_type scalar) const
    Get the difference of this matrix and another matrix or a scalar value.
  ------------------------------------------------------------------------------
  mat3_t &    subtract(const mat3_t &other)
  mat3_t &    subtract(value_type scalar)
    Subtract a matrix or a scalar value from this matrix.
  ------------------------------------------------------------------------------
  mat3_t      scaled(value_type scalar) const
  mat3_t &    scale(value_type scalar)
    Get the scaled matrix or scale the matrix by a scalar value.
  ------------------------------------------------------------------------------
  mat3_t      scaled(const mat3_t &other) const
  mat3_t &    scale(const mat3_t &other)
    Get the scaled matrix or scale the matrix by another matrix.
  ------------------------------------------------------------------------------
  mat3_t      scaled(const vec3 &vec) const
  mat3_t &    scale(const vec3 &vec)
    Get the scaled matrix or scale the matrix by a vec3 (this affects only
    columns/rows < 3).
  ------------------------------------------------------------------------------
  mat3_t      adjoint() const
    Get the adjoint of this matrix.
  ------------------------------------------------------------------------------
  mat3_t      cofactor() const
    Gets a matrix with this matrix's cofactors.
  ------------------------------------------------------------------------------
  value_type  determinant() const
    Get the determinant of the matrix.
  ------------------------------------------------------------------------------
  mat3_t      product(const mat3_t &other) const
  mat3_t &    multiply(const mat3_t &other)
    Get the product of multipling this and another matrix together, or multiply
    this matrix by another.
  ------------------------------------------------------------------------------
  vec3        rotate(const vec3 &vec) const
  vec3        inverse_rotate(const vec3 &vec) const
    Rotate a vec3.
  ------------------------------------------------------------------------------
  value_type &operator[] (int index)
  value_type  operator[] (int index) const
    Returns a reference (or the value at) the index in the matrix. Indices
    outside the range [0-8] will throw an exception.
  ------------------------------------------------------------------------------
              operator value_type* ()
              operator const value_type* () const
    Returns a pointer to the first element of the matrix.
  ------------------------------------------------------------------------------
  mat3_t &    operator *= (const mat3_t &other)
    Multiplies this mat3 by the other mat3.
  ------------------------------------------------------------------------------
  mat3_t &    operator *= (value_type scalar)
    Scales this matrix by the given scalar or vec3.
  ------------------------------------------------------------------------------
  mat3_t &    operator += (const mat3_t &other)
  mat3_t &    operator += (value_type scalar)
    Add operators (see mat3::add).
  ------------------------------------------------------------------------------
  mat3_t &    operator -= (const mat3_t &other)
  mat3_t &    operator -= (value_type scalar)
    Subtract operators (see mat3::subtract).
  ------------------------------------------------------------------------------
  mat3_t<T>   operator * (const mat3_t<T> &rhs, const mat3_t<Q> &lhs)
    Product operator (see mat3::product).
  ------------------------------------------------------------------------------
  vec4_t<T>   operator * (const mat3_t<T> &rhs, const vec4_t<Q> &lhs)
  vec3_t<T>   operator * (const mat3_t<T> &rhs, const vec3_t<Q> &lhs)
    Product operators (see mat3::multiply for vectors).
  ------------------------------------------------------------------------------
  mat3_t<T>   operator * (const mat3_t<T> &rhs, const Q &lhs)
    Scaled operator (see mat3::scaled).
  ------------------------------------------------------------------------------
  mat3_t<T>   operator + (const mat3_t<T> &rhs, const mat3_t<Q> &lhs)
  mat3_t<T>   operator + (const mat3_t<T> &rhs, const Q &lhs)
    Sum operators (see mat3::sum).
  ------------------------------------------------------------------------------
  mat3_t<T>   operator - (const mat3_t<T> &rhs, const mat3_t<Q> &lhs)
  mat3_t<T>   operator - (const mat3_t<T> &rhs, const Q &lhs)
    Sum operators (see mat3::difference).
  ------------------------------------------------------------------------------
  bool        operator == (const mat3_t<T> &rhs, const mat3_t<Q> &lhs)
  bool        operator != (const mat3_t<T> &rhs, const mat3_t<Q> &lhs)
    Equality operators.
}
*/

#include "inline/mat3.hh"

#include "vec2.hh"
#include "vec3.hh"
#include "quat.hh"
#include "mat4.hh"

#endif /* end __SNOW_COMMON__MAT3_HH__ include guard */
