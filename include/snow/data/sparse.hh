// sparse.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__SPARSE_HH__
#define __SNOW_COMMON__SPARSE_HH__

#include <snow/config.hh>
#include <functional>
#include <iostream>
#include <stack>
#include <stdexcept>

namespace snow {
namespace sparse {

enum source_kind_t : int
{
  SP_ERROR      = -1,
  SP_OPEN_NODE  =  0,
  SP_CLOSE_NODE =  1,
  SP_NAME       =  2,
  SP_VALUE      =  3,
  SP_DONE       =  4
};

enum option_flags_t : int
{
  SP_CONSUME_WHITESPACE   = 0x1 << 0,
  SP_TRIM_TRAILING_SPACES = 0x1 << 1,
  SP_NAMELESS_ROOT_NODES  = 0x1 << 2,
  SP_NAMELESS_NODES       = 0x1 << 3,
  SP_DEFAULT_OPTIONS      = (SP_TRIM_TRAILING_SPACES |
                             SP_NAMELESS_NODES |
                             SP_CONSUME_WHITESPACE)
};

struct S_EXPORT position_t {
  size_t line;
  size_t column;

  operator string() const;
};

S_EXPORT std::ostream &operator << (std::ostream&, const position_t&);

typedef std::function<void(source_kind_t, const string&, position_t)> parse_func_t;

struct S_EXPORT parser_t
{
  parser_t(int options, parse_func_t callback);
  parser_t(const parser_t &other); // Copies parser state, callback, and options
  virtual ~parser_t();

  virtual void add_source(const string &source);
  // Construction opens the parser. To finish parsing, close() must be called.
  virtual void close();

  inline virtual bool have_error() const { return !state_.error.empty(); }
  inline virtual const string &error() const { return state_.error; }

  inline virtual bool is_open() const { return !state_.closed; }

private:
  typedef std::stack<position_t> position_stack_t;

  enum parse_mode_t : int
  {
    FIND_NAME    = 0x1 << 0,
    FIND_VALUE   = 0x1 << 1,
    READ_NAME    = 0x1 << 2,
    READ_VALUE   = 0x1 << 3,
    READ_COMMENT = 0x1 << 4,
  };

  struct S_HIDDEN options_t {
    bool consume_ws;
    bool trim_spaces;
    bool nameless_roots;
    bool nameless_nodes;
  };

  struct S_EXPORT state_t {

    bool closed;
    position_t pos;
    position_t start;

    size_t space_count;

    int mode;
    bool escaped;
    char last_char;

    parse_func_t func;

    string buffer;
    string error;

    position_stack_t openings;

    // Note: source may be a reference to state_.buffer.
    S_HIDDEN void send_buffer_and_reset(source_kind_t kind, const options_t &options);
    S_HIDDEN void send_string(source_kind_t kind, const string &source);
    S_HIDDEN void buffer_char(char c, const options_t &options);
    S_EXPORT void close_with_error(const string &error);
    // Copies the buffer after resizing it
    S_HIDDEN const string &trimmed_buffer(const options_t &options);
  };

  options_t options_;
  state_t state_;

  static const state_t DEFAULT_STATE;
};

} // namespace sparse
} // namespace snow

#endif /* end __SNOW_COMMON__SPARSE_HH__ include guard */
