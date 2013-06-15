// quat.cc -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__QUAT_TCC__
#define __SNOW_COMMON__QUAT_TCC__


namespace snow {


template <typename T>
const quat_t<T> quat_t<T>::zero = { { 0, 0, 0 }, 0 };


template <typename T>
const quat_t<T> quat_t<T>::one = { { 1, 1, 1 }, 1 };


template <typename T>
const quat_t<T> quat_t<T>::identity = { { 0, 0, 0 }, 1 };



template <typename T>
auto quat_t<T>::make(T x, T y, T z, T w) -> quat_t
{
  return { { x, y, z }, w };
}



template <typename T>
auto quat_t<T>::make(vec3 xyz, T w) -> quat_t
{
  return { xyz, w };
}



template <typename T>
auto quat_t<T>::length() const -> T
{
  return xyz.length() + w * w;
}



template <typename T>
auto quat_t<T>::magnitude() const -> T
{
  const T sqrlen = length();
  return sqrlen != 0
         ? static_cast<T>(std::sqrt(sqrlen))
         : 0;
}



template <typename T>
auto quat_t<T>::inverse() const -> quat_t
{
  return {
    -xyz, w
  };
}



template <typename T>
auto quat_t<T>::invert() -> quat_t&
{
  xyz.negate();
  return *this;
}



template <typename T>
auto quat_t<T>::negated() const -> quat_t
{
  return {
    -xyz, -w
  };
}



template <typename T>
auto quat_t<T>::negate() -> quat_t&
{
  invert();
  w = -w;
  return *this;
}



template <typename T>
auto quat_t<T>::product(quat_t other) const -> quat_t
{
  T wt, w1, w2;
  vec3 temp_xyz;
  w1 = w;
  w2 = other.w;

  wt = w1 * w2 - xyz.dot_product(other.xyz);
  temp_xyz = other.xyz * w1;
  temp_xyz += xyz * w2;
  temp_xyz += other.xyz.cross_product(xyz);
  return {
    temp_xyz, wt
  };
}



template <typename T>
auto quat_t<T>::multiply(quat_t other) -> quat_t&
{
  T w1, w2;
  vec3 temp_xyz;
  w1 = w;
  w2 = other.w;

  w = w1 * w2 - xyz.dot_product(other.xyz);
  temp_xyz = other.xyz * w1;
  temp_xyz += xyz * w2;
  temp_xyz += other.xyz.cross_product(xyz);
  xyz = temp_xyz;
  return *this;
}



template <typename T>
auto quat_t<T>::normalized() const -> quat_t
{
  T sqrlen = xyz.length() + w * w;
  if (sqrlen != 0)
    sqrlen = T(1) / std::sqrt(sqrlen);

  return {
    xyz * sqrlen,
    w * sqrlen
  };
}



template <typename T>
auto quat_t<T>::normalize() -> quat_t&
{
  T sqrlen = xyz.length() + w * w;
  if (sqrlen != 0)
    sqrlen = T(1) / std::sqrt(sqrlen);

  xyz *= sqrlen;
  w *= sqrlen;

  return *this;
}



template <typename T>
auto quat_t<T>::difference(quat_t other) const -> quat_t
{
  return {
    xyz - other.xyz,
    w - other.w
  };
}



template <typename T>
auto quat_t<T>::subtract(quat_t other) -> quat_t&
{
  xyz -= other.xyz;
  w -= other.w;
  return *this;
}



template <typename T>
auto quat_t<T>::sum(quat_t other) const -> quat_t
{
  return {
    xyz + other.xyz,
    w + other.w
  };
}



template <typename T>
auto quat_t<T>::add(quat_t other) -> quat_t&
{
  xyz += other.xyz;
  w += other.w;
  return *this;
}



template <typename T>
auto quat_t<T>::scaled(T scalar) const -> quat_t
{
  return {
    xyz * scalar,
    w * scalar
  };
}



template <typename T>
auto quat_t<T>::scaled(quat_t other) const -> quat_t
{
  return {
    xyz * other.xyz,
    w * other.w
  };
}



template <typename T>
auto quat_t<T>::scale(T scalar) -> quat_t&
{
  xyz *= scalar;
  w *= scalar;
  return *this;
}



template <typename T>
auto quat_t<T>::scale(quat_t other) -> quat_t&
{
  xyz *= other.xyz;
  w *= other.w;
  return *this;
}



template <typename T>
auto quat_t<T>::dot_product(quat_t other) const -> T
{
  return xyz.dot_product(other.xyz) + w * other.w;
}



template <typename T>
auto quat_t<T>::from_angle_axis(T angle, vec3 axis) -> quat_t
{
  vec3 axis_n = axis.normalized();
  T s;
  angle *= S_DEG2RAD * 0.5;
  s = std::sin(angle);

  return {
    axis_n * s,
    std::cos(angle)
  };
}



template <typename T>
auto quat_t<T>::from_mat4(const mat4_t<T> &mat) -> quat_t
{
  quat_t out;
  T r;
  const T trace = mat.m00 + mat.m11 + mat.m22;
  const T *m = (T *)m;
  const T m01 = mat.m01, m02 = mat.m02, m10 = mat.m10,
                   m12 = mat.m12, m20 = mat.m20, m21 = mat.m21;

  if (trace > 0) {
    r = std::sqrt(trace + 1);
    out.w = r * 0.5;
    r = 0.5 / r;
    out.xyz = {
      (m12 - m21) * r,
      (m20 - m02) * r,
      (m01 - m10) * r
    };
  } else {
    int index = 0;
    if (mat.m11 > mat.m00) {
      index = 1;
    }
    if (mat.m22 > (index ? mat.m11 : mat.m00)) {
      index = 2;
    }

    switch (index) {
    default:
    case 0:
      r = out.xyz.x = std::sqrt(mat.m00 - (mat.m11 + mat.m22) + 1) * 0.5;
      if (r != 0) r = 0.5 / r;
      out.xyz.y = (m10 + m01) * r;
      out.xyz.z = (m20 + m02) * r;
      out.w     = (m12 - m21) * r;
      break;

    case 1:
      r = out.xyz.y = std::sqrt(mat.m11 - (mat.m22 + mat.m00) + 1) * 0.5;
      if (r != 0) r = 0.5 / r;
      out.xyz.x = (m10 + m01) * r;
      out.xyz.z = (m12 + m21) * r;
      out.w     = (m20 - m02) * r;
      break;

    case 2:
      r = out.xyz.z = std::sqrt(mat.m11 - (mat.m00 + mat.m11) + 1) * 0.5;
      if (r != 0) r = 0.5 / r;
      out.xyz.x = (m20 + m02) * r;
      out.xyz.y = (m21 + m12) * r;
      out.w     = (m01 - m10) * r;
      break;
    }
  }

  return out;
}



template <typename T>
auto quat_t<T>::from_mat3(const mat3_t<T> &mat) -> quat_t
{
  quat_t out;
  T r;
  const T trace = mat.r.x + mat.s.y + mat.t.z;
  const T *m = (T *)m;
  const T m01 = mat.s.x, m02 = mat.t.x, m10 = mat.r.y,
          m12 = mat.t.y, m20 = mat.r.z, m21 = mat.s.z;

  if (trace > 0) {
    r = std::sqrt(trace + 1);
    out.w = r * 0.5;
    r = 0.5 / r;
    out.xyz = {
      (m12 - m21) * r,
      (m20 - m02) * r,
      (m01 - m10) * r
    };
  } else {
    int index = 0;
    if (mat.s.y > mat.r.x) {
      index = 1;
    }
    if (mat.t.z > (index ? mat.s.y : mat.r.x)) {
      index = 2;
    }

    switch (index) {
    default:
    case 0:
      r = out.xyz.x = std::sqrt(mat.r.x - (mat.s.y + mat.t.z) + 1) * 0.5;
      if (r != 0) r = 0.5 / r;
      out.xyz.y = (m10 + m01) * r;
      out.xyz.z = (m20 + m02) * r;
      out.w     = (m12 - m21) * r;
      break;

    case 1:
      r = out.xyz.y = std::sqrt(mat.s.y - (mat.t.z + mat.r.x) + 1) * 0.5;
      if (r != 0) r = 0.5 / r;
      out.xyz.x = (m10 + m01) * r;
      out.xyz.z = (m12 + m21) * r;
      out.w     = (m20 - m02) * r;
      break;

    case 2:
      r = out.xyz.z = std::sqrt(mat.s.y - (mat.r.x + mat.s.y) + 1) * 0.5;
      if (r != 0) r = 0.5 / r;
      out.xyz.x = (m20 + m02) * r;
      out.xyz.y = (m21 + m12) * r;
      out.w     = (m01 - m10) * r;
      break;
    }
  }

  return out;
}



template <typename T>
auto quat_t<T>::slerp(quat_t to, T delta) const -> quat_t
{
  T dot, scale0, scale1, angle, inv_sin;
  quat_t dquat = to;

  if (delta < 0 || is_zero(delta)) {
    return *this;
  } else if (delta >= 1) {
    return to;
  }

  dot = dot_product(to);

  if (dot < 0) {
    dot = -dot;
    dquat.negate();
  }

  if (dot > T(0.9999)) {
    return lerp(to, delta);
  } else {
    angle = std::acos(dot);
    inv_sin = T(1) / std::sin(angle);
    scale0 = std::sin((T(1) - delta) * angle) * inv_sin;
    scale1 = std::sin(delta * angle) * inv_sin;
  }

  return dquat.scale(scale1).add(scaled(scale0));
}



template <typename T>
auto quat_t<T>::lerp(quat_t to, T delta) const -> quat_t
{
  if (delta < 0 || is_zero(delta)) {
    return *this;
  } else if (delta >= 1) {
    return to;
  }

  quat_t dquat = to;
  T scale1 = clamp_value(delta, 0, 1);
  T scale0 = T(1) - delta;
  return dquat.scale(scale1).add(scaled(scale0)).normalize();
}



template <typename T>
auto quat_t<T>::operator *= (quat_t other) -> quat_t&
{
  return multiply(other);
}



template <typename T>
auto quat_t<T>::operator *= (T scalar) -> quat_t&
{
  return scale(scalar);
}



template <typename T>
auto quat_t<T>::operator += (quat_t other) -> quat_t&
{
  return add(other);
}



template <typename T>
auto quat_t<T>::operator -= (quat_t other) -> quat_t&
{
  return subtract(other);
}



template <typename T>
auto quat_t<T>::operator - () const -> quat_t
{
  return negated();
}



template <typename T>
auto quat_t<T>::operator ~ () const -> quat_t
{
  return inverse();
}



template <typename T>
auto quat_t<T>::operator[] (int index) -> T&
{
  static_assert(std::is_pod<quat_t>::value, "quaternion must be POD to use subscript operator");
  if (index < 0 || index > 15)
    s_throw(std::out_of_range, "attempt to access out of range element");
  return (&xyz.x)[index];
}



template <typename T>
auto quat_t<T>::operator[] (int index) const -> T
{
  static_assert(std::is_pod<quat_t>::value, "quaternion must be POD to use subscript operator");
  if (index < 0 || index > 15)
    s_throw(std::out_of_range, "attempt to access out of range element");
  return (&xyz.x)[index];
}



template <typename T>
quat_t<T>::operator T* ()
{
  static_assert(std::is_pod<quat_t>::value, "quaternion must be POD to cast to T pointer");
  return &xyz.x;
}



template <typename T>
quat_t<T>::operator const T* () const
{
  static_assert(std::is_pod<quat_t>::value, "quaternion must be POD to cast to T pointer");
  return &xyz.x;
}



template <typename T>
quat_t<T>::operator mat3_t<T> () const
{
  return mat3_t<T>::from_quat(*this);
}



template <typename T>
quat_t<T>::operator mat4_t<T> () const
{
  return mat4_t<T>::from_quat(*this);
}



template <typename T>
template <typename Q>
quat_t<T>::operator quat_t<Q> () const
{
  return {
    static_cast<vec3_t<Q> >(xyz),
    static_cast<Q>(w)
  };
}



template <typename T>
auto operator << (std::ostream &out, quat_t<T> in) -> std::ostream&
{
  return out << "{x:" << in.xyz.x << ", y:" << in.xyz.y << ", z:" << in.xyz.z << ", w:" << in.w << "}";
}



template <typename T, typename Q>
quat_t<T> operator * (quat_t<T> lhs, quat_t<Q> rhs)
{
  return lhs.product(rhs);
}



template <typename T, typename Q>
quat_t<T> operator * (quat_t<T> lhs, Q rhs)
{
  return lhs.scaled(rhs);
}



template <typename T, typename Q>
quat_t<T> operator + (quat_t<T> lhs, quat_t<Q> rhs)
{
  return lhs.sum(rhs);
}



template <typename T, typename Q>
quat_t<T> operator - (quat_t<T> lhs, quat_t<Q> rhs)
{
  return lhs.difference(rhs);
}



template <typename T, typename Q>
T operator % (quat_t<T> lhs, quat_t<Q> rhs)
{
  return lhs.dot_product(rhs);
}



template <typename T, typename Q>
bool operator == (quat_t<T> lhs, quat_t<Q> rhs)
{
  return
    is_zero(lhs.xyz.x - rhs.xyz.x) &&
    is_zero(lhs.xyz.y - rhs.xyz.y) &&
    is_zero(lhs.xyz.z - rhs.xyz.z) &&
    is_zero(lhs.w - rhs.w);
}



template <typename T, typename Q>
bool operator != (quat_t<T> lhs, quat_t<Q> rhs)
{
  return !(lhs == rhs);
}


} // namespace snow

#endif /* end __SNOW_COMMON__QUAT_TCC__ include guard */
