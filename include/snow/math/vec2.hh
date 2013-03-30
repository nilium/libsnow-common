// vec2.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__VEC2_HH__
#define __SNOW_COMMON__VEC2_HH__

#include "math.hh"

#include <cmath>
#include <iostream>
#include <type_traits>

namespace snow
{

template <typename T> struct vec3_t;
template <typename T> struct vec4_t;

#include "inline/vec2.hh"

typedef vec2_t<float> vec2f_t;
typedef vec2_t<double> vec2d_t;

}

#include "vec3.hh"
#include "vec4.hh"

#endif /* end __SNOW_COMMON__VEC2_HH__ include guard */
