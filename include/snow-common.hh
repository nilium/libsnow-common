// snow-common.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__SNOW_COMMON_HH__
#define __SNOW_COMMON__SNOW_COMMON_HH__

#include "snow/config.hh"

#include "snow/thread.hh"

/*
Note: Doesn't include lbind.hh because that depends on whether Lua is installed
and linked to. So, it would produce some errors if in here.
*/

// Math
#include "snow/math/math.hh"
#ifndef S_NO_INCLUDE_MATH3D
// Because these headers are a bit large-ish, optionally exclude them.
#include "snow/math/math3d.hh"
#endif

// Types
#include "snow/types/binpack.hh"
#include "snow/types/range.hh"
#include "snow/types/range_set.hh"
#include "snow/types/triple_buffer.hh"
#include "snow/types/types_2d.hh"

// Data
#include "snow/data/sparse.hh"

#endif /* end __SNOW_COMMON__SNOW_COMMON_HH__ include guard */
