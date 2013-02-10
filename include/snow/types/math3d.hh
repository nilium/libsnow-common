// math3d.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_MATH3D_HH__
#define __SNOW_MATH3D_HH__

#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <cfloat>
#include <iostream>
#include <type_traits>

/*==============================================================================
                                   REFERENCE
================================================================================

This is a basic collection of templatized POD 3D maths structs for C++.

They're not optimized for SSE or any other special cases. Essentially, they are
generic and therefore likely to work just about anywhere a C++11 compiler will
work. So, more or less everywhere. In many instances, you'd likely be better
served by using something like GLM, though with this code, you can effectively
create an array of vec3_ts to represent vertices, screw with them, and then pass
them to OpenGL or something. So, there are some small benefits.

It may also help to have POD types for serialization, though in reality this
shouldn't be your concern with 3D maths types.


================================================================================


Included 3D maths types:
  - vec3_t<T>          3-component vector
  - vec4_t<T>          4-component vector
  - quat_t<T>          quaternion
  - line_t<T>          more or less a ray
  - mat4_t<T>          4x4 square matrix

All types must have a value_type of float or double. Long double may also work
but is completely untested and may break anything that depends on the data types
in the structures being packed.

================================================================================


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

  auto normalize() -> vec3_t&
    Get the result of normalizing this vector.
  ------------------------------------------------------------------------------
  auto normalized() const -> vec3_t
    Normalize this vector.
  ------------------------------------------------------------------------------
  auto magnitude() const -> value_type
    Get the magnitude of this vector.
  ------------------------------------------------------------------------------
  auto length() const -> value_type
    Get the squared length of this vector.
  ------------------------------------------------------------------------------
  auto difference(const vec3_t &other) const -> vec3_t
    Get the difference of this vector and another vector.
  ------------------------------------------------------------------------------
  auto subtract(const vec3_t &other) -> vec3_t&
    Subtract another vector from this vector.
  ------------------------------------------------------------------------------
  auto sum(const vec3_t &other) const -> vec3_t
    Get the sum of this vector and another vector.
  ------------------------------------------------------------------------------
  auto add(const vec3_t &other) -> vec3_t&
    Add another vector to this vector.
  ------------------------------------------------------------------------------
  auto scaled(const vec3_t &other) const -> vec3_t
    Get the result of scaling this vector by another vector.
  ------------------------------------------------------------------------------
  auto scale(const vec3_t &other) -> vec3_t&
    Scale this vector by another vector.
  ------------------------------------------------------------------------------
  auto scaled(value_type scalar) const -> vec3_t
    Get the result of scaling this vector by a scalar.
  ------------------------------------------------------------------------------
  auto scale(value_type scalar) -> vec3_t&
    Scale this vector by a scalar.
  ------------------------------------------------------------------------------
  auto negated() const -> vec3_t
    Get the negation of this vector.
  ------------------------------------------------------------------------------
  auto negate() -> vec3_t&
    Negate this vector.
  ------------------------------------------------------------------------------
  auto inverse() const -> vec4_t
    Returns the inverse of this vector. In other words, 1 / component for each
    component of the vector.
  ------------------------------------------------------------------------------
  auto invert() -> vec4_t&
    Inverts this vector.
  ------------------------------------------------------------------------------
  auto cross_product(const vec3_t &other) const -> vec3_t
    Get the cross product of this vector and another vector.
  ------------------------------------------------------------------------------
  auto dot_product(const vec3_t &other) const -> value_type
    Get the dot product of this vector and another vector.
  ------------------------------------------------------------------------------
  auto operator += (const vec3_t &other) -> vec3_t&
    Adds another vector to this vector.
  ------------------------------------------------------------------------------
  auto operator -= (const vec3_t &other) -> vec3_t&
    Subtracts another vector from this vector.
  ------------------------------------------------------------------------------
  auto operator *= (const vec3_t &other) -> vec3_t&
    Scales this vector by another vector.
  ------------------------------------------------------------------------------
  auto operator *= (value_type scalar) -> vec3_t&
    Scales this vector by a scalar.
  ------------------------------------------------------------------------------
  auto operator /= (const vec3_t &other) -> vec3_t&
    Scales this vector by the inverse of another vector.
  ------------------------------------------------------------------------------
  auto operator /= (value_type scalar) -> vec3_t&
    Scales this vector by one over the scalar provided. In the event that the
    scalar is zero, something bad might happen. Chances are it'll blow your
    foot off or something.
  ------------------------------------------------------------------------------
  auto operator - () const -> vec3_t
    Get the negation of this vector.
  ------------------------------------------------------------------------------
  auto operator ~ () const -> vec3_t
    Get the inverse of this vector.
  ------------------------------------------------------------------------------
  auto operator - (const vec3_t<T> &lhs, const vec3_t<Q> &rhs) -> vec3_t<T>
    Difference operator.
  ------------------------------------------------------------------------------
  auto operator + (const vec3_t<T> &lhs, const vec3_t<Q> &rhs) -> vec3_t<T>
    Sum operator.
  ------------------------------------------------------------------------------
  auto operator * (const vec3_t<T> &lhs, const vec3_t<Q> &rhs) -> vec3_t<T>
    Scale-by-vector operator.
  ------------------------------------------------------------------------------
  auto operator * (const vec3_t<T> &lhs, Q rhs) -> vec3_t<T>
    Scale-by-scalar operator.
  ------------------------------------------------------------------------------
  auto operator / (const vec3_t<T> &lhs, const vec3_t<Q> &rhs) -> vec3_t<T>
    Returns the product of scaling this vector by the inverse of another vector.
  ------------------------------------------------------------------------------
  auto operator / (const vec3_t<T> &lhs, Q rhs) -> vec3_t<T>
    Division operator - returns the result of scaling by one over rhs.
  ------------------------------------------------------------------------------
  auto operator % (const vec3_t<T> &lhs, const vec3_t<Q> &rhs) -> T
    Dot product operator.
  ------------------------------------------------------------------------------
  auto operator == (const vec3_t<T> &lhs, const vec3_t<Q> &rhs) -> bool
  auto operator != (const vec3_t<T> &lhs, const vec3_t<Q> &rhs) -> bool
    Equality operators.
  ------------------------------------------------------------------------------
  auto operator << (std::ostream &out, const vec3_t<T> &in) -> std::ostream&
    Write the vector to an ostream (as text).
  ------------------------------------------------------------------------------
  auto operator[] (int index) -> value_type&
  auto operator[] (int index) const -> value_type
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


================================================================================


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

  auto normalize() -> vec4_t&
    Get the result of normalizing this vector.
  ------------------------------------------------------------------------------
  auto normalized() const -> vec4_t
    Normalize this vector.
  ------------------------------------------------------------------------------
  auto magnitude() const -> value_type
    Get the magnitude of this vector.
  ------------------------------------------------------------------------------
  auto length() const -> value_type
    Get the squared length of this vector.
  ------------------------------------------------------------------------------
  auto difference(const vec4_t &other) const -> vec4_t
    Get the difference of this vector and another vector.
  ------------------------------------------------------------------------------
  auto subtract(const vec4_t &other) -> vec4_t&
    Subtract another vector from this vector.
  ------------------------------------------------------------------------------
  auto sum(const vec4_t &other) const -> vec4_t
    Get the sum of this vector and another vector.
  ------------------------------------------------------------------------------
  auto add(const vec4_t &other) -> vec4_t&
    Add another vector to this vector.
  ------------------------------------------------------------------------------
  auto scaled(const vec4_t &other) const -> vec4_t
    Get the result of scaling this vector by another vector.
  ------------------------------------------------------------------------------
  auto scale(const vec4_t &other) -> vec4_t&
    Scale this vector by another vector.
  ------------------------------------------------------------------------------
  auto scaled(value_type scalar) const -> vec4_t
    Get the result of scaling this vector by a scalar.
  ------------------------------------------------------------------------------
  auto scale(value_type scalar) -> vec4_t&
    Scale this vector by a scalar.
  ------------------------------------------------------------------------------
  auto negated() const -> vec4_t
    Get the negation of this vector.
  ------------------------------------------------------------------------------
  auto negate() -> vec4_t&
    Negate this vector.
    ------------------------------------------------------------------------------
  auto inverse() const -> vec4_t
    Returns the inverse of this vector. In other words, 1 / component for each
    component of the vector.
  ------------------------------------------------------------------------------
  auto invert() -> vec4_t&
    Inverts this vector.
  ------------------------------------------------------------------------------
  auto dot_product(const vec4_t &other) const -> value_type
    Get the dot product of this vector and another vector.
  ------------------------------------------------------------------------------
  auto operator += (const vec4_t &other) -> vec4_t&
    Adds another vector to this vector.
  ------------------------------------------------------------------------------
  auto operator -= (const vec4_t &other) -> vec4_t&
    Subtracts another vector from this vector.
  ------------------------------------------------------------------------------
  auto operator *= (const vec4_t &other) -> vec4_t&
    Scales this vector by another vector.
  ------------------------------------------------------------------------------
  auto operator *= (value_type scalar) -> vec4_t&
    Scales this vector by a scalar.
  ------------------------------------------------------------------------------
  auto operator /= (const vec4_t &other) -> vec4_t&
    Scales this vector by the inverse of another vector.
  ------------------------------------------------------------------------------
  auto operator /= (value_type scalar) -> vec4_t&
    Scales this vector by one over the scalar provided. In the event that the
    scalar is zero, something bad might happen. Chances are it'll blow your
    foot off or something.
  ------------------------------------------------------------------------------
  auto operator - () const -> vec4_t
    Get the negation of this vector.
  ------------------------------------------------------------------------------
  auto operator ~ () const -> vec4_t
    Get the inverse of this vector.
  ------------------------------------------------------------------------------
  auto operator - (const vec4_t<T> &lhs, const vec4_t<Q> &rhs) -> vec4_t<T>
    Difference operator.
  ------------------------------------------------------------------------------
  auto operator + (const vec4_t<T> &lhs, const vec4_t<Q> &rhs) -> vec4_t<T>
    Sum operator.
  ------------------------------------------------------------------------------
  auto operator * (const vec4_t<T> &lhs, const vec4_t<Q> &rhs) -> vec4_t<T>
    Scale-by-vector operator.
  ------------------------------------------------------------------------------
  auto operator * (const vec4_t<T> &lhs, Q rhs) -> vec4_t<T>
    Scale-by-scalar operator.
  ------------------------------------------------------------------------------
  auto operator / (const vec4_t<T> &lhs, const vec4_t<Q> &rhs) -> vec4_t<T>
    Returns the product of scaling this vector by the inverse of another vector.
  ------------------------------------------------------------------------------
  auto operator / (const vec4_t<T> &lhs, Q rhs) -> vec4_t<T>
    Division operator - returns the result of scaling by one over rhs.
  ------------------------------------------------------------------------------
  auto operator % (const vec4_t<T> &lhs, const vec4_t<Q> &rhs) -> T
    Dot product operator.
  ------------------------------------------------------------------------------
  auto operator == (const vec4_t<T> &lhs, const vec4_t<Q> &rhs) -> bool
  auto operator != (const vec4_t<T> &lhs, const vec4_t<Q> &rhs) -> bool
    Equality operators.
  ------------------------------------------------------------------------------
  auto operator << (std::ostream &out, const vec4_t<T> &in) -> std::ostream&
    Write the vector to an ostream (as text).
  ------------------------------------------------------------------------------
  auto operator[] (int index) -> value_type&
  auto operator[] (int index) const -> value_type
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


================================================================================


quat_t<value_type>
{
  ////// Member variables //////

  vec3_t      xyz
  value_type  w

  /////// Static Members ///////

  quat_t zero       -> Zero quaternion     -> { 0, 0, 0, 0 }
  quat_t one        -> One quaternion      -> { 1, 1, 1, 0 }
  quat_t identity   -> Identity quaternion -> { 0, 0, 0, 1 }


  auto from_mat4(const mat4_t& mat) -> quat_t
    Constructs a quaternion from a 4x4 matrix.
  ------------------------------------------------------------------------------
  auto from_angle_axis(value_type angle, const vec3_t &axis) -> quat_t
    Constructs a quaternion representation rotation about the given axis.

  ////// Instance Methods //////

  auto length() const -> value_type
    See vec4::length.
  ------------------------------------------------------------------------------
  auto magnitude() const -> value_type
    See vec4::magnitude.
  ------------------------------------------------------------------------------
  auto inverse() const -> quat_t
    Returns the inverse of this quaternion.
  ------------------------------------------------------------------------------
  auto invert() -> quat_t&
    Inverts this quaternion.
  ------------------------------------------------------------------------------
  auto negated() const -> quat_t
    Returns the negation of this quaternion (see vec4::negated).
  ------------------------------------------------------------------------------
  auto negate() -> quat_t&
    Negates this quaternion (see vec4::negate).
  ------------------------------------------------------------------------------
  auto product(const quat_t& other) const -> quat_t
    Gets the product of this and another quaternion.
  ------------------------------------------------------------------------------
  auto multiply(const quat_t& other) -> quat_t&
    Multiplies this quaternion by another quaternion.
  ------------------------------------------------------------------------------
  auto normalized() const -> quat_t
    See vec4::normalized.
  ------------------------------------------------------------------------------
  auto normalize() -> quat_t&
    See vec4::normalize.
  ------------------------------------------------------------------------------
  auto difference(const quat_t &other) const -> quat_t
    See vec4::difference.
  ------------------------------------------------------------------------------
  auto subtract(const quat_t &other) -> quat_t&
    See vec4::subtract.
  ------------------------------------------------------------------------------
  auto sum(const quat_t &other) const -> quat_t
    See vec4::sum.
  ------------------------------------------------------------------------------
  auto add(const quat_t &other) -> quat_t&
    See vec4::add.
  ------------------------------------------------------------------------------
  auto scaled(value_type scalar) const -> quat_t
    See vec4::scaled.
  ------------------------------------------------------------------------------
  auto scaled(const quat_t &other) const -> quat_t
    See vec4::scaled.
  ------------------------------------------------------------------------------
  auto scale(value_type scalar) -> quat_t&
    See vec4::scale.
  ------------------------------------------------------------------------------
  auto scale(const quat_t &other) -> quat_t&
    See vec4::scale.
  ------------------------------------------------------------------------------
  auto dot_product(const quat_t &other) const -> value_type
    See vec4::dot_product.
  ------------------------------------------------------------------------------
  auto slerp(const quat_t &to, value_type delta) const -> quat_t
    Interpolates from this quaternion to another using spherical linear
    interpolation and returns the result. For sufficiently small rotations,
    it will revert to linear interpolation (see quat::lerp).
  ------------------------------------------------------------------------------
  auto lerp(const quat_t &to, value_type delta) const -> quat_t
    Interpolates from this quaternion to another using linear interpolation.
    The result is always normalized.
  ------------------------------------------------------------------------------
  auto operator *= (const quat_t &other) -> quat_t&
    Multiplies this quaternion by another.
  ------------------------------------------------------------------------------
  auto operator *= (value_type scalar) -> quat_t&
    Scales this quaternion with a scalar.
  ------------------------------------------------------------------------------
  auto operator += (const quat_t &other) -> quat_t&
    Adds another quaternion to this quaternion.
  ------------------------------------------------------------------------------
  auto operator -= (const quat_t &other) -> quat_t&
    Subtracts another quaternion from this quaternion.
  ------------------------------------------------------------------------------
  auto operator - () const -> quat_t
    Returns the negation - not the inverse - of this quaternion.
  ------------------------------------------------------------------------------
  auto operator ~ () const -> quat_t
    Returns the inverse of this quaternion.
  ------------------------------------------------------------------------------
  auto operator * (const quat_t<T> &lhs, const quat_t<Q> &rhs) -> quat_t<T>
    Returns the product of this quaternion and another quaternion.
  ------------------------------------------------------------------------------
  auto operator * (const quat_t<T> &lhs, Q rhs) -> quat_t<T>
    Returns the product of this quaternion and a scalar.
  ------------------------------------------------------------------------------
  auto operator + (const quat_t<T> &lhs, const quat_t<Q> &rhs) -> quat_t<T>
    Returns the sum of this quaternion and another quaternion.
  ------------------------------------------------------------------------------
  auto operator - (const quat_t<T> &lhs, const quat_t<Q> &rhs) -> quat_t<T>
    Returns the difference of this quaternion and another quaternion.
  ------------------------------------------------------------------------------
  auto operator % (const quat_t<T> &lhs, const quat_t<Q> &rhs) -> T
    Returns the dot product of this quaternion and another quaternion.
  ------------------------------------------------------------------------------
  auto operator == (const quat_t<T> &lhs, const quat_t<Q> &rhs) -> bool
  auto operator != (const quat_t<T> &lhs, const quat_t<Q> &rhs) -> bool
    Equality operators.
  ------------------------------------------------------------------------------
  auto operator[] (int index) -> value_type&
  auto operator[] (int index) const -> value_type
    Get reference to member at index (0 = x, 1 = y, 2 = z, 3 = w; any other
    index throws an exception).
  ------------------------------------------------------------------------------
  operator value_type* ()
  operator const value_type* () const
    Convert to value_type pointer (same as getting &xyz.x).
  ------------------------------------------------------------------------------
  auto operator << (std::ostream &out, const quat_t<T> &in) -> std::ostream&
    Writes this quaternion to an output stream (uses text).
  ------------------------------------------------------------------------------
  operator quat_t<Q> () const
    Cast this quaternion to a quaternion of value_type Q.
}


================================================================================


line_t<value_type>
{
  ////// Member variables //////

  vec3_t  origin
  vec3_t  dist

  ////// Instance Methods //////

  auto end() const -> vec3
    Get the end-point of the line.
  ------------------------------------------------------------------------------
  auto translated(const vec3 &d) const -> line_t
  auto translate(const vec3 &d) -> line_t&
    Translate the line's origin by vector `d`.
  ------------------------------------------------------------------------------
  auto scaled(value_type d) const -> line_t
  auto scale(value_type d) -> line_t&
    Scale the line by scalar `d`.
  ------------------------------------------------------------------------------
  auto nearest_to(const vec3& p) const -> vec3
    Get the nearest point on the line to point `p`.
}


================================================================================


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


  auto translation(const vec3 &off) -> mat4_t
    Creates a translation matrix.
  ------------------------------------------------------------------------------
  auto scaling(const vec3 &off) -> mat4_t
    Creates a scale matrix. Named 'scaling' due to name overlap with
    mat4::scale(vec3) below.
  ------------------------------------------------------------------------------
  auto rotation(value_type angle, const vec3 &axis) -> mat4_t
    Creates a rotation matrix.
  ------------------------------------------------------------------------------
  auto frustum(value_type left, value_type right,
               value_type bottom, value_type top,
               value_type near, value_type far) -> mat4_t
    Creates a perspective projection matrix.
  ------------------------------------------------------------------------------
  auto orthographic(value_type left, value_type right,
                    value_type top, value_type bottom,
                    value_type near, value_type far) -> mat4_t
    Creates an orthographic projection matrix.
  ------------------------------------------------------------------------------
  auto perspective(value_type fovY, value_type aspect,
                   value_type near, value_type far) -> mat4_t
    Creates a perspective projection matrix.
  ------------------------------------------------------------------------------
  auto look_at(const vec3 &eye, const vec3& center, const vec3& up) -> mat4_t
    Creates a look-at matrix.
  ------------------------------------------------------------------------------
  auto from_quat(const quat &in) -> mat4_t
    Converts the given quaternion to a mat4.


  ////// Instance Methods //////

  auto translate(const vec3 &translation) -> mat4_t&
  auto translated(const vec3 &translation) const -> mat4_t
    Translate this matrix or get the translated matrix.
    In both cases, translates the matrix using a 3-component vector.
  ------------------------------------------------------------------------------
  auto transpose()        -> mat4_t&
  auto transposed() const -> mat4_t
    Tranpose this matrix or get the transposed matrix.
  ------------------------------------------------------------------------------
  auto rowvec4(int index) const -> vec4
  auto colvec4(int index) const -> vec4
  auto set_rowvec4(int index, const vec4 &row) -> mat4_t&
  auto set_colvec4(int index, const vec4 &col) -> mat4_t&
    Get and set 4-component row and column vectors.
  ------------------------------------------------------------------------------
  auto rowvec3(int index) const -> vec3
  auto colvec3(int index) const -> vec3
  auto set_rowvec3(int index, const vec3 &row) -> mat4_t&
  auto set_colvec3(int index, const vec3 &col) -> mat4_t&
    Get and set 3-component row and column vectors. Does not modify the fourth
    row or column for given columns and rows when using set_*vec3().
  ------------------------------------------------------------------------------
  auto inverse_orthogonal() -> mat4_t
    Get the inverse orthogonal matrix.
  ------------------------------------------------------------------------------
  auto negated() const -> mat4_t
  auto negate()        -> mat4_t&
    Get the negation of this matrix. Simply put, this just negates all elements
    of the matrix.
  ------------------------------------------------------------------------------
  auto sum(const mat4_t &other) const -> mat4_t
  auto sum(value_type scalar) const   -> mat4_t
    Get the sum of this matrix and another matrix or a scalar value.
  ------------------------------------------------------------------------------
  auto add(const mat4_t &other) -> mat4_t&
  auto add(value_type scalar)   -> mat4_t&
    Add a matrix or a scalar value to this matrix.
  ------------------------------------------------------------------------------
  auto difference(const mat4_t &other) const -> mat4_t
  auto difference(value_type scalar) const   -> mat4_t
    Get the difference of this matrix and another matrix or a scalar value.
  ------------------------------------------------------------------------------
  auto subtract(const mat4_t &other) -> mat4_t&
  auto subtract(value_type scalar)   -> mat4_t&
    Subtract a matrix or a scalar value from this matrix.
  ------------------------------------------------------------------------------
  auto scaled(value_type scalar) const -> mat4_t
  auto scale(value_type scalar) -> mat4_t&
    Get the scaled matrix or scale the matrix by a scalar value.
  ------------------------------------------------------------------------------
  auto scaled(const mat4_t &other) const -> mat4_t
  auto scale(const mat4_t &other) -> mat4_t&
    Get the scaled matrix or scale the matrix by another matrix.
  ------------------------------------------------------------------------------
  auto scaled(const vec3 &vec) const -> mat4_t
  auto scale(const vec3 &vec) -> mat4_t&
    Get the scaled matrix or scale the matrix by a vec3 (this affects only
    columns/rows < 3).
  ------------------------------------------------------------------------------
  auto inverse_affine(mat4_t &out) const -> bool
    Get the inverse affine matrix of this matrix.
  ------------------------------------------------------------------------------
  auto adjoint() const -> mat4_t
    Get the adjoint of this matrix.
  ------------------------------------------------------------------------------
  auto cofactor(int r0, int r1, int r2, int c0, int c1, int c2) const -> value_type
    Get the cofactor of the given indices.
  ------------------------------------------------------------------------------
  auto determinant() const -> value_type
    Get the determinant of the matrix.
  ------------------------------------------------------------------------------
  auto inverse_general(mat4_t &out) const -> bool
    Gets the inverse of this matrix if possible. Returns the inverse to the
    output argument. Returns true if successful, false if not.
  ------------------------------------------------------------------------------
  auto product(const mat4_t &other) const -> mat4_t
  auto multiply(const mat4_t &other) -> mat4_t&
    Get the product of multipling this and another matrix together, or multiply
    this matrix by another.
  ------------------------------------------------------------------------------
  auto multiply(const vec4 &vec) const -> vec4
  auto multiply(const vec3 &vec) const -> vec3
    Transform a vec4 and vec3, respectively.
  ------------------------------------------------------------------------------
  auto rotate(const vec3 &vec) const -> vec3
  auto inverse_rotate(const vec3 &vec) const -> vec3
    Rotate a vec3. rotate() is the same as multiply(vec3), albeit without
    translation added.
  ------------------------------------------------------------------------------
  auto operator[] (int index) -> value_type&
  auto operator[] (int index) const -> value_type
    Returns a reference (or the value at) the index in the matrix. Indices
    outside the range [0-15] will throw an exception.
  ------------------------------------------------------------------------------
  operator value_type* ()
  operator const value_type* () const
    Returns a pointer to the first element of the matrix.
  ------------------------------------------------------------------------------
  auto operator *= (const mat4_t &other) -> mat4_t&
    Multiplies this mat4 by the other mat4.
  ------------------------------------------------------------------------------
  auto operator *= (value_type scalar) -> mat4_t&
    Scales this matrix by the given scalar or vec3.
  ------------------------------------------------------------------------------
  auto operator += (const mat4_t &other) -> mat4_t&
  auto operator += (value_type scalar) -> mat4_t&
    Add operators (see mat4::add).
  ------------------------------------------------------------------------------
  auto operator -= (const mat4_t &other) -> mat4_t&
  auto operator -= (value_type scalar) -> mat4_t&
    Subtract operators (see mat4::subtract).
  ------------------------------------------------------------------------------
  auto operator * (const mat4_t<T> &rhs, const mat4_t<Q> &lhs) -> mat4_t<T>
    Product operator (see mat4::product).
  ------------------------------------------------------------------------------
  auto operator * (const mat4_t<T> &rhs, const vec4_t<Q> &lhs) -> vec4_t<T>
  auto operator * (const mat4_t<T> &rhs, const vec3_t<Q> &lhs) -> vec3_t<T>
    Product operators (see mat4::multiply for vectors).
  ------------------------------------------------------------------------------
  auto operator * (const mat4_t<T> &rhs, const Q &lhs) -> mat4_t<T>
    Scaled operator (see mat4::scaled).
  ------------------------------------------------------------------------------
  auto operator + (const mat4_t<T> &rhs, const mat4_t<Q> &lhs) -> mat4_t<T>
  auto operator + (const mat4_t<T> &rhs, const Q &lhs) -> mat4_t<T>
    Sum operators (see mat4::sum).
  ------------------------------------------------------------------------------
  auto operator - (const mat4_t<T> &rhs, const mat4_t<Q> &lhs) -> mat4_t<T>
  auto operator - (const mat4_t<T> &rhs, const Q &lhs) -> mat4_t<T>
    Sum operators (see mat4::difference).
  ------------------------------------------------------------------------------
  auto operator == (const mat4_t<T> &rhs, const mat4_t<Q> &lhs) -> bool
  auto operator != (const mat4_t<T> &rhs, const mat4_t<Q> &lhs) -> bool
    Equality operators.

}

================================================================================
                                 END REFERENCE
==============================================================================*/


namespace snow {


// General degrees-to-radians constant
#define S_DEG2RAD (0.01745329251)
#define S_EPSILON (1e-5)



/*==============================================================================
  is_zero

    Tests for whether a value is zero. Specialized for float, double, and long
    double types to see if the value is less than their respective epsilons.
    For all other types, simply returns (v == 0).
==============================================================================*/
template <typename T>
inline
bool is_zero(T value)
{
  return value == T(0);
}

template <>
inline
bool is_zero<float>(float value)
{
  return std::abs(value) < S_EPSILON;
}

template <>
inline
bool is_zero<double>(double value)
{
  return std::abs(value) < S_EPSILON;
}

template <>
inline
bool is_zero<long double>(long double value)
{
  return std::abs(value) < S_EPSILON;
}



/*==============================================================================
  max_value

    Returns the maximum value for a given type. Only intended for use with
    float, double, and long double. Other types will throw a runtime_error.
==============================================================================*/
template <typename T>
inline
T max_value() { throw std::runtime_error("no set maximum for type"); }

template <>
inline
float max_value<float>() { return FLT_MAX; }

template <>
inline
double max_value<double>() { return DBL_MAX; }

template <>
inline
long double max_value<long double>() { return LDBL_MAX; }



/*==============================================================================
  clamp_value

    Clamps a value from the given minimum value to the maximum value.
==============================================================================*/
template <typename T>
inline
T clamp_value(T val, T min, T max)
{
  return (val < min ? min : (val > max ? max : val));
}

template <typename T, typename Q, typename R>
inline
T clamp_value(T val, Q min, R max)
{
  return (val < min ? static_cast<T>(min) : (val > max ? static_cast<T>(max) : val));
}



/*==============================================================================

  Define mat4_t ahead of time

==============================================================================*/

template <typename T>
struct mat4_t;



/*==============================================================================

  3-Component Vector

==============================================================================*/

template <typename T = float>
struct alignas(4) vec3_t {
  static_assert(std::is_floating_point<T>::value,
                "value_type must be floating point type");
  static_assert(std::is_scalar<T>::value,
                "value_type must be scalar type");

  typedef T value_type;

  value_type x, y, z;

  static const vec3_t zero;
  static const vec3_t one;

  auto normalize() -> vec3_t&
  {
    value_type one_over_mag = magnitude();
    if (one_over_mag != 0) {
      one_over_mag = value_type(1) / one_over_mag;
    }

    x *= one_over_mag;
    y *= one_over_mag;
    z *= one_over_mag;

    return *this;
  }

  auto normalized() const -> vec3_t
  {
    value_type one_over_mag = magnitude();
    if (one_over_mag != 0) {
      one_over_mag = value_type(1) / one_over_mag;
    }

    return {
      x * one_over_mag,
      y * one_over_mag,
      z * one_over_mag
    };
  }

  auto magnitude() const -> value_type
  {
    const value_type sqrlen = length();
    return sqrlen != 0
           ? static_cast<value_type>(std::sqrt(sqrlen))
           : 0;
  }

  auto length() const -> value_type
  {
    return x * x + y * y + z * z;
  }

  auto difference(const vec3_t &other) const -> vec3_t
  {
    return {
      x - other.x,
      y - other.y,
      z - other.z
    };
  }

  auto subtract(const vec3_t &other) -> vec3_t&
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  auto sum(const vec3_t &other) const -> vec3_t
  {
    return {
      x + other.x,
      y + other.y,
      z + other.z
    };
  }

  auto add(const vec3_t &other) -> vec3_t&
  {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  auto scaled(value_type scalar) const -> vec3_t
  {
    return {
      x * scalar,
      y * scalar,
      z * scalar
    };
  }

  auto scaled(const vec3_t &other) const -> vec3_t
  {
    return {
      x * other.x,
      y * other.y,
      z * other.z
    };
  }

  auto scale(value_type scalar) -> vec3_t&
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  auto scale(const vec3_t &other) -> vec3_t&
  {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
  }

  auto negated() const -> vec3_t
  {
    return {
      -x, -y, -z
    };
  }

  auto negate() -> vec3_t&
  {
    x = -x;
    y = -y;
    z = -z;
    return *this;
  }

  auto inverse() const -> vec3_t
  {
    return {
      (x != 0 ? value_type(1) / x : x),
      (y != 0 ? value_type(1) / y : y),
      (z != 0 ? value_type(1) / z : z)
    };
  }

  auto invert() -> vec3_t&
  {
    if (x != 0) {
      x = value_type(1) / x;
    }
    if (y != 0) {
      y = value_type(1) / y;
    }
    if (z != 0) {
      z = value_type(1) / z;
    }
    return *this;
  }

  auto cross_product(const vec3_t &other) const -> vec3_t
  {
    return {
      y * other.z - z * other.y,
      z * other.x - x * other.z,
      x * other.y - y * other.x
    };
  }

  auto dot_product(const vec3_t &other) const -> value_type
  {
    return x * other.x + y * other.y + z * other.z;
  }

  auto operator += (const vec3_t &other) -> vec3_t&
  {
    return add(other);
  }

  auto operator -= (const vec3_t &other) -> vec3_t&
  {
    return subtract(other);
  }

  auto operator *= (value_type scalar) -> vec3_t&
  {
    return scale(scalar);
  }

  auto operator *= (const vec3_t &other) -> vec3_t&
  {
    return scale(other);
  }

  auto operator /= (value_type scalar) -> vec3_t&
  {
    return scale(value_type(1) / scalar);
  }

  auto operator /= (const vec3_t &other) -> vec3_t&
  {
    return scale(other.inverse());
  }

  auto operator - () const -> vec3_t
  {
    return negated();
  }

  auto operator ~ () const -> vec3_t
  {
    return inverse();
  }

  auto operator[] (int index) -> value_type&
  {
    static_assert(std::is_pod<vec3_t>::value, "vec3 must be POD to use subscript operator");
    if (index < 0 || index > 2)
      throw std::out_of_range("attempt to access out of range element");
    return (&x)[index];
  }

  auto operator[] (int index) const -> value_type
  {
    static_assert(std::is_pod<vec3_t>::value, "vec3 must be POD to use subscript operator");
    if (index < 0 || index > 2)
      throw std::out_of_range("attempt to access out of range element");
    return (&x)[index];
  }

  operator value_type* ()
  {
    static_assert(std::is_pod<vec3_t>::value, "vec3 must be POD to cast to value_type pointer");
    return &x;
  }

  operator const value_type* () const
  {
    static_assert(std::is_pod<vec3_t>::value, "vec3 must be POD to cast to value_type pointer");
    return &x;
  }

  template <typename Q>
  operator vec3_t<Q> () const
  {
    return {
      static_cast<Q>(x),
      static_cast<Q>(y),
      static_cast<Q>(z)
    };
  }
};

template <typename T>
const vec3_t<T> vec3_t<T>::zero = { 0, 0, 0 };

template <typename T>
const vec3_t<T> vec3_t<T>::one = { 1, 1, 1 };

template <typename T>
inline
auto operator << (std::ostream &out, const vec3_t<T> &in) -> std::ostream&
{
  return out << "{x:" << in.x << ", y:" << in.y << ", z:" << in.z << "}";
}

template <typename T, typename Q>
inline
auto operator - (const vec3_t<T> &lhs,
                 const vec3_t<Q> &rhs) -> vec3_t<T>
{
  return lhs.difference(rhs);
}

template <typename T, typename Q>
inline
auto operator + (const vec3_t<T> &lhs,
                 const vec3_t<Q> &rhs) -> vec3_t<T>
{
  return lhs.sum(rhs);
}

template <typename T, typename Q>
inline
auto operator * (const vec3_t<T> &lhs,
                 const vec3_t<Q> &rhs) -> vec3_t<T>
{
  return lhs.scaled(rhs);
}

template <typename T, typename Q>
inline
auto operator * (const vec3_t<T> &lhs,
                 Q rhs) -> vec3_t<T>
{
  return lhs.scaled(static_cast<T>(rhs));
}

template <typename T, typename Q>
inline
auto operator / (const vec3_t<T> &lhs,
                 const vec3_t<Q> &rhs) -> vec3_t<T>
{
  return lhs.scaled(rhs.inverse());
}

template <typename T, typename Q>
inline
auto operator / (const vec3_t<T> &lhs,
                 Q rhs) -> vec3_t<T>
{
  return lhs.scaled(T(1) / static_cast<T>(rhs));
}

template <typename T, typename Q>
inline
auto operator % (const vec3_t<T> &lhs,
                 const vec3_t<Q> &rhs) -> T
{
  return lhs.dot_product(rhs);
}

template <typename T, typename Q>
inline
auto operator == (const vec3_t<T> &lhs,
                 const vec3_t<Q> &rhs) -> bool
{
  return
    is_zero(lhs.x - rhs.x) &&
    is_zero(lhs.y - rhs.y) &&
    is_zero(lhs.z - rhs.z);;
}

template <typename T, typename Q>
inline
auto operator != (const vec3_t<T> &lhs,
                  const vec3_t<Q> &rhs) -> bool
{
  return !(lhs == rhs);
}



/*==============================================================================

  4-Component Vector

==============================================================================*/

template <typename T = float>
struct alignas(4) vec4_t {
  static_assert(std::is_floating_point<T>::value,
                "value_type must be floating point type");
  static_assert(std::is_scalar<T>::value,
                "value_type must be scalar type");

  typedef T value_type;

  value_type x, y, z, w;

  static const vec4_t zero;
  static const vec4_t one;

  auto normalize() -> vec4_t&
  {
    value_type one_over_mag = magnitude();
    if (one_over_mag != 0) {
      one_over_mag = value_type(1) / one_over_mag;
    }

    x *= one_over_mag;
    y *= one_over_mag;
    z *= one_over_mag;
    w *= one_over_mag;

    return *this;
  }

  auto normalized() const -> vec4_t
  {
    value_type one_over_mag = magnitude();
    if (one_over_mag != 0) {
      one_over_mag = value_type(1) / one_over_mag;
    }

    return {
      x * one_over_mag,
      y * one_over_mag,
      z * one_over_mag,
      w * one_over_mag
    };
  }

  auto magnitude() const -> value_type
  {
    const value_type sqrlen = length();
    return sqrlen != 0
           ? static_cast<value_type>(std::sqrt(sqrlen))
           : 0;
  }

  auto length() const -> value_type
  {
    return x * x + y * y + z * z + w * w;
  }

  auto difference(const vec4_t &other) const -> vec4_t
  {
    return {
      x - other.x,
      y - other.y,
      z - other.z,
      w - other.w
    };
  }

  auto subtract(const vec4_t &other) -> vec4_t&
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
  }

  auto sum(const vec4_t &other) const -> vec4_t
  {
    return {
      x + other.x,
      y + other.y,
      z + other.z,
      w + other.w
    };
  }

  auto add(const vec4_t &other) -> vec4_t&
  {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
  }

  auto scaled(value_type scalar) const -> vec4_t
  {
    return {
      x * scalar,
      y * scalar,
      z * scalar,
      w * scalar
    };
  }

  auto scaled(const vec4_t &other) const -> vec4_t
  {
    return {
      x * other.x,
      y * other.y,
      z * other.z,
      w * other.w
    };
  }

  auto scale(value_type scalar) -> vec4_t&
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
  }

  auto scale(const vec4_t &other) -> vec4_t&
  {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;
    return *this;
  }

  auto negated() const -> vec4_t
  {
    return {
      -x, -y, -z, -w
    };
  }

  auto negate() -> vec4_t&
  {
    x = -x;
    y = -y;
    z = -z;
    w = -w;
    return *this;
  }

  auto inverse() const -> vec4_t
  {
    return {
      (x != 0 ? value_type(1) / x : x),
      (y != 0 ? value_type(1) / y : y),
      (z != 0 ? value_type(1) / z : z),
      (w != 0 ? value_type(1) / w : w)
    };
  }

  auto invert() -> vec4_t&
  {
    if (x != 0) {
      x = value_type(1) / x;
    }
    if (y != 0) {
      y = value_type(1) / y;
    }
    if (z != 0) {
      z = value_type(1) / z;
    }
    if (w != 0) {
      w = value_type(1) / w;
    }
    return *this;
  }

  auto dot_product(const vec4_t &other) const -> value_type
  {
    return x * other.x + y * other.y + z * other.z + w * other.w;
  }

  auto operator += (const vec4_t &other) -> vec4_t&
  {
    return add(other);
  }

  auto operator -= (const vec4_t &other) -> vec4_t&
  {
    return subtract(other);
  }

  auto operator *= (value_type scalar) -> vec4_t&
  {
    return scale(scalar);
  }

  auto operator *= (const vec4_t &other) -> vec4_t&
  {
    return scale(other);
  }

  auto operator /= (value_type scalar) -> vec4_t&
  {
    return scale(value_type(1) / scalar);
  }

  auto operator /= (const vec4_t &other) -> vec4_t&
  {
    return scale(other.inverse());
  }

  auto operator - () const -> vec4_t
  {
    return negated();
  }

  auto operator ~ () const -> vec4_t
  {
    return inverse();
  }

  auto operator[] (int index) -> value_type&
  {
    static_assert(std::is_pod<vec4_t>::value, "vec4 must be POD to use subscript operator");
    if (index < 0 || index > 3)
      throw std::out_of_range("attempt to access out of range element");
    return (&x)[index];
  }

  auto operator[] (int index) const -> value_type
  {
    static_assert(std::is_pod<vec4_t>::value, "vec4 must be POD to use subscript operator");
    if (index < 0 || index > 3)
      throw std::out_of_range("attempt to access out of range element");
    return (&x)[index];
  }

  operator value_type* ()
  {
    static_assert(std::is_pod<vec4_t>::value, "vec4 must be POD to cast to value_type pointer");
    return &x;
  }

  operator const value_type* () const
  {
    static_assert(std::is_pod<vec4_t>::value, "vec4 must be POD to cast to value_type pointer");
    return &x;
  }

  template <typename Q>
  operator vec4_t<Q> () const
  {
    return {
      static_cast<Q>(x),
      static_cast<Q>(y),
      static_cast<Q>(z),
      static_cast<Q>(w)
    };
  }
};

template <typename T>
const vec4_t<T> vec4_t<T>::zero = { 0, 0, 0, 0 };

template <typename T>
const vec4_t<T> vec4_t<T>::one = { 1, 1, 1, 1 };

template <typename T>
inline
auto operator << (std::ostream &out, const vec4_t<T> &in) -> std::ostream&
{
  return out << "{x:" << in.x << ", y:" << in.y << ", z:" << in.z << ", w:" << in.w << "}";
}

template <typename T, typename Q>
inline
auto operator - (const vec4_t<T> &lhs,
                 const vec4_t<Q> &rhs) -> vec4_t<T>
{
  return lhs.difference(rhs);
}

template <typename T, typename Q>
inline
auto operator + (const vec4_t<T> &lhs,
                 const vec4_t<Q> &rhs) -> vec4_t<T>
{
  return lhs.sum(rhs);
}

template <typename T, typename Q>
inline
auto operator * (const vec4_t<T> &lhs,
                 const vec4_t<Q> &rhs) -> vec4_t<T>
{
  return lhs.scaled(rhs);
}

template <typename T, typename Q>
inline
auto operator * (const vec4_t<T> &lhs,
                 Q rhs) -> vec4_t<T>
{
  return lhs.scaled(static_cast<T>(rhs));
}

template <typename T, typename Q>
inline
auto operator / (const vec4_t<T> &lhs,
                 const vec4_t<Q> &rhs) -> vec4_t<T>
{
  return lhs.scaled(rhs.inverse());
}

template <typename T, typename Q>
inline
auto operator / (const vec4_t<T> &lhs,
                 Q rhs) -> vec4_t<T>
{
  return lhs.scaled(T(1) / static_cast<T>(rhs));
}

template <typename T, typename Q>
inline
auto operator % (const vec4_t<T> &lhs,
                 const vec4_t<Q> &rhs) -> T
{
  return lhs.dot_product(rhs);
}

template <typename T, typename Q>
inline
auto operator == (const vec4_t<T> &lhs,
                 const vec4_t<Q> &rhs) -> bool
{
  return
    is_zero(lhs.x - rhs.x) &&
    is_zero(lhs.y - rhs.y) &&
    is_zero(lhs.z - rhs.z) &&
    is_zero(lhs.w - rhs.w);
}

template <typename T, typename Q>
inline
auto operator != (const vec4_t<T> &lhs,
                  const vec4_t<Q> &rhs) -> bool
{
  return !(lhs == rhs);
}



/*==============================================================================

  Quaternion

==============================================================================*/

template <typename T = float>
struct alignas(4) quat_t {
  static_assert(std::is_floating_point<T>::value,
                "value_type must be floating point type");
  static_assert(std::is_scalar<T>::value,
                "value_type must be scalar type");

  typedef T value_type;
  typedef vec3_t<value_type> vec3;

  vec3 xyz;
  value_type w;

  static const quat_t zero;
  static const quat_t one;
  static const quat_t identity;

  static
  auto from_angle_axis(value_type angle, const vec3 &axis) -> quat_t;

  static
  auto from_mat4(const mat4_t<value_type> &mat) -> quat_t;

  auto length() const -> value_type
  {
    return xyz.length() + w * w;
  }

  auto magnitude() const -> value_type
  {
    const value_type sqrlen = length();
    return sqrlen != 0
           ? static_cast<value_type>(std::sqrt(sqrlen))
           : 0;
  }

  auto inverse() const -> quat_t
  {
    return {
      -xyz, w
    };
  }

  auto invert() -> quat_t&
  {
    xyz.negate();
    return *this;
  }

  auto negated() const -> quat_t
  {
    return {
      -xyz, -w
    };
  }

  auto negate() -> quat_t&
  {
    invert();
    w = -w;
    return *this;
  }

  auto product(const quat_t& other) const -> quat_t
  {
    value_type wt, w1, w2;
    vec3 temp_xyz;
    w1 = w;
    w2 = other.w;

    wt = w1 * w2 - xyz.dot_product(other.xyz);
    temp_xyz = other.xyz * w1;
    temp_xyz += xyz * w2;
    temp_xyz += other.xyz.cross_product(xyz);
    return {
      temp_xyz, wt
    };
  }

  auto multiply(const quat_t& other) -> quat_t&
  {
    value_type w1, w2;
    vec3 temp_xyz;
    w1 = w;
    w2 = other.w;

    w = w1 * w2 - xyz.dot_product(other.xyz);
    temp_xyz = other.xyz * w1;
    temp_xyz += xyz * w2;
    temp_xyz += other.xyz.cross_product(xyz);
    xyz = temp_xyz;
    return *this;
  }

  auto normalized() const -> quat_t
  {
    value_type sqrlen = xyz.length() + w * w;
    if (sqrlen != 0)
      sqrlen = value_type(1) / std::sqrt(sqrlen);

    return {
      xyz * sqrlen,
      w * sqrlen
    };
  }

  auto normalize() -> quat_t&
  {
    value_type sqrlen = xyz.length() + w * w;
    if (sqrlen != 0)
      sqrlen = value_type(1) / std::sqrt(sqrlen);

    xyz *= sqrlen;
    w *= sqrlen;

    return *this;
  }

  auto difference(const quat_t &other) const -> quat_t
  {
    return {
      xyz - other.xyz,
      w - other.w
    };
  }

  auto subtract(const quat_t &other) -> quat_t&
  {
    xyz -= other.xyz;
    w -= other.w;
    return *this;
  }

  auto sum(const quat_t &other) const -> quat_t
  {
    return {
      xyz + other.xyz,
      w + other.w
    };
  }

  auto add(const quat_t &other) -> quat_t&
  {
    xyz += other.xyz;
    w += other.w;
    return *this;
  }

  auto scaled(value_type scalar) const -> quat_t
  {
    return {
      xyz * scalar,
      w * scalar
    };
  }

  auto scaled(const quat_t &other) const -> quat_t
  {
    return {
      xyz * other.xyz,
      w * other.w
    };
  }

  auto scale(value_type scalar) -> quat_t&
  {
    xyz *= scalar;
    w *= scalar;
    return *this;
  }

  auto scale(const quat_t &other) -> quat_t&
  {
    xyz *= other.xyz;
    w *= other.w;
    return *this;
  }

  auto dot_product(const quat_t &other) const -> value_type
  {
    return xyz.dot_product(other.xyz) + w * other.w;
  }

  auto slerp(const quat_t &to, value_type delta) const -> quat_t;

  auto lerp(const quat_t &to, value_type delta) const -> quat_t;

  auto operator *= (const quat_t &other) -> quat_t&
  {
    return multiply(other);
  }

  auto operator *= (value_type scalar) -> quat_t&
  {
    return scale(scalar);
  }

  auto operator += (const quat_t &other) -> quat_t&
  {
    return add(other);
  }

  auto operator -= (const quat_t &other) -> quat_t&
  {
    return subtract(other);
  }

  auto operator - () const -> quat_t
  {
    return negated();
  }

  auto operator ~ () const -> quat_t
  {
    return inverse();
  }

  auto operator[] (int index) -> value_type&
  {
    static_assert(std::is_pod<quat_t>::value, "quaternion must be POD to use subscript operator");
    if (index < 0 || index > 15)
      throw std::out_of_range("attempt to access out of range element");
    return (&xyz.x)[index];
  }

  auto operator[] (int index) const -> value_type
  {
    static_assert(std::is_pod<quat_t>::value, "quaternion must be POD to use subscript operator");
    if (index < 0 || index > 15)
      throw std::out_of_range("attempt to access out of range element");
    return (&xyz.x)[index];
  }

  operator value_type* ()
  {
    static_assert(std::is_pod<quat_t>::value, "quaternion must be POD to cast to value_type pointer");
    return &xyz.x;
  }

  operator const value_type* () const
  {
    static_assert(std::is_pod<quat_t>::value, "quaternion must be POD to cast to value_type pointer");
    return &xyz.x;
  }

  template <typename Q>
  operator quat_t<Q> () const
  {
    return {
      static_cast<vec3_t<Q> >(xyz),
      static_cast<Q>(w)
    };
  }
};

template <typename T>
const quat_t<T> quat_t<T>::zero = { { 0, 0, 0 }, 0 };

template <typename T>
const quat_t<T> quat_t<T>::one = { { 1, 1, 1 }, 1 };

template <typename T>
const quat_t<T> quat_t<T>::identity = { { 0, 0, 0 }, 1 };

template <typename T>
auto quat_t<T>::from_angle_axis(value_type angle, const vec3 &axis) -> quat_t
{
  vec3 axis_n = axis.normalized();
  value_type s;
  angle *= S_DEG2RAD * 0.5;
  s = std::sin(angle);

  return {
    axis_n * s,
    std::cos(angle)
  };
}

template <typename T>
auto quat_t<T>::from_mat4(const mat4_t<value_type> &mat) -> quat_t
{
  quat_t out;
  value_type r;
  const value_type trace = mat.m00 + mat.m11 + mat.m22;
  const value_type *m = (value_type *)m;
  const value_type m01 = mat.m01, m02 = mat.m02, m10 = mat.m10,
                   m12 = mat.m12, m20 = mat.m20, m21 = mat.m21;

  if (trace > 0) {
    r = std::sqrt(trace + 1);
    out.w = r * 0.5;
    r = 0.5 / r;
    out.xyz = {
      (m12 - m21) * r,
      (m20 - m02) * r,
      (m01 - m10) * r
    };
  } else {
    int index = 0;
    if (mat.m11 > mat.m00) {
      index = 1;
    }
    if (mat.m22 > (index ? mat.m11 : mat.m00)) {
      index = 2;
    }

    switch (index) {
    default:
    case 0:
      r = out.xyz.x = std::sqrt(mat.m00 - (mat.m11 + mat.m22) + 1) * 0.5;
      if (r != 0) r = 0.5 / r;
      out.xyz.y = (m10 + m01) * r;
      out.xyz.z = (m20 + m02) * r;
      out.w     = (m12 - m21) * r;
      break;

    case 1:
      r = out.xyz.y = std::sqrt(mat.m11 - (mat.m22 + mat.m00) + 1) * 0.5;
      if (r != 0) r = 0.5 / r;
      out.xyz.x = (m10 + m01) * r;
      out.xyz.z = (m12 + m21) * r;
      out.w     = (m20 - m02) * r;
      break;

    case 2:
      r = out.xyz.z = std::sqrt(mat.m11 - (mat.m00 + mat.m11) + 1) * 0.5;
      if (r != 0) r = 0.5 / r;
      out.xyz.x = (m20 + m02) * r;
      out.xyz.y = (m21 + m12) * r;
      out.w     = (m01 - m10) * r;
      break;
    }
  }

  return out;
}

template <typename T>
auto quat_t<T>::slerp(const quat_t &to, value_type delta) const -> quat_t
{
  value_type dot, scale0, scale1, angle, inv_sin;
  quat_t dquat = to;

  if (delta < 0 || is_zero(delta)) {
    return *this;
  } else if (delta >= 1) {
    return to;
  }

  dot = dot_product(to);

  if (dot < 0) {
    dot = -dot;
    dquat.negate();
  }

  if (dot > value_type(0.9999)) {
    return lerp(to, delta);
  } else {
    angle = std::acos(dot);
    inv_sin = value_type(1) / std::sin(angle);
    scale0 = std::sin((value_type(1) - delta) * angle) * inv_sin;
    scale1 = std::sin(delta * angle) * inv_sin;
  }

  return dquat.scale(scale1).add(scaled(scale0));
}

template <typename T>
auto quat_t<T>::lerp(const quat_t &to, value_type delta) const -> quat_t
{
  if (delta < 0 || is_zero(delta)) {
    return *this;
  } else if (delta >= 1) {
    return to;
  }

  quat_t dquat = to;
  value_type scale1 = clamp_value(delta, 0, 1);
  value_type scale0 = value_type(1) - delta;
  return dquat.scale(scale1).add(scaled(scale0)).normalize();
}

template <typename T>
inline
auto operator << (std::ostream &out, const quat_t<T> &in) -> std::ostream&
{
  return out << "{x:" << in.xyz.x << ", y:" << in.xyz.y << ", z:" << in.xyz.z << ", w:" << in.w << "}";
}

template <typename T, typename Q>
inline
auto operator * (const quat_t<T> &lhs,
                 const quat_t<Q> &rhs) -> quat_t<T>
{
  return lhs.product(rhs);
}

template <typename T, typename Q>
inline
auto operator * (const quat_t<T> &lhs,
                 Q rhs) -> quat_t<T>
{
  return lhs.scaled(rhs);
}

template <typename T, typename Q>
inline
auto operator + (const quat_t<T> &lhs,
                 const quat_t<Q> &rhs) -> quat_t<T>
{
  return lhs.sum(rhs);
}

template <typename T, typename Q>
inline
auto operator - (const quat_t<T> &lhs,
                 const quat_t<Q> &rhs) -> quat_t<T>
{
  return lhs.difference(rhs);
}

template <typename T, typename Q>
inline
auto operator % (const quat_t<T> &lhs,
                 const quat_t<Q> &rhs) -> T
{
  return lhs.dot_product(rhs);
}

template <typename T, typename Q>
inline
auto operator == (const quat_t<T> &lhs,
                 const quat_t<Q> &rhs) -> bool
{
  return
    is_zero(lhs.xyz.x - rhs.xyz.x) &&
    is_zero(lhs.xyz.y - rhs.xyz.y) &&
    is_zero(lhs.xyz.z - rhs.xyz.z) &&
    is_zero(lhs.w - rhs.w);
}

template <typename T, typename Q>
inline
auto operator != (const quat_t<T> &lhs,
                  const quat_t<Q> &rhs) -> bool
{
  return !(lhs == rhs);
}



/*==============================================================================

  Line (ported over from Mark Sibly's Blitz3D maths routines)
  Not required to be a POD type, though it may end up being one due to
  restrictions placed on other types.

==============================================================================*/

template <typename T>
struct line_t
{
  typedef T value_type;
  typedef vec3_t<value_type> vec3;
  vec3 origin, dist;

  auto end() const -> vec3
  {
    return origin + dist;
  }

  auto translated(const vec3 &d) const -> line_t
  {
    return {
      origin + d, dist
    };
  }

  auto translate(const vec3 &d) -> line_t&
  {
    origin += d;
    return *this;
  }

  auto scaled(value_type d) const -> line_t
  {
    return {
        origin, dist * d
    };
  }

  auto scale(value_type d) -> line_t&
  {
    dist *= d;
    return *this;
  }

  auto nearest_to(const vec3& p) const -> vec3
  {
    // This is borrowed from Mark Sibly's Blitz3D maths code since it's
    // really rather nice.
    return end().scale(dist.dot_product(p - origin) / dist.dot_product(dist));
  }
};



/*==============================================================================

  Plane

==============================================================================*/





/*==============================================================================

  4x4 Square Matrix

==============================================================================*/

template <typename T = float>
struct alignas(4) mat4_t
{
  static_assert(std::is_floating_point<T>::value,
                "value_type must be floating point type");
  static_assert(std::is_scalar<T>::value,
                "value_type must be scalar type");

  typedef T value_type;
  typedef vec3_t<value_type> vec3;
  typedef vec4_t<value_type> vec4;
  typedef quat_t<value_type> quat;

  //         X    Y    Z    W
  value_type m00, m10, m20, m30; // X
  value_type m01, m11, m21, m31; // Y
  value_type m02, m12, m22, m32; // Z
  value_type m03, m13, m23, m33; // Translation

  static const mat4_t identity;
  static const mat4_t zero;

  static
  auto translation(const vec3 &off) -> mat4_t
  {
    return {
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      off.x, off.y, off.z, 1
    };
  }

  static
  auto scaling(const vec3 &off) -> mat4_t
  {
    return {
      off.x, 0, 0, 0,
      0, off.y, 0, 0,
      0, 0, off.z, 0,
      0, 0, 0, 1
    };
  }

  static
  auto rotation(value_type angle, const vec3 &axis) -> mat4_t;

  static
  auto frustum(value_type left, value_type right,
               value_type bottom, value_type top,
               value_type near, value_type far) -> mat4_t;

  static
  auto orthographic(value_type left, value_type right,
                    value_type top, value_type bottom,
                    value_type near, value_type far) -> mat4_t;

  static
  auto perspective(value_type fovY, value_type aspect,
                   value_type near, value_type far) -> mat4_t;

  static
  auto look_at(const vec3 &eye, const vec3& center, const vec3& up) -> mat4_t;

  static
  auto from_quat(const quat &in) -> mat4_t
  {
    value_type tx, ty, tz, xx, xy, xz, yy, yz, zz, wx, wy, wz;

    tx = 2.0 * in.xyz.x;
    ty = 2.0 * in.xyz.y;
    tz = 2.0 * in.xyz.z;

    xx = tx * in.xyz.x;
    xy = tx * in.xyz.y;
    xz = tx * in.xyz.z;

    yy = ty * in.xyz.y;
    yz = tz * in.xyz.y;

    zz = tz * in.w;

    wx = tx * in.w;
    wy = ty * in.w;
    wz = tz * in.w;

    return {
      value_type(1) - (yy + zz), xy - wz, xz + wy, 0,
      xy + wz, value_type(1) - (xx + zz), yz - wx, 0,
      xz - wy, yz + wx, value_type(1) - (xx + yy), 0,
      0, 0, 0, 1
    };
  }

  auto translate(const vec3 &translation) -> mat4_t&;


  auto translated(const vec3 &translation) const -> mat4_t
  {
    mat4_t r = *this;
    return r.translate(translation);
  }

  auto transpose() -> mat4_t&
  {
    #define S_SWAP_ELEM_(LHS, RHS) t=LHS;LHS=RHS;RHS=t;
    value_type t;
    S_SWAP_ELEM_(m10, m01);
    S_SWAP_ELEM_(m20, m02);
    S_SWAP_ELEM_(m30, m03);
    S_SWAP_ELEM_(m21, m12);
    S_SWAP_ELEM_(m31, m13);
    S_SWAP_ELEM_(m32, m23);
    #undef S_SWAP_ELEM_
    return *this;
  }

  auto transposed() const -> mat4_t
  {
    return {
      m00, m01, m02, m03,
      m10, m11, m12, m13,
      m20, m21, m22, m23,
      m30, m31, m32, m33
    };
  }

  auto rowvec4(int index) const -> vec4
  {
    switch (index) {
      case 0: return { m00, m10, m20, m30 };
      case 1: return { m01, m11, m21, m31 };
      case 2: return { m02, m12, m22, m32 };
      case 3: return { m03, m13, m23, m33 };
      default: throw std::out_of_range("attempt to access out of range row");
    }
  }

  auto colvec4(int index) const -> vec4
  {
    switch (index) {
      case 0: return { m00, m01, m02, m03 };
      case 1: return { m10, m11, m12, m13 };
      case 2: return { m20, m21, m22, m23 };
      case 3: return { m30, m31, m32, m33 };
      default: throw std::out_of_range("attempt to access out of range column");
    }
  }

  auto set_rowvec4(int index, const vec4 &row) -> mat4_t&
  {
    switch (index) {
      case 0: m00 = row.x; m10 = row.y; m20 = row.z; m30 = row.w; break;
      case 1: m01 = row.x; m11 = row.y; m21 = row.z; m31 = row.w; break;
      case 2: m02 = row.x; m12 = row.y; m22 = row.z; m32 = row.w; break;
      case 3: m03 = row.x; m13 = row.y; m23 = row.z; m33 = row.w; break;
      default: throw std::out_of_range("attempt to access out of range row");
    }
    return *this;
  }

  auto set_colvec4(int index, const vec4 &col) -> mat4_t&
  {
    switch (index) {
      case 0: m00 = col.x; m01 = col.y; m02 = col.z; m03 = col.w; break;
      case 1: m10 = col.x; m11 = col.y; m12 = col.z; m13 = col.w; break;
      case 2: m20 = col.x; m21 = col.y; m22 = col.z; m23 = col.w; break;
      case 3: m30 = col.x; m31 = col.y; m32 = col.z; m33 = col.w; break;
      default: throw std::out_of_range("attempt to access out of range column");
    }
    return *this;
  }

  auto rowvec3(int index) const -> vec3
  {
    switch (index) {
      case 0: return { m00, m10, m20 };
      case 1: return { m01, m11, m21 };
      case 2: return { m02, m12, m22 };
      case 3: return { m03, m13, m23 };
      default: throw std::out_of_range("attempt to access out of range row");
    }
  }

  auto colvec3(int index) const -> vec3
  {
    switch (index) {
      case 0: return { m00, m01, m02 };
      case 1: return { m10, m11, m12 };
      case 2: return { m20, m21, m22 };
      case 3: return { m30, m31, m32 };
      default: throw std::out_of_range("attempt to access out of range column");
    }
  }

  auto set_rowvec3(int index, const vec3 &row) -> mat4_t&
  {
    switch (index) {
      case 0: m00 = row.x; m10 = row.y; m20 = row.z; break;
      case 1: m01 = row.x; m11 = row.y; m21 = row.z; break;
      case 2: m02 = row.x; m12 = row.y; m22 = row.z; break;
      case 3: m03 = row.x; m13 = row.y; m23 = row.z; break;
      default: throw std::out_of_range("attempt to access out of range row");
    }
    return *this;
  }

  auto set_colvec3(int index, const vec3 &col) -> mat4_t&
  {
    switch (index) {
      case 0: m00 = col.x; m01 = col.y; m02 = col.z; break;
      case 1: m10 = col.x; m11 = col.y; m12 = col.z; break;
      case 2: m20 = col.x; m21 = col.y; m22 = col.z; break;
      case 3: m30 = col.x; m31 = col.y; m32 = col.z; break;
      default: throw std::out_of_range("attempt to access out of range column");
    }
    return *this;
  }

  auto inverse_orthogonal() -> mat4_t
  {
    vec3_t<value_type> row = { m03, m13, m23 };
    mat4_t temp = {
      m00, m01, m02, 0,
      m10, m11, m12, 0,
      m20, m21, m22, 0,
      0, 0, 0, 1
    };
    temp.m03 = -row.dot_product(temp.colvec3(0));
    temp.m13 = -row.dot_product(temp.colvec3(1));
    temp.m23 = -row.dot_product(temp.colvec3(2));
    return temp;
  }

  auto negated() const -> mat4_t
  {
    return {
      -m00, -m10, -m20, -m30,
      -m01, -m11, -m21, -m31,
      -m02, -m12, -m22, -m32,
      -m03, -m13, -m23, -m33
    };
  }

  auto negate() -> mat4_t&
  {
    m00 = -m00; m10 = -m10; m20 = -m20; m30 = -m30;
    m01 = -m01; m11 = -m11; m21 = -m21; m31 = -m31;
    m02 = -m02; m12 = -m12; m22 = -m22; m32 = -m32;
    m03 = -m03; m13 = -m13; m23 = -m23; m33 = -m33;
    return *this;
  }

  auto sum(const mat4_t &other) const -> mat4_t
  {
    return {
      m00 + other.m00,
      m10 + other.m10,
      m20 + other.m20,
      m30 + other.m30,
      m01 + other.m01,
      m11 + other.m11,
      m21 + other.m21,
      m31 + other.m31,
      m02 + other.m02,
      m12 + other.m12,
      m22 + other.m22,
      m32 + other.m32,
      m03 + other.m03,
      m13 + other.m13,
      m23 + other.m23,
      m33 + other.m33
    };
  }

  auto sum(value_type scalar) const -> mat4_t
  {
    return {
      m00 + scalar,
      m10 + scalar,
      m20 + scalar,
      m30 + scalar,
      m01 + scalar,
      m11 + scalar,
      m21 + scalar,
      m31 + scalar,
      m02 + scalar,
      m12 + scalar,
      m22 + scalar,
      m32 + scalar,
      m03 + scalar,
      m13 + scalar,
      m23 + scalar,
      m33 + scalar
    };
  }

  auto add(const mat4_t &other) -> mat4_t&
  {
    m00 += other.m00;
    m10 += other.m10;
    m20 += other.m20;
    m30 += other.m30;
    m01 += other.m01;
    m11 += other.m11;
    m21 += other.m21;
    m31 += other.m31;
    m02 += other.m02;
    m12 += other.m12;
    m22 += other.m22;
    m32 += other.m32;
    m03 += other.m03;
    m13 += other.m13;
    m23 += other.m23;
    m33 += other.m33;
    return *this;
  }

  auto add(value_type scalar) -> mat4_t&
  {
    m00 += scalar;
    m10 += scalar;
    m20 += scalar;
    m30 += scalar;
    m01 += scalar;
    m11 += scalar;
    m21 += scalar;
    m31 += scalar;
    m02 += scalar;
    m12 += scalar;
    m22 += scalar;
    m32 += scalar;
    m03 += scalar;
    m13 += scalar;
    m23 += scalar;
    m33 += scalar;
    return *this;
  }


  auto difference(const mat4_t &other) const -> mat4_t
  {
    return {
      m00 - other.m00,
      m10 - other.m10,
      m20 - other.m20,
      m30 - other.m30,
      m01 - other.m01,
      m11 - other.m11,
      m21 - other.m21,
      m31 - other.m31,
      m02 - other.m02,
      m12 - other.m12,
      m22 - other.m22,
      m32 - other.m32,
      m03 - other.m03,
      m13 - other.m13,
      m23 - other.m23,
      m33 - other.m33
    };
  }

  auto difference(value_type scalar) const -> mat4_t
  {
    return {
      m00 - scalar,
      m10 - scalar,
      m20 - scalar,
      m30 - scalar,
      m01 - scalar,
      m11 - scalar,
      m21 - scalar,
      m31 - scalar,
      m02 - scalar,
      m12 - scalar,
      m22 - scalar,
      m32 - scalar,
      m03 - scalar,
      m13 - scalar,
      m23 - scalar,
      m33 - scalar
    };
  }

  auto subtract(const mat4_t &other) -> mat4_t&
  {
    m00 -= other.m00;
    m10 -= other.m10;
    m20 -= other.m20;
    m30 -= other.m30;
    m01 -= other.m01;
    m11 -= other.m11;
    m21 -= other.m21;
    m31 -= other.m31;
    m02 -= other.m02;
    m12 -= other.m12;
    m22 -= other.m22;
    m32 -= other.m32;
    m03 -= other.m03;
    m13 -= other.m13;
    m23 -= other.m23;
    m33 -= other.m33;
    return *this;
  }

  auto subtract(value_type scalar) -> mat4_t&
  {
    m00 -= scalar;
    m10 -= scalar;
    m20 -= scalar;
    m30 -= scalar;
    m01 -= scalar;
    m11 -= scalar;
    m21 -= scalar;
    m31 -= scalar;
    m02 -= scalar;
    m12 -= scalar;
    m22 -= scalar;
    m32 -= scalar;
    m03 -= scalar;
    m13 -= scalar;
    m23 -= scalar;
    m33 -= scalar;
    return *this;
  }

  auto scaled(value_type scalar) const -> mat4_t
  {
    return {
      m00 * scalar,
      m10 * scalar,
      m20 * scalar,
      m30 * scalar,
      m01 * scalar,
      m11 * scalar,
      m21 * scalar,
      m31 * scalar,
      m02 * scalar,
      m12 * scalar,
      m22 * scalar,
      m32 * scalar,
      m03 * scalar,
      m13 * scalar,
      m23 * scalar,
      m33 * scalar
    };
  }

  auto scale(value_type scalar) -> mat4_t&
  {
    m00 *= scalar;
    m10 *= scalar;
    m20 *= scalar;
    m30 *= scalar;
    m01 *= scalar;
    m11 *= scalar;
    m21 *= scalar;
    m31 *= scalar;
    m02 *= scalar;
    m12 *= scalar;
    m22 *= scalar;
    m32 *= scalar;
    m03 *= scalar;
    m13 *= scalar;
    m23 *= scalar;
    m33 *= scalar;
    return *this;
  }

  auto scaled(const mat4_t &other) const -> mat4_t
  {
    return {
      m00 * other.m00,
      m10 * other.m10,
      m20 * other.m20,
      m30 * other.m30,
      m01 * other.m01,
      m11 * other.m11,
      m21 * other.m21,
      m31 * other.m31,
      m02 * other.m02,
      m12 * other.m12,
      m22 * other.m22,
      m32 * other.m32,
      m03 * other.m03,
      m13 * other.m13,
      m23 * other.m23,
      m33 * other.m33
    };
  }

  auto scale(const mat4_t &other) -> mat4_t&
  {
    m00 *= other.m00;
    m10 *= other.m10;
    m20 *= other.m20;
    m30 *= other.m30;
    m01 *= other.m01;
    m11 *= other.m11;
    m21 *= other.m21;
    m31 *= other.m31;
    m02 *= other.m02;
    m12 *= other.m12;
    m22 *= other.m22;
    m32 *= other.m32;
    m03 *= other.m03;
    m13 *= other.m13;
    m23 *= other.m23;
    m33 *= other.m33;
    return *this;
  }

  auto scaled(const vec3 &vec) const -> mat4_t
  {
    return {
      m00 * vec.x,
      m10 * vec.x,
      m20 * vec.x,
      m30,
      m01 * vec.y,
      m11 * vec.y,
      m21 * vec.y,
      m31,
      m02 * vec.z,
      m12 * vec.z,
      m22 * vec.z,
      m32,
      m03,
      m13,
      m23,
      m33
    };
  }

  auto scale(const vec3 &vec) -> mat4_t&
  {
    m00 *= vec.x;
    m10 *= vec.x;
    m20 *= vec.x;
    m01 *= vec.y;
    m11 *= vec.y;
    m21 *= vec.y;
    m02 *= vec.z;
    m12 *= vec.z;
    m22 *= vec.z;
    return *this;
  }

  auto operator[] (int index) -> value_type&
  {
    static_assert(std::is_pod<mat4_t>::value, "mat4 must be POD to use subscript operator");
    if (index < 0 || index > 15)
      throw std::out_of_range("attempt to access out of range element");
    return (&m00)[index];
  }

  auto operator[] (int index) const -> value_type
  {
    static_assert(std::is_pod<mat4_t>::value, "mat4 must be POD to use subscript operator");
    if (index < 0 || index > 15)
      throw std::out_of_range("attempt to access out of range element");
    return (&m00)[index];
  }

  operator value_type* ()
  {
    static_assert(std::is_pod<mat4_t>::value, "mat4 must be POD to cast to value_type pointer");
    return &m00;
  }

  operator const value_type* () const
  {
    static_assert(std::is_pod<mat4_t>::value, "mat4 must be POD to cast to value_type pointer");
    return &m00;
  }

  auto operator *= (const mat4_t &other) -> mat4_t&
  {
    return multiply(other);
  }

  auto operator *= (value_type scalar) -> mat4_t&
  {
    return scale(scalar);
  }

  auto operator += (const mat4_t &other) -> mat4_t&
  {
    return add(other);
  }

  auto operator += (value_type scalar) -> mat4_t&
  {
    return add(scalar);
  }

  auto operator -= (const mat4_t &other) -> mat4_t&
  {
    return subtract(other);
  }

  auto operator -= (value_type scalar) -> mat4_t&
  {
    return subtract(scalar);
  }

  auto inverse_affine(mat4_t &out) const -> bool;
  auto adjoint() const -> mat4_t;

  auto cofactor(int r0, int r1, int r2, int c0, int c1, int c2) const -> value_type
  {
    const value_type *m = &m00;
    // Macro for getting the value at the cofactor address provided
    #define S_CF_ADDR_(l, r) (m[(l * 4) + r])
    return (
      (S_CF_ADDR_(r0,c0) * ((S_CF_ADDR_(r1, c1) * S_CF_ADDR_(r2, c2)) - (S_CF_ADDR_(r2, c1) * S_CF_ADDR_(r1, c2)))) -
      (S_CF_ADDR_(r0,c1) * ((S_CF_ADDR_(r1, c0) * S_CF_ADDR_(r2, c2)) - (S_CF_ADDR_(r2, c0) * S_CF_ADDR_(r1, c2)))) +
      (S_CF_ADDR_(r0,c2) * ((S_CF_ADDR_(r1, c0) * S_CF_ADDR_(r2, c1)) - (S_CF_ADDR_(r2, c0) * S_CF_ADDR_(r1, c1))))
    );
    #undef S_CF_ADDR_
  }

  auto determinant() const -> value_type
  {
    return ((m00 * cofactor(1, 2, 3, 1, 2, 3)) -
            (m01 * cofactor(1, 2, 3, 0, 2, 3)) +
            (m02 * cofactor(1, 2, 3, 0, 1, 3)) -
            (m03 * cofactor(1, 2, 3, 0, 1, 2)));
  }

  auto inverse_general(mat4_t &out) const -> bool
  {
    value_type det = determinant();

    if (is_zero(det)) {
      return false;
    }
    out = adjoint().scale(value_type(1) / det);
    return true;
  }

  auto product(const mat4_t &other) const -> mat4_t
  {
    mat4_t temp;
    value_type *m = &temp.m00;
    const vec4 rowvecs[4] = {
      rowvec4(0),
      rowvec4(1),
      rowvec4(2),
      rowvec4(3)
    };

    for (int cindex = 0; cindex < 4; ++cindex) {
      const vec4 column = colvec4(cindex);
      m[cindex     ] = column.dot_product(rowvecs[0]);
      m[cindex + 4 ] = column.dot_product(rowvecs[1]);
      m[cindex + 8 ] = column.dot_product(rowvecs[2]);
      m[cindex + 12] = column.dot_product(rowvecs[3]);
    }

    return temp;
  }

  auto multiply(const mat4_t &other) -> mat4_t&
  {
    value_type *m = &m00;
    const vec4 rowvecs[4] = {
      rowvec4(0),
      rowvec4(1),
      rowvec4(2),
      rowvec4(3)
    };

    for (int cindex = 0; cindex < 4; ++cindex) {
      const vec4 column = colvec4(cindex);
      m[cindex     ] = column.dot_product(rowvecs[0]);
      m[cindex + 4 ] = column.dot_product(rowvecs[1]);
      m[cindex + 8 ] = column.dot_product(rowvecs[2]);
      m[cindex + 12] = column.dot_product(rowvecs[3]);
    }

    return *this;
  }

  auto multiply(const vec4 &vec) const -> vec4
  {
    return {
      vec.dot_product(colvec4(0)),
      vec.dot_product(colvec4(1)),
      vec.dot_product(colvec4(2)),
      vec.dot_product(colvec4(3))
    };
  }

  auto multiply(const vec3 &vec) const -> vec3
  {
    return {
      vec.dot_product(colvec3(0)) + m03,
      vec.dot_product(colvec3(1)) + m13,
      vec.dot_product(colvec3(2)) + m23
    };
  }

  auto rotate(const vec3 &vec) const -> vec3
  {
    return {
      vec.dot_product(colvec3(0)),
      vec.dot_product(colvec3(1)),
      vec.dot_product(colvec3(2))
    };
  }

  auto inverse_rotate(const vec3 &vec) const -> vec3
  {
    return {
      vec.dot_product(rowvec3(0)),
      vec.dot_product(rowvec3(1)),
      vec.dot_product(rowvec3(2))
    };
  }

}; // struct mat4_t<T>

template <typename T>
const mat4_t<T> mat4_t<T>::identity = {
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
};

template <typename T>
const mat4_t<T> mat4_t<T>::zero = {
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0
};

template <typename T>
auto mat4_t<T>::rotation(value_type angle, const vec3 &axis) -> mat4_t
{
  angle *= S_DEG2RAD;
  const value_type  c = static_cast<value_type>(std::cos(angle));
  const value_type  s = static_cast<value_type>(std::sin(angle));
  const value_type ic = value_type(1) - c;
  const value_type xy = axis.x * axis.y * ic;
  const value_type yz = axis.y * axis.z * ic;
  const value_type xz = axis.x * axis.z * ic;
  const value_type xs = s * axis.x;
  const value_type ys = s * axis.y;
  const value_type zs = s * axis.z;

  return {
    ((axis.x * axis.x) * ic) + c,  xy + zs,  xz - ys,  0,
    xy - zs,  ((axis.y * axis.y) * ic) + c,  yz + xs,  0,
    xz + ys,  yz - xs,  ((axis.z * axis.z) * ic) + c,  0,
    0,  0,  0,  1
  };
}

template <typename T>
auto mat4_t<T>::frustum(value_type left, value_type right,
                     value_type bottom, value_type top,
                     value_type near, value_type far) -> mat4_t
{
  const value_type xdelta = right - left;
  const value_type ydelta = top - bottom;
  const value_type zdelta = far - near;
  const value_type neardouble = 2.0 * near;

  return {
    neardouble / xdelta, 0, (right + left) / xdelta, 0,
    0, neardouble / ydelta, (top + bottom) / ydelta, 0,
    0, 0, -((far + near) / zdelta), -1.0,
    0, 0, -((neardouble * far) / zdelta), 0
  };
}

template <typename T>
auto mat4_t<T>::orthographic(value_type left, value_type right,
                          value_type top, value_type bottom,
                          value_type near, value_type far) -> mat4_t
{
  const value_type xdelta = right - left;
  const value_type ydelta = top - bottom;
  const value_type zdelta = far - near;

  return {
    value_type(2) / xdelta, 0, 0, 0,
    0, value_type(2) / ydelta, 0, 0,
    0, 0, value_type(-2) / zdelta, 0,
    -((right + left) / xdelta), -((top + bottom) / ydelta), -((far + near) / zdelta), 1
  };
}

template <typename T>
auto mat4_t<T>::perspective(value_type fovY, value_type aspect,
                         value_type near, value_type far) -> mat4_t
{
  const float r = static_cast<value_type>(tanf(fovY * 0.5 * S_DEG2RAD));
  const float left = -r * aspect;
  const float right = r * aspect;
  const float bottom = -r;
  const float top = r;
  const float twoNear = 2 * near;
  const float zdelta = value_type(1) / (near - far);

  return {
    twoNear * (right - left), 0, 0, 0,
    0, twoNear / (top - bottom), 0, 0,
    0, 0, (far + near) * zdelta, -1,
    0, 0, (twoNear * far) * zdelta, 0
  };
}

template <typename T>
auto mat4_t<T>::look_at(const vec3 &eye, const vec3& center,
                     const vec3& up) -> mat4_t
{
  vec3 facing_norm = (center - eye).normalize();
  vec3 up_norm = up.normalized();
  vec3 s = (facing_norm.cross_product(up_norm)).normalize();
  up_norm = s.cross_product(facing_norm);
  facing_norm.negate();
  up_norm.y = - up_norm.y;

  mat4_t looker = {
    up_norm.x, up_norm.y, up_norm.z, 0,
    facing_norm.x, facing_norm.y, facing_norm.z, 0,
    0, 0, 0, 0,
    0, 0, 0, 1
  };

  return looker.translate(eye.negated());
}

template <typename T>
auto mat4_t<T>::translate(const vec3 &translation) -> mat4_t&
{
  m03 += translation.dot_product(colvec3(0));
  m13 += translation.dot_product(colvec3(1));
  m23 += translation.dot_product(colvec3(2));
  m33 += translation.dot_product(colvec3(3));
  return *this;
}

template <typename T>
auto mat4_t<T>::inverse_affine(mat4_t& out) const -> bool
{
  value_type det;

  mat4_t temp = {
    (m11 * m22) - (m21 * m12),
    (m20 * m12) - (m01 * m22),
    (m01 * m21) - (m20 * m11),
    0,

    (m21 * m02) - (m01 * m22),
    (m00 * m22) - (m20 * m02),
    (m20 * m01) - (m00 * m21),
    0,

    (m01 * m12) - (m11 * m02),
    (m01 * m02) - (m00 * m12),
    (m00 * m11) - (m01 * m01),
    0,

    0, 0, 0, 0
  };

  det = m00 * temp.m00 + m10 * temp.m01 + m20 * temp.m02;
  if (is_zero(det)) {
    return false;
  }

  temp.scale(value_type(1) / det);
  vec3 txyz = rowvec3(3);

  temp.m03 = -txyz.dot_product(temp.colvec3(0));
  temp.m13 = -txyz.dot_product(temp.colvec3(1));
  temp.m23 = -txyz.dot_product(temp.colvec3(2));

  out = temp;

  return true;
}

template <typename T>
auto mat4_t<T>::adjoint() const -> mat4_t
{
  return {
     cofactor(1, 2, 3, 1, 2, 3),
    -cofactor(0, 2, 3, 1, 2, 3),
     cofactor(0, 1, 3, 1, 2, 3),
    -cofactor(0, 1, 2, 1, 2, 3),

    -cofactor(1, 2, 3, 0, 2, 3),
     cofactor(0, 2, 3, 0, 2, 3),
    -cofactor(0, 1, 3, 0, 2, 3),
     cofactor(0, 1, 2, 0, 2, 3),

     cofactor(1, 2, 3, 0, 1, 3),
    -cofactor(0, 2, 3, 0, 1, 3),
     cofactor(0, 1, 3, 0, 1, 3),
    -cofactor(0, 1, 2, 0, 1, 3),

    -cofactor(1, 2, 3, 0, 1, 2),
     cofactor(0, 2, 3, 0, 1, 2),
    -cofactor(0, 1, 3, 0, 1, 2),
     cofactor(0, 1, 2, 0, 1, 2)
  };
}

template <typename T>
inline
auto operator << (std::ostream &out,
                  const mat4_t<T> &in) -> std::ostream&
{
  out << "{";
  for (int index = 0; index < 16; ++index) {
    out << in[index];
    if (index < 15)
      out << ", ";
  }
  return out << "}";
}

template <typename T, typename Q>
inline
auto operator * (const mat4_t<T> &rhs,
                 const mat4_t<Q> &lhs) -> mat4_t<T>
{
  return rhs.product(lhs);
}


template <typename T, typename Q>
inline
auto operator * (const mat4_t<T> &rhs,
                 const vec4_t<Q> &lhs) -> vec4_t<T>
{
  return rhs.multiply(lhs);
}


template <typename T, typename Q>
inline
auto operator * (const mat4_t<T> &rhs,
                 const vec3_t<Q> &lhs) -> vec3_t<T>
{
  return rhs.multiply(lhs);
}

template <typename T, typename Q>
inline
auto operator * (const mat4_t<T> &rhs,
                 const Q &lhs) -> mat4_t<T>
{
  return rhs.scaled(lhs);
}

template <typename T, typename Q>
inline
auto operator + (const mat4_t<T> &rhs,
                 const mat4_t<Q> &lhs) -> mat4_t<T>
{
  return rhs.sum(lhs);
}

template <typename T, typename Q>
inline
auto operator + (const mat4_t<T> &rhs,
                 const Q &lhs) -> mat4_t<T>
{
  return rhs.sum(lhs);
}

template <typename T, typename Q>
inline
auto operator - (const mat4_t<T> &rhs,
                 const mat4_t<Q> &lhs) -> mat4_t<T>
{
  return rhs.difference(lhs);
}

template <typename T, typename Q>
inline
auto operator - (const mat4_t<T> &rhs,
                 const Q &lhs) -> mat4_t<T>
{
  return rhs.difference(lhs);
}

template <typename T, typename Q>
auto operator == (const mat4_t<T> &rhs,
                  const mat4_t<Q> &lhs) -> bool
{
  return
  is_zero(lhs.m00 - rhs.m00) &&
  is_zero(lhs.m10 - rhs.m10) &&
  is_zero(lhs.m20 - rhs.m20) &&

  is_zero(lhs.m01 - rhs.m01) &&
  is_zero(lhs.m11 - rhs.m11) &&
  is_zero(lhs.m21 - rhs.m21) &&

  is_zero(lhs.m02 - rhs.m02) &&
  is_zero(lhs.m12 - rhs.m12) &&
  is_zero(lhs.m22 - rhs.m22) &&

  is_zero(lhs.m03 - rhs.m03) &&
  is_zero(lhs.m13 - rhs.m13) &&
  is_zero(lhs.m23 - rhs.m23) &&

  is_zero(lhs.m30 - rhs.m30) &&
  is_zero(lhs.m31 - rhs.m31) &&
  is_zero(lhs.m32 - rhs.m32) &&
  is_zero(lhs.m33 - rhs.m33);
}

template <typename T, typename Q>
auto operator != (const mat4_t<T> &rhs,
                  const mat4_t<Q> &lhs) -> bool
{
  return !(rhs == lhs);
}

typedef vec3_t<float> vec3f_t;
typedef vec3_t<double> vec3d_t;
typedef vec4_t<float> vec4f_t;
typedef vec4_t<double> vec4d_t;
typedef quat_t<float> quatf_t;
typedef quat_t<double> quatd_t;
typedef line_t<float> linef_t;
typedef line_t<double> lined_t;
// typedef plane_t<float> planef_t;
// typedef plane_t<double> planed_t;
typedef mat4_t<float> mat4f_t;
typedef mat4_t<double> mat4d_t;


} // namespace snow

#endif /* end __SNOW_MATH3D_HH__ include guard */
