/*
 *              Copyright Noel Cower 2014.
 *
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef __SNOW_COMMON__ACCEPT_HH__
#define __SNOW_COMMON__ACCEPT_HH__

#include "../types/option.hh"
#include "../types/function_info.hh"
#include <algorithm>


/*==============================================================================

  accept.hh
  =========

  Covers basic iterator functions (i.e., accept_if, accept_while, accept_seq,
  accept_run, etc.). All iterator functions that take predicates can receive
  predicates with three different argument lists:

  - predicate() -> bool-ish:
    Receives no value.

  - predicate(T const &value) -> bool-ish:
    Receives the value being tested.

  - predicate(IT const &iter, IT const &end) -> bool-ish:
    Receives the iterator being tested and the end iterator.


  The return types for each function varies depending on two qualities:

  - whether the function receives an apply function.
  - whether the function can advance its iterator more than once.

  All functions that receive an apply function return an optional<T>, where T
  is the result type of the apply function.

  Functions that do not receive apply functions return booleans if they can
  advance the iterator only once, in which case they return true if they
  advance the iterator, otherwise false. Functions that can advance the
  iterator one or more times return an int for the number of times they advance
  the supplied iterator.


  Functions
  ---------

  - accept_if:
    Advances an iterator once only if its predicate is true.

  - accept_while:
    Advances an iterator while its predicate is true. The predicate function
    is called per-iteration.

  - accept_run:
    Advances an iterator while the iterator points to any value contained in
    a collection of items. The iterator need not match these items in any order.

  - accept_one:
    Advances an iterator once if the iterator points to any value contained in
    a collection of items. Same rules as accept_run.

  - accept_seq:
    Advances an iterator by however many elements are in a sequence if all
    the iterator matches all elements in the sequence in order. So, given a
    sequence 'a', 'b', the iterator must point to 'a' and then 'b'.

  - accept:
    Advances the iterator once if it points to something equal to a value.

==============================================================================*/


namespace snow
{


/*==============================================================================

  accept_util__

  Covers a few utility functions and types for accept_* functions.

==============================================================================*/
namespace accept_util__
{

template <typename IT, typename Val>
static bool contains__(IT start, IT end, Val const &value)
{
  return std::find(start, end, value) != end;
}


// Forwards calls to predicates based on the arity of the predicates.
// Arity of 0 passes no arguments, 1 passes the value at the iterator, 2 passes
// the iterator pointing to the value and the ending iterator. Any other arity
// is considered invalid.
//
// Wrapped in a struct to get a sort of pseudo-partial-specialization for
// functions.
template <size_t arity>
struct pred_forwarding_t__
{
};


template <typename FN>
using pred_forwarding__ = pred_forwarding_t__<function_info<FN>::args::arity>;


// Arity 0 - pred() -> bool-ish
template <>
struct pred_forwarding_t__<0>
{
  template <typename FN, typename IT>
  static bool call(FN &&pred, IT const &ptr, IT const &end)
  {
    (void)ptr;
    (void)end;
    return bool{pred()};
  }
};


// Arity 1 - pred(*ptr) -> bool-ish
template <>
struct pred_forwarding_t__<1>
{
  template <typename FN, typename IT>
  static bool call(FN &&pred, IT const &ptr, IT const &end)
  {
    (void)end;
    return bool{pred(*ptr)};
  }
};


// Arity 2 - pred(IT ptr, IT end) -> bool-ish
template <>
struct pred_forwarding_t__<2>
{
  template <typename FN, typename IT>
  static bool call(FN &&pred, IT const &ptr, IT const &end)
  {
    return bool{pred(ptr, end)};
  }
};


} // namespace accept_util__


/*==============================================================================
  peek(start, end)

    Returns the next element in the iterator's sequence, or none if at the end.
==============================================================================*/
template <typename IT>
auto peek(IT const start, IT const end) -> optional<decltype(*start)>
{
  if (start == end) {
    return none;
  }

  IT next { start };
  ++next;

  if (next == end) {
    return none;
  }

  return some(*next);
}


/*==============================================================================
  accept_if(start, end, pred(...)) -> bool

    Advances the start iterator once if the predicate function returns true and
    the start iterator is not the end iterator. Returns true if the iterator
    was advanced, otherwise false.
==============================================================================*/
template <bool Result = true, typename IT, typename Pred>
bool accept_if(IT &start, IT end, Pred &&pred)
{
  using PF = accept_util__::pred_forwarding__<Pred>;
  if (start == end || PF::call(std::forward<Pred &&>(pred), start, end) != Result) {
    return false;
  }

  ++start;

  return true;
}


/*==============================================================================
  accept_while(start, end, pred(...)) -> int

    Advances the start iterator while the predicate function returns true.
    Returns the number of times the iterator was advanced.
==============================================================================*/
template <bool Result = true, typename IT, typename Pred>
int accept_while(IT &start, IT end, Pred &&pred)
{
  int count = 0;

  while (accept_if<Result>(start, end, std::forward<Pred &&>(pred))) {
    ++count;
  }

  return count;
}


/*==============================================================================
  accept_run(start, end, run_start, run_end) -> int

    Advances the start iterator while it points to a value in the sequence
    of run_start and run_end. Returns the number of times the iterator was
    advanced.
==============================================================================*/
template <typename IT, typename CT>
int accept_run(IT &start, IT const end, CT const run_start, CT const run_end)
{
  return accept_while(start, end, [&](IT const &i, IT const &e) {
    return accept_util__::contains__(run_start, run_end, *i);
  });
}


/*==============================================================================
  accept_run(start, end, run_start, run_end, count) -> int

    Advances the start iterator while it points to a value in the sequence of
    run_start and run_end at most count times. Returns the number of times the
    iterator was advanced. If count is less than 1, the result is always 0.
==============================================================================*/
template <typename IT, typename CT>
int accept_run(IT &start, IT const end, CT const run_start, CT const run_end, int const count)
{
  using Val = typename std::decay<decltype(*start)>::type;

  int nth = 0;
  return accept_while(start, end, [&](IT const &i, IT const &e) {
    return (nth++ < count) && accept_util__::contains__(run_start, run_end, *i);
  });
}


/*==============================================================================
  accept_run(start, end, vals, count) -> int

    Wrapper for accept_run(start, end, std::begin(vals), std::end(vals), count).
==============================================================================*/
template <typename IT, typename Vals>
int accept_run(IT &start, IT const end, Vals const &vals, long count)
{
  return accept_run(start, end, std::begin(vals), std::end(vals), count);
}


/*==============================================================================
  accept_run(start, end, vals) -> int

    Wrapper for accept_run(start, end, std::begin(vals), std::end(vals)).
==============================================================================*/
template <typename IT, typename Vals>
int accept_run(IT &start, IT const end, Vals const &vals)
{
  return accept_run(start, end, std::begin(vals), std::end(vals));
}


/*==============================================================================
  accept_one(start, end, run_start, run_end) -> bool

    Alias for accept_run with a count argument of 1. Returns true if the
    iterator was advanced.
==============================================================================*/
template <typename IT, typename CT>
bool accept_one(IT &start, IT const end, CT const run_start, CT const run_end)
{
  return accept_run(start, end, run_start, run_end, 1) > 0;
}


/*==============================================================================
  accept_one(start, end, vals) -> bool

    Wrapper for accept_run(start, end, std::begin(vals), std::end(vals), 1).
==============================================================================*/
template <typename IT, typename Vals>
bool accept_one(IT &start, IT const end, Vals const &vals)
{
  return accept_run(start, end, std::begin(vals), std::end(vals), 1) > 0;
}


/*==============================================================================
  accept_run(start, end, run_start, run_end, count, apply(from, to)) -> optional<T>

    Same as other accept_run impls, except passes the iterator range that was
    accepted to the apply function. The result of the apply function is
    returned as an optional<T>, where it only has a defined value if a run
    was accepted.
==============================================================================*/
template <typename IT, typename CT, typename FN>
auto accept_run(IT &start, IT const end, CT const run_start, CT const run_end, long count, FN &&apply) -> optional<decltype(apply(start, end))>
{
  IT const origin { start };

  if (!accept_run(start, end, run_start, run_end, count)) {
    return none;
  }

  return some(apply(origin, IT { start }));
}


/*==============================================================================
  accept_run(start, end, vals, count, apply(from, to)) -> optional<T>

    Wrapper for
    accept_run(start, end, std::begin(vals), std::end(vals), count, apply).
==============================================================================*/
template <typename IT, typename Vals, typename FN>
auto accept_run(IT &start, IT const end, Vals const &vals, long count, FN &&apply) -> optional<decltype(apply(start, end))>
{
  IT const origin { start };

  if (!accept_run(start, end, std::begin(vals), std::end(vals), count)) {
    return none;
  }

  return some(apply(origin, IT { start }));
}


/*==============================================================================
  accept_run(start, end, run_start, run_end, apply(from, to)) -> optional<T>

    Same as other accept_run impls, except passes the iterator range that was
    accepted to the apply function. The result of the apply function is
    returned as an optional<T>, where it only has a defined value if a run
    was accepted.
==============================================================================*/
template <typename IT, typename CT, typename FN>
auto accept_run(IT &start, IT const end, CT const run_start, CT const run_end, FN &&apply) -> optional<decltype(apply(start, end))>
{
  IT const origin { start };

  if (!accept_run(start, end, run_start, run_end)) {
    return none;
  }

  return some(apply(origin, IT { start }));
}



/*==============================================================================
  accept_run(start, end, vals, apply(from, to)) -> optional<T>

    Wrapper for accept_run(start, end, std::begin(vals), std::end(vals), apply).
==============================================================================*/
template <typename IT, typename Vals, typename FN>
auto accept_run(IT &start, IT const end, Vals const &vals, FN &&apply) -> optional<decltype(apply(start, end))>
{
  IT const origin { start };

  if (!accept_run(start, end, std::begin(vals), std::end(vals))) {
    return none;
  }

  return some(apply(origin, IT { start }));
}


/*==============================================================================
  accept_one(start, end, run_start, run_end, apply(from, to)) -> optional<T>

    Wrapper for accept_run(start, end, run_start, run_end, 1, apply).
==============================================================================*/
template <typename IT, typename CT, typename FN>
auto accept_one(IT &start, IT const end, CT const run_start, CT const run_end, FN &&apply) -> optional<decltype(apply(start, end))>
{
  IT const origin { start };

  if (!accept_one(start, end, run_start, run_end)) {
    return none;
  }

  return some(apply(origin, IT { start }));
}


/*==============================================================================
  accept_one(start, end, vals, apply(from, to)) -> optional<T>

    Wrapper for
    accept_run(start, end, std::begin(vals), std::end(vals), 1, apply).
==============================================================================*/
template <typename IT, typename Vals, typename FN>
auto accept_one(IT &start, IT const end, Vals const &vals, FN &&apply) -> optional<decltype(apply(start, end))>
{
  IT const origin { start };

  if (!accept_one(start, end, std::begin(vals), std::end(vals))) {
    return none;
  }

  return some(apply(origin, IT { start }));
}


/*==============================================================================
  accept_seq(start, end, seq_start, seq_end) -> int

    Similar to accept_run, except only accepts values from the starting
    iterator if they are the same values in order as between seq_start and
    seq_end. Returns the number of times the iterator was advanced (typically
    the same number as would be returned by std::distance(seq_start, seq_end))
    if it matches the entire sequence, otherwise returns 0.
==============================================================================*/
template <typename IT, typename CT>
int accept_seq(IT &start, IT const end, CT seq_start, CT const end_seq)
{
  using Val = typename std::decay<decltype(*start)>::type;

  IT pointer { start };
  int const size = accept_while(pointer, end, [&seq_start, end_seq](IT const &i, IT const &e) {
    return seq_start != end_seq && *i == *(seq_start++);
  });

  if (!size || seq_start != end_seq) {
    return 0;
  }

  start = pointer;
  return size;
}


/*==============================================================================
  accept_seq(start, end, seq) -> int

    Wrapper for accept_seq(start, end, std::begin(seq), std::end(seq)).
==============================================================================*/
template <typename IT, typename Seq>
int accept_seq(IT &start, IT const end, Seq const &seq)
{
  return accept_seq(start, end, std::begin(seq), std::end(seq));
}


/*==============================================================================
  accept_seq(start, end, seq_start, seq_end, apply(from, to)) -> optional<T>

    Same as accept_seq, except takes an additional apply function that receives
    the start and end iterators for the sequence in the source iterator's
    sequence. Returns a defined optional<T> if the sequence is accepted,
    otherwise returns none.
==============================================================================*/
template <typename IT, typename CT, typename FN>
auto accept_seq(IT &start, IT const end, CT seq_start, CT const end_seq, FN &&apply) -> optional<decltype(apply(start, end))>
{
  using Val = typename std::decay<decltype(*start)>::type;

  IT const origin { start };

  if (!accept_seq(start, end, seq_start, end_seq)) {
    return none;
  }

  return some(apply(origin, start));
}


/*==============================================================================
  accept_seq(start, end, seq, apply) -> optional<T>

    Wrapper for accept_seq(start, end, std::begin(seq), std::end(seq), apply).
==============================================================================*/
template <typename IT, typename Seq, typename FN>
auto accept_seq(IT &start, IT const end, Seq const &seq, FN &&apply) -> optional<decltype(apply(start, end))>
{
  return accept_seq(start, end, std::begin(seq), std::end(seq), std::forward<FN &&>(apply));
}


/*==============================================================================
  accept_if(start, end, pred(..), apply(from, to)) -> optional<T>

    Like all other apply-taking accept functions, except for accept_if.
==============================================================================*/
template <bool Result = true, typename IT, typename Pred, typename FN>
auto accept_if(IT &start, IT const end, Pred &&pred, FN &&apply) -> optional<decltype(apply(start, end))>
{
  IT const origin { start };

  if (!accept_if<Result>(start, end, std::forward<Pred &&>(pred))) {
    return none;
  }

  return some(apply(origin, IT { start }));
}


/*==============================================================================
  accept_if(start, end, pred(..), apply(from, to)) -> optional<T>

    Like all other apply-taking accept functions, except for accept_while.
==============================================================================*/
template <bool Result = true, typename IT, typename Pred, typename FN>
auto accept_while(IT &start, IT const end, Pred &&pred, FN &&apply) -> optional<decltype(apply(start, end))>
{
  IT const origin { start };

  if (!accept_while<Result>(start, end, std::forward<Pred &&>(pred))) {
    return none;
  }

  return some(apply(origin, IT { start }));
}


/*==============================================================================
  accept(start, end, item) -> bool

    Advances the start iterator if the start iterator points to a value equal
    to item. Returns true if the iterator was advanced.
==============================================================================*/
template <typename IT, typename T>
bool accept(IT &start, IT const end, T const &item)
{
  using Val = typename std::decay<decltype(*start)>::type;
  return accept_if(start, end, [&item](Val const &e) { return e == item; });
}


/*==============================================================================
  accept_if(start, end, item, apply(from, to)) -> optional<T>

    Same as accept_if, except the accepted range is provided to the apply
    function if successful. The result of the apply function is returned as an
    optional<T>. If the accept is not successful, returns none.
==============================================================================*/
template <typename IT, typename T, typename FN>
auto accept(IT &start, IT const end, T const &item, FN &&apply) -> optional<decltype(apply(start, end))>
{
  IT const origin { start };

  if (!accept(start, end, item)) {
    return none;
  }

  return some(apply(origin, IT { start }));
}


} // namespace snow


#endif /* end __SNOW_COMMON__ACCEPT_HH__ include guard */
