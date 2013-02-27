// line.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__LINE_HH__
#define __SNOW_COMMON__LINE_HH__

/*
line_t<value_type>
{
  ////// Member variables //////

  vec3_t  origin
  vec3_t  dist


  ////// Instance Methods //////

  vec3    end() const
    Get the end-point of the line.
  ------------------------------------------------------------------------------
  line_t  translated(const vec3 &d) const
  line_t &translate(const vec3 &d)
    Translate the line's origin by vector `d`.
  ------------------------------------------------------------------------------
  line_t  scaled(value_type d) const
  line_t &scale(value_type d)
    Scale the line by scalar `d`.
  ------------------------------------------------------------------------------
  vec3    nearest_to(const vec3& p) const
    Get the nearest point on the line to point `p`.
}
*/

namespace snow
{

template <typename T> struct vec3_t;

#include "inline/line.thh"

typedef line_t<float> linef_t;
typedef line_t<double> lined_t;

}

#include "vec3.hh"

#endif /* end __SNOW_COMMON__LINE_HH__ include guard */
