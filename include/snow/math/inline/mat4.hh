/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef __SNOW_COMMON__MAT4_THH__
#define __SNOW_COMMON__MAT4_THH__


namespace snow {


/** @cond IGNORE */
template <typename t> struct vec2_t;
template <typename t> struct vec3_t;
template <typename t> struct vec4_t;
template <typename t> struct quat_t;
template <typename t> struct mat3_t;
/** @endcond */


/** @addtogroup Math3D */
///@{


template <typename T = float>
struct S_EXPORT alignas(T) mat4_t
{

  typedef T value_type;
  typedef vec2_t<T> vec2;
  typedef vec3_t<T> vec3;
  typedef vec4_t<T> vec4;
  typedef quat_t<T> quat;

  //         X    Y    Z    W
  value_type m00, m10, m20, m30; // X
  value_type m01, m11, m21, m31; // Y
  value_type m02, m12, m22, m32; // Z
  value_type m03, m13, m23, m33; // Translation

  static const mat4_t identity;
  static const mat4_t zero;

  static mat4_t make(T m00, T m10, T m20, T m30,
                     T m01, T m11, T m21, T m31,
                     T m02, T m12, T m22, T m32,
                     T m03, T m13, T m23, T m33);
  static mat4_t translation(vec3 off);
  static mat4_t scaling(vec3 off);
  static mat4_t rotation(T angle, vec3 axis);
  static mat4_t frustum(T left, T right, T bottom, T top, T near, T far);
  static mat4_t orthographic(T left, T right, T top, T bottom, T near, T far);
  static mat4_t perspective(T fovY, T aspect, T near, T far);
  static mat4_t look_at(vec3 eye, vec3 center, vec3 up);
  static mat4_t from_quat(quat in);


  mat4_t &      translate(vec3 translation);
  mat4_t        translated(vec3 translation) const;

  mat4_t &      transpose();
  mat4_t        transposed() const;

  vec4          rowvec4(int index) const;
  vec4          colvec4(int index) const;
  mat4_t &      set_rowvec4(int index, vec4 row);
  mat4_t &      set_colvec4(int index, vec4 col);

  vec3          rowvec3(int index) const;
  vec3          colvec3(int index) const;
  mat4_t &      set_rowvec3(int index, vec3 row);
  mat4_t &      set_colvec3(int index, vec3 col);


  mat4_t        negated() const;
  mat4_t &      negate();

  mat4_t        sum(const mat4_t &other) const;
  mat4_t        sum(T scalar) const;
  mat4_t &      add(const mat4_t &other);
  mat4_t &      add(T scalar);

  mat4_t        difference(const mat4_t &other) const;
  mat4_t        difference(T scalar) const;
  mat4_t &      subtract(const mat4_t &other);
  mat4_t &      subtract(T scalar);

  mat4_t        scaled(T scalar) const;
  mat4_t &      scale(T scalar);
  mat4_t        scaled(const mat4_t &other) const;
  mat4_t &      scale(const mat4_t &other);
  mat4_t        scaled(vec3 vec) const;
  mat4_t &      scale(vec3 vec);

  mat4_t        inverse_orthogonal() const;
  bool          inverse_affine(mat4_t &out) const;
  bool          inverse_general(mat4_t &out) const;
  T             cofactor(int r0, int r1, int r2, int c0, int c1, int c2) const;
  T             determinant() const;
  mat4_t        adjoint() const;

  mat4_t        product(const mat4_t &other) const;
  mat4_t &      multiply(const mat4_t &other);

  vec4          multiply(vec4 vec) const;
  vec3          multiply(vec3 vec) const;
  vec2          multiply(vec2 vec) const;
  vec4          rotate(vec4 vec) const;
  vec4          inverse_rotate(vec4 vec) const;
  vec3          rotate(vec3 vec) const;
  vec3          inverse_rotate(vec3 vec) const;
  vec2          rotate(vec2 vec) const;
  vec2          inverse_rotate(vec2 vec) const;

  T &           operator [] (int index);
  T             operator [] (int index) const;

                operator T* ();
                operator const T* () const;

                operator quat_t<T>() const;
                operator mat3_t<T>() const;

  mat4_t &      operator *= (const mat4_t &other);
  mat4_t &      operator *= (T scalar);

  mat4_t &      operator += (const mat4_t &other);
  mat4_t &      operator += (T scalar);

  mat4_t &      operator -= (const mat4_t &other);
  mat4_t &      operator -= (T scalar);
}; // struct mat4_t<T>

template <typename T>
std::ostream &operator << (std::ostream &out, const mat4_t<T> &in);
template <typename T, typename Q>
mat4_t<T>     operator *  (const mat4_t<T> &rhs, const mat4_t<Q> &lhs);
template <typename T, typename Q>
vec4_t<T>     operator *  (const mat4_t<T> &rhs, vec4_t<Q> lhs);
template <typename T, typename Q>
vec3_t<T>     operator *  (const mat4_t<T> &rhs, vec3_t<Q> lhs);
template <typename T, typename Q>
vec2_t<T>     operator *  (const mat4_t<T> &rhs, vec2_t<Q> lhs);
template <typename T, typename Q>
mat4_t<T>     operator *  (const mat4_t<T> &rhs, Q lhs);
template <typename T, typename Q>
mat4_t<T>     operator +  (const mat4_t<T> &rhs, const mat4_t<Q> &lhs);
template <typename T, typename Q>
mat4_t<T>     operator +  (const mat4_t<T> &rhs, Q lhs);
template <typename T, typename Q>
mat4_t<T>     operator -  (const mat4_t<T> &rhs, const mat4_t<Q> &lhs);
template <typename T, typename Q>
mat4_t<T>     operator -  (const mat4_t<T> &rhs, Q lhs);
template <typename T, typename Q>
bool          operator == (const mat4_t<T> &rhs, const mat4_t<Q> &lhs);
template <typename T, typename Q>
bool          operator != (const mat4_t<T> &rhs, const mat4_t<Q> &lhs);


typedef mat4_t<float> mat4f_t;
typedef mat4_t<double> mat4d_t;


///@}


} // namespace snow

#include "mat4.cc"

#endif /* end __SNOW_COMMON__MAT4_THH__ include guard */
