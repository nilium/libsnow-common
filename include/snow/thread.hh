/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#pragma once

#include <snow/config.hh>

#include <thread>

namespace snow {

// Basically std::async but without potential blocking due to std::future. In
// turn, there is no return value, but you can use std::async for that.
template <class FNT, class... ARGS>
S_EXPORT void async_thread(FNT &&func, ARGS&&... args)
{
  std::thread(std::forward<FNT>(func), std::forward<ARGS>(args)...).detach();
}

} // namespace snow
