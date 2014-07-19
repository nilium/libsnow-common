/*
 * Copyright Noel Cower 2013 - 2014.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#pragma once


namespace snow {


template <typename T>
const mat3_t<T> mat3_t<T>::identity = {
  {1, 0, 0},
  {0, 1, 0},
  {0, 0, 1}
};


template <typename T>
const mat3_t<T> mat3_t<T>::zero = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};



template <typename T>
auto mat3_t<T>::make(T rx, T ry, T rz,
                     T sx, T sy, T sz,
                     T tx, T ty, T tz) -> mat3_t
{
  return {
    {rx, ry, rz},
    {sx, sy, sz},
    {tx, ty, tz}
  };
}



template <typename T>
auto mat3_t<T>::make(vec3 x, vec3 y, vec3 z) -> mat3_t
{
  return {x, y, z};
}



template <typename T>
auto mat3_t<T>::scaling(vec3 off) -> mat3_t
{
  return {
    {off.x, 0, 0},
    {0, off.y, 0},
    {0, 0, off.z}
  };
}



template <typename T>
auto mat3_t<T>::rotation(T angle, vec3 axis) -> mat3_t
{
  angle *= S_DEG2RAD;
  const T c  = static_cast<T>(std::cos(angle));
  const T s  = static_cast<T>(std::sin(angle));
  const T ic = T(1) - c;
  const T xy = axis.x * axis.y * ic;
  const T yz = axis.y * axis.z * ic;
  const T xz = axis.x * axis.z * ic;
  const T xs = s * axis.x;
  const T ys = s * axis.y;
  const T zs = s * axis.z;
  const T xx = axis.x * axis.x;
  const T yy = axis.y * axis.y;
  const T zz = axis.z * axis.z;

  return {
    {
      xx + c * (T(1) - xx),
      xy * ic - zs,
      axis.z * axis.x * ic + ys
    },
    {
      xy * ic + zs,
      yy + c * (T(1) - yy),
      yz * ic - xs
    },
    {
      xz * ic - ys,
      yz * ic + xs,
      zz + c * (T(1) - zz)
    }
  };
}



template <typename T>
auto mat3_t<T>::from_quat(quat in) -> mat3_t
{
  const T xx = in.xyz.x * in.xyz.x;
  const T xy = in.xyz.x * in.xyz.y;
  const T xz = in.xyz.x * in.xyz.z;

  const T yy = in.xyz.y * in.xyz.y;
  const T yz = in.xyz.z * in.xyz.z;

  const T zz = in.xyz.z * in.xyz.z;

  const T wx = in.xyz.x * in.w;
  const T wy = in.xyz.y * in.w;
  const T wz = in.xyz.z * in.w;

  return {
    {
      T(1) - T(2) * (yy + zz),
      T(2) * (xy - wz),
      T(2) * (xz + wy)
    },
    {
      T(2) * (xy + wz),
      T(1) - T(2) * (xx + zz),
      T(2) * (yz - wx)
    },
    {
      T(2) * (xz - wy),
      T(2) * (yz + wx),
      T(1) - T(2) * (xx + yy)
    }
  };
}



template <typename T>
auto mat3_t<T>::transpose() -> mat3_t &
{
  #define S_SWAP_ELEM_(LHS, RHS) temp=(LHS);LHS=(RHS);RHS=temp;
  value_type temp;
  S_SWAP_ELEM_(r.y, s.x);
  S_SWAP_ELEM_(r.z, t.x);
  S_SWAP_ELEM_(s.z, t.y);
  #undef S_SWAP_ELEM_
  return *this;
}



template <typename T>
auto mat3_t<T>::transposed() const -> mat3_t
{
  return {
    {r.x, s.x, t.x},
    {r.y, s.y, t.y},
    {r.z, s.z, t.z}
  };
}



template <typename T>
auto mat3_t<T>::colvec3(int index) const -> vec3
{
  switch (index) {
    case 0: return { r.x, s.x, t.x };
    case 1: return { r.y, s.y, t.y };
    case 2: return { r.z, s.z, t.z };
    default: s_throw(std::out_of_range, "attempt to access out of range column");
  }
}



template <typename T>
auto mat3_t<T>::set_colvec3(int index, vec3 col) -> mat3_t &
{
  switch (index) {
    case 0: r.x = col.x; r.y = col.y; r.z = col.z; break;
    case 1: s.x = col.x; s.y = col.y; s.z = col.z; break;
    case 2: t.x = col.x; t.y = col.y; t.z = col.z; break;
    default: s_throw(std::out_of_range, "attempt to access out of range column");
  }
  return *this;
}



template <typename T>
auto mat3_t<T>::negated() const -> mat3_t
{
  return { -r, -s, -t };
}



template <typename T>
auto mat3_t<T>::negate() -> mat3_t &
{
  r = -r;
  s = -s;
  t = -t;
  return *this;
}



template <typename T>
auto mat3_t<T>::sum(const mat3_t &other) const -> mat3_t
{
  return {
    r + other.r,
    s + other.s,
    t + other.t
  };
}



template <typename T>
auto mat3_t<T>::sum(T scalar) const -> mat3_t
{
  return {
    {r.x + scalar, r.y + scalar, r.z + scalar},
    {s.x + scalar, s.y + scalar, s.z + scalar},
    {t.x + scalar, t.y + scalar, t.z + scalar}
  };
}



template <typename T>
auto mat3_t<T>::add(const mat3_t &other) -> mat3_t &
{
  r += other.r;
  s += other.s;
  t += other.t;
  return *this;
}



template <typename T>
auto mat3_t<T>::add(T scalar) -> mat3_t &
{
  r.x += scalar;
  r.y += scalar;
  r.z += scalar;
  s.x += scalar;
  s.y += scalar;
  s.z += scalar;
  t.x += scalar;
  t.y += scalar;
  t.z += scalar;
  return *this;
}


template <typename T>
auto mat3_t<T>::difference(const mat3_t &other) const -> mat3_t
{
  return {
    r - other.r,
    s - other.s,
    t - other.t
  };
}



template <typename T>
auto mat3_t<T>::difference(T scalar) const -> mat3_t
{
  return {
    {r.x - scalar, r.y - scalar, r.z - scalar},
    {s.x - scalar, s.y - scalar, s.z - scalar},
    {t.x - scalar, t.y - scalar, t.z - scalar}
  };
}



template <typename T>
auto mat3_t<T>::subtract(const mat3_t &other) -> mat3_t &
{
  r -= other.r;
  s -= other.s;
  t -= other.t;
  return *this;
}



template <typename T>
auto mat3_t<T>::subtract(T scalar) -> mat3_t &
{
  r.x -= scalar;
  r.y -= scalar;
  r.z -= scalar;
  s.x -= scalar;
  s.y -= scalar;
  s.z -= scalar;
  t.x -= scalar;
  t.y -= scalar;
  t.z -= scalar;
  return *this;
}



template <typename T>
auto mat3_t<T>::scaled(T scalar) const -> mat3_t
{
  return {
    r.scaled(scalar),
    s.scaled(scalar),
    t.scaled(scalar)
  };
}



template <typename T>
auto mat3_t<T>::scale(T scalar) -> mat3_t &
{
  r.scale(scalar);
  s.scale(scalar);
  t.scale(scalar);
  return *this;
}



template <typename T>
auto mat3_t<T>::scaled(const mat3_t &other) const -> mat3_t
{
  return {
    r * other.r,
    s * other.s,
    t * other.t
  };
}



template <typename T>
auto mat3_t<T>::scale(const mat3_t &other) -> mat3_t &
{
  r *= other.r;
  s *= other.s;
  t *= other.t;
  return *this;
}



template <typename T>
auto mat3_t<T>::scaled(vec3 vec) const -> mat3_t
{
  return {
    r.scaled(vec.x),
    s.scaled(vec.y),
    t.scaled(vec.z)
  };
}



template <typename T>
auto mat3_t<T>::scale(vec3 vec) -> mat3_t &
{
  r.scale(vec.x);
  s.scale(vec.y);
  t.scale(vec.z);
  return *this;
}



template <typename T>
auto mat3_t<T>::inverse() const -> mat3_t
{
  return cofactor().transpose().invert();
}



template <typename T>
auto mat3_t<T>::invert() -> mat3_t &
{
  value_type inv_det = T(1) / determinant();
  *this = cofactor();
  transpose().scale(inv_det);
  return *this;
}



template <typename T>
auto mat3_t<T>::adjoint() const -> mat3_t
{
  return cofactor().transpose();
}



template <typename T>
auto mat3_t<T>::cofactor() const -> mat3_t
{
  return {
    { (s.y * t.z - s.z * t.y),
     -(s.x * t.z - s.z * t.x),
      (s.x * t.y - s.y * t.x) },

    {-(r.y * t.z - r.z * t.y),
      (r.x * t.z - r.z * t.x),
     -(r.x * t.y - r.y * t.x) },

    { (r.y * s.z - r.z * s.y),
     -(r.x * s.z - r.z * s.x),
      (r.x * s.y - r.y * s.x) }
  };
}



template <typename T>
auto mat3_t<T>::determinant() const -> T
{
  return r.x * (s.y * t.z - s.z * t.y) +
         r.y * (s.z * t.x - s.x * t.z) +
         r.z * (s.x * t.y - s.y * t.x);
}



template <typename T>
auto mat3_t<T>::orthogonal() const -> mat3_t
{
  mat3_t temp = *this;
  temp.orthogonalize();
  return temp;
}



template <typename T>
auto mat3_t<T>::orthogonalize() -> mat3_t &
{
  r = s.cross_product(t.normalize()); r.normalize();
  s = t.cross_product(r);
  return *this;
}



template <typename T>
auto mat3_t<T>::product(const mat3_t &other) const -> mat3_t
{
  mat3_t temp;
  value_type *m = &temp.r.x;

  for (int cindex = 0; cindex < 3; ++cindex) {
    const vec3 column = colvec3(cindex);
    m[cindex    ] = column.dot_product(other.r);
    m[cindex + 3] = column.dot_product(other.s);
    m[cindex + 6] = column.dot_product(other.t);
  }

  return temp;
}



template <typename T>
auto mat3_t<T>::multiply(const mat3_t &other) -> mat3_t &
{
  value_type *m = &r.x;

  for (int cindex = 0; cindex < 3; ++cindex) {
    const vec3 column = colvec3(cindex);
    m[cindex    ] = column.dot_product(other.r);
    m[cindex + 3] = column.dot_product(other.s);
    m[cindex + 6] = column.dot_product(other.t);
  }

  return *this;
}



template <typename T>
auto mat3_t<T>::rotate(vec3 vec) const -> vec3
{
  return {
    vec.dot_product(colvec3(0)),
    vec.dot_product(colvec3(1)),
    vec.dot_product(colvec3(2))
  };
}



template <typename T>
auto mat3_t<T>::inverse_rotate(vec3 vec) const -> vec3
{
  return {
    vec.dot_product(r),
    vec.dot_product(s),
    vec.dot_product(t)
  };
}



template <typename T>
auto mat3_t<T>::rotate(vec2 vec) const -> vec2
{
  return {
    vec.dot_product(colvec3(0)),
    vec.dot_product(colvec3(1))
  };
}



template <typename T>
auto mat3_t<T>::inverse_rotate(vec2 vec) const -> vec2
{
  return {
    vec.dot_product(r),
    vec.dot_product(s)
  };
}



template <typename T>
auto mat3_t<T>::operator[] (int index) -> T&
{
  static_assert(std::is_pod<mat3_t>::value, "mat4 must be POD to use subscript operator");
  if (index < 0 || index > 9)
    s_throw(std::out_of_range, "attempt to access out of range element");
  return (&r.x)[index];
}



template <typename T>
auto mat3_t<T>::operator[] (int index) const -> T
{
  static_assert(std::is_pod<mat3_t>::value, "mat4 must be POD to use subscript operator");
  if (index < 0 || index > 9)
    s_throw(std::out_of_range, "attempt to access out of range element");
  return (&r.x)[index];
}



template <typename T>
mat3_t<T>::operator T* ()
{
  static_assert(std::is_pod<mat3_t>::value, "mat4 must be POD to cast to T pointer");
  return &r.x;
}



template <typename T>
mat3_t<T>::operator const T* () const
{
  static_assert(std::is_pod<mat3_t>::value, "mat4 must be POD to cast to T pointer");
  return &r.x;
}



template <typename T>
mat3_t<T>::operator mat4_t<T> () const
{
  return {
    splat_vec3(r), 0,
    splat_vec3(s), 0,
    splat_vec3(t), 0,
    0, 0, 0, 1
  };
}



template <typename T>
mat3_t<T>::operator quat_t<T> () const
{
  return quat_t<T>::from_mat3(*this);
}



template <typename T>
auto mat3_t<T>::operator *= (const mat3_t &other) -> mat3_t &
{
  return multiply(other);
}



template <typename T>
auto mat3_t<T>::operator *= (T scalar) -> mat3_t &
{
  return scale(scalar);
}



template <typename T>
auto mat3_t<T>::operator += (const mat3_t &other) -> mat3_t &
{
  return add(other);
}



template <typename T>
auto mat3_t<T>::operator += (T scalar) -> mat3_t &
{
  return add(scalar);
}



template <typename T>
auto mat3_t<T>::operator -= (const mat3_t &other) -> mat3_t &
{
  return subtract(other);
}



template <typename T>
auto mat3_t<T>::operator -= (T scalar) -> mat3_t &
{
  return subtract(scalar);
}



template <typename T>
std::ostream &operator << (std::ostream &out, const mat3_t<T> &in)
{
  out << "{";
  for (int index = 0; index < 9; ++index) {
    out << in[index];
    if (index < 8)
      out << ", ";
  }
  return out << "}";
}



template <typename T, typename Q>
mat3_t<T> operator * (const mat3_t<T> &rhs, const mat3_t<Q> &lhs)
{
  return rhs.product(lhs);
}



template <typename T, typename Q>
vec3_t<T> operator * (const mat3_t<T> &rhs, vec3_t<Q> lhs)
{
  return rhs.rotate(lhs);
}



template <typename T, typename Q>
vec2_t<T> operator * (const mat3_t<T> &rhs, vec2_t<Q> lhs)
{
  return rhs.rotate(lhs);
}



template <typename T, typename Q>
mat3_t<T> operator * (const mat3_t<T> &rhs, Q lhs)
{
  return rhs.scaled(lhs);
}



template <typename T, typename Q>
mat3_t<T> operator + (const mat3_t<T> &rhs, const mat3_t<Q> &lhs)
{
  return rhs.sum(lhs);
}



template <typename T, typename Q>
mat3_t<T> operator + (const mat3_t<T> &rhs, Q lhs)
{
  return rhs.sum(lhs);
}



template <typename T, typename Q>
mat3_t<T> operator - (const mat3_t<T> &rhs, const mat3_t<Q> &lhs)
{
  return rhs.difference(lhs);
}



template <typename T, typename Q>
mat3_t<T> operator - (const mat3_t<T> &rhs, Q lhs)
{
  return rhs.difference(lhs);
}



template <typename T, typename Q>
bool operator == (const mat3_t<T> &rhs, const mat3_t<Q> &lhs)
{
  return lhs.r == rhs.r && lhs.s == rhs.s && lhs.t == rhs.t;
}



template <typename T, typename Q>
bool operator != (const mat3_t<T> &rhs, const mat3_t<Q> &lhs)
{
  return !(rhs == lhs);
}


} // namespace snow
