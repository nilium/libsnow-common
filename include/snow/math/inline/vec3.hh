// vec3.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__VEC3_THH__
#define __SNOW_COMMON__VEC3_THH__


namespace snow {


/** @cond IGNORE */
template <typename T> struct vec2_t;
template <typename T> struct vec4_t;
/** @endcond */


/** @addtogroup Math3D */
///@{


/** @brief A three-component vector type.

  @see vec2_t, vec4_t, quat_t, mat3_t, mat4_t, plane_t, line_t
*/
template <typename T = float>
struct S_EXPORT alignas(T) vec3_t {

  /** @brief The type of the vector components. */
  typedef T value_type;

  ///@{
  /** @name Components */
  /** @brief The vector's X component. */
  value_type x;
  /** @brief The vector's Y component. */
  value_type y;
  /** @brief The vector's Z component. */
  value_type z;
  ///@}

  ///@{
  /** @name Predefined Vectors */
  /** @brief A vector of \f$\langle 1, 0, 0 \rangle\f$. */
  static const vec3_t pos_X;
  /** @brief A vector of \f$\langle 0, 1, 0 \rangle\f$. */
  static const vec3_t pos_Y;
  /** @brief A vector of \f$\langle 0, 0, 1 \rangle\f$. */
  static const vec3_t pos_Z;
  /** @brief A vector of \f$\langle -1, 0, 0 \rangle\f$. */
  static const vec3_t neg_X;
  /** @brief A vector of \f$\langle 0, -1, 0 \rangle\f$. */
  static const vec3_t neg_Y;
  /** @brief A vector of \f$\langle 0, 0, -1 \rangle\f$. */
  static const vec3_t neg_Z;
  /** @brief A vector of \f$\langle 0, 0, 0 \rangle\f$. */
  static const vec3_t zero;
  /** @brief A vector of \f$\langle 1, 1, 1 \rangle\f$. */
  static const vec3_t one;
  ///@}

  /** @brief Convenience function to make a vec3_t. */
  static vec3_t make(value_type x, value_type y, value_type z);
  /** @brief @copybrief make(value_type, value_type, value_type) */
  static vec3_t make(value_type value);

  /** @brief Normalizes the vector. */
  vec3_t &      normalize();
  /** @brief Returns a normalized copy of the vector. */
  vec3_t        normalized() const;
  /** @brief Returns the magnitude of the vector.

    This is the same as getting the square root of the length of the vector.
  */
  value_type    magnitude() const;
  /** @brief Returns the square length of the vector. */
  value_type    length() const;
  /** @brief Returns the difference of two vectors. */
  vec3_t        difference(vec3_t other) const;
  /** @brief Subtracts a vector from this vector. */
  vec3_t &      subtract(vec3_t other);
  /** @brief Returns the sum of two vectors. */
  vec3_t        sum(vec3_t other) const;
  /** @brief Adds a vector to this vector. */
  vec3_t &      add(vec3_t other);
  /** @brief Returns a copy of this vector whose components have been scaled by
    a scalar value.
  */
  vec3_t        scaled(value_type scalar) const;
  /** @brief Returns a copy of this vector whose components have been scaled by
    another vector's components.
  */
  vec3_t        scaled(vec3_t other) const;
  /** @brief Scales this vector's components by a scalar value. */
  vec3_t &      scale(value_type scalar);
  /** @brief Scales this vector's components by another vector's components. */
  vec3_t &      scale(vec3_t other);
  /** @brief Returns a copy of this vector with all components negated. */
  vec3_t        negated() const;
  /** @brief Negates this vector's components. */
  vec3_t &      negate();
  /** @brief Returns a copy of this vector with the inverse of all of its components.

    This is simply the result of returning a vector whose components are their
    multiplicative inverse:
    \f$
      \displaystyle
      \left\langle
        \frac{1}{\mathbf{V}_1},
        \frac{1}{\mathbf{V}_2},
        \cdots,
        \frac{1}{\mathbf{V}_n}
      \right\rangle
    \f$

    Where a component is zero, its inverse is also zero, as division by zero
    isn't really a preferable alternative.
  */
  vec3_t        inverse() const;
  /** @brief Inverts this vector.
    @see inverse()
  */
  vec3_t &      invert();
  /** @brief Returns the cross product of this and another vector. */
  vec3_t        cross_product(vec3_t other) const;
  /** @brief Returns the dot product of this and another vector. */
  value_type    dot_product(vec3_t other) const;
  /** @brief Rotates the components of this vector forward.
    @see rotated_elems()
  */
  vec3_t &      rotate_elems();
  /** @brief Returns a copy of this vector with its components rotated forward.

    This function returns a copy of the vector such that its components are
    all rotated forward, with the last component as the first component, the
    first as the second, and so on:
    \f$
      \displaystyle
      \left\langle
        \mathbf{V}_{n},
        \mathbf{V}_{1},
        \mathbf{V}_{2},
        \cdots,
        \mathbf{V}_{n - 1}
      \right\rangle
    \f$

    Of course, for 2D vectors this is a simple swap. For 3D vectors and 4D
    vectors, it just copies the first N-1 components forward by one component
    and places its last component in the first component's place.
  */
  vec3_t        rotated_elems() const;

  ///@{
  /** @name Operators */
  /** @copydoc add */
  vec3_t &      operator += (vec3_t other);
  /** @copydoc subtract */
  vec3_t &      operator -= (vec3_t other);

  /** @copydoc scale(value_type) */
  vec3_t &      operator *= (value_type scalar);
  /** @copydoc scale(vec3_t) */
  vec3_t &      operator *= (vec3_t other);

  /** @brief Divides the vector's components by a scalar.

    This is the same as calling `scale(1.0 / scalar)`.
  */
  vec3_t &      operator /= (value_type scalar);
  /** @brief Divides the vector's components by another vector's components.

    This is the same as calling `scale(other.inverse())`.
  */
  vec3_t &      operator /= (vec3_t other);

  /** @copydoc negate */
  vec3_t        operator - () const;
  /** @copydoc inverse */
  vec3_t        operator ~ () const;

  /** @brief Returns the component at the given index.

    @param index The index of the component. Must be one of the following for
        the given vector type:

        Index | Component | Applies To
        ----- | --------- | ----------
        0 | X | vec2_t, vec3_t, vec4_t, and quat_t
        1 | Y | vec2_t, vec3_t, vec4_t, and quat_t
        2 | Z | vec3_t, vec4_t, and quat_t
        3 | W | vec4_t and quat_t

        Any other index is undefined behavior.
  */
  value_type &  operator[] (int index);
  /** @copydoc snow::vec3_t::operator[](int index) */
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

  ///@}

  #include "vec3_swizzles.hh"
};

/** @brief Writes a vector to an output stream. */
template <typename T>
std::ostream &operator << (std::ostream &out, vec3_t<T> in);

/** @copydoc vec3_t::difference */
template <typename T, typename Q>
vec3_t<T> operator - (vec3_t<T> lhs, vec3_t<Q> rhs);
/** @copydoc vec3_t::sum */
template <typename T, typename Q>
vec3_t<T> operator + (vec3_t<T> lhs, vec3_t<Q> rhs);

/** @copydoc vec3_t::scaled(vec3_t)const */
template <typename T, typename Q>
vec3_t<T> operator * (vec3_t<T> lhs, vec3_t<Q> rhs);
/** @copydoc vec3_t::scaled(value_type)const */
template <typename T, typename Q>
vec3_t<T> operator * (vec3_t<T> lhs, Q rhs);

/** @brief Returns a copy of the left-vector whose components have been divided
  by the components of the right-vector.

  This is the same as `lhs.scale(rhs.inverse())`.
*/
template <typename T, typename Q>
vec3_t<T> operator / (vec3_t<T> lhs, vec3_t<Q> rhs);
/** @brief Returns a copy of the left-vector whose components have been divided
  by an arbitrary value.

  This is the same as `lhs.scale(1.0 / rhs)`.
*/
template <typename T, typename Q>
vec3_t<T> operator / (vec3_t<T> lhs, Q rhs);

/** @brief Returns the dot product of two vector. */
template <typename T, typename Q>
T operator % (vec3_t<T> lhs, vec3_t<Q> rhs);

/** @brief Returns whether two vectors are equivalent -- not whether they're
  equals. This is done by checking if their absolute delta is less than
  S_EPSILON.
*/
template <typename T, typename Q>
bool operator == (vec3_t<T> lhs, vec3_t<Q> rhs);
/** @brief Returns whether two vectors are not equivalent.
  @see snow::operator==(vec3_t<T>, vec3_t<Q>)
*/
template <typename T, typename Q>
bool operator != (vec3_t<T> lhs, vec3_t<Q> rhs);


using vec3f_t = vec3_t<float>;
using vec3d_t = vec3_t<double>;
using vec3i_t = vec3_t<int>;
using vec3l_t = vec3_t<long>;


///@}


} // namespace snow


#include "vec3.cc"


#define splat_vec3(V) (V).x, (V).y, (V).z


#endif /* end __SNOW_COMMON__VEC3_THH__ include guard */
