// option.hh -- Noel Cower -- Public Domain

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


template <typename T>
option_t<T> some(const T &val);

template <typename T>
option_t<T> some(T &&val);



/*==============================================================================

  Option -- may be either defined (has some value) or undefined, in which case
  it has no value.

==============================================================================*/
template <typename T>
class option_t final
{

  // Bit of a hack to keep the private emplace-like constructor. By passing a
  // ctor_bit_t as the constructor's first argument, this basically ensures it
  // knows which ctor to use.
  enum ctor_bit_t { CTOR };

  enum store_bit_t {
    SIZE = sizeof(T),
    ALIGNMENT = std::alignment_of<T>::value
  };

  using store_t = typename std::aligned_storage<SIZE, ALIGNMENT>::type;

  store_t _data;
  bool _defined;

  T *typed()
  {
    return (T *)&_data;
  }

  const T *typed() const
  {
    return (const T *)&_data;
  }

  void dispose();

  friend option_t<T> some<T>(const T &);
  friend option_t<T> some<T>(T &&);
  friend class option_t<std::nullptr_t>;

  template <typename... ARGS>
  option_t(ctor_bit_t, ARGS&&... args);

public:
  // Occasionally useful if you need the value type of an option, but this is
  // mainly here so flat_map can infer the value type of its returned option_t.
  using value_type = T;


  /*============================================================================
    make(args...)

      Returns an option with a defined value constructed in-place rather than by
      copying or moving an existing defined (or undefined) option). Passes any
      arguments received to one of the optional value type's constructors,
      assuming such a constructor exists.
  ============================================================================*/
  template <typename... ARGS>
  static option_t make(ARGS&&... args);

  option_t() noexcept;
  ~option_t();

  option_t(const none_t &) noexcept; // same as default ctor
  option_t(const option_t &copied);
  option_t(option_t &&moved);

  option_t &operator = (const none_t &);
  option_t &operator = (const option_t &);
  option_t &operator = (option_t &&);

  option_t &assign(const none_t &);
  option_t &assign(const option_t &);
  option_t &assign(option_t &&);

  bool is_defined() const noexcept { return _defined; }
  bool is_empty() const noexcept { return !is_defined(); }

  T &get()
  {
    if (is_empty()) {
      s_throw(std::runtime_error, "Attempt to access empty option");
    }
    return *typed();
  }

  const T &get() const
  {
    if (is_empty()) {
      s_throw(std::runtime_error, "Attempt to access empty option");
    }
    return *typed();
  }

  T &operator * () { return *typed(); }
  const T &operator * () const { return *typed(); }

  T *operator -> () const { return typed(); }

  static auto empty() noexcept -> option_t;


  /*============================================================================
    operator () (application_fn) -- call / application operator

      Given an application function that takes a reference of the type held by
      the underlying option, the call operator will only call the given function
      if the value of the option is defined. If empty, it returns early.
  ============================================================================*/
  // Apply functions
  template <typename F>
  void operator () (F &f) const
  {
    if (is_defined()) {
      f(get());
    }
  }


  template <typename F>
  void operator () (F &&f) const
  {
    if (is_defined()) {
      f(get());
    }
  }


  template <typename F>
  void operator () (F &f)
  {
    if (is_defined()) {
      f(get());
    }
  }


  template <typename F>
  void operator () (F &&f)
  {
    if (is_defined()) {
      f(get());
    }
  }


  /*============================================================================
    map

      This is a simple map function -- given a function that takes T and returns
      type U (may be anything), it returns an option_t<U> with a defined value
      if the option is defined, otherwise returns an empty option. Contrast this
      with flat_map's behavior, which is the same, but uses a function that
      returns an option_t<U> instead of just U. If your map function may map
      from a defined value to an empty value, then use flat_map. If your map
      function always has a defined value, use map.
  ============================================================================*/
  template <typename F>
  auto map(F &f) const -> option_t<decltype(f(get()))>;
  template <typename F>
  auto map(F &&f) const -> option_t<decltype(f(get()))>;


  /*============================================================================
    flat_map

      Similar to map, but the map function must return an option_t<U> instead of
      just a U value. This allows it to map any defined value to an empty value.
      If your function always returns a defined value, use map.
  ============================================================================*/
  // map T -> option_t<U>
  template <typename F>
  auto flat_map(F &f) const -> option_t<typename decltype(f(get()))::value_type>;
  template <typename F>
  auto flat_map(F &&f) const -> option_t<typename decltype(f(get()))::value_type>;
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
option_t<T>::option_t(const none_t &n) noexcept
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
option_t<T>::option_t(const option_t &copied)
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
auto option_t<T>::operator = (const none_t &n) -> option_t &
{
  dispose();
  return *this;
}


template <typename T>
auto option_t<T>::operator = (const option_t &copied) -> option_t &
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
auto option_t<T>::assign(const none_t &n) -> option_t &
{
  dispose();
  return *this;
}


template <typename T>
auto option_t<T>::assign(const option_t &other) -> option_t &
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
auto option_t<T>::map(F &f) const -> option_t<decltype(f(get()))>
{
  using U = decltype(f(get()));
  if (is_defined()) {
    return some<U>(f(get()));
  } else {
    return option_t<U>();
  }
}


template <typename T>
template <typename F>
auto option_t<T>::map(F &&f) const -> option_t<decltype(f(get()))>
{
  using U = decltype(f(get()));
  if (is_defined()) {
    return some<U>(f(get()));
  } else {
    return option_t<U>();
  }
}



template <typename T>
template <typename F>
auto option_t<T>::flat_map(F &f) const -> option_t<typename decltype(f(get()))::value_type>
{
  using U = typename decltype(f(get()))::value_type;
  if (is_defined()) {
    return f(get());
  } else {
    return option_t<U>();
  }
}



template <typename T>
template <typename F>
auto option_t<T>::flat_map(F &&f) const -> option_t<typename decltype(f(get()))::value_type>
{
  using U = typename decltype(f(get()))::value_type;
  if (is_defined()) {
    return f(get());
  } else {
    return option_t<U>();
  }
}



template<typename T, typename U>
bool operator == (const option_t<T> &lhs, const option_t<U> &rhs)
{
  return (lhs.is_defined() && rhs.is_defined()) && (lhs.get() == rhs.get());
}



template<typename T, typename U>
bool operator != (const option_t<T> &lhs, const option_t<U> &rhs)
{
  return !(lhs == rhs);
}



template<typename T>
bool operator == (const option_t<T> &lhs, const none_t &rhs) noexcept
{
  return lhs.is_empty();
}



template<typename T>
bool operator == (const none_t &lhs, const option_t<T> &rhs) noexcept
{
  return rhs.is_empty();
}



template<typename T>
bool operator != (const option_t<T> &lhs, const none_t &rhs) noexcept
{
  return lhs.is_defined();
}



template<typename T>
bool operator != (const none_t &lhs, const option_t<T> &rhs) noexcept
{
  return rhs.is_defined();
}



constexpr bool operator == (const none_t &lhs, const none_t &rhs) noexcept
{
  return true;
}



constexpr bool operator != (const none_t &lhs, const none_t &rhs) noexcept
{
  return false;
}



template<typename T>
std::ostream &operator << (std::ostream &out, const option_t<T> &opt)
{
  if (opt.is_defined()) {
    return out << "some(" << opt.get() << ')';
  } else {
    return out << "none";
  }
}



/*==============================================================================

  none_t is an empty type that has no value and is mostly only ever used when
  assigning the constexpr value none to options. It really has no purpose
  outside of that use.

==============================================================================*/
struct none_t final {
  struct value_type final {};

  constexpr none_t() {}
  constexpr bool is_defined() const { return false; }
  constexpr bool is_empty() const { return true; }

  template <typename T>
  operator option_t<T> () const {
    return option_t<T>();
  }
};



constexpr none_t none = none_t();



inline std::ostream &operator << (std::ostream &out, const none_t &non)
{
  return out << "none";
}



inline std::ostream &operator << (std::ostream &out, const none_t::value_type &) {
  return out << "()";
}



/*==============================================================================

  nullptr_t specialization of option_t -- generally a bad idea to use this, but
  the specialization's needed to avoid attempts to do horrible things like
  `new std::nullptr_t` which as we all know is bad. Implements a compatible
  subset of option_t's interface, though less, as this shouldn't be used except
  to do things like assign some(nullptr) to an option_t<T != nullptr_t>.

==============================================================================*/
template <>
class option_t<std::nullptr_t> final {
  bool _defined;

  enum ctor_bit_t { CTOR };

  friend option_t<std::nullptr_t> some<std::nullptr_t>(const std::nullptr_t &);
  friend option_t<std::nullptr_t> some<std::nullptr_t>(std::nullptr_t &&);

  option_t(ctor_bit_t, std::nullptr_t) : _defined(true) { /* nop */ }

public:
  using value_type = std::nullptr_t;

  static option_t empty() noexcept { return option_t(); }

  bool is_defined() const noexcept { return _defined; }
  bool is_empty() const noexcept { return !is_defined(); }



  option_t() noexcept {}

  option_t(const option_t &opt) noexcept
  : _defined(opt._defined) { /* nop */ }



  option_t(option_t &&opt) noexcept
  : _defined(opt._defined)
  {
    opt._defined = false;
  }



  option_t &operator = (const option_t &opt) noexcept
  {
    return assign(opt);
  }



  option_t &operator = (option_t &&opt) noexcept
  {
    return assign(std::forward<option_t>(opt));
  }



  option_t &operator = (const none_t &non) noexcept
  {
    return assign(non);
  }



  option_t &assign(const none_t &non) noexcept
  {
    _defined = false;
    return *this;
  }



  option_t &assign(const option_t &opt) noexcept
  {
    _defined = opt._defined;
    return *this;
  }



  option_t &assign(option_t &&opt) noexcept
  {
    _defined = opt._defined;
    opt._defined = false;
    return *this;
  }



  std::nullptr_t get() const {
    if (is_empty()) {
      s_throw(std::runtime_error, "Attempt to access empty option");
    }
    return nullptr;
  }



  std::nullptr_t operator * () const {
    if (is_empty()) {
      s_throw(std::runtime_error, "Attempt to access empty option");
    }
    return nullptr;
  }



  template <typename T>
  operator option_t<T> () const {
    if (is_defined()) {
      return option_t<T>(option_t<T>::CTOR, nullptr);
    } else {
      return option_t<T>();
    }
  }



  template <typename F>
  void operator () (F &f) const
  {
    if (is_defined()) {
      f(nullptr);
    }
  }



  template <typename F>
  void operator () (F &&f) const
  {
    if (is_defined()) {
      f(nullptr);
    }
  }



  template <typename U, typename F>
  auto map(F &f) const -> option_t<decltype(F(get()))>
  {
    if (is_defined()) {
      return some(f(nullptr));
    } else {
      return option_t<U>();
    }
  }



  template <typename F>
  auto map(F &f) const -> option_t<decltype(f(nullptr))>;

  template <typename F>
  auto map(F &&f) const -> option_t<decltype(f(nullptr))>;

  template <typename F>
  auto flat_map(F &f) const -> option_t<typename decltype(f(nullptr))::value_type>;

  template <typename F>
  auto flat_map(F &&f) const -> option_t<typename decltype(f(nullptr))::value_type>;
};



template <typename F>
auto option_t<std::nullptr_t>::map(F &f) const -> option_t<decltype(f(nullptr))>
{
  using U = decltype(f(nullptr));
  if (is_defined()) {
    return some<U>(f(nullptr));
  } else {
    return option_t<U>();
  }
}



template <typename F>
auto option_t<std::nullptr_t>::map(F &&f) const -> option_t<decltype(f(nullptr))>
{
  using U = decltype(f(nullptr));
  if (is_defined()) {
    return some<U>(f(nullptr));
  } else {
    return option_t<U>();
  }
}



template <typename F>
auto option_t<std::nullptr_t>::flat_map(F &f) const -> option_t<typename decltype(f(nullptr))::value_type>
{
  using U = typename decltype(f(nullptr))::value_type;
  if (is_defined()) {
    return f(nullptr);
  } else {
    return option_t<U>();
  }
}



template <typename F>
auto option_t<std::nullptr_t>::flat_map(F &&f) const -> option_t<typename decltype(f(nullptr))::value_type>
{
  using U = typename decltype(f(nullptr))::value_type;
  if (is_defined()) {
    return f(nullptr);
  } else {
    return option_t<U>();
  }
}



/*==============================================================================
  some(const T&) and some(T&&)

    Creates a new option_t<T> and returns it, either copying or moving the value
    provided.
==============================================================================*/
template <typename T>
option_t<T> some(const T &val)
{
  return option_t<T>(option_t<T>::CTOR, val);
}



template <typename T>
option_t<T> some(T &&val)
{
  return option_t<T>(option_t<T>::CTOR, std::forward<T>(val));
}


} // namespace snow


#endif /* end __SNOW_COMMON__OPTION_HH__ include guard */
