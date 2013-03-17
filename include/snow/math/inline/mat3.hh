// mat3.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__MAT3_THH__
#define __SNOW_COMMON__MAT3_THH__

/*==============================================================================

  3x3 Square Matrix

==============================================================================*/

template <typename T = float>
struct alignas(4) mat3_t
{

  typedef T value_type;
  typedef vec3_t<T> vec3;
  typedef quat_t<T> quat;

  vec3 r, s, t;
  /*
    r.x   r.y   r.z
    s.x   s.y   s.z
    t.x   t.y   t.z
  */

  static const mat3_t identity;
  static const mat3_t zero;

  static mat3_t make(T rx, T ry, T rz,
                     T sx, T sy, T sz,
                     T tx, T ty, T tz);
  static mat3_t make(const vec3 &x,
                     const vec3 &y,
                     const vec3 &z);
  static mat3_t scaling(const vec3 &off);
  static mat3_t rotation(T angle, const vec3 &axis);
  static mat3_t from_quat(const quat &in);


  mat3_t &      transpose();
  mat3_t        transposed() const;

  vec3          colvec3(int index) const;
  mat3_t &      set_colvec3(int index, const vec3 &col);


  mat3_t        negated() const;
  mat3_t &      negate();

  mat3_t        sum(const mat3_t &other) const;
  mat3_t        sum(T scalar) const;
  mat3_t &      add(const mat3_t &other);
  mat3_t &      add(T scalar);

  mat3_t        difference(const mat3_t &other) const;
  mat3_t        difference(T scalar) const;
  mat3_t &      subtract(const mat3_t &other);
  mat3_t &      subtract(T scalar);

  mat3_t        scaled(T scalar) const;
  mat3_t &      scale(T scalar);
  mat3_t        scaled(const mat3_t &other) const;
  mat3_t &      scale(const mat3_t &other);
  mat3_t        scaled(const vec3 &vec) const;
  mat3_t &      scale(const vec3 &vec);

  mat3_t        inverse() const;
  mat3_t &      invert();

  mat3_t        adjoint() const;
  mat3_t        cofactor() const;
  T             determinant() const;
  mat3_t        orthogonal() const;
  mat3_t &      orthogonalize();

  mat3_t        product(const mat3_t &other) const;
  mat3_t &      multiply(const mat3_t &other);

  vec3          rotate(const vec3 &vec) const;
  vec3          inverse_rotate(const vec3 &vec) const;

  T &           operator [] (int index);
  T             operator [] (int index) const;

                operator T* ();
                operator const T* () const;

                operator quat_t<T>() const;
                operator mat4_t<T>() const;

  mat3_t &      operator *= (const mat3_t &other);
  mat3_t &      operator *= (T scalar);

  mat3_t &      operator += (const mat3_t &other);
  mat3_t &      operator += (T scalar);

  mat3_t &      operator -= (const mat3_t &other);
  mat3_t &      operator -= (T scalar);
}; // struct mat3_t<T>

template <typename T>
std::ostream &operator << (std::ostream &out, const mat3_t<T> &in);
template <typename T, typename Q>
mat3_t<T>     operator *  (const mat3_t<T> &rhs, const mat3_t<Q> &lhs);
template <typename T, typename Q>
vec3_t<T>     operator *  (const mat3_t<T> &rhs, const vec3_t<Q> &lhs);
template <typename T, typename Q>
mat3_t<T>     operator *  (const mat3_t<T> &rhs, const Q &lhs);
template <typename T, typename Q>
mat3_t<T>     operator +  (const mat3_t<T> &rhs, const mat3_t<Q> &lhs);
template <typename T, typename Q>
mat3_t<T>     operator +  (const mat3_t<T> &rhs, const Q &lhs);
template <typename T, typename Q>
mat3_t<T>     operator -  (const mat3_t<T> &rhs, const mat3_t<Q> &lhs);
template <typename T, typename Q>
mat3_t<T>     operator -  (const mat3_t<T> &rhs, const Q &lhs);
template <typename T, typename Q>
bool          operator == (const mat3_t<T> &rhs, const mat3_t<Q> &lhs);
template <typename T, typename Q>
bool          operator != (const mat3_t<T> &rhs, const mat3_t<Q> &lhs);

#include "mat3.cc"

#endif /* end __SNOW_COMMON__MAT3_THH__ include guard */
