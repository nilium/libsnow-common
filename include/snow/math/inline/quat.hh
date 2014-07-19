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
template <typename T> struct mat4_t;
template <typename T> struct mat3_t;
/** @endcond */


/** @addtogroup Math3D */
///@{


template <typename T = float>
struct S_EXPORT alignas(T) quat_t {

  typedef T value_type;
  typedef vec3_t<value_type> vec3;

  vec3       xyz;
  value_type w;

  static const quat_t zero;
  static const quat_t one;
  static const quat_t identity;

  static quat_t make(T x, T y, T z, T w);
  static quat_t make(vec3 xyz, T w);
  static quat_t from_angle_axis(T angle, vec3 axis);
  static quat_t from_mat4(const mat4_t<T> &mat);
  static quat_t from_mat3(const mat3_t<T> &mat);

  value_type    length() const;
  value_type    magnitude() const;

  quat_t        inverse() const;
  quat_t &      invert();

  quat_t        negated() const;
  quat_t &      negate();

  quat_t        product(quat_t other) const;
  quat_t &      multiply(quat_t other);

  quat_t        normalized() const;
  quat_t &      normalize();

  quat_t        difference(quat_t other) const;
  quat_t &      subtract(quat_t other);

  quat_t        sum(quat_t other) const;
  quat_t &      add(quat_t other);

  quat_t        scaled(value_type scalar) const;
  quat_t        scaled(quat_t other) const;
  quat_t &      scale(value_type scalar);
  quat_t &      scale(quat_t other);

  value_type    dot_product(quat_t other) const;

  quat_t        slerp(quat_t to, value_type delta) const;
  quat_t        lerp(quat_t to, value_type delta) const;

  quat_t &      operator *= (quat_t other);
  quat_t &      operator *= (value_type scalar);
  quat_t &      operator += (quat_t other);
  quat_t &      operator -= (quat_t other);
  quat_t        operator - () const;
  quat_t        operator ~ () const;
  value_type &  operator[] (int index);
  value_type    operator[] (int index) const;
                operator value_type* ();
                operator const T* () const;
                operator mat4_t<T>() const;
                operator mat3_t<T>() const;

  template <typename Q>
                operator quat_t<Q> () const;
};

template <typename T>
std::ostream& operator << (std::ostream &out, quat_t<T> in);

template <typename T, typename Q>
quat_t<T>     operator *  (quat_t<T> lhs, quat_t<Q> rhs);
template <typename T, typename Q>
quat_t<T>     operator *  (quat_t<T> lhs, Q rhs);

template <typename T, typename Q>
quat_t<T>     operator +  (quat_t<T> lhs, quat_t<Q> rhs);
template <typename T, typename Q>
quat_t<T>     operator -  (quat_t<T> lhs, quat_t<Q> rhs);

template <typename T, typename Q>
T             operator %  (quat_t<T> lhs, quat_t<Q> rhs);

template <typename T, typename Q>
bool          operator == (quat_t<T> lhs, quat_t<Q> rhs);
template <typename T, typename Q>
bool          operator != (quat_t<T> lhs, quat_t<Q> rhs);


using quatf_t = quat_t<float>;
using quatd_t = quat_t<double>;


///@}


} // namespace snow

#include "quat.cc"
