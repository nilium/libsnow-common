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
template <typename T> struct vec4_t;
/** @endcond */


/** @addtogroup Math3D */
///@{


/** @brief A three-component vector type.

  @see vec3_t, vec4_t, quat_t, mat3_t, mat4_t, plane_t, line_t
*/
template <typename T = float>
struct S_EXPORT alignas(T) vec2_t {
  /// @copydoc vec3_t::value_type
  typedef T value_type;

  ///@{
  /** @name Components */
  /** @brief The vector's X component. */
  value_type x;
  /** @brief The vector's Y component. */
  value_type y;
  ///@}

  ///@{
  /** @name Predefined Vectors */
  /** @brief A vector of \f$\langle 1, 0 \rangle\f$. */
  static const vec2_t pos_X;
  /** @brief A vector of \f$\langle 0, 1 \rangle\f$. */
  static const vec2_t pos_Y;
  /** @brief A vector of \f$\langle -1, 0 \rangle\f$. */
  static const vec2_t neg_X;
  /** @brief A vector of \f$\langle 0, -1 \rangle\f$. */
  static const vec2_t neg_Y;
  /** @brief A vector of \f$\langle 0, 0 \rangle\f$. */
  static const vec2_t zero;
  /** @brief A vector of \f$\langle 1, 1 \rangle\f$. */
  static const vec2_t one;
  ///@}

  /// @brief A convenience function for creating a 2D vector.
  static vec2_t make(value_type x, value_type y);

  /// @copydoc vec3_t::normalize
  vec2_t &      normalize();
  /// @copydoc vec3_t::normalized() const
  vec2_t        normalized() const;
  /// @copydoc vec3_t::magnitude() const
  value_type    magnitude() const;
  /// @copydoc vec3_t::length() const
  value_type    length() const;
  /// @copydoc vec3_t::difference(vec3_t) const
  vec2_t        difference(vec2_t other) const;
  /// @copydoc vec3_t::subtract(vec3_t)
  vec2_t &      subtract(vec2_t other);
  /// @copydoc vec3_t::sum(vec3_t) const
  vec2_t        sum(vec2_t other) const;
  /// @copydoc vec3_t::add(vec3_t)
  vec2_t &      add(vec2_t other);
  /// @copydoc vec3_t::scaled(value_type) const
  vec2_t        scaled(value_type scalar) const;
  /// @copydoc vec3_t::scaled(vec3_t) const
  vec2_t        scaled(vec2_t other) const;
  /// @copydoc vec3_t::scale(value_type)
  vec2_t &      scale(value_type scalar);
  /// @copydoc vec3_t::scale(vec3_t)
  vec2_t &      scale(vec2_t other);
  /// @copydoc vec3_t::negated() const
  vec2_t        negated() const;
  /// @copydoc vec3_t::negate()
  vec2_t &      negate();
  /// @copydoc vec3_t::inverse() const
  vec2_t        inverse() const;
  /// @copydoc vec3_t::invert()
  vec2_t &      invert();
  /// @copydoc vec3_t::dot_product(vec3_t) const
  value_type    dot_product(vec2_t other) const;
  /// @copydoc vec3_t::rotate_elems()
  vec2_t &      rotate_elems();
  /// @copydoc vec3_t::rotated_elems() const
  vec2_t        rotated_elems() const;

  vec2_t &      operator += (vec2_t other);
  vec2_t &      operator -= (vec2_t other);

  vec2_t &      operator *= (value_type scalar);
  vec2_t &      operator *= (vec2_t other);

  vec2_t &      operator /= (value_type scalar);
  vec2_t &      operator /= (vec2_t other);

  vec2_t        operator - () const;
  vec2_t        operator ~ () const;

  value_type &  operator[] (int index);
  value_type    operator[] (int index) const;

                operator value_type* ();
                operator const value_type* () const;

  template <typename Q>
                operator vec2_t<Q> () const;
  template <typename Q>
                operator vec3_t<Q> () const;
  template <typename Q>
                operator vec4_t<Q> () const;

  #include "vec2_swizzles.hh"
};

template <typename T>
std::ostream &operator << (std::ostream &out, vec2_t<T> in);

template <typename T, typename Q>
vec2_t<T> operator - (vec2_t<T> lhs, vec2_t<Q> rhs);
template <typename T, typename Q>
vec2_t<T> operator + (vec2_t<T> lhs, vec2_t<Q> rhs);

template <typename T, typename Q>
vec2_t<T> operator * (vec2_t<T> lhs, vec2_t<Q> rhs);
template <typename T, typename Q>
vec2_t<T> operator * (vec2_t<T> lhs, Q rhs);

template <typename T, typename Q>
vec2_t<T> operator / (vec2_t<T> lhs, vec2_t<Q> rhs);
template <typename T, typename Q>
vec2_t<T> operator / (vec2_t<T> lhs, Q rhs);

template <typename T, typename Q>
T operator % (vec2_t<T> lhs, vec2_t<Q> rhs);

template <typename T, typename Q>
bool operator == (vec2_t<T> lhs, vec2_t<Q> rhs);
template <typename T, typename Q>
bool operator != (vec2_t<T> lhs, vec2_t<Q> rhs);


using vec2f_t = vec2_t<float>;
using vec2d_t = vec2_t<double>;
using vec2i_t = vec2_t<int>;
using vec2l_t = vec2_t<long>;


///@}


} // namespace snow


#include "vec2.cc"

#define splat_vec2(V) (V).x, (V).y, (V).z
