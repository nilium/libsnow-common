#ifndef __SNOW_THREAD_HH__
#define __SNOW_THREAD_HH__

#include "snow-common.hh"

#include <thread>

BEGIN_SNOW_NS

// Basically std::async but without potential blocking due to std::future. In
// turn, there is no return value, but you can use std::async for that.
template <class FNT, class... ARGS>
void async_thread(FNT &&func, ARGS&&... args)
{
  std::thread(func, args...).detach();
}

END_SNOW_NS

#endif /* end __SNOW_THREAD_HH__ include guard */
