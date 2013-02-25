// lbind.hh -- Noel Cower -- Licensed under CC BY-SA 2.5
// Attribution to Johannes Schaub for his answer on parameter packs on
// Stack Overflow: http://stackoverflow.com/a/7858971/457812
// For license information, see: http://creativecommons.org/licenses/by-sa/2.5/
#ifndef __SNOW_LBIND_HH__
#define __SNOW_LBIND_HH__

#include <stdexcept>
#include <string>

extern "C" {
#include <lua.h>
}

#include <snow-common.hh>

BEGIN_SNOW_NS

/*==============================================================================

  See lb_push_function below for the important bit.

  To provide push options for for non-integral/floating point/string types, you
  need to specialize on lub_push_value and lb_to_value. lb_push_value may take
  both a const reference or a non-const reference, so one or the other needs to
  be provided, but generally you'll likely only need to specialize the const
  reference version.

==============================================================================*/

typedef int (*lfunc_t)(lua_State *);

/*
  Parameter pack shamelessly stolen from Johannes Schaub's awesome answer on
  Stack Overflow: http://stackoverflow.com/a/7858971/457812
*/
template<int ...>
struct lb_pseq__ { };

template<int N, int ...S>
struct lb_gens__ : lb_gens__<N-1, N-1, S...> { };

template<int ...S>
struct lb_gens__<0, S...> {
  typedef lb_pseq__<S...> type;
};

template <typename T>
T lb_to_value(lua_State *L, int index)
{
  throw std::runtime_error("Cannot convert type from Lua value");
}

template <typename T>
void lb_push_value(lua_State *L, T &value)
{
  throw std::runtime_error("Cannot convert type to Lua value");
}

template <typename T>
void lb_push_value(lua_State *L, const T &value)
{
  throw std::runtime_error("Cannot convert type to Lua value");
}

template <typename T>
void lb_glue_from_(lua_State *L, int index, T &out)
{
  if (std::is_integral<T>::value)
    out = static_cast<T>(lua_tointeger(L, index));
  else if (std::is_floating_point<T>::value)
    out = static_cast<T>(lua_tonumber(L, index));
  else
    out = lb_to_value<T>(L, index);
}

template <>
void lb_glue_from_<std::string>(lua_State *L, int index, std::string &out)
{
  size_t length;
  const char *cstring = lua_tolstring(L, index, &length);
  if (cstring == NULL)
    throw std::runtime_error("Non-string value passed to string parameter of C function");
  out = std::string(cstring, length);
}

template <int index, typename... T>
void lb_unpack_values_into__(lua_State *L, std::tuple<T...> &values, const int max_index)
{
  typedef std::tuple<T...> tuple_type;
  if (index < max_index)
    lb_glue_from_(L, index + 1, std::get<index>(values));
  if (index > 0)
    lb_unpack_values_into__<(index <= 0 ? 0 : index - 1), T...>(L, values, max_index);
}

template <typename T>
void lb_push_value__(lua_State *L, T &value)
{
  if (std::is_integral<T>::value)
    lua_pushinteger(L, static_cast<lua_Integer>(value));
  else if (std::is_floating_point<T>::value)
    lua_pushnumber(L, static_cast<lua_Number>(value));
  else
    lb_push_value(L, value);
}

template <typename T>
void lb_push_value__(lua_State *L, const T &value)
{
  if (std::is_integral<T>::value)
    lua_pushinteger(L, static_cast<lua_Integer>(value));
  else if (std::is_floating_point<T>::value)
    lua_pushnumber(L, static_cast<lua_Number>(value));
  else
    lb_push_value(L, value);
}

template <>
void lb_push_value__<std::string>(lua_State *L, const std::string &value)
{
  lua_pushlstring(L, value.c_str(), value.length());
}

template <typename F, typename R, typename... T, int... S>
R lb_call_func__(F f,  std::tuple<T...> &params, lb_pseq__<S...>)
{
  return f(std::get<S>(params) ...);
}

template <typename F, typename R, typename... T>
int lb_glue_func__(lua_State *L)
{
  typedef std::tuple<T...> values_t;
  F func = *(F*)lua_touserdata(L, lua_upvalueindex(1));
  const int top = lua_gettop(L);
  const int num_values = sizeof...(T);
  const int num_returns = static_cast<int>(!std::is_void<R>::value);
  values_t values;
  lb_unpack_values_into__<sizeof...(T) - 1, T...>(L, values, top);
  if (num_returns);
    lb_push_value__<R>(L, lb_call_func__<F, R, T...>(func, values, typename lb_gens__<sizeof...(T)>::type()));
  return num_returns;
}

// For functions with no arguments - includes dummy template argument to make
// use easier.
template <int D, typename F, typename R>
int lb_glue_func__(lua_State *L)
{
  F func = *(F*)lua_touserdata(L, lua_upvalueindex(1));
  const int num_returns = static_cast<int>(!std::is_void<R>::value);
  if (num_returns);
    lb_push_value__<R>(L, func());
  return num_returns;
}

template <typename F, typename R, typename... T>
lfunc_t lb_glue_binding_fn__(R (*func)(T...))
{
  return lb_glue_func__<F, R, T...>;
}

// For functions with no arguments.
template <typename F, typename R>
lfunc_t lb_glue_binding_fn__(R (*func)())
{
  return lb_glue_func__<1, F, R>;
}

/*==============================================================================
  lb_glue_binding_fn

    Returns the Lua glue function for the given function (func).
==============================================================================*/
template <class F>
lfunc_t lb_glue_binding_fn(F func)
{
  return lb_glue_binding_fn__<F>(func);
}

/*==============================================================================
  lb_push_function

    Pushes the given function's wrapper-function onto the Lua stack.
==============================================================================*/
template <class F>
void lb_push_function(lua_State *L, F f)
{
  // Store the original function in an upvalue (necessary since I haven't found
  // any other way to provide the function with its original implementation)
  *(F*)lua_newuserdata(L, sizeof(F)) = f;
  lua_pushcclosure(L, lb_glue_binding_fn(f), 1);
}

END_SNOW_NS

#endif /* end __SNOW_LBIND_HH__ include guard */
