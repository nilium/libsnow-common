// math3d.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_MATH3D_HH__
#define __SNOW_MATH3D_HH__

/*==============================================================================
                                   REFERENCE
================================================================================

This is a basic collection of templatized POD 3D maths structs for C++.

They're not optimized for SSE or any other special cases. Essentially, they are
generic and therefore likely to work just about anywhere a C++11 compiler will
work. So, more or less everywhere. In many instances, you'd likely be better
served by using something like GLM, though with this code, you can effectively
create an array of vec3_ts to represent vertices, screw with them, and then pass
them to OpenGL or something. So, there are some small benefits.

It may also help to have POD types for serialization, though in reality this
shouldn't be your concern with 3D maths types.

--------------------------------------------------------------------------------

Included 3D maths types:
  - vec3_t<T>          3-component vector
  - vec4_t<T>          4-component vector
  - quat_t<T>          quaternion
  - line_t<T>          more or less a ray
  - mat4_t<T>          4x4 square matrix

All types must have a value_type of float or double. Long double may also work
but is completely untested and may break anything that depends on the data types
in the structures being packed.
*/

#include "vec3.hh"
#include "vec4.hh"
#include "quat.hh"
#include "line.hh"
#include "mat4.hh"

#endif /* end __SNOW_MATH3D_HH__ include guard */
