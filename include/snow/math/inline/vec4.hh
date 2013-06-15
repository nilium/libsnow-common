// vec4.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__VEC4_THH__
#define __SNOW_COMMON__VEC4_THH__


namespace snow {


/** @cond IGNORE */
template <typename T> struct vec2_t;
template <typename T> struct vec3_t;
/** @endcond */


/** @addtogroup Math3D */
///@{


/** @brief A four-component vector type.

  @see vec2_t, vec3_t, quat_t, mat3_t, mat4_t, plane_t, line_t
*/
template <typename T = float>
struct S_EXPORT alignas(T) vec4_t {

  /** @copydoc snow::vec3_t::value_type */
  typedef T value_type;

  ///@{
  /** @name Components */
  /** @brief The vector's X component. */
  value_type x;
  /** @brief The vector's Y component. */
  value_type y;
  /** @brief The vector's Z component. */
  value_type z;
  /** @brief The vector's W component. */
  value_type w;
  ///@}

  ///@{
  /** @name Predefined Vectors */
  /** @brief A vector with components \f$\langle 1, 0, 0, 1 \rangle\f$. */
  static const vec4_t pos_X;
  /** @brief A vector with components \f$\langle 0, 1, 0, 1 \rangle\f$. */
  static const vec4_t pos_Y;
  /** @brief A vector with components \f$\langle 0, 0, 1, 1 \rangle\f$. */
  static const vec4_t pos_Z;
  /** @brief A vector with components \f$\langle -1, 0, 0, 1 \rangle\f$. */
  static const vec4_t neg_X;
  /** @brief A vector with components \f$\langle 0, -1, 0, 1 \rangle\f$. */
  static const vec4_t neg_Y;
  /** @brief A vector with components \f$\langle 0, 0, -1, 1 \rangle\f$. */
  static const vec4_t neg_Z;
  /** @brief A vector with components \f$\langle 0, 0, 0, 0 \rangle\f$. */
  static const vec4_t zero;
  /** @brief A vector with components \f$\langle 1, 1, 1, 1 \rangle\f$. */
  static const vec4_t one;
  ///@}

  /** @brief A convenience function for creating a 4D vector. */
  static vec4_t make(value_type x, value_type y, value_type z, value_type w = value_type(1));


  /** @copydoc snow::vec3_t::normalize */
  vec4_t &      normalize();
  /** @copydoc snow::vec3_t::normalized */
  vec4_t        normalized() const;

  /** @copydoc snow::vec3_t::magnitude */
  value_type    magnitude() const;
  /** @copydoc snow::vec3_t::length */
  value_type    length() const;

  /** @copydoc snow::vec3_t::difference */
  vec4_t        difference(vec4_t other) const;
  /** @copydoc snow::vec3_t::subtract */
  vec4_t &      subtract(vec4_t other);

  /** @copydoc snow::vec3_t::sum */
  vec4_t        sum(vec4_t other) const;
  /** @copydoc snow::vec3_t::add */
  vec4_t &      add(vec4_t other);

  /** @copydoc snow::vec3_t::scaled */
  vec4_t        scaled(value_type scalar) const;
  /** @copydoc snow::vec3_t::scaled */
  vec4_t        scaled(vec4_t other) const;
  /** @copydoc snow::vec3_t::scale */
  vec4_t &      scale(value_type scalar);
  /** @copydoc snow::vec3_t::scale */
  vec4_t &      scale(vec4_t other);

  /** @copydoc snow::vec3_t::negated */
  vec4_t        negated() const;
  /** @copydoc snow::vec3_t::negate */
  vec4_t &      negate();

  /** @copydoc snow::vec3_t::inverse */
  vec4_t        inverse() const;
  /** @copydoc snow::vec3_t::invert */
  vec4_t &      invert();

  /** @copydoc snow::vec3_t::dot_product */
  value_type    dot_product(vec4_t other) const;

  /** @copydoc snow::vec3_t::rotate_elems */
  vec4_t &      rotate_elems();
  /** @copydoc snow::vec3_t::rotated_elems */
  vec4_t        rotated_elems() const;

  /** @copydoc add */
  vec4_t &      operator += (vec4_t other);
  /** @copydoc subtract */
  vec4_t &      operator -= (vec4_t other);

  /** @copydoc scale(value_type) */
  vec4_t &      operator *= (value_type scalar);
  /** @copydoc scale(vec4_t) */
  vec4_t &      operator *= (vec4_t other);

  /** @copydoc snow::vec3_t::operator/=(value_type) */
  vec4_t &      operator /= (value_type scalar);
  /** @copydoc snow::vec3_t::operator/=(vec3_t) */
  vec4_t &      operator /= (vec4_t other);

  /** @copydoc snow::vec3_t::operator-() */
  vec4_t        operator - () const;
  /** @copydoc snow::vec3_t::operator~() */
  vec4_t        operator ~ () const;

  /** @copydoc snow::vec3_t::operator[](int) */
  value_type &  operator[] (int index);
  /** @copydoc snow::vec3_t::operator[](int) */
  value_type    operator[] (int index) const;

  /** @brief Returns a pointer to the first component of the vector. */
                operator value_type* ();
  /** @brief Returns a const pointer to the first component of the vector. */
                operator const value_type* () const;

  /** @brief Casts the vector to a vec2_t with value_type Q. */
  template <typename Q>
                operator vec2_t<Q> () const;
  /** @brief Casts the vector to a vec3_t with value_type Q. */
  template <typename Q>
                operator vec3_t<Q> () const;
  /** @brief Casts the vector to a vec4_t with value_type Q. */
  template <typename Q>
                operator vec4_t<Q> () const;

  #include "vec4_swizzles.hh"
};

template <typename T>
std::ostream &operator << (std::ostream &out, vec4_t<T> in);

/** @copydoc snow::operator-(vec3_t<T>,vec3_t<Q>) */
template <typename T, typename Q>
vec4_t<T> operator - (vec4_t<T> lhs, vec4_t<Q> rhs);
/** @copydoc snow::operator+(vec3_t<T>,vec3_t<Q>) */
template <typename T, typename Q>
vec4_t<T> operator + (vec4_t<T> lhs, vec4_t<Q> rhs);

/** @copydoc snow::operator*(vec3_t<T>,vec3_t<Q>) */
template <typename T, typename Q>
vec4_t<T> operator * (vec4_t<T> lhs, vec4_t<Q> rhs);
/** @copydoc snow::operator*(vec3_t<T>,Q) */
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


using vec4f_t = vec4_t<float>;
using vec4d_t = vec4_t<double>;
using vec4i_t = vec4_t<int>;
using vec4l_t = vec4_t<long>;


/** @} */


} // namespace snow


#include "vec4.cc"

#define splat_vec4(V) (V).x, (V).y, (V).z, (V).w


#endif /* end __SNOW_COMMON__VEC4_THH__ include guard */
