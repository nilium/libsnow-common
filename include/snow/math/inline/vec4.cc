// vec4.cc -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__VEC4_TCC__
#define __SNOW_COMMON__VEC4_TCC__

template <typename T>
const vec4_t<T> vec4_t<T>::pos_X = { 1, 0, 0, 1 };

template <typename T>
const vec4_t<T> vec4_t<T>::pos_Y = { 0, 1, 0, 1 };

template <typename T>
const vec4_t<T> vec4_t<T>::pos_Z = { 0, 0, 1, 1 };

template <typename T>
const vec4_t<T> vec4_t<T>::neg_X = { -1, 0, 0, 1 };

template <typename T>
const vec4_t<T> vec4_t<T>::neg_Y = { 0, -1, 0, 1 };

template <typename T>
const vec4_t<T> vec4_t<T>::neg_Z = { 0, 0, -1, 1 };

template <typename T>
const vec4_t<T> vec4_t<T>::zero = { 0, 0, 0, 0 };

template <typename T>
const vec4_t<T> vec4_t<T>::one = { 1, 1, 1, 1 };

template <typename T>
auto vec4_t<T>::make(value_type x, value_type y, value_type z, value_type w) -> vec4_t
{
  return { x, y, z, w };
}

template <typename T>
auto vec4_t<T>::normalize() -> vec4_t&
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

template <typename T>
auto vec4_t<T>::normalized() const -> vec4_t
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

template <typename T>
auto vec4_t<T>::magnitude() const -> value_type
{
  const value_type sqrlen = length();
  return sqrlen != 0
         ? static_cast<value_type>(std::sqrt(sqrlen))
         : 0;
}

template <typename T>
auto vec4_t<T>::length() const -> value_type
{
  return x * x + y * y + z * z + w * w;
}

template <typename T>
auto vec4_t<T>::difference(const vec4_t &other) const -> vec4_t
{
  return {
    x - other.x,
    y - other.y,
    z - other.z,
    w - other.w
  };
}

template <typename T>
auto vec4_t<T>::subtract(const vec4_t &other) -> vec4_t&
{
  x -= other.x;
  y -= other.y;
  z -= other.z;
  w -= other.w;
  return *this;
}

template <typename T>
auto vec4_t<T>::sum(const vec4_t &other) const -> vec4_t
{
  return {
    x + other.x,
    y + other.y,
    z + other.z,
    w + other.w
  };
}

template <typename T>
auto vec4_t<T>::add(const vec4_t &other) -> vec4_t&
{
  x += other.x;
  y += other.y;
  z += other.z;
  w += other.w;
  return *this;
}

template <typename T>
auto vec4_t<T>::scaled(value_type scalar) const -> vec4_t
{
  return {
    x * scalar,
    y * scalar,
    z * scalar,
    w * scalar
  };
}

template <typename T>
auto vec4_t<T>::scaled(const vec4_t &other) const -> vec4_t
{
  return {
    x * other.x,
    y * other.y,
    z * other.z,
    w * other.w
  };
}

template <typename T>
auto vec4_t<T>::scale(value_type scalar) -> vec4_t&
{
  x *= scalar;
  y *= scalar;
  z *= scalar;
  w *= scalar;
  return *this;
}

template <typename T>
auto vec4_t<T>::scale(const vec4_t &other) -> vec4_t&
{
  x *= other.x;
  y *= other.y;
  z *= other.z;
  w *= other.w;
  return *this;
}

template <typename T>
auto vec4_t<T>::negated() const -> vec4_t
{
  return {
    -x, -y, -z, -w
  };
}

template <typename T>
auto vec4_t<T>::negate() -> vec4_t&
{
  x = -x;
  y = -y;
  z = -z;
  w = -w;
  return *this;
}

template <typename T>
auto vec4_t<T>::inverse() const -> vec4_t
{
  return {
    (x != 0 ? value_type(1) / x : x),
    (y != 0 ? value_type(1) / y : y),
    (z != 0 ? value_type(1) / z : z),
    (w != 0 ? value_type(1) / w : w)
  };
}

template <typename T>
auto vec4_t<T>::invert() -> vec4_t&
{
  if (x != 0) {
    x = value_type(1) / x;
  }
  if (y != 0) {
    y = value_type(1) / y;
  }
  if (z != 0) {
    z = value_type(1) / z;
  }
  if (w != 0) {
    w = value_type(1) / w;
  }
  return *this;
}

template <typename T>
auto vec4_t<T>::dot_product(const vec4_t &other) const -> value_type
{
  return x * other.x + y * other.y + z * other.z + w * other.w;
}

template <typename T>
auto vec4_t<T>::rotate_elems() -> vec4_t &
{
  value_type wt = w;
  w = z;
  z = y;
  y = x;
  x = wt;
  return *this;
}

template <typename T>
auto vec4_t<T>::rotated_elems() const -> vec4_t
{
  return {
    w, x, y, z
  };
}

template <typename T>
auto vec4_t<T>::operator += (const vec4_t &other) -> vec4_t&
{
  return add(other);
}

template <typename T>
auto vec4_t<T>::operator -= (const vec4_t &other) -> vec4_t&
{
  return subtract(other);
}

template <typename T>
auto vec4_t<T>::operator *= (value_type scalar) -> vec4_t&
{
  return scale(scalar);
}

template <typename T>
auto vec4_t<T>::operator *= (const vec4_t &other) -> vec4_t&
{
  return scale(other);
}

template <typename T>
auto vec4_t<T>::operator /= (value_type scalar) -> vec4_t&
{
  return scale(value_type(1) / scalar);
}

template <typename T>
auto vec4_t<T>::operator /= (const vec4_t &other) -> vec4_t&
{
  return scale(other.inverse());
}

template <typename T>
auto vec4_t<T>::operator - () const -> vec4_t
{
  return negated();
}

template <typename T>
auto vec4_t<T>::operator ~ () const -> vec4_t
{
  return inverse();
}

template <typename T>
auto vec4_t<T>::operator[] (int index) -> value_type&
{
  static_assert(std::is_pod<vec4_t<T>>::value, "vec4 must be POD to use subscript operator");
  if (index < 0 || index > 3)
    s_throw(std::out_of_range, "attempt to access out of range element");
  return (&x)[index];
}

template <typename T>
auto vec4_t<T>::operator[] (int index) const -> value_type
{
  static_assert(std::is_pod<vec4_t<T>>::value, "vec4 must be POD to use subscript operator");
  if (index < 0 || index > 3)
    s_throw(std::out_of_range, "attempt to access out of range element");
  return (&x)[index];
}

template <typename T>
vec4_t<T>::operator value_type* ()
{
  static_assert(std::is_pod<vec4_t>::value, "vec4 must be POD to cast to value_type pointer");
  return &x;
}

template <typename T>
vec4_t<T>::operator const value_type* () const
{
  static_assert(std::is_pod<vec4_t>::value, "vec4 must be POD to cast to value_type pointer");
  return &x;
}

template <typename T>
template <typename Q>
vec4_t<T>::operator vec2_t<Q> () const
{
  return {
    static_cast<Q>(x),
    static_cast<Q>(y)
  };
}

template <typename T>
template <typename Q>
vec4_t<T>::operator vec3_t<Q> () const
{
  return {
    static_cast<Q>(x),
    static_cast<Q>(y),
    static_cast<Q>(z)
  };
}

template <typename T>
template <typename Q>
vec4_t<T>::operator vec4_t<Q> () const
{
  return {
    static_cast<Q>(x),
    static_cast<Q>(y),
    static_cast<Q>(z),
    static_cast<Q>(w)
  };
}

template <typename T>
std::ostream &operator << (std::ostream &out, const vec4_t<T> &in)
{
  return out << "{x:" << in.x << ", y:" << in.y << ", z:" << in.z << ", w:" << in.w << "}";
}

template <typename T, typename Q>
vec4_t<T> operator - (const vec4_t<T> &lhs, const vec4_t<Q> &rhs)
{
  return lhs.difference(rhs);
}

template <typename T, typename Q>
vec4_t<T> operator + (const vec4_t<T> &lhs, const vec4_t<Q> &rhs)
{
  return lhs.sum(rhs);
}

template <typename T, typename Q>
vec4_t<T> operator * (const vec4_t<T> &lhs, const vec4_t<Q> &rhs)
{
  return lhs.scaled(rhs);
}

template <typename T, typename Q>
vec4_t<T> operator * (const vec4_t<T> &lhs, Q rhs)
{
  return lhs.scaled(static_cast<T>(rhs));
}

template <typename T, typename Q>
vec4_t<T> operator / (const vec4_t<T> &lhs, const vec4_t<Q> &rhs)
{
  return lhs.scaled(rhs.inverse());
}

template <typename T, typename Q>
vec4_t<T> operator / (const vec4_t<T> &lhs, Q rhs)
{
  return lhs.scaled(T(1) / static_cast<T>(rhs));
}

template <typename T, typename Q>
T operator % (const vec4_t<T> &lhs, const vec4_t<Q> &rhs)
{
  return lhs.dot_product(rhs);
}

template <typename T, typename Q>
bool operator == (const vec4_t<T> &lhs, const vec4_t<Q> &rhs)
{
  return
    is_zero(lhs.x - rhs.x) &&
    is_zero(lhs.y - rhs.y) &&
    is_zero(lhs.z - rhs.z) &&
    is_zero(lhs.w - rhs.w);
}

template <typename T, typename Q>
bool operator != (const vec4_t<T> &lhs, const vec4_t<Q> &rhs)
{
  return !(lhs == rhs);
}

#endif /* end __SNOW_COMMON__VEC4_TCC__ include guard */
