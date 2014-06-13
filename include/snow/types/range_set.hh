/*
 * Copyright Noel Cower 2013.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef __SNOW_COMMON__RANGESET_HH__
#define __SNOW_COMMON__RANGESET_HH__

#include <snow/config.hh>
#include <stdexcept>
#include <list>


namespace snow {


template <typename T>
struct S_EXPORT range_set_t
{
  using range_type             = T;
  using value_type             = typename range_type::value_type;
  // Currently backed by a linked list. Could probably be backed by a set with
  // some changes to the routines below.
  using range_list_t           = std::list<range_type>;
  using iterator               = typename range_list_t::iterator;
  using const_iterator         = typename range_list_t::const_iterator;
  using const_reverse_iterator = typename range_list_t::const_reverse_iterator;


  auto add(const range_type &range) -> range_set_t&
  {
    if (range.length == 0)
      return *this;

    iterator iter = ranges_.begin();
    range_type morphed = range;
    iterator insert_pos = iter;

    while (iter != ranges_.end())
    {
      if (morphed.max() < iter->min() || iter->max() < morphed.min()) {
        insert_pos = iter;
        break;
      }

      if (iter->contains(morphed)) {
        // if the set already contains the range, end early
        return *this;
      } else if (morphed.contains(*iter)) {
        // remove the old range if it's smaller than the new one
        insert_pos = iter = ranges_.erase(iter);
      } else if (morphed.intersects(*iter)) {
        // create a union of the ranges and remove the old range if the two
        // intersect or touch
        morphed = morphed.joined(*iter);
        insert_pos = iter = ranges_.erase(iter);
      } else {
        ++iter;
      }
    }

    ranges_.insert(insert_pos, morphed);
    return *this;
  }



  auto subtract(const range_type &range) -> range_set_t&
  {
    if (range.length == 0)
      return *this;

    iterator iter = ranges_.begin();

    while (iter != ranges_.end())
    {
      if (range.max() < iter->min() || iter->max() < range.min())
        break;

      if (range.contains(*iter)) {
        // if the set's range is equal to the subtracted range, remove it entirely
        ranges_.erase(iter);
        break;
      } else if (iter->contains(range)) {
        range_type left, right;
        if (iter->split(range, &left, &right)) {
          if (right.length == 0) {
            std::swap(left, right);
          }

          if (right.length != 0) {
            *iter = right;

            if (left.length != 0) {
              ranges_.insert(iter, left);
            }
          }
        } else {
          s_throw(std::runtime_error, "Failed to split range");
        }
        break;
      } else if (range.intersects(*iter)) {
        range_type subbed = iter->difference(range);
        std::cout << subbed << std::endl;
        if (subbed.length != 0) {
          *iter = subbed;
        } else {
          iter = ranges_.erase(iter);
        }
      } else {
        ++iter;
      }
    }

    return *this;
  }



  auto clear() -> range_set_t&
  {
    ranges_.clear();
    return *this;
  }



  auto empty() const -> bool
  {
    return ranges_.empty();
  }



  auto intersects(const range_type &range) const -> bool
  {
    for (auto &subrange : *this)
      if (subrange.intersects(range))

    return false;
  }



  // Returns true if any portion of range is contained by a range in the set.
  template <typename Q>
  auto contains(const Q &inner) const -> bool
  {
    for (auto &subrange : *this)
      if (subrange.contains(inner))
        return true;

    return false;
  }



  auto begin() const -> const_iterator {
    return ranges_.cbegin();
  }



  auto end() const -> const_iterator {
    return ranges_.cend();
  }



  auto cbegin() const -> const_iterator {
    return ranges_.cbegin();
  }



  auto cend() const -> const_iterator {
    return ranges_.cend();
  }



  auto rbegin() const -> const_reverse_iterator {
    return ranges_.crbegin();
  }



  auto rend() const -> const_reverse_iterator {
    return ranges_.crend();
  }



  auto crbegin() const -> const_reverse_iterator {
    return ranges_.crbegin();
  }



  auto crend() const -> const_reverse_iterator {
    return ranges_.crend();
  }


private:

  range_list_t ranges_;

};


} // namespace snow


#endif /* end __SNOW_COMMON__RANGESET_HH__ include guard */
