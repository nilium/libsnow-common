// math3d.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_MATH3D_HH__
#define __SNOW_MATH3D_HH__

#include <stdexcept>
#include <cmath>
#include <cfloat>
#include <iostream>


namespace snow {


// General degrees-to-radians constant
#define S_DEG2RAD (0.01745329251)



/*==============================================================================
  is_zero

    Tests for whether a value is zero. Specialized for float, double, and long
    double types to see if the value is less than their respective epsilons.
    For all other types, simply returns (v == 0).
==============================================================================*/
template <typename T>
inline
bool is_zero(T value)
{
  return value == T(0);
}

template <>
inline
bool is_zero<float>(float value)
{
  return fabs(value) < FLT_EPSILON;
}

template <>
inline
bool is_zero<double>(double value)
{
  return fabs(value) < DBL_EPSILON;
}

template <>
inline
bool is_zero<long double>(long double value)
{
  return fabs(value) < LDBL_EPSILON;
}


/*==============================================================================
  max_value

    Returns the maximum value for a given type. Only intended for use with
    float, double, and long double. Other types will throw a runtime_error.
==============================================================================*/
template <typename T>
inline
T max_value() { throw std::runtime_error("no set maximum for type"); }

template <>
inline
float max_value<float>() { return FLT_MAX; }

template <>
inline
double max_value<double>() { return DBL_MAX; }

template <>
inline
long double max_value<long double>() { return LDBL_MAX; }



/*==============================================================================

  3-Component Vector

==============================================================================*/

template <typename T = float>
struct vec3_t {
  typedef T value_type;

  value_type x, y, z;

  static const vec3_t zero;
  static const vec3_t one;

  auto normalize() -> vec3_t&
  {
    value_type one_over_mag = magnitude();
    if (one_over_mag != 0) {
      one_over_mag = value_type(1) / one_over_mag;
    }

    x *= one_over_mag;
    y *= one_over_mag;
    z *= one_over_mag;

    return *this;
  }

  auto normalized() const -> vec3_t
  {
    value_type one_over_mag = magnitude();
    if (one_over_mag != 0) {
      one_over_mag = value_type(1) / one_over_mag;
    }

    return {
      x * one_over_mag,
      y * one_over_mag,
      z * one_over_mag
    };
  }

  auto magnitude() const -> value_type
  {
    const value_type sqrlen = length();
    return sqrlen != 0
           ? static_cast<value_type>(sqrtf(sqrlen))
           : 0;
  }

  auto length() const -> value_type
  {
    return x * x + y * y + z * z;
  }

  auto difference(const vec3_t &other) const -> vec3_t
  {
    return {
      x - other.x,
      y - other.y,
      z - other.z
    };
  }

  auto subtract(const vec3_t &other) -> vec3_t&
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  auto sum(const vec3_t &other) const -> vec3_t
  {
    return {
      x + other.x,
      y + other.y,
      z + other.z
    };
  }

  auto add(const vec3_t &other) -> vec3_t&
  {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  auto scaled(value_type scalar) const -> vec3_t
  {
    return {
      x * scalar,
      y * scalar,
      z * scalar
    };
  }

  auto scaled(const vec3_t &other) const -> vec3_t
  {
    return {
      x * other.x,
      y * other.y,
      z * other.z
    };
  }

  auto scale(value_type scalar) -> vec3_t&
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  auto scale(const vec3_t &other) -> vec3_t&
  {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
  }

  auto negated() const -> vec3_t
  {
    return {
      -x, -y, -z
    };
  }

  auto negate() -> vec3_t&
  {
    x = -x;
    y = -y;
    z = -z;
    return *this;
  }

  auto cross_product(const vec3_t &other) const -> vec3_t
  {
    return {
      y * other.z - z * other.y,
      z * other.x - x * other.z,
      x * other.y - y * other.x
    };
  }

  auto dot_product(const vec3_t &other) const -> value_type
  {
    return x * other.x + y * other.y + z * other.z;
  }

  auto operator += (const vec3_t &other) -> vec3_t&
  {
    return add(other);
  }

  auto operator -= (const vec3_t &other) -> vec3_t&
  {
    return subtract(other);
  }

  auto operator *= (value_type scalar) -> vec3_t&
  {
    return scale(scalar);
  }

  auto operator /= (value_type scalar) -> vec3_t&
  {
    return scale(value_type(1) / scalar);
  }

  template <typename Q>
  operator vec3_t<Q> () const
  {
    vec3_t<Q> result;
    result.x = static_cast<Q>(x);
    result.y = static_cast<Q>(y);
    result.z = static_cast<Q>(z);
    return result;
  }
};

template <typename T>
const vec3_t<T> vec3_t<T>::zero = { 0, 0, 0 };

template <typename T>
const vec3_t<T> vec3_t<T>::one = { 1, 1, 1 };

template <typename T>
inline
auto operator << (std::ostream &out, const vec3_t<T> &in) -> std::ostream&
{
  return out << "{ x: " << in.x << ", y: " << in.y << ", z: " << in.z << " }";
}

template <typename T, typename Q>
inline
auto operator - (const vec3_t<T> &lhs,
                 const vec3_t<Q> &rhs) -> vec3_t<T>
{
  return lhs.difference(rhs);
}

template <typename T, typename Q>
inline
auto operator + (const vec3_t<T> &lhs,
                 const vec3_t<Q> &rhs) -> vec3_t<T>
{
  return lhs.sum(rhs);
}

template <typename T, typename Q>
inline
auto operator * (const vec3_t<T> &lhs,
                 const vec3_t<Q> &rhs) -> vec3_t<T>
{
  return lhs.scaled(rhs);
}

template <typename T, typename Q>
inline
auto operator * (const vec3_t<T> &lhs,
                 T rhs) -> vec3_t<T>
{
  return lhs.scaled(rhs);
}

template <typename T, typename Q>
inline
auto operator * (const vec3_t<T> &lhs,
                 Q rhs) -> vec3_t<T>
{
  return lhs.scaled(static_cast<T>(rhs));
}

template <typename T, typename Q>
inline
auto operator / (const vec3_t<T> &lhs,
                 Q rhs) -> vec3_t<T>
{
  return lhs.scaled(T(1) / static_cast<T>(rhs));
}

template <typename T, typename Q>
inline
auto operator % (const vec3_t<T> &lhs,
                 const vec3_t<Q> &rhs) -> T
{
  return lhs.dot_product(rhs);
}

template <typename T, typename Q>
inline
auto operator == (const vec3_t<T> &lhs,
                 const vec3_t<Q> &rhs) -> bool
{
  return
    is_zero(lhs.x - rhs.x) &&
    is_zero(lhs.y - rhs.y) &&
    is_zero(lhs.z - rhs.z);;
}

template <typename T, typename Q>
inline
auto operator != (const vec3_t<T> &lhs,
                  const vec3_t<Q> &rhs) -> bool
{
  return !(lhs == rhs);
}



/*==============================================================================

  4-Component Vector

==============================================================================*/

template <typename T = float>
struct vec4_t {
  typedef T value_type;

  value_type x, y, z, w;

  static const vec4_t zero;
  static const vec4_t one;

  auto normalize() -> vec4_t&
  {
    value_type one_over_mag = magnitude();
    if (one_over_mag != 0) {
      one_over_mag = value_type(1) / one_over_mag;
    }

    x *= one_over_mag;
    y *= one_over_mag;
    z *= one_over_mag;
    w *= one_over_mag;

    return *this;
  }

  auto normalized() const -> vec4_t
  {
    value_type one_over_mag = magnitude();
    if (one_over_mag != 0) {
      one_over_mag = value_type(1) / one_over_mag;
    }

    return {
      x * one_over_mag,
      y * one_over_mag,
      z * one_over_mag,
      w * one_over_mag
    };
  }

  auto magnitude() const -> value_type
  {
    const value_type sqrlen = length();
    return sqrlen != 0
           ? static_cast<value_type>(sqrtf(sqrlen))
           : 0;
  }

  auto length() const -> value_type
  {
    return x * x + y * y + z * z + w * w;
  }

  auto difference(const vec4_t &other) const -> vec4_t
  {
    return {
      x - other.x,
      y - other.y,
      z - other.z,
      w - other.w
    };
  }

  auto subtract(const vec4_t &other) -> vec4_t&
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
  }

  auto sum(const vec4_t &other) const -> vec4_t
  {
    return {
      x + other.x,
      y + other.y,
      z + other.z,
      w + other.w
    };
  }

  auto add(const vec4_t &other) -> vec4_t&
  {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
  }

  auto scaled(value_type scalar) const -> vec4_t
  {
    return {
      x * scalar,
      y * scalar,
      z * scalar,
      w * scalar
    };
  }

  auto scaled(const vec4_t &other) const -> vec4_t
  {
    return {
      x * other.x,
      y * other.y,
      z * other.z,
      w * other.w
    };
  }

  auto scale(value_type scalar) -> vec4_t&
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
  }

  auto scale(const vec4_t &other) -> vec4_t&
  {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;
    return *this;
  }

  auto negated() const -> vec4_t
  {
    return {
      -x, -y, -z, -w
    };
  }

  auto negate() -> vec4_t&
  {
    x = -x;
    y = -y;
    z = -z;
    w = -w;
    return *this;
  }

  auto dot_product(const vec4_t &other) const -> value_type
  {
    return x * other.x + y * other.y + z * other.z + w * other.w;
  }

  auto operator += (const vec4_t &other) -> vec4_t&
  {
    return add(other);
  }

  auto operator -= (const vec4_t &other) -> vec4_t&
  {
    return subtract(other);
  }

  auto operator *= (value_type scalar) -> vec4_t&
  {
    return scale(scalar);
  }

  auto operator /= (value_type scalar) -> vec4_t&
  {
    return scale(value_type(1) / scalar);
  }

  template <typename Q>
  operator vec4_t<Q> ()
  {
    return {
      static_cast<Q>(x),
      static_cast<Q>(y),
      static_cast<Q>(z),
      static_cast<Q>(w)
    };
  }
};

template <typename T>
const vec4_t<T> vec4_t<T>::zero = { 0, 0, 0, 0 };

template <typename T>
const vec4_t<T> vec4_t<T>::one = { 1, 1, 1, 1 };

template <typename T>
inline
auto operator << (std::ostream &out, const vec4_t<T> &in) -> std::ostream&
{
  return out << "{ x: " << in.x << ", y: " << in.y << ", z: " << in.z << ", w: " << in.w << " }";
}

template <typename T, typename Q>
inline
auto operator - (const vec4_t<T> &lhs,
                 const vec4_t<Q> &rhs) -> vec4_t<T>
{
  return lhs.difference(rhs);
}

template <typename T, typename Q>
inline
auto operator + (const vec4_t<T> &lhs,
                 const vec4_t<Q> &rhs) -> vec4_t<T>
{
  return lhs.sum(rhs);
}

template <typename T, typename Q>
inline
auto operator * (const vec4_t<T> &lhs,
                 const vec4_t<Q> &rhs) -> vec4_t<T>
{
  return lhs.scaled(rhs);
}

template <typename T, typename Q>
inline
auto operator * (const vec4_t<T> &lhs,
                 T rhs) -> vec4_t<T>
{
  return lhs.scaled(rhs);
}

template <typename T, typename Q>
inline
auto operator / (const vec4_t<T> &lhs,
                 Q rhs) -> vec4_t<T>
{
  return lhs.scaled(T(1) / static_cast<T>(rhs));
}

template <typename T, typename Q>
inline
auto operator % (const vec4_t<T> &lhs,
                 const vec4_t<Q> &rhs) -> T
{
  return lhs.dot_product(rhs);
}

template <typename T, typename Q>
inline
auto operator == (const vec4_t<T> &lhs,
                 const vec4_t<Q> &rhs) -> bool
{
  return
    is_zero(lhs.x - rhs.x) &&
    is_zero(lhs.y - rhs.y) &&
    is_zero(lhs.z - rhs.z) &&
    is_zero(lhs.w - rhs.w);
}

template <typename T, typename Q>
inline
auto operator != (const vec4_t<T> &lhs,
                  const vec4_t<Q> &rhs) -> bool
{
  return !(lhs == rhs);
}



/*==============================================================================

  Quaternion

==============================================================================*/

template <typename T = float>
struct quat_t {
  typedef T value_type;

  value_type x, y, z, w;

  static const quat_t zero;
  static const quat_t one;
  static const quat_t identity;
};

template <typename T>
const quat_t<T> quat_t<T>::zero = { 0, 0, 0, 0 };

template <typename T>
const quat_t<T> quat_t<T>::one = { 1, 1, 1, 1 };

template <typename T>
const quat_t<T> quat_t<T>::identity = { 0, 0, 0, 1 };



/*==============================================================================

  Line (ported over from Mark Sibly's Blitz3D maths routines)

==============================================================================*/

template <typename T>
struct line_t
{
  typedef T value_type;
  typedef vec3_t<value_type> vec3;
  vec3 origin, dest;

  auto end() const -> vec3
  {
    return origin + dest;
  }

  auto translated(const vec3 &d) const -> line_t
  {
    return {
      origin + d, dest
    };
  }

  auto translate(const vec3 &d) -> line_t&
  {
    origin += d;
    return *this;
  }

  auto scaled(value_type d) const -> line_t
  {
    return {
        origin, dest * d
    };
  }

  auto scale(value_type d) -> line_t&
  {
    dest *= d;
    return *this;
  }

  auto nearest_to(const vec3& p) const -> vec3
  {
    // This is borrowed from Mark Sibly's Blitz3D maths code since it's
    // really rather nice.
    return end().scale(dest.dot_product(p - origin) / dest.dot_product(dest));
  }
};



/*==============================================================================

  Plane

==============================================================================*/





/*==============================================================================

  4x4 Square Matrix

==============================================================================*/

template <typename T = float>
struct mat4_t
{
  typedef T value_type;
  typedef vec3_t<value_type> vec3;
  typedef vec4_t<value_type> vec4;
  typedef quat_t<value_type> quat;

  //         X    Y    Z    W
  value_type m00, m10, m20, m30; // X
  value_type m01, m11, m21, m31; // Y
  value_type m02, m12, m22, m32; // Z
  value_type m03, m13, m23, m33; // Translation

  static const mat4_t identity;
  static const mat4_t zero;

  static
  auto translation(const vec3 &off) -> mat4_t
  {
    return {
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      off.x, off.y, off.z, 1
    };
  }

  static
  auto rotation(value_type angle, const vec3 &axis) -> mat4_t;

  static
  auto frustum(value_type left, value_type right,
               value_type bottom, value_type top,
               value_type near, value_type far) -> mat4_t;

  static
  auto orthographic(value_type left, value_type right,
                    value_type top, value_type bottom,
                    value_type near, value_type far) -> mat4_t;

  static
  auto perspective(value_type fovY, value_type aspect,
                   value_type near, value_type far) -> mat4_t;

  static
  auto look_at(const vec3 &eye, const vec3& center, const vec3& up) -> mat4_t;

  static
  auto from_quat(const quat &in) -> mat4_t
  {
    value_type tx, ty, tz, xx, xy, xz, yy, yz, zz, wx, wy, wz;

    tx = 2.0 * in.x;
    ty = 2.0 * in.y;
    tz = 2.0 * in.z;

    xx = tx * in.x;
    xy = tx * in.y;
    xz = tx * in.z;

    yy = ty * in.y;
    yz = tz * in.y;

    zz = tz * in.w;

    wx = tx * in.w;
    wy = ty * in.w;
    wz = tz * in.w;

    return {
      value_type(1) - (yy + zz), xy - wz, xz + wy, 0,
      xy + wz, value_type(1) - (xx + zz), yz - wx, 0,
      xz - wy, yz + wx, value_type(1) - (xx + yy), 0,
      0, 0, 0, 1
    };
  }

  auto translate(const vec3 &translation) -> mat4_t&;


  auto translated(const vec3 &translation) const -> mat4_t
  {
    mat4_t r = *this;
    return r.translate(translation);
  }

  auto transpose() -> mat4_t&
  {
    #define S_SWAP_ELEM_(LHS, RHS) t=LHS;LHS=RHS;RHS=t;
    value_type t;
    S_SWAP_ELEM_(m10, m01);
    S_SWAP_ELEM_(m20, m02);
    S_SWAP_ELEM_(m30, m03);
    S_SWAP_ELEM_(m21, m12);
    S_SWAP_ELEM_(m31, m13);
    S_SWAP_ELEM_(m32, m23);
    #undef S_SWAP_ELEM_
    return *this;
  }

  auto transposed() const -> mat4_t
  {
    mat4_t t = *this;
    return t.transpose();
  }

  auto rowvec4(int index) const -> vec4
  {
    switch (index) {
      case 0: return { m00, m10, m20, m30 };
      case 1: return { m01, m11, m21, m31 };
      case 2: return { m02, m12, m22, m32 };
      case 3: return { m03, m13, m23, m33 };
      default: throw std::out_of_range("attempt to access out of range row");
    }
  }

  auto colvec4(int index) const -> vec4
  {
    switch (index) {
      case 0: return { m00, m01, m02, m03 };
      case 1: return { m10, m11, m12, m13 };
      case 2: return { m20, m21, m22, m23 };
      case 3: return { m30, m31, m32, m33 };
      default: throw std::out_of_range("attempt to access out of range column");
    }
  }

  auto set_rowvec4(int index, const vec4 &row) -> mat4_t&
  {
    switch (index) {
      case 0: m00 = row.x; m01 = row.y; m02 = row.z; m03 = row.w; break;
      case 1: m10 = row.x; m11 = row.y; m12 = row.z; m13 = row.w; break;
      case 2: m20 = row.x; m21 = row.y; m22 = row.z; m23 = row.w; break;
      case 3: m30 = row.x; m31 = row.y; m32 = row.z; m33 = row.w; break;
      default: throw std::out_of_range("attempt to access out of range row");
    }
    return *this;
  }

  auto set_colvec4(int index, const vec4 &col) -> mat4_t&
  {
    switch (index) {
      case 0: m00 = col.x; m01 = col.y; m02 = col.z; m03 = col.w; break;
      case 1: m10 = col.x; m11 = col.y; m12 = col.z; m13 = col.w; break;
      case 2: m20 = col.x; m21 = col.y; m22 = col.z; m23 = col.w; break;
      case 3: m30 = col.x; m31 = col.y; m32 = col.z; m33 = col.w; break;
      default: throw std::out_of_range("attempt to access out of range column");
    }
    return *this;
  }

  auto rowvec3(int index) const -> vec3
  {
    switch (index) {
      case 0: return { m00, m10, m20 };
      case 1: return { m01, m11, m21 };
      case 2: return { m02, m12, m22 };
      case 3: return { m03, m13, m23 };
      default: throw std::out_of_range("attempt to access out of range row");
    }
  }

  auto colvec3(int index) const -> vec3
  {
    switch (index) {
      case 0: return { m00, m01, m02 };
      case 1: return { m10, m11, m12 };
      case 2: return { m20, m21, m22 };
      case 3: return { m30, m31, m32 };
      default: throw std::out_of_range("attempt to access out of range column");
    }
  }

  auto set_rowvec3(int index, const vec3 &row) -> mat4_t&
  {
    switch (index) {
      case 0: m00 = row.x; m01 = row.y; m02 = row.z; m03 = 0; break;
      case 1: m10 = row.x; m11 = row.y; m12 = row.z; m13 = 0; break;
      case 2: m20 = row.x; m21 = row.y; m22 = row.z; m23 = 0; break;
      case 3: m30 = row.x; m31 = row.y; m32 = row.z; m33 = 1; break;
      default: throw std::out_of_range("attempt to access out of range row");
    }
    return *this;
  }

  auto set_colvec3(int index, const vec3 &col) -> mat4_t&
  {
    switch (index) {
      case 0: m00 = col.x; m01 = col.y; m02 = col.z; m03 = 0; break;
      case 1: m10 = col.x; m11 = col.y; m12 = col.z; m13 = 0; break;
      case 2: m20 = col.x; m21 = col.y; m22 = col.z; m23 = 0; break;
      case 3: m30 = col.x; m31 = col.y; m32 = col.z; m33 = 1; break;
      default: throw std::out_of_range("attempt to access out of range column");
    }
    return *this;
  }

  auto inverse_orthogonal() -> mat4_t
  {
    vec3_t<value_type> row = { m03, m13, m23 };
    mat4_t temp = {
      m00, m01, m02, 0,
      m10, m11, m12, 0,
      m20, m21, m22, 0,
      0, 0, 0, 1
    };
    temp.m03 = -row.dot_product(temp.colvec3(0));
    temp.m13 = -row.dot_product(temp.colvec3(1));
    temp.m23 = -row.dot_product(temp.colvec3(2));
    return temp;
  }

  auto sum(value_type scalar) const -> mat4_t
  {
    return {
      m00 + scalar,
      m10 + scalar,
      m20 + scalar,
      m30 + scalar,
      m01 + scalar,
      m11 + scalar,
      m21 + scalar,
      m31 + scalar,
      m02 + scalar,
      m12 + scalar,
      m22 + scalar,
      m32 + scalar,
      m03 + scalar,
      m13 + scalar,
      m23 + scalar,
      m33 + scalar
    };
  }

  auto add(value_type scalar) -> mat4_t&
  {
    m00 += scalar;
    m10 += scalar;
    m20 += scalar;
    m30 += scalar;
    m01 += scalar;
    m11 += scalar;
    m21 += scalar;
    m31 += scalar;
    m02 += scalar;
    m12 += scalar;
    m22 += scalar;
    m32 += scalar;
    m03 += scalar;
    m13 += scalar;
    m23 += scalar;
    m33 += scalar;
    return *this;
  }

  auto scaled(value_type scalar) const -> mat4_t
  {
    return {
      m00 * scalar,
      m10 * scalar,
      m20 * scalar,
      m30 * scalar,
      m01 * scalar,
      m11 * scalar,
      m21 * scalar,
      m31 * scalar,
      m02 * scalar,
      m12 * scalar,
      m22 * scalar,
      m32 * scalar,
      m03 * scalar,
      m13 * scalar,
      m23 * scalar,
      m33 * scalar
    };
  }

  auto scale(value_type scalar) -> mat4_t&
  {
    m00 *= scalar;
    m10 *= scalar;
    m20 *= scalar;
    m30 *= scalar;
    m01 *= scalar;
    m11 *= scalar;
    m21 *= scalar;
    m31 *= scalar;
    m02 *= scalar;
    m12 *= scalar;
    m22 *= scalar;
    m32 *= scalar;
    m03 *= scalar;
    m13 *= scalar;
    m23 *= scalar;
    m33 *= scalar;
    return *this;
  }

  auto scaled(const vec3 &vec) const -> mat4_t
  {
    return {
      m00 * vec.x,
      m10 * vec.x,
      m20 * vec.x,
      m30,
      m01 * vec.y,
      m11 * vec.y,
      m21 * vec.y,
      m31,
      m02 * vec.z,
      m12 * vec.z,
      m22 * vec.z,
      m32,
      m03,
      m13,
      m23,
      m33
    };
  }

  auto scale(const vec3 &vec) -> mat4_t&
  {
    m00 *= vec.x;
    m10 *= vec.x;
    m20 *= vec.x;
    m01 *= vec.y;
    m11 *= vec.y;
    m21 *= vec.y;
    m02 *= vec.z;
    m12 *= vec.z;
    m22 *= vec.z;
    return *this;
  }

  auto operator[] (int index) -> value_type&
  {
    if (index < 0 || index > 15)
      throw std::out_of_range("attempt to access out of range element");
    return (&m00)[index];
  }

  auto operator[] (int index) const -> value_type
  {
    if (index < 0 || index > 15)
      throw std::out_of_range("attempt to access out of range element");
    return (&m00)[index];
  }

  operator value_type* ()
  {
    return &m00;
  }

  operator const value_type* () const
  {
    return &m00;
  }

  auto inverse_affine(mat4_t &out) const -> bool;
  auto adjoint() const -> mat4_t;

  auto cofactor(int r0, int r1, int r2, int c0, int c1, int c2) const -> value_type
  {
    const value_type *m = &m00;
    // Macro for getting the value at the cofactor address provided
    #define S_CF_ADDR_(l, r) (m[(l * 4) + r])
    return (
      (S_CF_ADDR_(r0,c0) * ((S_CF_ADDR_(r1, c1) * S_CF_ADDR_(r2, c2)) - (S_CF_ADDR_(r2, c1) * S_CF_ADDR_(r1, c2)))) -
      (S_CF_ADDR_(r0,c1) * ((S_CF_ADDR_(r1, c0) * S_CF_ADDR_(r2, c2)) - (S_CF_ADDR_(r2, c0) * S_CF_ADDR_(r1, c2)))) +
      (S_CF_ADDR_(r0,c2) * ((S_CF_ADDR_(r1, c0) * S_CF_ADDR_(r2, c1)) - (S_CF_ADDR_(r2, c0) * S_CF_ADDR_(r1, c1))))
    );
    #undef S_CF_ADDR_
  }

  auto determinant() const -> value_type
  {
    return ((m00 * cofactor(1, 2, 3, 1, 2, 3)) -
            (m01 * cofactor(1, 2, 3, 0, 2, 3)) +
            (m02 * cofactor(1, 2, 3, 0, 1, 3)) -
            (m03 * cofactor(1, 2, 3, 0, 1, 2)));
  }

  auto inverse_general(mat4_t &out) const -> bool
  {
    value_type det = determinant();

    if (is_zero(det)) {
      return false;
    }
    out = adjoint().scale(value_type(1) / det);
    return true;
  }

  auto product(const mat4_t &other) const -> mat4_t
  {
    mat4_t temp;
    value_type *m = &temp.m00;
    const vec4 rowvecs[4] = {
      rowvec4(0),
      rowvec4(1),
      rowvec4(2),
      rowvec4(3)
    };

    for (int cindex = 0; cindex < 4; ++cindex) {
      vec4 column = colvec4(cindex);
      m[cindex     ] = column.dot_product(rowvecs[0]);
      m[cindex + 4 ] = column.dot_product(rowvecs[1]);
      m[cindex + 8 ] = column.dot_product(rowvecs[2]);
      m[cindex + 12] = column.dot_product(rowvecs[3]);
    }

    return temp;
  }

  auto multiply(const mat4_t &other) -> mat4_t&
  {
    value_type *m = &m00;
    const vec4 rowvecs[4] = {
      rowvec4(0),
      rowvec4(1),
      rowvec4(2),
      rowvec4(3)
    };

    for (int cindex = 0; cindex < 4; ++cindex) {
      const vec4 column = colvec4(cindex);
      m[cindex     ] = column.dot_product(rowvecs[0]);
      m[cindex + 4 ] = column.dot_product(rowvecs[1]);
      m[cindex + 8 ] = column.dot_product(rowvecs[2]);
      m[cindex + 12] = column.dot_product(rowvecs[3]);
    }

    return *this;
  }

  auto multiply(const vec4 &vec) -> vec4
  {
    return {
      vec.dot_product(colvec4(0)),
      vec.dot_product(colvec4(1)),
      vec.dot_product(colvec4(2)),
      vec.dot_product(colvec4(3))
    };
  }

  auto multiply(const vec3 &vec) -> vec3
  {
    return {
      vec.dot_product(colvec3(0)) + m03,
      vec.dot_product(colvec3(1)) + m13,
      vec.dot_product(colvec3(2)) + m23
    };
  }

  auto rotate(const vec3 &vec) -> vec3
  {
    return {
      vec.dot_product(colvec3(0)),
      vec.dot_product(colvec3(1)),
      vec.dot_product(colvec3(2))
    };
  }

  auto inverse_rotate(const vec3 &vec) -> vec3
  {
    return {
      vec.dot_product(rowvec3(0)),
      vec.dot_product(rowvec3(1)),
      vec.dot_product(rowvec3(2))
    };
  }

}; // struct mat4_t<T>

template <typename T>
const mat4_t<T> mat4_t<T>::identity = {
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
};

template <typename T>
const mat4_t<T> mat4_t<T>::zero = {
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0
};

template <typename T>
auto mat4_t<T>::rotation(value_type angle, const vec3 &axis) -> mat4_t
{
  angle *= S_DEG2RAD;
  const value_type  c = static_cast<value_type>(cosf(angle));
  const value_type  s = static_cast<value_type>(sinf(angle));
  const value_type ic = value_type(1) - c;
  const value_type xy = axis.x * axis.y * ic;
  const value_type yz = axis.y * axis.z * ic;
  const value_type xz = axis.x * axis.z * ic;
  const value_type xs = s * axis.x;
  const value_type ys = s * axis.y;
  const value_type zs = s * axis.z;

  return {
    ((axis.x * axis.x) * ic) + c,  xy + zs,  xz - ys,  0,
    xy - zs,  ((axis.y * axis.y) * ic) + c,  yz + xs,  0,
    xz + ys,  yz - xs,  ((axis.z * axis.z) * ic) + c,  0,
    0,  0,  0,  1
  };
}

template <typename T>
auto mat4_t<T>::frustum(value_type left, value_type right,
                     value_type bottom, value_type top,
                     value_type near, value_type far) -> mat4_t
{
  const value_type xdelta = right - left;
  const value_type ydelta = top - bottom;
  const value_type zdelta = far - near;
  const value_type neardouble = 2.0 * near;

  return {
    neardouble / xdelta, 0, (right + left) / xdelta, 0,
    0, neardouble / ydelta, (top + bottom) / ydelta, 0,
    0, 0, -((far + near) / zdelta), -1.0,
    0, 0, -((neardouble * far) / zdelta), 0
  };
}

template <typename T>
auto mat4_t<T>::orthographic(value_type left, value_type right,
                          value_type top, value_type bottom,
                          value_type near, value_type far) -> mat4_t
{
  const value_type xdelta = right - left;
  const value_type ydelta = top - bottom;
  const value_type zdelta = far - near;

  return {
    value_type(2) / xdelta, 0, 0, 0,
    0, value_type(2) / ydelta, 0, 0,
    0, 0, value_type(-2) / zdelta, 0,
    -((right + left) / xdelta), -((top + bottom) / ydelta), -((far + near) / zdelta), 1
  };
}

template <typename T>
auto mat4_t<T>::perspective(value_type fovY, value_type aspect,
                         value_type near, value_type far) -> mat4_t
{
  const float r = static_cast<value_type>(tanf(fovY * 0.5 * S_DEG2RAD));
  const float left = -r * aspect;
  const float right = r * aspect;
  const float bottom = -r;
  const float top = r;
  const float twoNear = 2 * near;
  const float zdelta = value_type(1) / (near - far);

  return {
    twoNear * (right - left), 0, 0, 0,
    0, twoNear / (top - bottom), 0, 0,
    0, 0, (far + near) * zdelta, -1,
    0, 0, (twoNear * far) * zdelta, 0
  };
}

template <typename T>
auto mat4_t<T>::look_at(const vec3 &eye, const vec3& center,
                     const vec3& up) -> mat4_t
{
  vec3 facing_norm = (center - eye).normalize();
  vec3 up_norm = up.normalized();
  vec3 s = (facing_norm.cross_product(up_norm)).normalize();
  up_norm = s.cross_product(facing_norm);
  facing_norm.negate();
  up_norm.y = - up_norm.y;

  mat4_t looker = {
    up_norm.x, up_norm.y, up_norm.z, 0,
    facing_norm.x, facing_norm.y, facing_norm.z, 0,
    0, 0, 0, 0,
    0, 0, 0, 1
  };

  return looker.translate(eye.negated());
}

template <typename T>
auto mat4_t<T>::translate(const vec3 &translation) -> mat4_t&
{
  m03 += translation.dot_product(colvec3(0));
  m13 += translation.dot_product(colvec3(1));
  m23 += translation.dot_product(colvec3(2));
  m33 += translation.dot_product(colvec3(3));
  return *this;
}

template <typename T>
auto mat4_t<T>::inverse_affine(mat4_t& out) const -> bool
{
  value_type det;

  mat4_t temp = {
    (m11 * m22) - (m21 * m12),
    (m20 * m12) - (m01 * m22),
    (m01 * m21) - (m20 * m11),
    0,

    (m21 * m02) - (m01 * m22),
    (m00 * m22) - (m20 * m02),
    (m20 * m01) - (m00 * m21),
    0,

    (m01 * m12) - (m11 * m02),
    (m01 * m02) - (m00 * m12),
    (m00 * m11) - (m01 * m01),
    0,

    0, 0, 0, 0
  };

  det = m00 * temp.m00 + m10 * temp.m01 + m20 * temp.m02;
  if (is_zero(det)) {
    return false;
  }

  temp.scale(value_type(1) / det);
  vec3 txyz = rowvec3(3);

  temp.m03 = -txyz.dot_product(temp.colvec3(0));
  temp.m13 = -txyz.dot_product(temp.colvec3(1));
  temp.m23 = -txyz.dot_product(temp.colvec3(2));

  out = temp;

  return true;
}

template <typename T>
auto mat4_t<T>::adjoint() const -> mat4_t
{
  return {
     cofactor(1, 2, 3, 1, 2, 3),
    -cofactor(0, 2, 3, 1, 2, 3),
     cofactor(0, 1, 3, 1, 2, 3),
    -cofactor(0, 1, 2, 1, 2, 3),

    -cofactor(1, 2, 3, 0, 2, 3),
     cofactor(0, 2, 3, 0, 2, 3),
    -cofactor(0, 1, 3, 0, 2, 3),
     cofactor(0, 1, 2, 0, 2, 3),

     cofactor(1, 2, 3, 0, 1, 3),
    -cofactor(0, 2, 3, 0, 1, 3),
     cofactor(0, 1, 3, 0, 1, 3),
    -cofactor(0, 1, 2, 0, 1, 3),

    -cofactor(1, 2, 3, 0, 1, 2),
     cofactor(0, 2, 3, 0, 1, 2),
    -cofactor(0, 1, 3, 0, 1, 2),
     cofactor(0, 1, 2, 0, 1, 2)
  };
}

template <typename T>
auto operator == (const mat4_t<T> &rhs,
                  const mat4_t<T> &lhs) -> bool
{
  return
  is_zero(lhs.m00 - rhs.m00) &&
  is_zero(lhs.m10 - rhs.m10) &&
  is_zero(lhs.m20 - rhs.m20) &&

  is_zero(lhs.m01 - rhs.m01) &&
  is_zero(lhs.m11 - rhs.m11) &&
  is_zero(lhs.m21 - rhs.m21) &&

  is_zero(lhs.m02 - rhs.m02) &&
  is_zero(lhs.m12 - rhs.m12) &&
  is_zero(lhs.m22 - rhs.m22) &&

  is_zero(lhs.m03 - rhs.m03) &&
  is_zero(lhs.m13 - rhs.m13) &&
  is_zero(lhs.m23 - rhs.m23) &&

  is_zero(lhs.m30 - rhs.m30) &&
  is_zero(lhs.m31 - rhs.m31) &&
  is_zero(lhs.m32 - rhs.m32) &&
  is_zero(lhs.m33 - rhs.m33);
}

typedef vec3_t<float> vec3f_t;
typedef vec3_t<double> vec3d_t;
typedef vec4_t<float> vec4f_t;
typedef vec4_t<double> vec4d_t;
typedef quat_t<float> quatf_t;
typedef quat_t<double> quatd_t;
typedef line_t<float> linef_t;
typedef line_t<double> lined_t;
// typedef plane_t<float> planef_t;
// typedef plane_t<double> planed_t;
typedef mat4_t<float> mat4f_t;
typedef mat4_t<double> mat4d_t;


} // namespace snow

#endif /* end __SNOW_MATH3D_HH__ include guard */
