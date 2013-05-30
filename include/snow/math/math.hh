// math.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__MATH_HH__
#define __SNOW_COMMON__MATH_HH__

#include <snow/config.hh>
#include <cfloat>
#include <cmath>
#include <stdexcept>


namespace snow {


// General degrees-to-radians constant
#define S_DEG2RAD (0.01745329251)
#define S_RAD2DEG (57.29577951308233)
// Epsilon for testing for zero-ish
#define S_EPSILON (1e-5)



/*==============================================================================
  is_zero

    Tests for whether a value is zero. Specialized for float, double, and long
    double types to see if the value is less than their respective epsilons.
    For all other types, simply returns (v == 0).
==============================================================================*/
template <typename T>
bool is_zero(T value)
{
  return value == T(0);
}



template <>
inline bool is_zero<float>(float value)
{
  return std::abs(value) < S_EPSILON;
}



template <>
inline bool is_zero<double>(double value)
{
  return std::abs(value) < S_EPSILON;
}



template <>
inline bool is_zero<long double>(long double value)
{
  return std::abs(value) < S_EPSILON;
}



/*==============================================================================
  are_equiv

    Compares whether two values are equivalent. For non-floating-point values,
    this is the same as ==. For floating point values, this tests whether the
    difference of the two values is less than S_EPSILON using is_zero.
==============================================================================*/

template <typename T>
bool are_equiv(T lhs, T rhs)
{
  return (std::is_floating_point<T>::value
          ? is_zero(lhs - rhs)
          : (lhs == rhs));
}



/*==============================================================================
  max_value

    Returns the maximum value for a given type. Only intended for use with
    float, double, and long double. Other types will throw a runtime_error.
    Really only useful for some specific cases.
==============================================================================*/
template <typename T>
T max_value() { s_throw(std::runtime_error, "no set maximum for type"); }



template <>
inline float max_value<float>() { return FLT_MAX; }



template <>
inline double max_value<double>() { return DBL_MAX; }



template <>
inline long double max_value<long double>() { return LDBL_MAX; }



/*==============================================================================
  clamp_value

    Clamps a value from the given minimum value to the maximum value.
==============================================================================*/
template <typename T>
constexpr T clamp_value(T val, T min, T max)
{
  return (val < min ? min : (val > max ? max : val));
}




template <typename T, typename Q, typename R>
constexpr T clamp_value(T val, Q min, R max)
{
  return (val < min ? static_cast<T>(min) : (val > max ? static_cast<T>(max) : val));
}


} // namespace snow

#endif /* end __SNOW_COMMON__MATH_HH__ include guard */
