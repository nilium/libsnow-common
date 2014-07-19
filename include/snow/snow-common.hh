/*
 * Copyright Noel Cower 2013 - 2014.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#pragma once

#include "config.hh"

#include "thread.hh"

// Math
#include "math/math.hh"
#ifndef S_NO_INCLUDE_MATH3D
// Because these headers are a bit large-ish, optionally exclude them.
#include "math/math3d.hh"
#endif

// Types
#include "types/binpack.hh"
#include "types/object_pool.hh"
#include "types/range.hh"
#include "types/range_set.hh"
#include "types/slot_image.hh"
#include "types/slot_mask.hh"
#include "types/triple_buffer.hh"
#include "types/types_2d.hh"
#include "types/option.hh"
#include "types/function_info.hh"

// Data
#include "data/hash.hh"
#include "data/sparse.hh"

// Strings
#include "string/string.hh"
#include "string/compare.hh"
#include "string/split.hh"
#include "string/utf8.hh"

// Memory
#include "memory/ref_counter.hh"

// Iterators
#include "iterator/accept.hh"
