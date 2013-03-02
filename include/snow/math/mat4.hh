// mat4.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__MAT4_HH__
#define __SNOW_COMMON__MAT4_HH__

#include "math.hh"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <type_traits>

/*
mat4_t<value_type>
{
  ////// Member variables //////

  value_type  m00, m10, m20, m30
  value_type  m01, m11, m21, m31
  value_type  m02, m12, m22, m32
  value_type  m03, m13, m23, m33


  /////// Static Members ///////

  mat4_t zero       -> Zero matrix
  mat4_t identity   -> Identity matrix


  mat4_t      translation(const vec3 &off)
    Creates a translation matrix.
  ------------------------------------------------------------------------------
  mat4_t      scaling(const vec3 &off)
    Creates a scale matrix. Named 'scaling' due to name overlap with
    mat4::    scale(vec3) below.
  ------------------------------------------------------------------------------
  mat4_t      rotation(value_type angle, const vec3 &axis)
    Creates a rotation matrix.
  ------------------------------------------------------------------------------
  mat4_t      frustum(value_type left, value_type right,
                      value_type bottom, value_type top,
                      value_type near, value_type far)
    Creates a perspective projection matrix.
  ------------------------------------------------------------------------------
  mat4_t      orthographic(value_type left, value_type right,
                           value_type top, value_type bottom,
                           value_type near, value_type far)
    Creates an orthographic projection matrix.
  ------------------------------------------------------------------------------
  mat4_t      perspective(value_type fovY, value_type aspect,
                          value_type near, value_type far)
    Creates a perspective projection matrix.
  ------------------------------------------------------------------------------
  mat4_t      look_at(const vec3 &eye, const vec3& center, const vec3& up)
    Creates a look-at matrix.
  ------------------------------------------------------------------------------
  mat4_t      from_quat(const quat &in)
    Converts the given quaternion to a mat4.


  ////// Instance Methods //////

  mat4_t &    translate(const vec3 &translation)
  mat4_t      translated(const vec3 &translation) const
    Translate this matrix or get the translated matrix.
    In both cases, translates the matrix using a 3-component vector.
  ------------------------------------------------------------------------------
  mat4_t &    transpose()
  mat4_t      transposed() const
    Tranpose this matrix or get the transposed matrix.
  ------------------------------------------------------------------------------
  vec4        rowvec4(int index) const
  vec4        colvec4(int index) const
  mat4_t &    set_rowvec4(int index, const vec4 &row)
  mat4_t &    set_colvec4(int index, const vec4 &col)
    Get and set 4-component row and column vectors.
  ------------------------------------------------------------------------------
  vec3        rowvec3(int index) const
  vec3        colvec3(int index) const
  mat4_t &    set_rowvec3(int index, const vec3 &row)
  mat4_t &    set_colvec3(int index, const vec3 &col)
    Get and set 3-component row and column vectors. Does not modify the fourth
    row or column for given columns and rows when using set_*vec3().
  ------------------------------------------------------------------------------
  mat4_t      inverse_orthogonal()
    Get the inverse orthogonal matrix.
  ------------------------------------------------------------------------------
  mat4_t      negated() const
  mat4_t &    negate()
    Get the negation of this matrix. Simply put, this just negates all elements
    of the matrix.
  ------------------------------------------------------------------------------
  mat4_t      sum(const mat4_t &other) const
  mat4_t      sum(value_type scalar) const
    Get the sum of this matrix and another matrix or a scalar value.
  ------------------------------------------------------------------------------
  mat4_t &    add(const mat4_t &other)
  mat4_t &    add(value_type scalar)
    Add a matrix or a scalar value to this matrix.
  ------------------------------------------------------------------------------
  mat4_t      difference(const mat4_t &other) const
  mat4_t      difference(value_type scalar) const
    Get the difference of this matrix and another matrix or a scalar value.
  ------------------------------------------------------------------------------
  mat4_t &    subtract(const mat4_t &other)
  mat4_t &    subtract(value_type scalar)
    Subtract a matrix or a scalar value from this matrix.
  ------------------------------------------------------------------------------
  mat4_t      scaled(value_type scalar) const
  mat4_t &    scale(value_type scalar)
    Get the scaled matrix or scale the matrix by a scalar value.
  ------------------------------------------------------------------------------
  mat4_t      scaled(const mat4_t &other) const
  mat4_t &    scale(const mat4_t &other)
    Get the scaled matrix or scale the matrix by another matrix.
  ------------------------------------------------------------------------------
  mat4_t      scaled(const vec3 &vec) const
  mat4_t &    scale(const vec3 &vec)
    Get the scaled matrix or scale the matrix by a vec3 (this affects only
    columns/rows < 3).
  ------------------------------------------------------------------------------
  bool        inverse_affine(mat4_t &out) const
    Get the inverse affine matrix of this matrix.
  ------------------------------------------------------------------------------
  mat4_t      adjoint() const
    Get the adjoint of this matrix.
  ------------------------------------------------------------------------------
  value_type  cofactor(int r0, int r1, int r2, int c0, int c1, int c2) const
    Get the cofactor of the given indices.
  ------------------------------------------------------------------------------
  value_type  determinant() const
    Get the determinant of the matrix.
  ------------------------------------------------------------------------------
  bool        inverse_general(mat4_t &out) const
    Gets the inverse of this matrix if possible. Returns the inverse to the
    output argument. Returns true if successful, false if not.
  ------------------------------------------------------------------------------
  mat4_t      product(const mat4_t &other) const
  mat4_t &    multiply(const mat4_t &other)
    Get the product of multipling this and another matrix together, or multiply
    this matrix by another.
  ------------------------------------------------------------------------------
  vec4        multiply(const vec4 &vec) const
  vec3        multiply(const vec3 &vec) const
    Transform a vec4 and vec3, respectively.
  ------------------------------------------------------------------------------
  vec3        rotate(const vec3 &vec) const
  vec3        inverse_rotate(const vec3 &vec) const
    Rotate a vec3. rotate() is the same as multiply(vec3), albeit without
    translation added.
  ------------------------------------------------------------------------------
  value_type &operator[] (int index)
  value_type  operator[] (int index) const
    Returns a reference (or the value at) the index in the matrix. Indices
    outside the range [0-15] will throw an exception.
  ------------------------------------------------------------------------------
              operator value_type* ()
              operator const value_type* () const
    Returns a pointer to the first element of the matrix.
  ------------------------------------------------------------------------------
  mat4_t &    operator *= (const mat4_t &other)
    Multiplies this mat4 by the other mat4.
  ------------------------------------------------------------------------------
  mat4_t &    operator *= (value_type scalar)
    Scales this matrix by the given scalar or vec3.
  ------------------------------------------------------------------------------
  mat4_t &    operator += (const mat4_t &other)
  mat4_t &    operator += (value_type scalar)
    Add operators (see mat4::add).
  ------------------------------------------------------------------------------
  mat4_t &    operator -= (const mat4_t &other)
  mat4_t &    operator -= (value_type scalar)
    Subtract operators (see mat4::subtract).
  ------------------------------------------------------------------------------
  mat4_t<T>   operator * (const mat4_t<T> &rhs, const mat4_t<Q> &lhs)
    Product operator (see mat4::product).
  ------------------------------------------------------------------------------
  vec4_t<T>   operator * (const mat4_t<T> &rhs, const vec4_t<Q> &lhs)
  vec3_t<T>   operator * (const mat4_t<T> &rhs, const vec3_t<Q> &lhs)
    Product operators (see mat4::multiply for vectors).
  ------------------------------------------------------------------------------
  mat4_t<T>   operator * (const mat4_t<T> &rhs, const Q &lhs)
    Scaled operator (see mat4::scaled).
  ------------------------------------------------------------------------------
  mat4_t<T>   operator + (const mat4_t<T> &rhs, const mat4_t<Q> &lhs)
  mat4_t<T>   operator + (const mat4_t<T> &rhs, const Q &lhs)
    Sum operators (see mat4::sum).
  ------------------------------------------------------------------------------
  mat4_t<T>   operator - (const mat4_t<T> &rhs, const mat4_t<Q> &lhs)
  mat4_t<T>   operator - (const mat4_t<T> &rhs, const Q &lhs)
    Sum operators (see mat4::difference).
  ------------------------------------------------------------------------------
  bool        operator == (const mat4_t<T> &rhs, const mat4_t<Q> &lhs)
  bool        operator != (const mat4_t<T> &rhs, const mat4_t<Q> &lhs)
    Equality operators.
}
*/

namespace snow
{

template <typename t> struct vec3_t;
template <typename t> struct vec4_t;
template <typename t> struct quat_t;
template <typename t> struct mat3_t;

#include "inline/mat4.thh"

typedef mat4_t<float> mat4f_t;
typedef mat4_t<double> mat4d_t;

}

#include "vec3.hh"
#include "vec4.hh"
#include "quat.hh"

#endif /* end __SNOW_COMMON__MAT4_HH__ include guard */
