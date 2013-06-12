// vec4.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__VEC4_THH__
#define __SNOW_COMMON__VEC4_THH__

/*==============================================================================

  4-Component Vector

==============================================================================*/

template <typename T = float>
struct S_EXPORT alignas(T) vec4_t {

  typedef T value_type;

  value_type x, y, z, w;

  static const vec4_t pos_X;
  static const vec4_t pos_Y;
  static const vec4_t pos_Z;
  static const vec4_t neg_X;
  static const vec4_t neg_Y;
  static const vec4_t neg_Z;
  static const vec4_t zero;
  static const vec4_t one;

  static vec4_t make(value_type x, value_type y, value_type z, value_type w);

  vec4_t &      normalize();
  vec4_t        normalized() const;

  value_type    magnitude() const;
  value_type    length() const;

  vec4_t        difference(vec4_t other) const;
  vec4_t &      subtract(vec4_t other);

  vec4_t        sum(vec4_t other) const;
  vec4_t &      add(vec4_t other);

  vec4_t        scaled(value_type scalar) const;
  vec4_t        scaled(vec4_t other) const;
  vec4_t &      scale(value_type scalar);
  vec4_t &      scale(vec4_t other);

  vec4_t        negated() const;
  vec4_t &      negate();

  vec4_t        inverse() const;
  vec4_t &      invert();

  value_type    dot_product(vec4_t other) const;

  vec4_t &      rotate_elems();
  vec4_t        rotated_elems() const;

  vec4_t &      operator += (vec4_t other);
  vec4_t &      operator -= (vec4_t other);

  vec4_t &      operator *= (value_type scalar);
  vec4_t &      operator *= (vec4_t other);

  vec4_t &      operator /= (value_type scalar);
  vec4_t &      operator /= (vec4_t other);

  vec4_t        operator - () const;
  vec4_t        operator ~ () const;

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
};

template <typename T>
std::ostream &operator << (std::ostream &out, vec4_t<T> in);

template <typename T, typename Q>
vec4_t<T> operator - (vec4_t<T> lhs, vec4_t<Q> rhs);
template <typename T, typename Q>
vec4_t<T> operator + (vec4_t<T> lhs, vec4_t<Q> rhs);

template <typename T, typename Q>
vec4_t<T> operator * (vec4_t<T> lhs, vec4_t<Q> rhs);
template <typename T, typename Q>
vec4_t<T> operator * (vec4_t<T> lhs, Q rhs);

template <typename T, typename Q>
vec4_t<T> operator / (vec4_t<T> lhs, vec4_t<Q> rhs);
template <typename T, typename Q>
vec4_t<T> operator / (vec4_t<T> lhs, Q rhs);

template <typename T, typename Q>
T operator % (vec4_t<T> lhs, vec4_t<Q> rhs);

template <typename T, typename Q>
bool operator == (vec4_t<T> lhs, vec4_t<Q> rhs);
template <typename T, typename Q>
bool operator != (vec4_t<T> lhs, vec4_t<Q> rhs);

#include "vec4.cc"

#define splat_vec4(V) (V).x, (V).y, (V).z, (V).w

#endif /* end __SNOW_COMMON__VEC4_THH__ include guard */
