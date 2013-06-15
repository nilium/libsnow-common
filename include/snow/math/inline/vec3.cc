// vec3.cc -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__VEC3_TCC__
#define __SNOW_COMMON__VEC3_TCC__


namespace snow {


template <typename T>
const vec3_t<T> vec3_t<T>::pos_X = { 1, 0, 0 };


template <typename T>
const vec3_t<T> vec3_t<T>::pos_Y = { 0, 1, 0 };


template <typename T>
const vec3_t<T> vec3_t<T>::pos_Z = { 0, 0, 1 };


template <typename T>
const vec3_t<T> vec3_t<T>::neg_X = { -1, 0, 0 };


template <typename T>
const vec3_t<T> vec3_t<T>::neg_Y = { 0, -1, 0 };


template <typename T>
const vec3_t<T> vec3_t<T>::neg_Z = { 0, 0, -1 };


template <typename T>
const vec3_t<T> vec3_t<T>::zero = { 0, 0, 0 };


template <typename T>
const vec3_t<T> vec3_t<T>::one = { 1, 1, 1 };



template <typename T>
auto vec3_t<T>::make(value_type x, value_type y, value_type z) -> vec3_t
{
  return { x, y, z };
}



template <typename T>
auto vec3_t<T>::make(value_type value) -> vec3_t
{
  return { value, value, value };
}



template <typename T>
auto vec3_t<T>::normalize() -> vec3_t&
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



template <typename T>
auto vec3_t<T>::normalized() const -> vec3_t
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



template <typename T>
auto vec3_t<T>::magnitude() const -> value_type
{
  const value_type sqrlen = length();
  return sqrlen != 0
         ? static_cast<value_type>(std::sqrt(sqrlen))
         : 0;
}



template <typename T>
auto vec3_t<T>::length() const -> value_type
{
  return x * x + y * y + z * z;
}



template <typename T>
auto vec3_t<T>::difference(vec3_t other) const -> vec3_t
{
  return {
    x - other.x,
    y - other.y,
    z - other.z
  };
}



template <typename T>
auto vec3_t<T>::subtract(vec3_t other) -> vec3_t&
{
  x -= other.x;
  y -= other.y;
  z -= other.z;
  return *this;
}



template <typename T>
auto vec3_t<T>::sum(vec3_t other) const -> vec3_t
{
  return {
    x + other.x,
    y + other.y,
    z + other.z
  };
}



template <typename T>
auto vec3_t<T>::add(vec3_t other) -> vec3_t&
{
  x += other.x;
  y += other.y;
  z += other.z;
  return *this;
}



template <typename T>
auto vec3_t<T>::scaled(value_type scalar) const -> vec3_t
{
  return {
    x * scalar,
    y * scalar,
    z * scalar
  };
}



template <typename T>
auto vec3_t<T>::scaled(vec3_t other) const -> vec3_t
{
  return {
    x * other.x,
    y * other.y,
    z * other.z
  };
}



template <typename T>
auto vec3_t<T>::scale(value_type scalar) -> vec3_t&
{
  x *= scalar;
  y *= scalar;
  z *= scalar;
  return *this;
}



template <typename T>
auto vec3_t<T>::scale(vec3_t other) -> vec3_t&
{
  x *= other.x;
  y *= other.y;
  z *= other.z;
  return *this;
}



template <typename T>
auto vec3_t<T>::negated() const -> vec3_t
{
  return {
    -x, -y, -z
  };
}



template <typename T>
auto vec3_t<T>::negate() -> vec3_t&
{
  x = -x;
  y = -y;
  z = -z;
  return *this;
}



template <typename T>
auto vec3_t<T>::inverse() const -> vec3_t
{
  return {
    (x != 0 ? value_type(1) / x : 0),
    (y != 0 ? value_type(1) / y : 0),
    (z != 0 ? value_type(1) / z : 0)
  };
}



template <typename T>
auto vec3_t<T>::invert() -> vec3_t&
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
  return *this;
}



template <typename T>
auto vec3_t<T>::cross_product(vec3_t other) const -> vec3_t
{
  return {
    y * other.z - z * other.y,
    z * other.x - x * other.z,
    x * other.y - y * other.x
  };
}



template <typename T>
auto vec3_t<T>::dot_product(vec3_t other) const -> value_type
{
  return x * other.x + y * other.y + z * other.z;
}



template <typename T>
auto vec3_t<T>::rotate_elems() -> vec3_t &
{
  value_type zt = z;
  z = y;
  y = x;
  x = zt;
  return *this;
}



template <typename T>
auto vec3_t<T>::rotated_elems() const -> vec3_t
{
  return {
    z, x, y
  };
}



template <typename T>
auto vec3_t<T>::operator += (vec3_t other) -> vec3_t&
{
  return add(other);
}



template <typename T>
auto vec3_t<T>::operator -= (vec3_t other) -> vec3_t&
{
  return subtract(other);
}



template <typename T>
auto vec3_t<T>::operator *= (value_type scalar) -> vec3_t&
{
  return scale(scalar);
}



template <typename T>
auto vec3_t<T>::operator *= (vec3_t other) -> vec3_t&
{
  return scale(other);
}



template <typename T>
auto vec3_t<T>::operator /= (value_type scalar) -> vec3_t&
{
  return scale(value_type(1) / scalar);
}



template <typename T>
auto vec3_t<T>::operator /= (vec3_t other) -> vec3_t&
{
  return scale(other.inverse());
}



template <typename T>
auto vec3_t<T>::operator - () const -> vec3_t
{
  return negated();
}



template <typename T>
auto vec3_t<T>::operator ~ () const -> vec3_t
{
  return inverse();
}



template <typename T>
auto vec3_t<T>::operator[] (int index) -> value_type&
{
  static_assert(std::is_pod<vec3_t>::value, "vec3 must be POD to use subscript operator");
  if (index < 0 || index > 2)
    s_throw(std::out_of_range, "attempt to access out of range element");
  return (&x)[index];
}



template <typename T>
auto vec3_t<T>::operator[] (int index) const -> value_type
{
  static_assert(std::is_pod<vec3_t>::value, "vec3 must be POD to use subscript operator");
  if (index < 0 || index > 2)
    s_throw(std::out_of_range, "attempt to access out of range element");
  return (&x)[index];
}



template <typename T>
vec3_t<T>::operator value_type* ()
{
  static_assert(std::is_pod<vec3_t>::value, "vec3 must be POD to cast to value_type pointer");
  return &x;
}



template <typename T>
vec3_t<T>::operator const value_type* () const
{
  static_assert(std::is_pod<vec3_t>::value, "vec3 must be POD to cast to value_type pointer");
  return &x;
}



template <typename T>
template <typename Q>
vec3_t<T>::operator vec2_t<Q> () const
{
  return {
    static_cast<Q>(x),
    static_cast<Q>(y)
  };
}



template <typename T>
template <typename Q>
vec3_t<T>::operator vec3_t<Q> () const
{
  return {
    static_cast<Q>(x),
    static_cast<Q>(y),
    static_cast<Q>(z)
  };
}



template <typename T>
template <typename Q>
vec3_t<T>::operator vec4_t<Q> () const
{
  return {
    static_cast<Q>(x),
    static_cast<Q>(y),
    static_cast<Q>(z),
    Q(1)
  };
}



template <typename T>
std::ostream &operator << (std::ostream &out, vec3_t<T> in)
{
  return out << "{x:" << in.x << ", y:" << in.y << ", z:" << in.z << "}";
}



template <typename T, typename Q>
vec3_t<T> operator - (vec3_t<T> lhs, vec3_t<Q> rhs)
{
  return lhs.difference(rhs);
}



template <typename T, typename Q>
vec3_t<T> operator + (vec3_t<T> lhs, vec3_t<Q> rhs)
{
  return lhs.sum(rhs);
}



template <typename T, typename Q>
vec3_t<T> operator * (vec3_t<T> lhs, vec3_t<Q> rhs)
{
  return lhs.scaled(rhs);
}



template <typename T, typename Q>
vec3_t<T> operator * (vec3_t<T> lhs, Q rhs)
{
  return lhs.scaled(static_cast<T>(rhs));
}



template <typename T, typename Q>
vec3_t<T> operator / (vec3_t<T> lhs, vec3_t<Q> rhs)
{
  return lhs.scaled(rhs.inverse());
}



template <typename T, typename Q>
vec3_t<T> operator / (vec3_t<T> lhs, Q rhs)
{
  return lhs.scaled(T(1) / static_cast<T>(rhs));
}



template <typename T, typename Q>
T operator % (vec3_t<T> lhs, vec3_t<Q> rhs)
{
  return lhs.dot_product(rhs);
}



template <typename T, typename Q>
bool operator == (vec3_t<T> lhs, vec3_t<Q> rhs)
{
  return
    is_zero(lhs.x - rhs.x) &&
    is_zero(lhs.y - rhs.y) &&
    is_zero(lhs.z - rhs.z);;
}



template <typename T, typename Q>
bool operator != (vec3_t<T> lhs, vec3_t<Q> rhs)
{
  return !(lhs == rhs);
}


} // namespace snow

#endif /* end __SNOW_COMMON__VEC3_TCC__ include guard */
