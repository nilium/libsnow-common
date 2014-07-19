/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#pragma once

#include "../config.hh"
#include <cstdint>


/**
  @file
  @ingroup HashFunctions
*/


namespace snow {

/**
  @addtogroup HashFunction Hash Functions
  @{
*/


/**
  Default 32-bit seed for snow::hash32. It's more or less random garbage that
  produces better results than some other random garbage.
*/
const uint32_t DEFAULT_HASH_SEED_32 = 0x9E2030F1U;
/**
  Default 64-bit seed for snow::hash64.
*/
const uint64_t DEFAULT_HASH_SEED_64 = 0x9E2030F19E2030F1ULL;



/**
  Produces a 32-bit hash of the input string.
  @see snow::hash32(const char *, const size_t, uint32_t)
*/
S_EXPORT uint32_t hash32(const string &str, uint32_t seed = DEFAULT_HASH_SEED_32);

/**
  Produces a 32-bit hash of the input data.
  @param str    The input data.
  @param length The length of the input data.
  @param seed   The seed for the input. If you want to concatenate string
  hashes, you can use a previous string hash as the seed and it will be the
  same as hashing the combined strings.
*/
S_EXPORT uint32_t hash32(const char *str, const size_t length,
                uint32_t seed = DEFAULT_HASH_SEED_32);

/**
  Produces a 64-bit hash of the input string.
  @see snow::hash64(const char *, const size_t, uint64_t)
*/
S_EXPORT uint64_t hash64(const string &str, uint64_t seed = DEFAULT_HASH_SEED_64);

/**
  Produces a 64-bit hash of the input data.
  @param str    The input data.
  @param length The length of the input data.
  @param seed   The seed for the input. If you want to concatenate string
  hashes, you can use a previous string hash as the seed and it will be the
  same as hashing the combined strings.
*/
S_EXPORT uint64_t hash64(const char *str, const size_t length,
                uint64_t seed = DEFAULT_HASH_SEED_64);


/** @} */


} // namespace snow
