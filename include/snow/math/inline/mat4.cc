// mat4.cc -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__MAT4_TCC__
#define __SNOW_COMMON__MAT4_TCC__

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
auto mat4_t<T>::make(T m00, T m10, T m20, T m30,
                     T m01, T m11, T m21, T m31,
                     T m02, T m12, T m22, T m32,
                     T m03, T m13, T m23, T m33) -> mat4_t
{
  return {
    m00, m10, m20, m30,
    m01, m11, m21, m31,
    m02, m12, m22, m32,
    m03, m13, m23, m33
  };
}

template <typename T>
auto mat4_t<T>::translation(const vec3 &off) -> mat4_t
{
  return {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    off.x, off.y, off.z, 1
  };
}

template <typename T>
auto mat4_t<T>::scaling(const vec3 &off) -> mat4_t
{
  return {
    off.x, 0, 0, 0,
    0, off.y, 0, 0,
    0, 0, off.z, 0,
    0, 0, 0, 1
  };
}

template <typename T>
auto mat4_t<T>::rotation(T angle, const vec3 &axis) -> mat4_t
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

  return {
    ((axis.x * axis.x) * ic) + c,  xy + zs,  xz - ys,  0,
    xy - zs,  ((axis.y * axis.y) * ic) + c,  yz + xs,  0,
    xz + ys,  yz - xs,  ((axis.z * axis.z) * ic) + c,  0,
    0,  0,  0,  1
  };
}

template <typename T>
auto mat4_t<T>::frustum(T left, T right, T bottom, T top, T near, T far) -> mat4_t
{
  const T xdelta = right - left;
  const T ydelta = top - bottom;
  const T zdelta = far - near;
  const T neardouble = 2.0 * near;

  return {
    neardouble / xdelta, 0, (right + left) / xdelta, 0,
    0, neardouble / ydelta, (top + bottom) / ydelta, 0,
    0, 0, -((far + near) / zdelta), -1.0,
    0, 0, -((neardouble * far) / zdelta), 0
  };
}

template <typename T>
auto mat4_t<T>::orthographic(T left, T right,
                          T top, T bottom,
                          T near, T far) -> mat4_t
{
  const T xdelta = right - left;
  const T ydelta = top - bottom;
  const T zdelta = far - near;

  return {
    T(2) / xdelta, 0, 0, 0,
    0, T(2) / ydelta, 0, 0,
    0, 0, T(-2) / zdelta, 0,
    -((right + left) / xdelta), -((top + bottom) / ydelta), -((far + near) / zdelta), 1
  };
}

template <typename T>
auto mat4_t<T>::perspective(T fovY, T aspect, T near, T far) -> mat4_t
{
  const float r = static_cast<T>(tanf(fovY * 0.5 * S_DEG2RAD));
  const float left = -r * aspect;
  const float right = r * aspect;
  const float bottom = -r;
  const float top = r;
  const float twoNear = 2 * near;
  const float zdelta = T(1) / (near - far);

  return {
    twoNear * (right - left), 0, 0, 0,
    0, twoNear / (top - bottom), 0, 0,
    0, 0, (far + near) * zdelta, -1,
    0, 0, (twoNear * far) * zdelta, 0
  };
}

template <typename T>
auto mat4_t<T>::look_at(const vec3 &eye, const vec3& center, const vec3& up) -> mat4_t
{
  vec3 f = (center - eye).normalize();
  vec3 up_norm = up.normalized();
  vec3 s = f.cross_product(up_norm);
  vec3 u = s.cross_product(f);
  return mat4_t::make(
    s.x, s.y, s.z, 0,
    u.x, u.y, u.z, 0,
    -f.x, -f.y, -f.z, 0,
    0, 0, 0, 1
  ).translate(-eye);
}

template <typename T>
auto mat4_t<T>::from_quat(const quat &in) -> mat4_t
{
  T tx, ty, tz, xx, xy, xz, yy, yz, zz, wx, wy, wz;

  tx = 2.0 * in.xyz.x;
  ty = 2.0 * in.xyz.y;
  tz = 2.0 * in.xyz.z;

  xx = tx * in.xyz.x;
  xy = tx * in.xyz.y;
  xz = tx * in.xyz.z;

  yy = ty * in.xyz.y;
  yz = tz * in.xyz.y;

  zz = tz * in.w;

  wx = tx * in.w;
  wy = ty * in.w;
  wz = tz * in.w;

  return {
    T(1) - (yy + zz), xy - wz, xz + wy, 0,
    xy + wz, T(1) - (xx + zz), yz - wx, 0,
    xz - wy, yz + wx, T(1) - (xx + yy), 0,
    0, 0, 0, 1
  };
}

template <typename T>
auto mat4_t<T>::translated(const vec3 &translation) const -> mat4_t
{
  mat4_t r = *this;
  return r.translate(translation);
}

template <typename T>
auto mat4_t<T>::transpose() -> mat4_t &
{
  #define S_SWAP_ELEM_(LHS, RHS) t=LHS;LHS=RHS;RHS=t;
  T t;
  S_SWAP_ELEM_(m10, m01);
  S_SWAP_ELEM_(m20, m02);
  S_SWAP_ELEM_(m30, m03);
  S_SWAP_ELEM_(m21, m12);
  S_SWAP_ELEM_(m31, m13);
  S_SWAP_ELEM_(m32, m23);
  #undef S_SWAP_ELEM_
  return *this;
}

template <typename T>
auto mat4_t<T>::transposed() const -> mat4_t
{
  return {
    m00, m01, m02, m03,
    m10, m11, m12, m13,
    m20, m21, m22, m23,
    m30, m31, m32, m33
  };
}

template <typename T>
auto mat4_t<T>::rowvec4(int index) const -> vec4
{
  switch (index) {
    case 0: return { m00, m10, m20, m30 };
    case 1: return { m01, m11, m21, m31 };
    case 2: return { m02, m12, m22, m32 };
    case 3: return { m03, m13, m23, m33 };
    default: s_throw(std::out_of_range, "attempt to access out of range row");
  }
}

template <typename T>
auto mat4_t<T>::colvec4(int index) const -> vec4
{
  switch (index) {
    case 0: return { m00, m01, m02, m03 };
    case 1: return { m10, m11, m12, m13 };
    case 2: return { m20, m21, m22, m23 };
    case 3: return { m30, m31, m32, m33 };
    default: s_throw(std::out_of_range, "attempt to access out of range column");
  }
}

template <typename T>
auto mat4_t<T>::set_rowvec4(int index, const vec4 &row) -> mat4_t &
{
  switch (index) {
    case 0: m00 = row.x; m10 = row.y; m20 = row.z; m30 = row.w; break;
    case 1: m01 = row.x; m11 = row.y; m21 = row.z; m31 = row.w; break;
    case 2: m02 = row.x; m12 = row.y; m22 = row.z; m32 = row.w; break;
    case 3: m03 = row.x; m13 = row.y; m23 = row.z; m33 = row.w; break;
    default: s_throw(std::out_of_range, "attempt to access out of range row");
  }
  return *this;
}

template <typename T>
auto mat4_t<T>::set_colvec4(int index, const vec4 &col) -> mat4_t &
{
  switch (index) {
    case 0: m00 = col.x; m01 = col.y; m02 = col.z; m03 = col.w; break;
    case 1: m10 = col.x; m11 = col.y; m12 = col.z; m13 = col.w; break;
    case 2: m20 = col.x; m21 = col.y; m22 = col.z; m23 = col.w; break;
    case 3: m30 = col.x; m31 = col.y; m32 = col.z; m33 = col.w; break;
    default: s_throw(std::out_of_range, "attempt to access out of range column");
  }
  return *this;
}

template <typename T>
auto mat4_t<T>::rowvec3(int index) const -> vec3
{
  switch (index) {
    case 0: return { m00, m10, m20 };
    case 1: return { m01, m11, m21 };
    case 2: return { m02, m12, m22 };
    case 3: return { m03, m13, m23 };
    default: s_throw(std::out_of_range, "attempt to access out of range row");
  }
}

template <typename T>
auto mat4_t<T>::colvec3(int index) const -> vec3
{
  switch (index) {
    case 0: return { m00, m01, m02 };
    case 1: return { m10, m11, m12 };
    case 2: return { m20, m21, m22 };
    case 3: return { m30, m31, m32 };
    default: s_throw(std::out_of_range, "attempt to access out of range column");
  }
}

template <typename T>
auto mat4_t<T>::set_rowvec3(int index, const vec3 &row) -> mat4_t &
{
  switch (index) {
    case 0: m00 = row.x; m10 = row.y; m20 = row.z; break;
    case 1: m01 = row.x; m11 = row.y; m21 = row.z; break;
    case 2: m02 = row.x; m12 = row.y; m22 = row.z; break;
    case 3: m03 = row.x; m13 = row.y; m23 = row.z; break;
    default: s_throw(std::out_of_range, "attempt to access out of range row");
  }
  return *this;
}

template <typename T>
auto mat4_t<T>::set_colvec3(int index, const vec3 &col) -> mat4_t &
{
  switch (index) {
    case 0: m00 = col.x; m01 = col.y; m02 = col.z; break;
    case 1: m10 = col.x; m11 = col.y; m12 = col.z; break;
    case 2: m20 = col.x; m21 = col.y; m22 = col.z; break;
    case 3: m30 = col.x; m31 = col.y; m32 = col.z; break;
    default: s_throw(std::out_of_range, "attempt to access out of range column");
  }
  return *this;
}

template <typename T>
auto mat4_t<T>::inverse_orthogonal() const -> mat4_t
{
  vec3_t<T> row = { m03, m13, m23 };
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

template <typename T>
auto mat4_t<T>::negated() const -> mat4_t
{
  return {
    -m00, -m10, -m20, -m30,
    -m01, -m11, -m21, -m31,
    -m02, -m12, -m22, -m32,
    -m03, -m13, -m23, -m33
  };
}

template <typename T>
auto mat4_t<T>::negate() -> mat4_t &
{
  m00 = -m00; m10 = -m10; m20 = -m20; m30 = -m30;
  m01 = -m01; m11 = -m11; m21 = -m21; m31 = -m31;
  m02 = -m02; m12 = -m12; m22 = -m22; m32 = -m32;
  m03 = -m03; m13 = -m13; m23 = -m23; m33 = -m33;
  return *this;
}

template <typename T>
auto mat4_t<T>::sum(const mat4_t &other) const -> mat4_t
{
  return {
    m00 + other.m00,
    m10 + other.m10,
    m20 + other.m20,
    m30 + other.m30,
    m01 + other.m01,
    m11 + other.m11,
    m21 + other.m21,
    m31 + other.m31,
    m02 + other.m02,
    m12 + other.m12,
    m22 + other.m22,
    m32 + other.m32,
    m03 + other.m03,
    m13 + other.m13,
    m23 + other.m23,
    m33 + other.m33
  };
}

template <typename T>
auto mat4_t<T>::sum(T scalar) const -> mat4_t
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

template <typename T>
auto mat4_t<T>::add(const mat4_t &other) -> mat4_t &
{
  m00 += other.m00;
  m10 += other.m10;
  m20 += other.m20;
  m30 += other.m30;
  m01 += other.m01;
  m11 += other.m11;
  m21 += other.m21;
  m31 += other.m31;
  m02 += other.m02;
  m12 += other.m12;
  m22 += other.m22;
  m32 += other.m32;
  m03 += other.m03;
  m13 += other.m13;
  m23 += other.m23;
  m33 += other.m33;
  return *this;
}

template <typename T>
auto mat4_t<T>::add(T scalar) -> mat4_t &
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


template <typename T>
auto mat4_t<T>::difference(const mat4_t &other) const -> mat4_t
{
  return {
    m00 - other.m00,
    m10 - other.m10,
    m20 - other.m20,
    m30 - other.m30,
    m01 - other.m01,
    m11 - other.m11,
    m21 - other.m21,
    m31 - other.m31,
    m02 - other.m02,
    m12 - other.m12,
    m22 - other.m22,
    m32 - other.m32,
    m03 - other.m03,
    m13 - other.m13,
    m23 - other.m23,
    m33 - other.m33
  };
}

template <typename T>
auto mat4_t<T>::difference(T scalar) const -> mat4_t
{
  return {
    m00 - scalar,
    m10 - scalar,
    m20 - scalar,
    m30 - scalar,
    m01 - scalar,
    m11 - scalar,
    m21 - scalar,
    m31 - scalar,
    m02 - scalar,
    m12 - scalar,
    m22 - scalar,
    m32 - scalar,
    m03 - scalar,
    m13 - scalar,
    m23 - scalar,
    m33 - scalar
  };
}

template <typename T>
auto mat4_t<T>::subtract(const mat4_t &other) -> mat4_t &
{
  m00 -= other.m00;
  m10 -= other.m10;
  m20 -= other.m20;
  m30 -= other.m30;
  m01 -= other.m01;
  m11 -= other.m11;
  m21 -= other.m21;
  m31 -= other.m31;
  m02 -= other.m02;
  m12 -= other.m12;
  m22 -= other.m22;
  m32 -= other.m32;
  m03 -= other.m03;
  m13 -= other.m13;
  m23 -= other.m23;
  m33 -= other.m33;
  return *this;
}

template <typename T>
auto mat4_t<T>::subtract(T scalar) -> mat4_t &
{
  m00 -= scalar;
  m10 -= scalar;
  m20 -= scalar;
  m30 -= scalar;
  m01 -= scalar;
  m11 -= scalar;
  m21 -= scalar;
  m31 -= scalar;
  m02 -= scalar;
  m12 -= scalar;
  m22 -= scalar;
  m32 -= scalar;
  m03 -= scalar;
  m13 -= scalar;
  m23 -= scalar;
  m33 -= scalar;
  return *this;
}

template <typename T>
auto mat4_t<T>::scaled(T scalar) const -> mat4_t
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

template <typename T>
auto mat4_t<T>::scale(T scalar) -> mat4_t &
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

template <typename T>
auto mat4_t<T>::scaled(const mat4_t &other) const -> mat4_t
{
  return {
    m00 * other.m00,
    m10 * other.m10,
    m20 * other.m20,
    m30 * other.m30,
    m01 * other.m01,
    m11 * other.m11,
    m21 * other.m21,
    m31 * other.m31,
    m02 * other.m02,
    m12 * other.m12,
    m22 * other.m22,
    m32 * other.m32,
    m03 * other.m03,
    m13 * other.m13,
    m23 * other.m23,
    m33 * other.m33
  };
}

template <typename T>
auto mat4_t<T>::scale(const mat4_t &other) -> mat4_t &
{
  m00 *= other.m00;
  m10 *= other.m10;
  m20 *= other.m20;
  m30 *= other.m30;
  m01 *= other.m01;
  m11 *= other.m11;
  m21 *= other.m21;
  m31 *= other.m31;
  m02 *= other.m02;
  m12 *= other.m12;
  m22 *= other.m22;
  m32 *= other.m32;
  m03 *= other.m03;
  m13 *= other.m13;
  m23 *= other.m23;
  m33 *= other.m33;
  return *this;
}

template <typename T>
auto mat4_t<T>::scaled(const vec3 &vec) const -> mat4_t
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

template <typename T>
auto mat4_t<T>::scale(const vec3 &vec) -> mat4_t &
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

template <typename T>
auto mat4_t<T>::cofactor(int r0, int r1, int r2, int c0, int c1, int c2) const -> T
{
  const T *m = &m00;
  // Macro for getting the value at the cofactor address provided
  #define S_CF_ADDR_(l, r) (m[(l * 4) + r])
  return (
    (S_CF_ADDR_(r0,c0) * ((S_CF_ADDR_(r1, c1) * S_CF_ADDR_(r2, c2)) - (S_CF_ADDR_(r2, c1) * S_CF_ADDR_(r1, c2)))) -
    (S_CF_ADDR_(r0,c1) * ((S_CF_ADDR_(r1, c0) * S_CF_ADDR_(r2, c2)) - (S_CF_ADDR_(r2, c0) * S_CF_ADDR_(r1, c2)))) +
    (S_CF_ADDR_(r0,c2) * ((S_CF_ADDR_(r1, c0) * S_CF_ADDR_(r2, c1)) - (S_CF_ADDR_(r2, c0) * S_CF_ADDR_(r1, c1))))
  );
  #undef S_CF_ADDR_
}

template <typename T>
auto mat4_t<T>::determinant() const -> T
{
  return ((m00 * cofactor(1, 2, 3, 1, 2, 3)) -
          (m01 * cofactor(1, 2, 3, 0, 2, 3)) +
          (m02 * cofactor(1, 2, 3, 0, 1, 3)) -
          (m03 * cofactor(1, 2, 3, 0, 1, 2)));
}

template <typename T>
auto mat4_t<T>::inverse_general(mat4_t &out) const -> bool
{
  T det = determinant();

  if (is_zero(det)) {
    return false;
  }
  out = adjoint().scale(T(1) / det);
  return true;
}

template <typename T>
auto mat4_t<T>::product(const mat4_t &other) const -> mat4_t
{
  mat4_t temp;
  T *m = &temp.m00;
  const vec4 rowvecs[4] = {
    other.rowvec4(0),
    other.rowvec4(1),
    other.rowvec4(2),
    other.rowvec4(3)
  };

  for (int cindex = 0; cindex < 4; ++cindex) {
    const vec4 column = colvec4(cindex);
    m[cindex     ] = column.dot_product(rowvecs[0]);
    m[cindex + 4 ] = column.dot_product(rowvecs[1]);
    m[cindex + 8 ] = column.dot_product(rowvecs[2]);
    m[cindex + 12] = column.dot_product(rowvecs[3]);
  }

  return temp;
}

template <typename T>
auto mat4_t<T>::multiply(const mat4_t &other) -> mat4_t &
{
  T *m = &m00;
  const vec4 rowvecs[4] = {
    other.rowvec4(0),
    other.rowvec4(1),
    other.rowvec4(2),
    other.rowvec4(3)
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

template <typename T>
auto mat4_t<T>::multiply(const vec4 &vec) const -> vec4
{
  return {
    vec.dot_product(colvec4(0)),
    vec.dot_product(colvec4(1)),
    vec.dot_product(colvec4(2)),
    vec.dot_product(colvec4(3))
  };
}

template <typename T>
auto mat4_t<T>::multiply(const vec3 &vec) const -> vec3
{
  return {
    vec.dot_product(colvec3(0)) + m03,
    vec.dot_product(colvec3(1)) + m13,
    vec.dot_product(colvec3(2)) + m23
  };
}

template <typename T>
auto mat4_t<T>::multiply(const vec2 &vec) const -> vec2
{
  return {
    vec.dot_product(colvec3(0)) + m03,
    vec.dot_product(colvec3(1)) + m13
  };
}

template <typename T>
auto mat4_t<T>::rotate(const vec4 &vec) const -> vec4
{
  return {
    vec.dot_product(colvec4(0)),
    vec.dot_product(colvec4(1)),
    vec.dot_product(colvec4(2))
  };
}

template <typename T>
auto mat4_t<T>::inverse_rotate(const vec4 &vec) const -> vec4
{
  return {
    vec.dot_product(rowvec4(0)),
    vec.dot_product(rowvec4(1)),
    vec.dot_product(rowvec4(2))
  };
}

template <typename T>
auto mat4_t<T>::rotate(const vec3 &vec) const -> vec3
{
  return {
    vec.dot_product(colvec3(0)),
    vec.dot_product(colvec3(1)),
    vec.dot_product(colvec3(2))
  };
}

template <typename T>
auto mat4_t<T>::inverse_rotate(const vec3 &vec) const -> vec3
{
  return {
    vec.dot_product(rowvec3(0)),
    vec.dot_product(rowvec3(1)),
    vec.dot_product(rowvec3(2))
  };
}

template <typename T>
auto mat4_t<T>::rotate(const vec2 &vec) const -> vec2
{
  return {
    vec.dot_product(colvec3(0)),
    vec.dot_product(colvec3(1))
  };
}

template <typename T>
auto mat4_t<T>::inverse_rotate(const vec2 &vec) const -> vec2
{
  return {
    vec.dot_product(rowvec3(0)),
    vec.dot_product(rowvec3(1))
  };
}

template <typename T>
auto mat4_t<T>::translate(const vec3 &translation) -> mat4_t &
{
  m03 += translation.dot_product(colvec3(0));
  m13 += translation.dot_product(colvec3(1));
  m23 += translation.dot_product(colvec3(2));
  m33 += translation.dot_product(colvec3(3));
  return *this;
}

template <typename T>
auto mat4_t<T>::inverse_affine(mat4_t & out) const -> bool
{
  T det;

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

  temp.scale(T(1) / det);
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
auto mat4_t<T>::operator[] (int index) -> T&
{
  static_assert(std::is_pod<mat4_t>::value, "mat4 must be POD to use subscript operator");
  if (index < 0 || index > 15)
    s_throw(std::out_of_range, "attempt to access out of range element");
  return (&m00)[index];
}

template <typename T>
auto mat4_t<T>::operator[] (int index) const -> T
{
  static_assert(std::is_pod<mat4_t>::value, "mat4 must be POD to use subscript operator");
  if (index < 0 || index > 15)
    s_throw(std::out_of_range, "attempt to access out of range element");
  return (&m00)[index];
}

template <typename T>
mat4_t<T>::operator T* ()
{
  static_assert(std::is_pod<mat4_t>::value, "mat4 must be POD to cast to T pointer");
  return &m00;
}

template <typename T>
mat4_t<T>::operator const T* () const
{
  static_assert(std::is_pod<mat4_t>::value, "mat4 must be POD to cast to T pointer");
  return &m00;
}

template <typename T>
mat4_t<T>::operator mat3_t<T> () const
{
  return {
    rowvec3(0),
    rowvec3(1),
    rowvec3(2)
  };
}

template <typename T>
mat4_t<T>::operator quat_t<T> () const
{
  return quat_t<T>::from_mat4(*this);
}

template <typename T>
auto mat4_t<T>::operator *= (const mat4_t &other) -> mat4_t &
{
  return multiply(other);
}

template <typename T>
auto mat4_t<T>::operator *= (T scalar) -> mat4_t &
{
  return scale(scalar);
}

template <typename T>
auto mat4_t<T>::operator += (const mat4_t &other) -> mat4_t &
{
  return add(other);
}

template <typename T>
auto mat4_t<T>::operator += (T scalar) -> mat4_t &
{
  return add(scalar);
}

template <typename T>
auto mat4_t<T>::operator -= (const mat4_t &other) -> mat4_t &
{
  return subtract(other);
}

template <typename T>
auto mat4_t<T>::operator -= (T scalar) -> mat4_t &
{
  return subtract(scalar);
}

template <typename T>
std::ostream &operator << (std::ostream &out, const mat4_t<T> &in)
{
  out << "{";
  for (int index = 0; index < 16; ++index) {
    out << in[index];
    if (index < 15)
      out << ", ";
  }
  return out << "}";
}

template <typename T, typename Q>
mat4_t<T> operator * (const mat4_t<T> &rhs, const mat4_t<Q> &lhs)
{
  return rhs.product(lhs);
}


template <typename T, typename Q>
vec4_t<T> operator * (const mat4_t<T> &rhs, const vec4_t<Q> &lhs)
{
  return rhs.multiply(lhs);
}


template <typename T, typename Q>
vec3_t<T> operator * (const mat4_t<T> &rhs, const vec3_t<Q> &lhs)
{
  return rhs.multiply(lhs);
}


template <typename T, typename Q>
vec2_t<T> operator * (const mat4_t<T> &rhs, const vec2_t<Q> &lhs)
{
  return rhs.multiply(lhs);
}

template <typename T, typename Q>
mat4_t<T> operator * (const mat4_t<T> &rhs, const Q &lhs)
{
  return rhs.scaled(lhs);
}

template <typename T, typename Q>
mat4_t<T> operator + (const mat4_t<T> &rhs, const mat4_t<Q> &lhs)
{
  return rhs.sum(lhs);
}

template <typename T, typename Q>
mat4_t<T> operator + (const mat4_t<T> &rhs, const Q &lhs)
{
  return rhs.sum(lhs);
}

template <typename T, typename Q>
mat4_t<T> operator - (const mat4_t<T> &rhs, const mat4_t<Q> &lhs)
{
  return rhs.difference(lhs);
}

template <typename T, typename Q>
mat4_t<T> operator - (const mat4_t<T> &rhs, const Q &lhs)
{
  return rhs.difference(lhs);
}

template <typename T, typename Q>
bool operator == (const mat4_t<T> &rhs, const mat4_t<Q> &lhs)
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

template <typename T, typename Q>
bool operator != (const mat4_t<T> &rhs, const mat4_t<Q> &lhs)
{
  return !(rhs == lhs);
}


#endif /* end __SNOW_COMMON__MAT4_TCC__ include guard */
