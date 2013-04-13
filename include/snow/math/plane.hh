// plane.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__PLANE_HH__
#define __SNOW_COMMON__PLANE_HH__

#include <snow/config.hh>
#include <stdexcept>


namespace snow {


template <typename T> struct vec3_t;
template <typename T> struct line_t;


#include "inline/plane.hh"


typedef plane_t<float> planef_t;
typedef plane_t<double> planed_t;


} // namespace snow


#include "vec3.hh"
#include "line.hh"

#endif /* end __SNOW_COMMON__PLANE_HH__ include guard */
