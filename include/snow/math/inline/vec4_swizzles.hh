/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


vec2_t<value_type> xx() const { return { x, x }; }
vec3_t<value_type> xxx() const { return { x, x, x }; }
vec4_t xxxx() const { return { x, x, x, x }; }
vec4_t xxxy() const { return { x, x, x, y }; }
vec4_t xxxz() const { return { x, x, x, z }; }
vec4_t xxxw() const { return { x, x, x, w }; }
vec3_t<value_type> xxy() const { return { x, x, y }; }
vec4_t xxyx() const { return { x, x, y, x }; }
vec4_t xxyy() const { return { x, x, y, y }; }
vec4_t xxyz() const { return { x, x, y, z }; }
vec4_t xxyw() const { return { x, x, y, w }; }
vec3_t<value_type> xxz() const { return { x, x, z }; }
vec4_t xxzx() const { return { x, x, z, x }; }
vec4_t xxzy() const { return { x, x, z, y }; }
vec4_t xxzz() const { return { x, x, z, z }; }
vec4_t xxzw() const { return { x, x, z, w }; }
vec3_t<value_type> xxw() const { return { x, x, w }; }
vec4_t xxwx() const { return { x, x, w, x }; }
vec4_t xxwy() const { return { x, x, w, y }; }
vec4_t xxwz() const { return { x, x, w, z }; }
vec4_t xxww() const { return { x, x, w, w }; }
vec2_t<value_type> xy() const { return { x, y }; }
vec3_t<value_type> xyx() const { return { x, y, x }; }
vec4_t xyxx() const { return { x, y, x, x }; }
vec4_t xyxy() const { return { x, y, x, y }; }
vec4_t xyxz() const { return { x, y, x, z }; }
vec4_t xyxw() const { return { x, y, x, w }; }
vec3_t<value_type> xyy() const { return { x, y, y }; }
vec4_t xyyx() const { return { x, y, y, x }; }
vec4_t xyyy() const { return { x, y, y, y }; }
vec4_t xyyz() const { return { x, y, y, z }; }
vec4_t xyyw() const { return { x, y, y, w }; }
vec3_t<value_type> xyz() const { return { x, y, z }; }
vec4_t xyzx() const { return { x, y, z, x }; }
vec4_t xyzy() const { return { x, y, z, y }; }
vec4_t xyzz() const { return { x, y, z, z }; }
vec4_t xyzw() const { return *this; }
vec3_t<value_type> xyw() const { return { x, y, w }; }
vec4_t xywx() const { return { x, y, w, x }; }
vec4_t xywy() const { return { x, y, w, y }; }
vec4_t xywz() const { return { x, y, w, z }; }
vec4_t xyww() const { return { x, y, w, w }; }
vec2_t<value_type> xz() const { return { x, z }; }
vec3_t<value_type> xzx() const { return { x, z, x }; }
vec4_t xzxx() const { return { x, z, x, x }; }
vec4_t xzxy() const { return { x, z, x, y }; }
vec4_t xzxz() const { return { x, z, x, z }; }
vec4_t xzxw() const { return { x, z, x, w }; }
vec3_t<value_type> xzy() const { return { x, z, y }; }
vec4_t xzyx() const { return { x, z, y, x }; }
vec4_t xzyy() const { return { x, z, y, y }; }
vec4_t xzyz() const { return { x, z, y, z }; }
vec4_t xzyw() const { return { x, z, y, w }; }
vec3_t<value_type> xzz() const { return { x, z, z }; }
vec4_t xzzx() const { return { x, z, z, x }; }
vec4_t xzzy() const { return { x, z, z, y }; }
vec4_t xzzz() const { return { x, z, z, z }; }
vec4_t xzzw() const { return { x, z, z, w }; }
vec3_t<value_type> xzw() const { return { x, z, w }; }
vec4_t xzwx() const { return { x, z, w, x }; }
vec4_t xzwy() const { return { x, z, w, y }; }
vec4_t xzwz() const { return { x, z, w, z }; }
vec4_t xzww() const { return { x, z, w, w }; }
vec2_t<value_type> xw() const { return { x, w }; }
vec3_t<value_type> xwx() const { return { x, w, x }; }
vec4_t xwxx() const { return { x, w, x, x }; }
vec4_t xwxy() const { return { x, w, x, y }; }
vec4_t xwxz() const { return { x, w, x, z }; }
vec4_t xwxw() const { return { x, w, x, w }; }
vec3_t<value_type> xwy() const { return { x, w, y }; }
vec4_t xwyx() const { return { x, w, y, x }; }
vec4_t xwyy() const { return { x, w, y, y }; }
vec4_t xwyz() const { return { x, w, y, z }; }
vec4_t xwyw() const { return { x, w, y, w }; }
vec3_t<value_type> xwz() const { return { x, w, z }; }
vec4_t xwzx() const { return { x, w, z, x }; }
vec4_t xwzy() const { return { x, w, z, y }; }
vec4_t xwzz() const { return { x, w, z, z }; }
vec4_t xwzw() const { return { x, w, z, w }; }
vec3_t<value_type> xww() const { return { x, w, w }; }
vec4_t xwwx() const { return { x, w, w, x }; }
vec4_t xwwy() const { return { x, w, w, y }; }
vec4_t xwwz() const { return { x, w, w, z }; }
vec4_t xwww() const { return { x, w, w, w }; }
vec2_t<value_type> yx() const { return { y, x }; }
vec3_t<value_type> yxx() const { return { y, x, x }; }
vec4_t yxxx() const { return { y, x, x, x }; }
vec4_t yxxy() const { return { y, x, x, y }; }
vec4_t yxxz() const { return { y, x, x, z }; }
vec4_t yxxw() const { return { y, x, x, w }; }
vec3_t<value_type> yxy() const { return { y, x, y }; }
vec4_t yxyx() const { return { y, x, y, x }; }
vec4_t yxyy() const { return { y, x, y, y }; }
vec4_t yxyz() const { return { y, x, y, z }; }
vec4_t yxyw() const { return { y, x, y, w }; }
vec3_t<value_type> yxz() const { return { y, x, z }; }
vec4_t yxzx() const { return { y, x, z, x }; }
vec4_t yxzy() const { return { y, x, z, y }; }
vec4_t yxzz() const { return { y, x, z, z }; }
vec4_t yxzw() const { return { y, x, z, w }; }
vec3_t<value_type> yxw() const { return { y, x, w }; }
vec4_t yxwx() const { return { y, x, w, x }; }
vec4_t yxwy() const { return { y, x, w, y }; }
vec4_t yxwz() const { return { y, x, w, z }; }
vec4_t yxww() const { return { y, x, w, w }; }
vec2_t<value_type> yy() const { return { y, y }; }
vec3_t<value_type> yyx() const { return { y, y, x }; }
vec4_t yyxx() const { return { y, y, x, x }; }
vec4_t yyxy() const { return { y, y, x, y }; }
vec4_t yyxz() const { return { y, y, x, z }; }
vec4_t yyxw() const { return { y, y, x, w }; }
vec3_t<value_type> yyy() const { return { y, y, y }; }
vec4_t yyyx() const { return { y, y, y, x }; }
vec4_t yyyy() const { return { y, y, y, y }; }
vec4_t yyyz() const { return { y, y, y, z }; }
vec4_t yyyw() const { return { y, y, y, w }; }
vec3_t<value_type> yyz() const { return { y, y, z }; }
vec4_t yyzx() const { return { y, y, z, x }; }
vec4_t yyzy() const { return { y, y, z, y }; }
vec4_t yyzz() const { return { y, y, z, z }; }
vec4_t yyzw() const { return { y, y, z, w }; }
vec3_t<value_type> yyw() const { return { y, y, w }; }
vec4_t yywx() const { return { y, y, w, x }; }
vec4_t yywy() const { return { y, y, w, y }; }
vec4_t yywz() const { return { y, y, w, z }; }
vec4_t yyww() const { return { y, y, w, w }; }
vec2_t<value_type> yz() const { return { y, z }; }
vec3_t<value_type> yzx() const { return { y, z, x }; }
vec4_t yzxx() const { return { y, z, x, x }; }
vec4_t yzxy() const { return { y, z, x, y }; }
vec4_t yzxz() const { return { y, z, x, z }; }
vec4_t yzxw() const { return { y, z, x, w }; }
vec3_t<value_type> yzy() const { return { y, z, y }; }
vec4_t yzyx() const { return { y, z, y, x }; }
vec4_t yzyy() const { return { y, z, y, y }; }
vec4_t yzyz() const { return { y, z, y, z }; }
vec4_t yzyw() const { return { y, z, y, w }; }
vec3_t<value_type> yzz() const { return { y, z, z }; }
vec4_t yzzx() const { return { y, z, z, x }; }
vec4_t yzzy() const { return { y, z, z, y }; }
vec4_t yzzz() const { return { y, z, z, z }; }
vec4_t yzzw() const { return { y, z, z, w }; }
vec3_t<value_type> yzw() const { return { y, z, w }; }
vec4_t yzwx() const { return { y, z, w, x }; }
vec4_t yzwy() const { return { y, z, w, y }; }
vec4_t yzwz() const { return { y, z, w, z }; }
vec4_t yzww() const { return { y, z, w, w }; }
vec2_t<value_type> yw() const { return { y, w }; }
vec3_t<value_type> ywx() const { return { y, w, x }; }
vec4_t ywxx() const { return { y, w, x, x }; }
vec4_t ywxy() const { return { y, w, x, y }; }
vec4_t ywxz() const { return { y, w, x, z }; }
vec4_t ywxw() const { return { y, w, x, w }; }
vec3_t<value_type> ywy() const { return { y, w, y }; }
vec4_t ywyx() const { return { y, w, y, x }; }
vec4_t ywyy() const { return { y, w, y, y }; }
vec4_t ywyz() const { return { y, w, y, z }; }
vec4_t ywyw() const { return { y, w, y, w }; }
vec3_t<value_type> ywz() const { return { y, w, z }; }
vec4_t ywzx() const { return { y, w, z, x }; }
vec4_t ywzy() const { return { y, w, z, y }; }
vec4_t ywzz() const { return { y, w, z, z }; }
vec4_t ywzw() const { return { y, w, z, w }; }
vec3_t<value_type> yww() const { return { y, w, w }; }
vec4_t ywwx() const { return { y, w, w, x }; }
vec4_t ywwy() const { return { y, w, w, y }; }
vec4_t ywwz() const { return { y, w, w, z }; }
vec4_t ywww() const { return { y, w, w, w }; }
vec2_t<value_type> zx() const { return { z, x }; }
vec3_t<value_type> zxx() const { return { z, x, x }; }
vec4_t zxxx() const { return { z, x, x, x }; }
vec4_t zxxy() const { return { z, x, x, y }; }
vec4_t zxxz() const { return { z, x, x, z }; }
vec4_t zxxw() const { return { z, x, x, w }; }
vec3_t<value_type> zxy() const { return { z, x, y }; }
vec4_t zxyx() const { return { z, x, y, x }; }
vec4_t zxyy() const { return { z, x, y, y }; }
vec4_t zxyz() const { return { z, x, y, z }; }
vec4_t zxyw() const { return { z, x, y, w }; }
vec3_t<value_type> zxz() const { return { z, x, z }; }
vec4_t zxzx() const { return { z, x, z, x }; }
vec4_t zxzy() const { return { z, x, z, y }; }
vec4_t zxzz() const { return { z, x, z, z }; }
vec4_t zxzw() const { return { z, x, z, w }; }
vec3_t<value_type> zxw() const { return { z, x, w }; }
vec4_t zxwx() const { return { z, x, w, x }; }
vec4_t zxwy() const { return { z, x, w, y }; }
vec4_t zxwz() const { return { z, x, w, z }; }
vec4_t zxww() const { return { z, x, w, w }; }
vec2_t<value_type> zy() const { return { z, y }; }
vec3_t<value_type> zyx() const { return { z, y, x }; }
vec4_t zyxx() const { return { z, y, x, x }; }
vec4_t zyxy() const { return { z, y, x, y }; }
vec4_t zyxz() const { return { z, y, x, z }; }
vec4_t zyxw() const { return { z, y, x, w }; }
vec3_t<value_type> zyy() const { return { z, y, y }; }
vec4_t zyyx() const { return { z, y, y, x }; }
vec4_t zyyy() const { return { z, y, y, y }; }
vec4_t zyyz() const { return { z, y, y, z }; }
vec4_t zyyw() const { return { z, y, y, w }; }
vec3_t<value_type> zyz() const { return { z, y, z }; }
vec4_t zyzx() const { return { z, y, z, x }; }
vec4_t zyzy() const { return { z, y, z, y }; }
vec4_t zyzz() const { return { z, y, z, z }; }
vec4_t zyzw() const { return { z, y, z, w }; }
vec3_t<value_type> zyw() const { return { z, y, w }; }
vec4_t zywx() const { return { z, y, w, x }; }
vec4_t zywy() const { return { z, y, w, y }; }
vec4_t zywz() const { return { z, y, w, z }; }
vec4_t zyww() const { return { z, y, w, w }; }
vec2_t<value_type> zz() const { return { z, z }; }
vec3_t<value_type> zzx() const { return { z, z, x }; }
vec4_t zzxx() const { return { z, z, x, x }; }
vec4_t zzxy() const { return { z, z, x, y }; }
vec4_t zzxz() const { return { z, z, x, z }; }
vec4_t zzxw() const { return { z, z, x, w }; }
vec3_t<value_type> zzy() const { return { z, z, y }; }
vec4_t zzyx() const { return { z, z, y, x }; }
vec4_t zzyy() const { return { z, z, y, y }; }
vec4_t zzyz() const { return { z, z, y, z }; }
vec4_t zzyw() const { return { z, z, y, w }; }
vec3_t<value_type> zzz() const { return { z, z, z }; }
vec4_t zzzx() const { return { z, z, z, x }; }
vec4_t zzzy() const { return { z, z, z, y }; }
vec4_t zzzz() const { return { z, z, z, z }; }
vec4_t zzzw() const { return { z, z, z, w }; }
vec3_t<value_type> zzw() const { return { z, z, w }; }
vec4_t zzwx() const { return { z, z, w, x }; }
vec4_t zzwy() const { return { z, z, w, y }; }
vec4_t zzwz() const { return { z, z, w, z }; }
vec4_t zzww() const { return { z, z, w, w }; }
vec2_t<value_type> zw() const { return { z, w }; }
vec3_t<value_type> zwx() const { return { z, w, x }; }
vec4_t zwxx() const { return { z, w, x, x }; }
vec4_t zwxy() const { return { z, w, x, y }; }
vec4_t zwxz() const { return { z, w, x, z }; }
vec4_t zwxw() const { return { z, w, x, w }; }
vec3_t<value_type> zwy() const { return { z, w, y }; }
vec4_t zwyx() const { return { z, w, y, x }; }
vec4_t zwyy() const { return { z, w, y, y }; }
vec4_t zwyz() const { return { z, w, y, z }; }
vec4_t zwyw() const { return { z, w, y, w }; }
vec3_t<value_type> zwz() const { return { z, w, z }; }
vec4_t zwzx() const { return { z, w, z, x }; }
vec4_t zwzy() const { return { z, w, z, y }; }
vec4_t zwzz() const { return { z, w, z, z }; }
vec4_t zwzw() const { return { z, w, z, w }; }
vec3_t<value_type> zww() const { return { z, w, w }; }
vec4_t zwwx() const { return { z, w, w, x }; }
vec4_t zwwy() const { return { z, w, w, y }; }
vec4_t zwwz() const { return { z, w, w, z }; }
vec4_t zwww() const { return { z, w, w, w }; }
vec2_t<value_type> wx() const { return { w, x }; }
vec3_t<value_type> wxx() const { return { w, x, x }; }
vec4_t wxxx() const { return { w, x, x, x }; }
vec4_t wxxy() const { return { w, x, x, y }; }
vec4_t wxxz() const { return { w, x, x, z }; }
vec4_t wxxw() const { return { w, x, x, w }; }
vec3_t<value_type> wxy() const { return { w, x, y }; }
vec4_t wxyx() const { return { w, x, y, x }; }
vec4_t wxyy() const { return { w, x, y, y }; }
vec4_t wxyz() const { return { w, x, y, z }; }
vec4_t wxyw() const { return { w, x, y, w }; }
vec3_t<value_type> wxz() const { return { w, x, z }; }
vec4_t wxzx() const { return { w, x, z, x }; }
vec4_t wxzy() const { return { w, x, z, y }; }
vec4_t wxzz() const { return { w, x, z, z }; }
vec4_t wxzw() const { return { w, x, z, w }; }
vec3_t<value_type> wxw() const { return { w, x, w }; }
vec4_t wxwx() const { return { w, x, w, x }; }
vec4_t wxwy() const { return { w, x, w, y }; }
vec4_t wxwz() const { return { w, x, w, z }; }
vec4_t wxww() const { return { w, x, w, w }; }
vec2_t<value_type> wy() const { return { w, y }; }
vec3_t<value_type> wyx() const { return { w, y, x }; }
vec4_t wyxx() const { return { w, y, x, x }; }
vec4_t wyxy() const { return { w, y, x, y }; }
vec4_t wyxz() const { return { w, y, x, z }; }
vec4_t wyxw() const { return { w, y, x, w }; }
vec3_t<value_type> wyy() const { return { w, y, y }; }
vec4_t wyyx() const { return { w, y, y, x }; }
vec4_t wyyy() const { return { w, y, y, y }; }
vec4_t wyyz() const { return { w, y, y, z }; }
vec4_t wyyw() const { return { w, y, y, w }; }
vec3_t<value_type> wyz() const { return { w, y, z }; }
vec4_t wyzx() const { return { w, y, z, x }; }
vec4_t wyzy() const { return { w, y, z, y }; }
vec4_t wyzz() const { return { w, y, z, z }; }
vec4_t wyzw() const { return { w, y, z, w }; }
vec3_t<value_type> wyw() const { return { w, y, w }; }
vec4_t wywx() const { return { w, y, w, x }; }
vec4_t wywy() const { return { w, y, w, y }; }
vec4_t wywz() const { return { w, y, w, z }; }
vec4_t wyww() const { return { w, y, w, w }; }
vec2_t<value_type> wz() const { return { w, z }; }
vec3_t<value_type> wzx() const { return { w, z, x }; }
vec4_t wzxx() const { return { w, z, x, x }; }
vec4_t wzxy() const { return { w, z, x, y }; }
vec4_t wzxz() const { return { w, z, x, z }; }
vec4_t wzxw() const { return { w, z, x, w }; }
vec3_t<value_type> wzy() const { return { w, z, y }; }
vec4_t wzyx() const { return { w, z, y, x }; }
vec4_t wzyy() const { return { w, z, y, y }; }
vec4_t wzyz() const { return { w, z, y, z }; }
vec4_t wzyw() const { return { w, z, y, w }; }
vec3_t<value_type> wzz() const { return { w, z, z }; }
vec4_t wzzx() const { return { w, z, z, x }; }
vec4_t wzzy() const { return { w, z, z, y }; }
vec4_t wzzz() const { return { w, z, z, z }; }
vec4_t wzzw() const { return { w, z, z, w }; }
vec3_t<value_type> wzw() const { return { w, z, w }; }
vec4_t wzwx() const { return { w, z, w, x }; }
vec4_t wzwy() const { return { w, z, w, y }; }
vec4_t wzwz() const { return { w, z, w, z }; }
vec4_t wzww() const { return { w, z, w, w }; }
vec2_t<value_type> ww() const { return { w, w }; }
vec3_t<value_type> wwx() const { return { w, w, x }; }
vec4_t wwxx() const { return { w, w, x, x }; }
vec4_t wwxy() const { return { w, w, x, y }; }
vec4_t wwxz() const { return { w, w, x, z }; }
vec4_t wwxw() const { return { w, w, x, w }; }
vec3_t<value_type> wwy() const { return { w, w, y }; }
vec4_t wwyx() const { return { w, w, y, x }; }
vec4_t wwyy() const { return { w, w, y, y }; }
vec4_t wwyz() const { return { w, w, y, z }; }
vec4_t wwyw() const { return { w, w, y, w }; }
vec3_t<value_type> wwz() const { return { w, w, z }; }
vec4_t wwzx() const { return { w, w, z, x }; }
vec4_t wwzy() const { return { w, w, z, y }; }
vec4_t wwzz() const { return { w, w, z, z }; }
vec4_t wwzw() const { return { w, w, z, w }; }
vec3_t<value_type> www() const { return { w, w, w }; }
vec4_t wwwx() const { return { w, w, w, x }; }
vec4_t wwwy() const { return { w, w, w, y }; }
vec4_t wwwz() const { return { w, w, w, z }; }
vec4_t wwww() const { return { w, w, w, w }; }
