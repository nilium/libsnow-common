// vec2.cc -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__VEC2_TCC__
#define __SNOW_COMMON__VEC2_TCC__

template <typename T>
const vec2_t<T> vec2_t<T>::pos_X = { 1, 0 };

template <typename T>
const vec2_t<T> vec2_t<T>::pos_Y = { 0, 1 };

template <typename T>
const vec2_t<T> vec2_t<T>::neg_X = { -1, 0 };

template <typename T>
const vec2_t<T> vec2_t<T>::neg_Y = { 0, -1 };

template <typename T>
const vec2_t<T> vec2_t<T>::zero = { 0, 0 };

template <typename T>
const vec2_t<T> vec2_t<T>::one = { 1, 1 };

template <typename T>
auto vec2_t<T>::make(value_type x, value_type y) -> vec2_t
{
  return { x, y };
}

template <typename T>
auto vec2_t<T>::normalize() -> vec2_t&
{
  value_type one_over_mag = magnitude();
  if (one_over_mag != 0) {
    one_over_mag = value_type(1) / one_over_mag;
  }

  x *= one_over_mag;
  y *= one_over_mag;

  return *this;
}

template <typename T>
auto vec2_t<T>::normalized() const -> vec2_t
{
  value_type one_over_mag = magnitude();
  if (one_over_mag != 0) {
    one_over_mag = value_type(1) / one_over_mag;
  }

  return {
    x * one_over_mag,
    y * one_over_mag
  };
}

template <typename T>
auto vec2_t<T>::magnitude() const -> value_type
{
  const value_type sqrlen = length();
  return sqrlen != 0
         ? static_cast<value_type>(std::sqrt(sqrlen))
         : 0;
}

template <typename T>
auto vec2_t<T>::length() const -> value_type
{
  return x * x + y * y;
}

template <typename T>
auto vec2_t<T>::difference(const vec2_t &other) const -> vec2_t
{
  return {
    x - other.x,
    y - other.y
  };
}

template <typename T>
auto vec2_t<T>::subtract(const vec2_t &other) -> vec2_t&
{
  x -= other.x;
  y -= other.y;
  return *this;
}

template <typename T>
auto vec2_t<T>::sum(const vec2_t &other) const -> vec2_t
{
  return {
    x + other.x,
    y + other.y
  };
}

template <typename T>
auto vec2_t<T>::add(const vec2_t &other) -> vec2_t&
{
  x += other.x;
  y += other.y;
  return *this;
}

template <typename T>
auto vec2_t<T>::scaled(value_type scalar) const -> vec2_t
{
  return {
    x * scalar,
    y * scalar
  };
}

template <typename T>
auto vec2_t<T>::scaled(const vec2_t &other) const -> vec2_t
{
  return {
    x * other.x,
    y * other.y
  };
}

template <typename T>
auto vec2_t<T>::scale(value_type scalar) -> vec2_t&
{
  x *= scalar;
  y *= scalar;
  return *this;
}

template <typename T>
auto vec2_t<T>::scale(const vec2_t &other) -> vec2_t&
{
  x *= other.x;
  y *= other.y;
  return *this;
}

template <typename T>
auto vec2_t<T>::negated() const -> vec2_t
{
  return {
    -x, -y
  };
}

template <typename T>
auto vec2_t<T>::negate() -> vec2_t&
{
  x = -x;
  y = -y;
  return *this;
}

template <typename T>
auto vec2_t<T>::inverse() const -> vec2_t
{
  return {
    (x != 0 ? value_type(1) / x : x),
    (y != 0 ? value_type(1) / y : y)
  };
}

template <typename T>
auto vec2_t<T>::invert() -> vec2_t&
{
  if (x != 0) {
    x = value_type(1) / x;
  }
  if (y != 0) {
    y = value_type(1) / y;
  }
  return *this;
}

template <typename T>
auto vec2_t<T>::dot_product(const vec2_t &other) const -> value_type
{
  return x * other.x + y * other.y;
}

template <typename T>
auto vec2_t<T>::rotate_elems() -> vec2_t &
{
  std::swap(x, y);
  return *this;
}

template <typename T>
auto vec2_t<T>::rotated_elems() const -> vec2_t
{
  return {
    y, x
  };
}

template <typename T>
auto vec2_t<T>::operator += (const vec2_t &other) -> vec2_t&
{
  return add(other);
}

template <typename T>
auto vec2_t<T>::operator -= (const vec2_t &other) -> vec2_t&
{
  return subtract(other);
}

template <typename T>
auto vec2_t<T>::operator *= (value_type scalar) -> vec2_t&
{
  return scale(scalar);
}

template <typename T>
auto vec2_t<T>::operator *= (const vec2_t &other) -> vec2_t&
{
  return scale(other);
}

template <typename T>
auto vec2_t<T>::operator /= (value_type scalar) -> vec2_t&
{
  return scale(value_type(1) / scalar);
}

template <typename T>
auto vec2_t<T>::operator /= (const vec2_t &other) -> vec2_t&
{
  return scale(other.inverse());
}

template <typename T>
auto vec2_t<T>::operator - () const -> vec2_t
{
  return negated();
}

template <typename T>
auto vec2_t<T>::operator ~ () const -> vec2_t
{
  return inverse();
}

template <typename T>
auto vec2_t<T>::operator[] (int index) -> value_type&
{
  static_assert(std::is_pod<vec2_t>::value, "vec2 must be POD to use subscript operator");
  if (index < 0 || index > 2)
    throw std::out_of_range("attempt to access out of range element");
  return (&x)[index];
}

template <typename T>
auto vec2_t<T>::operator[] (int index) const -> value_type
{
  static_assert(std::is_pod<vec2_t>::value, "vec2 must be POD to use subscript operator");
  if (index < 0 || index > 2)
    throw std::out_of_range("attempt to access out of range element");
  return (&x)[index];
}

template <typename T>
vec2_t<T>::operator value_type* ()
{
  static_assert(std::is_pod<vec2_t>::value, "vec2 must be POD to cast to value_type pointer");
  return &x;
}

template <typename T>
vec2_t<T>::operator const value_type* () const
{
  static_assert(std::is_pod<vec2_t>::value, "vec2 must be POD to cast to value_type pointer");
  return &x;
}

template <typename T>
template <typename Q>
vec2_t<T>::operator vec2_t<Q> () const
{
  return {
    static_cast<Q>(x),
    static_cast<Q>(y)
  };
}

template <typename T>
template <typename Q>
vec2_t<T>::operator vec3_t<Q> () const
{
  return {
    static_cast<Q>(x),
    static_cast<Q>(y),
    Q(0)
  };
}

template <typename T>
template <typename Q>
vec2_t<T>::operator vec4_t<Q> () const
{
  return {
    static_cast<Q>(x),
    static_cast<Q>(y),
    Q(0),
    Q(1)
  };
}

template <typename T>
std::ostream &operator << (std::ostream &out, const vec2_t<T> &in)
{
  return out << "{x:" << in.x << ", y:" << in.y << "}";
}

template <typename T, typename Q>
vec2_t<T> operator - (const vec2_t<T> &lhs, const vec2_t<Q> &rhs)
{
  return lhs.difference(rhs);
}

template <typename T, typename Q>
vec2_t<T> operator + (const vec2_t<T> &lhs, const vec2_t<Q> &rhs)
{
  return lhs.sum(rhs);
}

template <typename T, typename Q>
vec2_t<T> operator * (const vec2_t<T> &lhs, const vec2_t<Q> &rhs)
{
  return lhs.scaled(rhs);
}

template <typename T, typename Q>
vec2_t<T> operator * (const vec2_t<T> &lhs, Q rhs)
{
  return lhs.scaled(static_cast<T>(rhs));
}

template <typename T, typename Q>
vec2_t<T> operator / (const vec2_t<T> &lhs, const vec2_t<Q> &rhs)
{
  return lhs.scaled(rhs.inverse());
}

template <typename T, typename Q>
vec2_t<T> operator / (const vec2_t<T> &lhs, Q rhs)
{
  return lhs.scaled(T(1) / static_cast<T>(rhs));
}

template <typename T, typename Q>
T operator % (const vec2_t<T> &lhs, const vec2_t<Q> &rhs)
{
  return lhs.dot_product(rhs);
}

template <typename T, typename Q>
bool operator == (const vec2_t<T> &lhs, const vec2_t<Q> &rhs)
{
  return
    is_zero(lhs.x - rhs.x) &&
    is_zero(lhs.y - rhs.y);
}

template <typename T, typename Q>
bool operator != (const vec2_t<T> &lhs, const vec2_t<Q> &rhs)
{
  return !(lhs == rhs);
}

#endif /* end __SNOW_COMMON__VEC2_TCC__ include guard */
