/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#pragma once

/** @addtogroup Math3D 3D Math Types
@brief A basic collection of templatized POD 3D maths structs for C++.

They're not optimized for SSE nor any other special cases. Essentially, they are
generic and therefore likely to work just about anywhere a C++11 compiler will
work. So, more or less everywhere. In many instances, you'd likely be better
served by using something like GLM, though with this code, you can effectively
create an array of vec3_ts to represent vertices, screw with them, and then pass
them to OpenGL or something. So, there are some small benefits.

It may also help to have POD types for serialization, though in reality this
shouldn't be your concern with 3D maths types.

@note
All types should have a value_type of float or double in order to be useful,
with the exception of vectors which are pretty handy regardless of their type.
Long double may also work but is completely untested and may break anything that
depends on the data types in the structures being packed.
*/

#include <snow/config.hh>
#include "vec2.hh"
#include "vec3.hh"
#include "vec4.hh"
#include "quat.hh"
#include "line.hh"
#include "plane.hh"
#include "mat4.hh"
#include "mat3.hh"
