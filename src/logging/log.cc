/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#include <snow/logging/log.hh>
#include <mutex>


namespace snow {


namespace {


std::mutex g_log_lock;
s_log_fn_t g_log_callback = nullptr;
void *g_log_callback_context = nullptr;


} // namespace <anon>


void s_log_callback(const char *msg, size_t length)
{
  if (g_log_callback) {
    g_log_callback(msg, length, g_log_callback_context);
  }
}



void s_set_log_callback(s_log_fn_t cb, void *ctx)
{
  s_log_lock();
  g_log_callback = cb;
  g_log_callback_context = ctx;
  s_log_unlock();
}



void s_log_lock()
{
  g_log_lock.lock();
}



void s_log_unlock()
{
  g_log_lock.unlock();
}



void s_log_impl(const char *format, ...)
{
  va_list arguments_null;
  va_list arguments_real;

  va_start(arguments_null, format);
  va_copy(arguments_real, arguments_null);

  int length = vsnprintf(NULL, 0, format, arguments_null);
  va_end(arguments_null);

  if (length == 0) {
    return;
  }
  std::vector<char> strbuf;
  strbuf.resize(length + 1);
  char *barebuf = strbuf.data();
  vsnprintf(barebuf, length + 1, format, arguments_real);
  va_end(arguments_real);

  s_log_lock();
  std::cout << barebuf;
  std::cout.flush();

  if (length > 0 && strbuf[length - 1] == '\n') {
    strbuf.pop_back();
    strbuf[--length] = '\0';
  }

  if (length) {
    s_log_callback(barebuf, length);
  }

  s_log_unlock();
}


} // namespace snow
