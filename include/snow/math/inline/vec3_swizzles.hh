vec2_t<value_type> xx() const { return { x, x }; }
vec3_t xxx() const { return { x, x, x }; }
vec4_t<value_type> xxxx() const { return { x, x, x, x }; }
vec4_t<value_type> xxxy() const { return { x, x, x, y }; }
vec4_t<value_type> xxxz() const { return { x, x, x, z }; }
vec3_t xxy() const { return { x, x, y }; }
vec4_t<value_type> xxyx() const { return { x, x, y, x }; }
vec4_t<value_type> xxyy() const { return { x, x, y, y }; }
vec4_t<value_type> xxyz() const { return { x, x, y, z }; }
vec3_t xxz() const { return { x, x, z }; }
vec4_t<value_type> xxzx() const { return { x, x, z, x }; }
vec4_t<value_type> xxzy() const { return { x, x, z, y }; }
vec4_t<value_type> xxzz() const { return { x, x, z, z }; }
vec2_t<value_type> xy() const { return { x, y }; }
vec3_t xyx() const { return { x, y, x }; }
vec4_t<value_type> xyxx() const { return { x, y, x, x }; }
vec4_t<value_type> xyxy() const { return { x, y, x, y }; }
vec4_t<value_type> xyxz() const { return { x, y, x, z }; }
vec3_t xyy() const { return { x, y, y }; }
vec4_t<value_type> xyyx() const { return { x, y, y, x }; }
vec4_t<value_type> xyyy() const { return { x, y, y, y }; }
vec4_t<value_type> xyyz() const { return { x, y, y, z }; }
vec3_t xyz() const { return *this; }
vec4_t<value_type> xyzx() const { return { x, y, z, x }; }
vec4_t<value_type> xyzy() const { return { x, y, z, y }; }
vec4_t<value_type> xyzz() const { return { x, y, z, z }; }
vec2_t<value_type> xz() const { return { x, z }; }
vec3_t xzx() const { return { x, z, x }; }
vec4_t<value_type> xzxx() const { return { x, z, x, x }; }
vec4_t<value_type> xzxy() const { return { x, z, x, y }; }
vec4_t<value_type> xzxz() const { return { x, z, x, z }; }
vec3_t xzy() const { return { x, z, y }; }
vec4_t<value_type> xzyx() const { return { x, z, y, x }; }
vec4_t<value_type> xzyy() const { return { x, z, y, y }; }
vec4_t<value_type> xzyz() const { return { x, z, y, z }; }
vec3_t xzz() const { return { x, z, z }; }
vec4_t<value_type> xzzx() const { return { x, z, z, x }; }
vec4_t<value_type> xzzy() const { return { x, z, z, y }; }
vec4_t<value_type> xzzz() const { return { x, z, z, z }; }
vec2_t<value_type> yx() const { return { y, x }; }
vec3_t yxx() const { return { y, x, x }; }
vec4_t<value_type> yxxx() const { return { y, x, x, x }; }
vec4_t<value_type> yxxy() const { return { y, x, x, y }; }
vec4_t<value_type> yxxz() const { return { y, x, x, z }; }
vec3_t yxy() const { return { y, x, y }; }
vec4_t<value_type> yxyx() const { return { y, x, y, x }; }
vec4_t<value_type> yxyy() const { return { y, x, y, y }; }
vec4_t<value_type> yxyz() const { return { y, x, y, z }; }
vec3_t yxz() const { return { y, x, z }; }
vec4_t<value_type> yxzx() const { return { y, x, z, x }; }
vec4_t<value_type> yxzy() const { return { y, x, z, y }; }
vec4_t<value_type> yxzz() const { return { y, x, z, z }; }
vec2_t<value_type> yy() const { return { y, y }; }
vec3_t yyx() const { return { y, y, x }; }
vec4_t<value_type> yyxx() const { return { y, y, x, x }; }
vec4_t<value_type> yyxy() const { return { y, y, x, y }; }
vec4_t<value_type> yyxz() const { return { y, y, x, z }; }
vec3_t yyy() const { return { y, y, y }; }
vec4_t<value_type> yyyx() const { return { y, y, y, x }; }
vec4_t<value_type> yyyy() const { return { y, y, y, y }; }
vec4_t<value_type> yyyz() const { return { y, y, y, z }; }
vec3_t yyz() const { return { y, y, z }; }
vec4_t<value_type> yyzx() const { return { y, y, z, x }; }
vec4_t<value_type> yyzy() const { return { y, y, z, y }; }
vec4_t<value_type> yyzz() const { return { y, y, z, z }; }
vec2_t<value_type> yz() const { return { y, z }; }
vec3_t yzx() const { return { y, z, x }; }
vec4_t<value_type> yzxx() const { return { y, z, x, x }; }
vec4_t<value_type> yzxy() const { return { y, z, x, y }; }
vec4_t<value_type> yzxz() const { return { y, z, x, z }; }
vec3_t yzy() const { return { y, z, y }; }
vec4_t<value_type> yzyx() const { return { y, z, y, x }; }
vec4_t<value_type> yzyy() const { return { y, z, y, y }; }
vec4_t<value_type> yzyz() const { return { y, z, y, z }; }
vec3_t yzz() const { return { y, z, z }; }
vec4_t<value_type> yzzx() const { return { y, z, z, x }; }
vec4_t<value_type> yzzy() const { return { y, z, z, y }; }
vec4_t<value_type> yzzz() const { return { y, z, z, z }; }
vec2_t<value_type> zx() const { return { z, x }; }
vec3_t zxx() const { return { z, x, x }; }
vec4_t<value_type> zxxx() const { return { z, x, x, x }; }
vec4_t<value_type> zxxy() const { return { z, x, x, y }; }
vec4_t<value_type> zxxz() const { return { z, x, x, z }; }
vec3_t zxy() const { return { z, x, y }; }
vec4_t<value_type> zxyx() const { return { z, x, y, x }; }
vec4_t<value_type> zxyy() const { return { z, x, y, y }; }
vec4_t<value_type> zxyz() const { return { z, x, y, z }; }
vec3_t zxz() const { return { z, x, z }; }
vec4_t<value_type> zxzx() const { return { z, x, z, x }; }
vec4_t<value_type> zxzy() const { return { z, x, z, y }; }
vec4_t<value_type> zxzz() const { return { z, x, z, z }; }
vec2_t<value_type> zy() const { return { z, y }; }
vec3_t zyx() const { return { z, y, x }; }
vec4_t<value_type> zyxx() const { return { z, y, x, x }; }
vec4_t<value_type> zyxy() const { return { z, y, x, y }; }
vec4_t<value_type> zyxz() const { return { z, y, x, z }; }
vec3_t zyy() const { return { z, y, y }; }
vec4_t<value_type> zyyx() const { return { z, y, y, x }; }
vec4_t<value_type> zyyy() const { return { z, y, y, y }; }
vec4_t<value_type> zyyz() const { return { z, y, y, z }; }
vec3_t zyz() const { return { z, y, z }; }
vec4_t<value_type> zyzx() const { return { z, y, z, x }; }
vec4_t<value_type> zyzy() const { return { z, y, z, y }; }
vec4_t<value_type> zyzz() const { return { z, y, z, z }; }
vec2_t<value_type> zz() const { return { z, z }; }
vec3_t zzx() const { return { z, z, x }; }
vec4_t<value_type> zzxx() const { return { z, z, x, x }; }
vec4_t<value_type> zzxy() const { return { z, z, x, y }; }
vec4_t<value_type> zzxz() const { return { z, z, x, z }; }
vec3_t zzy() const { return { z, z, y }; }
vec4_t<value_type> zzyx() const { return { z, z, y, x }; }
vec4_t<value_type> zzyy() const { return { z, z, y, y }; }
vec4_t<value_type> zzyz() const { return { z, z, y, z }; }
vec3_t zzz() const { return { z, z, z }; }
vec4_t<value_type> zzzx() const { return { z, z, z, x }; }
vec4_t<value_type> zzzy() const { return { z, z, z, y }; }
vec4_t<value_type> zzzz() const { return { z, z, z, z }; }
