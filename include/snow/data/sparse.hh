// sparse.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_SPARSE_HH__
#define __SNOW_SPARSE_HH__

#include <snow-common.hh>
#include <functional>
#include <iostream>
#include <stack>
#include <stdexcept>

#ifndef BEGIN_SPARSE_NS
#define BEGIN_SPARSE_NS namespace sparse {
#define END_SPARSE_NS   }
#endif

BEGIN_SNOW_NS
BEGIN_SPARSE_NS

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

struct position_t {
  size_t line;
  size_t column;

  operator string() const;
};

std::ostream &operator << (std::ostream&, const position_t&);

typedef std::function<void(source_kind_t, string, position_t)> parse_func_t;

struct parser_t
{
  parser_t(int options, parse_func_t callback);
  parser_t(const parser_t &other); // Copies parser state, callback, and options
  virtual ~parser_t();

  virtual void add_source(const string &source);
  // Construction opens the parser. To finish parsing, close() must be called.
  virtual void close();

  inline virtual bool have_error() const { return !state_.error.empty(); }
  inline virtual const string &error() { return state_.error; }

  inline virtual bool is_open() const
  {
    return !state_.closed;
  }

private:
  // Note: source may be a reference to state_.buffer.
  inline virtual void send_buffer_and_reset(source_kind_t kind)
  {
    // Unlike send_string, send the starting position for a token instead of the
    // current reader position
    if (func_) func_(kind, trimmed_buffer(), state_.start);
    state_.buffer.clear();
  }

  inline virtual void send_string(source_kind_t kind, const string &source)
  {
    if (func_) func_(kind, source, state_.pos);
  }

  inline virtual void buffer_char(char c)
  {
    if (c != ' ' || state_.escaped)
      state_.space_count = 0;
    else if ((c == ' ' || c == '\t') && options_.trim_spaces)
      state_.space_count += 1;

    state_.buffer.push_back(c);
  }

  inline virtual void close_with_error(const string &error)
  {
    if (state_.closed)
      throw std::runtime_error("Attempt to close with error when already closed.");
    send_string(SP_ERROR, error);
    state_.error = error;
    state_.closed = true;
  }

  // Copies the buffer after resizing it
  virtual const string &trimmed_buffer();

  typedef std::stack<position_t> position_stack_t;

  enum parse_mode_t : int
  {
    FIND_NAME    = 0x1 << 0,
    FIND_VALUE   = 0x1 << 1,
    READ_NAME    = 0x1 << 2,
    READ_VALUE   = 0x1 << 3,
    READ_COMMENT = 0x1 << 4,
  };

  struct options_t {
    bool consume_ws;
    bool trim_spaces;
    bool nameless_roots;
    bool nameless_nodes;
  };

  struct state_t {
    bool closed;
    position_t pos;
    position_t start;

    size_t space_count;

    int mode;
    bool escaped;
    char last_char;

    string buffer;
    string error;
    position_stack_t openings;
  };

  parse_func_t func_;
  options_t options_;
  state_t state_;

  static const state_t DEFAULT_STATE;
};

END_SPARSE_NS
END_SNOW_NS

#endif /* end __SNOW_SPARSE_HH__ include guard */
