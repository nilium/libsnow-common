// vec3.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__VEC3_THH__
#define __SNOW_COMMON__VEC3_THH__

/*==============================================================================

  3-Component Vector

==============================================================================*/

template <typename T = float>
struct S_EXPORT alignas(T) vec3_t {

  typedef T value_type;

  value_type x, y, z;

  static const vec3_t pos_X;
  static const vec3_t pos_Y;
  static const vec3_t pos_Z;
  static const vec3_t neg_X;
  static const vec3_t neg_Y;
  static const vec3_t neg_Z;
  static const vec3_t zero;
  static const vec3_t one;

  static vec3_t make(value_type x, value_type y, value_type z);
  static vec3_t make(value_type value);

  vec3_t &      normalize();
  vec3_t        normalized() const;
  value_type    magnitude() const;
  value_type    length() const;
  vec3_t        difference(vec3_t other) const;
  vec3_t &      subtract(vec3_t other);
  vec3_t        sum(vec3_t other) const;
  vec3_t &      add(vec3_t other);
  vec3_t        scaled(value_type scalar) const;
  vec3_t        scaled(vec3_t other) const;
  vec3_t &      scale(value_type scalar);
  vec3_t &      scale(vec3_t other);
  vec3_t        negated() const;
  vec3_t &      negate();
  vec3_t        inverse() const;
  vec3_t &      invert();
  vec3_t        cross_product(vec3_t other) const;
  value_type    dot_product(vec3_t other) const;
  vec3_t &      rotate_elems();
  vec3_t        rotated_elems() const;

  vec3_t &      operator += (vec3_t other);
  vec3_t &      operator -= (vec3_t other);

  vec3_t &      operator *= (value_type scalar);
  vec3_t &      operator *= (vec3_t other);

  vec3_t &      operator /= (value_type scalar);
  vec3_t &      operator /= (vec3_t other);

  vec3_t        operator - () const;
  vec3_t        operator ~ () const;

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

  #include "vec3_swizzles.hh"
};

template <typename T>
std::ostream &operator << (std::ostream &out, vec3_t<T> in);

template <typename T, typename Q>
vec3_t<T> operator - (vec3_t<T> lhs, vec3_t<Q> rhs);
template <typename T, typename Q>
vec3_t<T> operator + (vec3_t<T> lhs, vec3_t<Q> rhs);

template <typename T, typename Q>
vec3_t<T> operator * (vec3_t<T> lhs, vec3_t<Q> rhs);
template <typename T, typename Q>
vec3_t<T> operator * (vec3_t<T> lhs, Q rhs);

template <typename T, typename Q>
vec3_t<T> operator / (vec3_t<T> lhs, vec3_t<Q> rhs);
template <typename T, typename Q>
vec3_t<T> operator / (vec3_t<T> lhs, Q rhs);

template <typename T, typename Q>
T operator % (vec3_t<T> lhs, vec3_t<Q> rhs);

template <typename T, typename Q>
bool operator == (vec3_t<T> lhs, vec3_t<Q> rhs);
template <typename T, typename Q>
bool operator != (vec3_t<T> lhs, vec3_t<Q> rhs);

#include "vec3.cc"

#define splat_vec3(V) (V).x, (V).y, (V).z

#endif /* end __SNOW_COMMON__VEC3_THH__ include guard */
