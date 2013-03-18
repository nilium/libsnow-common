// vec2.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__VEC2_THH__
#define __SNOW_COMMON__VEC2_THH__

/*==============================================================================

  2-Component Vector

==============================================================================*/

template <typename T = float>
struct alignas(T) vec2_t {

  typedef T value_type;

  value_type x, y;

  static const vec2_t pos_X;
  static const vec2_t pos_Y;
  static const vec2_t neg_X;
  static const vec2_t neg_Y;
  static const vec2_t zero;
  static const vec2_t one;

  static vec2_t make(value_type x, value_type y);

  vec2_t &      normalize();
  vec2_t        normalized() const;
  value_type    magnitude() const;
  value_type    length() const;
  vec2_t        difference(const vec2_t &other) const;
  vec2_t &      subtract(const vec2_t &other);
  vec2_t        sum(const vec2_t &other) const;
  vec2_t &      add(const vec2_t &other);
  vec2_t        scaled(value_type scalar) const;
  vec2_t        scaled(const vec2_t &other) const;
  vec2_t &      scale(value_type scalar);
  vec2_t &      scale(const vec2_t &other);
  vec2_t        negated() const;
  vec2_t &      negate();
  vec2_t        inverse() const;
  vec2_t &      invert();
  value_type    dot_product(const vec2_t &other) const;
  vec2_t &      rotate_elems();
  vec2_t        rotated_elems() const;

  vec2_t &      operator += (const vec2_t &other);
  vec2_t &      operator -= (const vec2_t &other);

  vec2_t &      operator *= (value_type scalar);
  vec2_t &      operator *= (const vec2_t &other);

  vec2_t &      operator /= (value_type scalar);
  vec2_t &      operator /= (const vec2_t &other);

  vec2_t        operator - () const;
  vec2_t        operator ~ () const;

  value_type &  operator[] (int index);
  value_type    operator[] (int index) const;

                operator value_type* ();
                operator const value_type* () const;

  template <typename Q>
                operator vec2_t<Q> () const;
};

template <typename T>
std::ostream &operator << (std::ostream &out, const vec2_t<T> &in);

template <typename T, typename Q>
vec2_t<T> operator - (const vec2_t<T> &lhs, const vec2_t<Q> &rhs);
template <typename T, typename Q>
vec2_t<T> operator + (const vec2_t<T> &lhs, const vec2_t<Q> &rhs);

template <typename T, typename Q>
vec2_t<T> operator * (const vec2_t<T> &lhs, const vec2_t<Q> &rhs);
template <typename T, typename Q>
vec2_t<T> operator * (const vec2_t<T> &lhs, Q rhs);

template <typename T, typename Q>
vec2_t<T> operator / (const vec2_t<T> &lhs, const vec2_t<Q> &rhs);
template <typename T, typename Q>
vec2_t<T> operator / (const vec2_t<T> &lhs, Q rhs);

template <typename T, typename Q>
T operator % (const vec2_t<T> &lhs, const vec2_t<Q> &rhs);

template <typename T, typename Q>
bool operator == (const vec2_t<T> &lhs, const vec2_t<Q> &rhs);
template <typename T, typename Q>
bool operator != (const vec2_t<T> &lhs, const vec2_t<Q> &rhs);

#include "vec2.cc"

#define splat_vec2(V) (V).x, (V).y, (V).z

#endif /* end __SNOW_COMMON__VEC2_THH__ include guard */
