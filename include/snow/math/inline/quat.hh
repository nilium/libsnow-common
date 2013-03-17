// quat.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__QUAT_THH__
#define __SNOW_COMMON__QUAT_THH__

/*==============================================================================

  Quaternion

==============================================================================*/

template <typename T = float>
struct alignas(T) quat_t {

  typedef T value_type;
  typedef vec3_t<value_type> vec3;

  vec3       xyz;
  value_type w;

  static const quat_t zero;
  static const quat_t one;
  static const quat_t identity;

  static quat_t make(T x, T y, T z, T w);
  static quat_t make(vec3 xyz, T w);
  static quat_t from_angle_axis(T angle, const vec3 &axis);
  static quat_t from_mat4(const mat4_t<T> &mat);
  static quat_t from_mat3(const mat3_t<T> &mat);

  value_type    length() const;
  value_type    magnitude() const;

  quat_t        inverse() const;
  quat_t &      invert();

  quat_t        negated() const;
  quat_t &      negate();

  quat_t        product(const quat_t &other) const;
  quat_t &      multiply(const quat_t &other);

  quat_t        normalized() const;
  quat_t &      normalize();

  quat_t        difference(const quat_t &other) const;
  quat_t &      subtract(const quat_t &other);

  quat_t        sum(const quat_t &other) const;
  quat_t &      add(const quat_t &other);

  quat_t        scaled(value_type scalar) const;
  quat_t        scaled(const quat_t &other) const;
  quat_t &      scale(value_type scalar);
  quat_t &      scale(const quat_t &other);

  value_type    dot_product(const quat_t &other) const;

  quat_t        slerp(const quat_t &to, value_type delta) const;
  quat_t        lerp(const quat_t &to, value_type delta) const;

  quat_t &      operator *= (const quat_t &other);
  quat_t &      operator *= (value_type scalar);
  quat_t &      operator += (const quat_t &other);
  quat_t &      operator -= (const quat_t &other);
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
std::ostream& operator << (std::ostream &out, const quat_t<T> &in);

template <typename T, typename Q>
quat_t<T>     operator *  (const quat_t<T> &lhs, const quat_t<Q> &rhs);
template <typename T, typename Q>
quat_t<T>     operator *  (const quat_t<T> &lhs, Q rhs);

template <typename T, typename Q>
quat_t<T>     operator +  (const quat_t<T> &lhs, const quat_t<Q> &rhs);
template <typename T, typename Q>
quat_t<T>     operator -  (const quat_t<T> &lhs, const quat_t<Q> &rhs);

template <typename T, typename Q>
T             operator %  (const quat_t<T> &lhs, const quat_t<Q> &rhs);

template <typename T, typename Q>
bool          operator == (const quat_t<T> &lhs, const quat_t<Q> &rhs);
template <typename T, typename Q>
bool          operator != (const quat_t<T> &lhs, const quat_t<Q> &rhs);

#include "quat.cc"

#endif /* end __SNOW_COMMON__QUAT_THH__ include guard */
