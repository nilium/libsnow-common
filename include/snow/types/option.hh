/*
 * Copyright Noel Cower 2013 - 2014.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef __SNOW_COMMON__OPTION_HH__
#define __SNOW_COMMON__OPTION_HH__

#include <snow/config.hh>
#include <type_traits>
#include <ostream>


namespace snow {

/*==============================================================================

  Classes for a basic Scala-like option. All options have one of two states:

  - defined
  - empty

  When an option is defined, it has a value that can be acquired either by
  dereferencing the option or calling its get() function. If an option is empty,
  then trying do either will raise an exception.

  A simple example of an option is as follows:

    option_t<int> maybe_int = none; // Declare an empty int option
    maybe_int = some(24); // Define its value to be 24
    maybe_int = none; // and make it empty again

    auto easier = some(24); // this will also work.

  If using it to hold a pointer, keep in mind that options do not manage any
  memory other than their own -- if its value is allocated memory, you will
  have to free it yourself, as usual. In that case, you probably want to use a
  shared_ptr.

  That said, if you do declare a pointer option and assign nullptr to it, take
  care when using auto to provide the option's type by doing either of the
  following:

    auto pointer_one = some<int *>(nullptr);
    option_t<int *> pointer_two = some(nullptr);

  Both of these work, but using auto and attempting to assign just some(nullptr)
  will of course result in an option_t<std::nullptr_t>, which isn't very useful.

==============================================================================*/



struct none_t;


template <typename T>
class option_t;


/**
  Struct contianing type aliases for an option's value_type and self type.
*/
template <typename T>
struct optional_bits
{
  /// An option of type T's value_type. An alias for the decayed form of T.
  using value_type = typename std::decay<T>::type;
  /// An option of type T's self type, otherwise an option of type
  /// option_t<value_type>.
  using type = option_t<value_type>;
};


/**
  Type alias using the decayed form of T for an option. Ensures that you don't
  try to create an option of a reference, volatile, etc. type.
*/
template <typename T>
using optional = typename optional_bits<T>::type;


template <typename T>
optional<T> some(T &&val);

template <typename T, typename... ARGS>
optional<T> make_optional(ARGS&&... args);



/**
  @brief Optional value type. Holds a possibly-defined value.

  Options may be either defined (has some value) or undefined, in which case
  it has no value. All options hold storage for their own value.
*/
template <typename T>
class option_t final
{
public:
  // Occasionally useful if you need the value type of an option, but this is
  // mainly here so flat_map can infer the value type of its returned option_t.
  // Recommended that you use the optional<T> type alias to avoid issues with
  // incompatible types despite using the same value_type.

  /// @brief The bits structure used to determine this option's value_type.
  using bits = optional_bits<T>;
  /// @brief The value_type of the option, obtained from bits::value_type.
  using value_type = typename bits::value_type;

private:
  /// Bit of a hack to keep the private emplace-like constructor. By passing a
  /// ctor_bit_t as the constructor's first argument, this basically ensures it
  /// knows which ctor to use.
  enum ctor_bit_t
  {
    /// CTOR enum argument used to disambiguate which constructor to use when
    /// constructing optional values in place.
    CTOR
  };

  enum store_bit_t
  {
    SIZE = sizeof(value_type),
    ALIGNMENT = std::alignment_of<value_type>::value
  };

  using store_t = typename std::aligned_storage<SIZE, ALIGNMENT>::type;

  /// Whether the option has a defined value (true if so, otherwise false).
  bool _defined;
  /// Storage for any value to be held by the option. Is aligned according to
  /// the value provided by std::alignment_of.
  store_t _data;

  /// Returns a value_type pointer to the data used by the option. May have
  /// an undefined value, so dereferencing it isn't guaranteed to be safe
  /// unless _defined is true.
  value_type *typed()
  {
    return reinterpret_cast<value_type *>(&_data);
  }

  /// Returns a value_type pointer to the data used by the option. May have
  /// an undefined value, so dereferencing it isn't guaranteed to be safe
  /// unless _defined is true.
  value_type const *typed() const
  {
    return reinterpret_cast<value_type const *>(&_data);
  }

  /// Throws an std::runtime_error if the option is undefined.
  void throw_if_undefined() const
  {
    if (!_defined) {
      s_throw(std::runtime_error, "Attempt to access empty option");
    }
  }

  /// Destroys any value held by the option and makes it undefined.
  void dispose();

  friend class option_t<std::nullptr_t>;

  /**
    In-place constructor for options with defined values. Must pass CTOR
    enum value as the first argument in order for the compiler to properly
    disambiguate the constructor.
  */
  template <typename... ARGS>
  option_t(ctor_bit_t, ARGS&&... args);

public:

  /**
    @brief Constructs a new instance of an option with a defined value in-place.

    Returns an option with a defined value constructed in-place rather than by
    copying or moving an existing defined (or undefined) option). Passes any
    arguments received to one of the optional value type's constructors,
    assuming such a constructor exists.

    The reason for make vs. just providing a forwarding ctor is that the
    forwarding ctor could be option_t's ctor, in which case multiple
    definitions of an option_t ctor are created. So, this keeps things
    sort of normal.
  */
  template <typename... ARGS>
  static option_t make(ARGS&&... args);

  /**
    @brief Default constructor.

    Constructs an empty optional.
  */
  option_t() noexcept;
  ~option_t();

  /// @brief Constructs an empty optional by copying nothing.
  option_t(none_t const &) noexcept; // same as default ctor
  /// @brief Copy constructor.
  option_t(option_t const &copied);
  /// @brief Move constructor.
  option_t(option_t &&moved);

  /// @brief None-assignment operator. Assigns nothing, making the option
  /// undefined.
  option_t &operator = (none_t const &);
  /// @brief Copy-assignment operator.
  option_t &operator = (option_t const &);
  /// @brief Move-assignment operator.
  option_t &operator = (option_t &&);

  /// @brief None-assignment. Assigns nothing, making the option undefined.
  option_t &assign(none_t const &);
  /// @brief Copy-assignment.
  option_t &assign(option_t const &);
  /// @brief Move-assignment.
  option_t &assign(option_t &&);

  /// @brief Returns whether the option is defined.
  bool is_defined() const noexcept { return _defined; }
  /// @brief Returns whether the option is empty (undefined). Opposite of
  /// is_defined.
  bool is_empty() const noexcept { return !is_defined(); }

  /// @brief Gets the value of the option if defined, otherwise throws an
  /// std::runtime_error exception.
  value_type &get()
  {
    throw_if_undefined();
    return *typed();
  }

  /// @brief Gets the value of the option if defined, otherwise throws an
  /// std::runtime_error exception.
  value_type const &get() const
  {
    throw_if_undefined();
    return *typed();
  }

  /// @brief Dereference operator. Gets the value of the option if defined,
  /// otherwise throws an std::runtime_error exception.
  value_type &operator * () { return get(); }
  /// @brief Dereference operator. Gets the value of the option if defined,
  /// otherwise throws an std::runtime_error exception.
  value_type const &operator * () const { return get(); }

  /// @brief Dereference operator -- returns a pointer to the value of the
  /// option.
  value_type *operator -> () const { return get(); }

  /// @brief Implicit bool conversion -- returns whether the option is defined.
  operator bool () const { return is_defined(); }
  /// @brief Unary logical not operator -- returns whether the option is empty.
  bool operator ! () const { return is_empty(); }

  /// @brief Returns an empty (default constructed) option_t.
  static auto empty() noexcept -> option_t;


  /**
   @brief Apply function -- calls function f, passing the value of the option
   to f, if and only if the option has a defined value. Receives a const
   reference to or copy of the option's value.

   Given an application function that takes a reference of the type held by
   the underlying option, the call operator will only call the given function
   if the value of the option is defined. If empty, it returns early.
  */
  template <typename F>
  void operator () (F &&f) const
  {
    if (is_defined()) {
      f(get());
    }
  }


  /**
   @brief Apply function -- calls function f, passing the value of the option
   to f, if and only if the option has a defined value. Receives a reference to
   or copy of the option's value.

   Given an application function that takes a reference of the type held by
   the underlying option, the call operator will only call the given function
   if the value of the option is defined. If empty, it returns early.
  */
  template <typename F>
  void operator () (F &&f)
  {
    if (is_defined()) {
      f(get());
    }
  }


  /**
    @brief Maps the value of this option to another value, returning a new
    option with the result of function f.

    This is a simple map function -- given a function that takes T and returns
    type U (may be anything), it returns an optional<U> with a defined value
    if the option is defined, otherwise returns an empty option. Contrast this
    with flat_map's behavior, which is the same, but uses a function that
    returns an optional<U> instead of just U. If your map function may map
    from a defined value to an empty value, then use flat_map. If your map
    function always has a defined value, use map.
  */
  template <typename F>
  auto map(F &&f) const -> optional<decltype(f(get()))>;


  /**
    @brief Maps the value of this option to another value, returning the result
    of function f. Function f must return an optional type.

    Similar to map, but the map function must return an optional<U> instead of
    just a U value. This allows it to map any defined value to an empty value.
    If your function always returns a defined value, use map.
  */
  template <typename F>
  auto flat_map(F &&f) const -> optional<typename decltype(f(get()))::value_type>;
};



template <typename T>
template <typename... ARGS>
auto option_t<T>::make(ARGS&&... args) -> option_t
{
  return option_t(CTOR, std::forward<ARGS>(args)...);
}



template <typename T>
auto option_t<T>::empty() noexcept -> option_t
{
  return option_t();
}


template <typename T>
void option_t<T>::dispose()
{
  if (_defined) {
    typed()->~T();
  }
  _defined = false;
}


template <typename T>
template <typename... ARGS>
option_t<T>::option_t(ctor_bit_t, ARGS&&... args)
: _defined(true)
{
  new(typed()) T(std::forward<ARGS>(args)...);
}


template <typename T>
option_t<T>::option_t() noexcept
: _defined(false)
{
  /* nop */
}


template <typename T>
option_t<T>::option_t(none_t const &n) noexcept
: _defined(false)
{
  /* nop */
}


template <typename T>
option_t<T>::~option_t()
{
  dispose();
}


template <typename T>
option_t<T>::option_t(option_t const &copied)
: _defined(false)
{
  assign(copied);
}


template <typename T>
option_t<T>::option_t(option_t &&moved)
: _defined(false)
{
  assign(moved);
}


template <typename T>
auto option_t<T>::operator = (none_t const &n) -> option_t &
{
  dispose();
  return *this;
}


template <typename T>
auto option_t<T>::operator = (option_t const &copied) -> option_t &
{
  assign(copied);
  return *this;
}


template <typename T>
auto option_t<T>::operator = (option_t &&moved) -> option_t &
{
  assign(std::forward<option_t>(moved));
  return *this;
}


template <typename T>
auto option_t<T>::assign(none_t const &n) -> option_t &
{
  dispose();
  return *this;
}


template <typename T>
auto option_t<T>::assign(option_t const &other) -> option_t &
{
  dispose();

  if (other.is_defined()) {
    new(typed()) T(other.get());
    _defined = true;
  }


  return *this;
}


template <typename T>
auto option_t<T>::assign(option_t &&other) -> option_t &
{
  dispose();

  if (other.is_defined()) {
    new(typed()) T(std::move(other.get()));
    _defined = true;
    other.dispose();
  }

  return *this;
}


template <typename T>
template <typename F>
auto option_t<T>::map(F &&f) const -> optional<decltype(f(get()))>
{
  using U = decltype(f(get()));
  if (is_defined()) {
    return some<U>(f(get()));
  } else {
    return optional<U>();
  }
}



template <typename T>
template <typename F>
auto option_t<T>::flat_map(F &&f) const -> optional<typename decltype(f(get()))::value_type>
{
  using U = typename decltype(f(get()))::value_type;
  if (is_defined()) {
    return f(get());
  } else {
    return optional<U>();
  }
}



template<typename T, typename U>
bool operator == (option_t<T> const &lhs, option_t<U> const &rhs)
{
  return (lhs.is_defined() && rhs.is_defined()) && (lhs.get() == rhs.get());
}



template<typename T, typename U>
bool operator != (option_t<T> const &lhs, option_t<U> const &rhs)
{
  return !(lhs == rhs);
}



template<typename T>
bool operator == (option_t<T> const &lhs, none_t const &rhs) noexcept
{
  return lhs.is_empty();
}



template<typename T>
bool operator == (none_t const &lhs, option_t<T> const &rhs) noexcept
{
  return rhs.is_empty();
}



template<typename T>
bool operator != (option_t<T> const &lhs, none_t const &rhs) noexcept
{
  return lhs.is_defined();
}



template<typename T>
bool operator != (none_t const &lhs, option_t<T> const &rhs) noexcept
{
  return rhs.is_defined();
}



constexpr bool operator == (none_t const &lhs, none_t const &rhs) noexcept
{
  return true;
}



constexpr bool operator != (none_t const &lhs, none_t const &rhs) noexcept
{
  return false;
}



template<typename T>
std::ostream &operator << (std::ostream &out, option_t<T> const &opt)
{
  if (opt.is_defined()) {
    return out << "some(" << opt.get() << ')';
  } else {
    return out << "none";
  }
}



/**
  @brief Class representing an empty type -- i.e., a unit value, or nothing.
  May be implicitly cast to an empty optional<T> of any kind.

  none_t is an empty type that has no value and is mostly only ever used when
  assigning the constexpr value none to options. It really has no purpose
  outside of that use.
*/
struct none_t final
{
  struct value_type final {};

  constexpr none_t() { /* nop */ }
  constexpr bool is_defined() const { return false; }
  constexpr bool is_empty() const { return true; }

  constexpr operator bool () const { return false; }
  constexpr bool operator ! () const { return true; }

  template <typename T>
  operator option_t<T> () const
  {
    return option_t<T>();
  }
};



/// @brief Global constant none -- may be used to assign no value to an optional
/// or otherwise passed around to indicate no value, since it can be implicitly
/// converted to an optional of any type.
constexpr none_t none {};



inline std::ostream &operator << (std::ostream &out, none_t const &non)
{
  return out << "none";
}



inline std::ostream &operator << (std::ostream &out, none_t::value_type const &)
{
  return out << "()";
}



/**
  @brief nullptr_t specialization of option_t.

  Generally a bad idea to use this, but the specialization's needed to avoid
  attempts to do horrible things like `new std::nullptr_t` which as we all know
  is bad. Implements a compatible subset of option_t's interface, though less,
  as this shouldn't be used except to do things like assign some(nullptr) to
  an option_t<T != nullptr_t>.
*/
template <>
class option_t<std::nullptr_t> final
{
  bool _defined;

  enum ctor_bit_t { CTOR };

  option_t(ctor_bit_t, std::nullptr_t) : _defined(true) { /* nop */ }

public:
  using value_type = std::nullptr_t;

  static option_t empty() noexcept { return option_t(); }

  bool is_defined() const noexcept { return _defined; }
  bool is_empty() const noexcept { return !is_defined(); }



  /**
    @brief Default constructor.

    Constructs an empty optional of nullptr_t.
  */
  option_t() noexcept : _defined(false) { /* nop */ }



  /// @brief Copy constructor.
  option_t(option_t const &opt) noexcept
  : _defined(opt._defined) { /* nop */ }



  /// @brief Move constructor.
  option_t(option_t &&opt) noexcept
  : _defined(opt._defined)
  {
    opt._defined = false;
  }



  /// @brief Copy-assignment operator.
  option_t &operator = (option_t const &opt) noexcept
  {
    return assign(opt);
  }



  /// @brief Move-assignment operator.
  option_t &operator = (option_t &&opt) noexcept
  {
    return assign(std::forward<option_t>(opt));
  }



  /// @brief Assign nothing to the option.
  option_t &operator = (none_t const &non) noexcept
  {
    return assign(non);
  }



  /// @brief Assign nothing to the option.
  option_t &assign(none_t const &non) noexcept
  {
    _defined = false;
    return *this;
  }



  /// @brief Copy assignment.
  option_t &assign(option_t const &opt) noexcept
  {
    _defined = opt._defined;
    return *this;
  }



  /// @brief Move assignment.
  option_t &assign(option_t &&opt) noexcept
  {
    _defined = opt._defined;
    opt._defined = false;
    return *this;
  }



  /// @brief Gets the value of the option if defined, otherwise throws an
  /// std::runtime_error exception.
  std::nullptr_t get() const
  {
    if (is_empty()) {
      s_throw(std::runtime_error, "Attempt to access empty option");
    }
    return nullptr;
  }



  /// @brief Gets the value of the option if defined, otherwise throws an
  /// std::runtime_error exception.
  std::nullptr_t operator * () const
  {
    if (is_empty()) {
      s_throw(std::runtime_error, "Attempt to access empty option");
    }
    return nullptr;
  }



  /**
    @brief Recast option and its value ti an option of type U.

    This attempts to construct a new object of type U by passing a const
    reference or the value of this option to the new option's constructor.
    The new option's value is constructed in place.
  */
  template <typename U>
  operator option_t<U> () const
  {
    if (is_defined()) {
      return option_t<U>(option_t<U>::CTOR, U { **this });
    } else {
      return option_t<U>();
    }
  }



  /**
    @brief Apply the function f to the value of the option if and only if the
    option has a defined value.
  */
  template <typename F>
  void operator () (F &&f) const
  {
    if (is_defined()) {
      f(nullptr);
    }
  }



  /**
    @brief Apply the function f to the value of the option if and only if the
    option has a defined value and return a new option with the result of the
    function.

    The function f must return a value of type U (whatever type the function
    returns), which will be stored as the value of an optional<U>. The function
    must return something. If you need to be able to return nothing for
    possibly unwanted values, use flat_map to return either some(U) or none.
  */
  template <typename F>
  auto map(F &&f) const -> optional<decltype(f(nullptr))>;

  /**
    @brief Apply the function f to the value of the option if and only if the
    option has a defined value and return a new option with the result of the
    function.

    Similar to map, except that function f must return an optional<U> instead
    of just a U. As such, the function may choose to return either nothing or
    something.
  */
  template <typename F>
  auto flat_map(F &&f) const -> optional<typename decltype(f(nullptr))::value_type>;
};



template <typename F>
auto option_t<std::nullptr_t>::map(F &&f) const -> optional<decltype(f(nullptr))>
{
  using U = decltype(f(nullptr));
  if (is_defined()) {
    return some<U>(f(nullptr));
  } else {
    return optional<U>();
  }
}



template <typename F>
auto option_t<std::nullptr_t>::flat_map(F &&f) const -> optional<typename decltype(f(nullptr))::value_type>
{
  using U = typename decltype(f(nullptr))::value_type;
  if (is_defined()) {
    return f(nullptr);
  } else {
    return optional<U>();
  }
}



/**
  @brief Creates a new option_t<T> and returns it, either copying or moving the
  value provided.
*/
template <typename T>
optional<T> some(T &&val)
{
  return make_optional<T>(std::forward<T>(val));
}


/**
  @brief Creates a new optional and constructs its value in-place using the
  given arguments, forwarded to T's cnstructor. Just an alias around
  optional<T>::make(...).
*/
template <typename T, typename... ARGS>
optional<T> make_optional(ARGS&&... args)
{
  return optional<T>::make(std::forward<ARGS>(args)...);
}


} // namespace snow


#endif /* end __SNOW_COMMON__OPTION_HH__ include guard */
