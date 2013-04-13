// thread.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__THREAD_HH__
#define __SNOW_COMMON__THREAD_HH__

#include <snow/config.hh>

#include <thread>

namespace snow {

// Basically std::async but without potential blocking due to std::future. In
// turn, there is no return value, but you can use std::async for that.
template <class FNT, class... ARGS>
S_EXPORT void async_thread(FNT &&func, ARGS&&... args)
{
  std::thread(func, args...).detach();
}

} // namespace snow

#endif /* end __SNOW_COMMON__THREAD_HH__ include guard */
