vec2_t xx() const { return { x, x }; }
vec3_t<value_type> xxx() const { return { x, x, x }; }
vec4_t<value_type> xxxx() const { return { x, x, x, x }; }
vec4_t<value_type> xxxy() const { return { x, x, x, y }; }
vec3_t<value_type> xxy() const { return { x, x, y }; }
vec4_t<value_type> xxyx() const { return { x, x, y, x }; }
vec4_t<value_type> xxyy() const { return { x, x, y, y }; }
vec2_t xy() const { return *this; }
vec3_t<value_type> xyx() const { return { x, y, x }; }
vec4_t<value_type> xyxx() const { return { x, y, x, x }; }
vec4_t<value_type> xyxy() const { return { x, y, x, y }; }
vec3_t<value_type> xyy() const { return { x, y, y }; }
vec4_t<value_type> xyyx() const { return { x, y, y, x }; }
vec4_t<value_type> xyyy() const { return { x, y, y, y }; }
vec2_t yx() const { return { y, x }; }
vec3_t<value_type> yxx() const { return { y, x, x }; }
vec4_t<value_type> yxxx() const { return { y, x, x, x }; }
vec4_t<value_type> yxxy() const { return { y, x, x, y }; }
vec3_t<value_type> yxy() const { return { y, x, y }; }
vec4_t<value_type> yxyx() const { return { y, x, y, x }; }
vec4_t<value_type> yxyy() const { return { y, x, y, y }; }
vec2_t yy() const { return { y, y }; }
vec3_t<value_type> yyx() const { return { y, y, x }; }
vec4_t<value_type> yyxx() const { return { y, y, x, x }; }
vec4_t<value_type> yyxy() const { return { y, y, x, y }; }
vec3_t<value_type> yyy() const { return { y, y, y }; }
vec4_t<value_type> yyyx() const { return { y, y, y, x }; }
vec4_t<value_type> yyyy() const { return { y, y, y, y }; }
