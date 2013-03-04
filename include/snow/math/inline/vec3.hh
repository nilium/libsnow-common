// vec3.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__VEC3_THH__
#define __SNOW_COMMON__VEC3_THH__

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

  static const vec3_t pos_X;
  static const vec3_t pos_Y;
  static const vec3_t pos_Z;
  static const vec3_t neg_X;
  static const vec3_t neg_Y;
  static const vec3_t neg_Z;
  static const vec3_t zero;
  static const vec3_t one;

  static vec3_t make(value_type x, value_type y, value_type z);

  vec3_t &      normalize();
  vec3_t        normalized() const;
  value_type    magnitude() const;
  value_type    length() const;
  vec3_t        difference(const vec3_t &other) const;
  vec3_t &      subtract(const vec3_t &other);
  vec3_t        sum(const vec3_t &other) const;
  vec3_t &      add(const vec3_t &other);
  vec3_t        scaled(value_type scalar) const;
  vec3_t        scaled(const vec3_t &other) const;
  vec3_t &      scale(value_type scalar);
  vec3_t &      scale(const vec3_t &other);
  vec3_t        negated() const;
  vec3_t &      negate();
  vec3_t        inverse() const;
  vec3_t &      invert();
  vec3_t        cross_product(const vec3_t &other) const;
  value_type    dot_product(const vec3_t &other) const;

  vec3_t &      operator += (const vec3_t &other);
  vec3_t &      operator -= (const vec3_t &other);

  vec3_t &      operator *= (value_type scalar);
  vec3_t &      operator *= (const vec3_t &other);

  vec3_t &      operator /= (value_type scalar);
  vec3_t &      operator /= (const vec3_t &other);

  vec3_t        operator - () const;
  vec3_t        operator ~ () const;

  value_type &  operator[] (int index);
  value_type    operator[] (int index) const;

                operator value_type* ();
                operator const value_type* () const;

  template <typename Q>
                operator vec3_t<Q> () const;
};

template <typename T>
std::ostream &operator << (std::ostream &out, const vec3_t<T> &in);

template <typename T, typename Q>
vec3_t<T> operator - (const vec3_t<T> &lhs, const vec3_t<Q> &rhs);
template <typename T, typename Q>
vec3_t<T> operator + (const vec3_t<T> &lhs, const vec3_t<Q> &rhs);

template <typename T, typename Q>
vec3_t<T> operator * (const vec3_t<T> &lhs, const vec3_t<Q> &rhs);
template <typename T, typename Q>
vec3_t<T> operator * (const vec3_t<T> &lhs, Q rhs);

template <typename T, typename Q>
vec3_t<T> operator / (const vec3_t<T> &lhs, const vec3_t<Q> &rhs);
template <typename T, typename Q>
vec3_t<T> operator / (const vec3_t<T> &lhs, Q rhs);

template <typename T, typename Q>
T operator % (const vec3_t<T> &lhs, const vec3_t<Q> &rhs);

template <typename T, typename Q>
bool operator == (const vec3_t<T> &lhs, const vec3_t<Q> &rhs);
template <typename T, typename Q>
bool operator != (const vec3_t<T> &lhs, const vec3_t<Q> &rhs);

#include "vec3.cc"

#define splat_vec3(V) (V).x, (V).y, (V).z

#endif /* end __SNOW_COMMON__VEC3_THH__ include guard */
