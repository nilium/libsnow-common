/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#pragma once

#include <snow/build-config.hh>


#ifndef S_NETWORK_IS_BIG_ENDIAN
#define S_NETWORK_IS_BIG_ENDIAN 0
#endif


#ifndef S_HOST_IS_BIG_ENDIAN
#define S_HOST_IS_BIG_ENDIAN 0
#endif


namespace snow
{


enum class endian_t
{
  little  = 1,
  big     = 2,
  // Mixed endianness unsupported

  #if S_HOST_IS_BIG_ENDIAN
  host = big,
  #else
  host = little,
  #endif

  #if S_NETWORK_IS_BIG_ENDIAN
  network = big,
  #else
  network = little,
  #endif
};


} // namespace snow
