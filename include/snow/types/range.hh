/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef __SNOW_COMMON__RANGE_HH__
#define __SNOW_COMMON__RANGE_HH__

#include <iostream>
#include <algorithm>

#if TARGET_OS_MAC && __OBJC__
#import <Foundation/NSRange.h>
#endif

namespace snow
{


template <typename T> struct range_t;


template <typename T>
struct S_EXPORT range_iterator_t
    : public std::iterator<std::bidirectional_iterator_tag, T>
{
private:

  T begin_, end_;
  T step_;
  T location_;


public:

  range_iterator_t(const range_t<T> &range, const T &step = 1, bool at_end = false) :
    begin_(range.location), end_(range.location + range.length),
    step_(step < 0 ? -step : step)
  {
    if (at_end) {
      if (range.length >= 0) {
        location_ = end_ + step_;
      } else {
        location_ = end_ - step_;
      }
    } else {
      location_ = begin_;
    }

    if (range.length < 0) {
      --end_;
      --begin_;
      --location_;
    }
  }



  range_iterator_t(const T &location, const T& length, const T &step = 1, bool at_end = false) :
    begin_(location), end_(location + length),
    step_(step < 0 ? -step : step)
  {
    if (at_end) {
      if (length >= 0) {
        location_ = end_ + step_;
      } else {
        location_ = end_ - step_;
      }
    } else {
      location_ = begin_;
    }

    if (length < 0) {
      --end_;
      --begin_;
      --location_;
    }
  }



  auto operator *  () const -> const T&
  {
    return location_;
  }



  auto operator ++ () -> range_iterator_t&
  {
    if ( ! at_end()) {
      if (begin_ <= end_) {
        location_ += step_;
      } else {
        location_ -= step_;
      }
    }
    return *this;
  }



  auto operator -- () -> range_iterator_t&
  {
    if (begin_ <= end_) {
      if (location_ > begin_) {
        location_ -= step_;
      }
    } else {
      if (location_ < begin_)
        location_ += step_;
    }
    return *this;
  }



  auto operator ++ (int dummy) const -> range_iterator_t
  {
    (void)dummy;
    range_iterator_t temp = *this;
    ++(*this);
    return temp;
  }



  auto operator -- (int dummy) const -> range_iterator_t
  {
    (void)dummy;
    range_iterator_t temp = *this;
    --(*this);
    return temp;
  }



  auto less_than(const range_iterator_t &lhs) -> bool
  {
    return location_ < lhs.location_;
  }



  auto greater_than(const range_iterator_t &lhs) -> bool
  {
    return location_ > lhs.location_;
  }



  auto at_end() const -> bool
  {
    return (begin_ <= end_ ? location_ >= end_ : end_ >= location_);
  }



  auto equal(const range_iterator_t &rhs) const -> bool
  {
    return begin_ == rhs.begin_ && end_ == rhs.end_ &&
          ((at_end() && rhs.at_end()) || location_ == rhs.location_);
  }
};



template <typename T = int>
struct S_EXPORT range_t
{

  using value_type = T;
  using const_iterator = range_iterator_t<value_type>;
  using iterator       = const_iterator;


  value_type location;
  value_type length;


  static
  auto make(const value_type &location, const value_type &length) -> range_t
  {
    return {
      location,
      length
    };
  }



  auto max() const -> value_type
  {
    if (length >= 0)
      return location + length;
    else
      return location;
  }



  auto min() const -> value_type
  {
    if (length >= 0)
      return location;
    else
      return location + length;
  }



  auto contains(const value_type &loc) const -> bool
  {
    return min() <= loc && loc <= max();
  }



  auto contains(const range_t &other) const -> bool
  {
    return min() <= other.min() && other.max() <= max();
  }



  auto inverted() const -> range_t
  {
    if (length == 0)
      return *this;

    return {
      location + length,
      -length
    };
  }



  auto invert() -> range_t&
  {
    if (length == 0)
      return *this;

    location = location + length;
    length = -length;
    return *this;
  }



  auto normalized() const -> range_t
  {
    if (length < 0) {
      return inverted();
    }
    return *this;
  }



  auto normalize() -> range_t&
  {
    if (length < 0)
      invert();
    return *this;
  }

  // Always results in a range with length >= 0.


  // Would be called 'union' but that's reserved.
  auto joined(const range_t &other) const -> range_t
  {
    const value_type minLocation = std::min(min(), other.min());
    return {
      minLocation,
      std::max(max(), other.max()) - minLocation
    };
  }



  auto join(const range_t &other) -> range_t&
  {
    *this = joined(other);
    return *this;
  }

  // Given a region that extends into this region (but is not contained by this
  // region), remove the overlap from this region and return the result.
  // If this region is equal to the other region, a zero-length version of this
  // region is returned.
  // If this region contains but is not equal to the other region, this region
  // is returned without changes.
  // If successful, `success` will be set to true, if provided. If not, it will


  // be set to false.
  auto difference(const range_t &other, bool *success = nullptr) const -> range_t
  {
    if (other == *this) {
      if (success)
        *success = true;
      return {
        location,
        0
      };
    }

    const value_type minLoc = min();
    const value_type maxLoc = max();
    const value_type otherMin = other.min();
    const value_type otherMax = other.max();

    if (!contains(other) && intersects(other)) {
      if (otherMin <= minLoc && minLoc < otherMax) {
        if (success)
          *success = true;
        return {
          otherMax,
          std::max(0, maxLoc - otherMax)
        };
      } else if (otherMin < maxLoc && maxLoc <= otherMax) {
        if (success)
          *success = true;
        return {
          std::min(minLoc, otherMin),
          std::max(otherMin - minLoc, 0)
        };
      }
    } else if (success) {
      *success = false;
    }

    return *this;
  }

  // For ranges where other is contained by this region, splits this region into
  // two regions (left and right respectively). Returns true if successfully


  // split, false if not.
  auto split(const range_t &other, range_t *left_out, range_t *right_out) const -> bool
  {
    if (other.length == 0) {
      return false;
    } else if (other.length < 0 || length < 0) {
      // Guarantee both inputs have positive lengths
      bool result =
        (length < 0
         ? normalized()
         : *this).split((other.length < 0
                         ? other.normalized()
                         : other),
                        left_out, right_out);

      if (result && length < 0) {
        if (left_out)
          left_out->invert();
        if (right_out)
          right_out->invert();
      }

      return result;
    } else if (!contains(other)) {
      return false;
    } else {
      const range_t left = {
        location,
        other.location - location
      };
      const value_type otherMax = other.max();
      const range_t right = {
        otherMax,
        max() - otherMax
      };

      if (left_out)
        *left_out = left;
      if (right_out)
        *right_out = right;

      return true;
    }
  }



  auto intersects(const range_t &other) const -> bool
  {
    return max() > other.min() && min() < other.max();
  }



  auto intersection(const range_t &other) const -> range_t
  {
    const value_type minLocation = std::max(min(), other.min());
    const value_type maxLocation = std::min(max(), other.max());
    return {
      minLocation,
      std::max(0, maxLocation - minLocation)
    };
  }



  auto operator ^ (const range_t &other) const -> bool
  {
    return intersects(other);
  }



  // Intersects range
  auto operator % (const range_t &other) const -> bool
  {
    return contains(other);
  }



  // Contains location
  auto operator % (const value_type &value) const -> bool
  {
    return contains(value);
  }



  // union
  auto operator + (const range_t &other) const -> range_t
  {
    return joined(other);
  }



  auto operator += (const range_t &other) -> range_t&
  {
    return join(other);
  }



  // intersection
  auto operator / (const range_t &other) const -> range_t
  {
    return intersection(other);
  }



  auto operator /= (const range_t &other) -> range_t&
  {
    return *this = intersection(other);
  }



  auto operator < (const range_t &other) const -> bool
  {
    return min() < other.min();
  }



  auto operator > (const range_t &other) const -> bool
  {
    return min() > other.min();
  }



  auto operator == (const range_t &other) const -> bool
  {
    return min() == other.min() && max() == other.max();
  }



  auto operator != (const range_t &other) const -> bool
  {
    return min() != other.min() || max() != other.max();
  }



  auto operator >= (const range_t &other) const -> bool
  {
    return *this > other || *this == other;
  }



  auto operator <= (const range_t &other) const -> bool
  {
    return *this < other || *this == other;
  }

  // All range iterators are const_iterators, technically.
  // Note about steps: it's recommended you use a step that the range's length


  // can be evenly divided by, otherwise you won't get the end location.
  auto end(const value_type &step = 1) const -> iterator
  {
    return iterator(*this, step, true);
  }



  auto begin(const value_type &step = 1) const -> iterator
  {
    return iterator(*this, step, false);
  }



  auto rbegin(const value_type &step = 1) const -> iterator
  {
    return iterator(inverted(), step, false);
  }



  auto rend(const value_type &step = 1) const -> iterator
  {
    return iterator(inverted(), step, true);
  }



  auto cend(const value_type &step = 1) const -> const_iterator
  {
    return const_iterator(*this, step, true);
  }



  auto cbegin(const value_type &step = 1) const -> const_iterator
  {
    return const_iterator(*this, step, false);
  }



  auto crbegin(const value_type &step = 1) const -> const_iterator
  {
    return const_iterator(inverted(), step, false);
  }



  auto crend(const value_type &step = 1) const -> const_iterator
  {
    return const_iterator(inverted(), step, true);
  }



#if TARGET_OS_MAC && __OBJC__
  operator NSRange () const
  {
    return to_nsrange();
  }



  auto to_nsrange() const -> NSRange
  {
    return {
      static_cast<NSUInteger>(min()),
      static_cast<NSUInteger>(max() - min())
    };
  }
#endif



  template <typename Q>
  operator range_t<Q> () const
  {
    range_t<Q> result = {
      static_cast<Q>(location),
      static_cast<Q>(length)
    };
    return result;
  }

}; // struct range_t



template <typename T = int>
auto make_range(T location, T length) -> range_t<T>
{
  return {
    location,
    length
  };
}



#if TARGET_OS_MAC && __OBJC__
template <typename T = int>
auto make_range(const NSRange &range) -> range_t<T>
{
  return {
    static_cast<T>(range.location),
    static_cast<T>(range.length)
  };
}
#endif



template <typename T>
bool operator > (const range_iterator_t<T> &lhs,
                 const range_iterator_t<T> &rhs)
{
  return lhs.greater_than(rhs);
}



template <typename T>
bool operator < (const range_iterator_t<T> &lhs,
                 const range_iterator_t<T> &rhs)
{
  return lhs.less_than(rhs);
}



template <typename T>
bool operator ==(const range_iterator_t<T> &lhs,
                 const range_iterator_t<T> &rhs)
{
  return lhs.equal(rhs);
}



template <typename T>
bool operator !=(const range_iterator_t<T> &lhs,
                 const range_iterator_t<T> &rhs)
{
  return !lhs.equal(rhs);
}



template <typename T>
std::ostream &operator << (std::ostream &out, const range_t<T> in)
{
  return out << "{ location: " << in.location << ", length: " << in.length << " }";
}



/*******************************************************************************
*                                Common Aliases                                *
*******************************************************************************/

using rangei_t  = range_t<int>;
using rangel_t  = range_t<long>;
using rangeui_t = range_t<unsigned int>;
using rangeul_t = range_t<unsigned long>;
using rangef_t  = range_t<float>;
using ranged_t  = range_t<double>;

}

#endif /* end __SNOW_COMMON__RANGE_HH__ include guard */
