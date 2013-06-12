// line.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__LINE_THH__
#define __SNOW_COMMON__LINE_THH__

/*==============================================================================

  Line (ported over from Mark Sibly's Blitz3D maths routines)
  Not required to be a POD type, though it may end up being one due to
  restrictions placed on other types.

==============================================================================*/

template <typename T>
struct S_EXPORT line_t
{
  typedef T value_type;
  typedef vec3_t<T> vec3;

  vec3 origin;
  vec3 dist;

  auto end() const -> vec3;
  auto translated(vec3 d) const -> line_t;
  auto translate(vec3 d) -> line_t&;
  auto scaled(value_type d) const -> line_t;
  auto scale(value_type d) -> line_t&;
  auto nearest_to(const vec3& p) const -> vec3;
};

#include "line.cc"

#endif /* end __SNOW_COMMON__LINE_THH__ include guard */
