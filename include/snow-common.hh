/*
 * Copyright Noel Cower 2013 - 2014.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef __SNOW_COMMON__SNOW_COMMON_HH__
#define __SNOW_COMMON__SNOW_COMMON_HH__

#include "snow/config.hh"

#include "snow/thread.hh"

// Math
#include "snow/math/math.hh"
#ifndef S_NO_INCLUDE_MATH3D
// Because these headers are a bit large-ish, optionally exclude them.
#include "snow/math/math3d.hh"
#endif

// Types
#include "snow/types/binpack.hh"
#include "snow/types/object_pool.hh"
#include "snow/types/range.hh"
#include "snow/types/range_set.hh"
#include "snow/types/slot_image.hh"
#include "snow/types/slot_mask.hh"
#include "snow/types/triple_buffer.hh"
#include "snow/types/types_2d.hh"

// Data
#include "snow/data/hash.hh"
#include "snow/data/sparse.hh"

// Strings
#include "snow/string/string.hh"
#include "snow/string/compare.hh"
#include "snow/string/split.hh"

// Memory
#include "snow/memory/ref_counter.hh"

#endif /* end __SNOW_COMMON__SNOW_COMMON_HH__ include guard */
