// sparse.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__SPARSE_HH__
#define __SNOW_COMMON__SPARSE_HH__

#include <snow/config.hh>

#include <functional>
#include <iostream>
#include <stack>
#include <stdexcept>


namespace snow {


/** @addtogroup Sparse Sparse
  @{
*/


/** The Sparse parser namespace. */
namespace sparse {


/** Constants for node elements provided to a parse_func_t. */
enum source_kind_t : int
{
  /** An error occurred reading the source. */
  SP_ERROR      = -1,
  /** The parser encountered a node opening. */
  SP_OPEN_NODE  =  0,
  /** The parser encountered a node closing. */
  SP_CLOSE_NODE =  1,
  /** The parser encountered a name. */
  SP_NAME       =  2,
  /** The parser encountered a value. Values necessarily follow names. */
  SP_VALUE      =  3,
  /** The parser finished. */
  SP_DONE       =  4
};


/** Flags to indicate to a parser_t how to parse a document. */
enum option_flags_t : int
{
  /**
    Flag to tell the parser to consume excess whitespace in values. More than
    one space in a row will be truncated to a single space.
  */
  SP_CONSUME_WHITESPACE   = 0x1 << 0,
  /**
    Tells the parser to trim trailing spaces from values.
  */
  SP_TRIM_TRAILING_SPACES = 0x1 << 1,
  /**
    Allows root nodes to be nameless.
    @see SP_NAMELESS_NODES for more information.
  */
  SP_NAMELESS_ROOT_NODES  = 0x1 << 2,
  /**
    Allows nameless nodes anywhere in a Sparse document.

    @note If this flag is set, before a nameless node is parsed, the parser
    callback will receive a message for a name with an empty string.

    This flag implies SP_NAMELESS_ROOT_NODES.
  */
  SP_NAMELESS_NODES       = 0x1 << 3 | SP_NAMELESS_ROOT_NODES,
  /** Default parser options. Essentially an or-ing of all flags. */
  SP_DEFAULT_OPTIONS      = (SP_TRIM_TRAILING_SPACES |
                             SP_NAMELESS_NODES |
                             SP_CONSUME_WHITESPACE)
};


/**
  A position in a Sparse document.
*/
struct S_EXPORT position_t {
  /** The line in the document. */
  size_t line;
  /** The column in the document. */
  size_t column;

  operator string() const;
};


S_EXPORT std::ostream &operator << (std::ostream&, const position_t&);


/**
  The parser function provided to a parser_t.
  @param kind The kind of element the parser encountered.
  @param str  The string for the encountered element. May be empty in the case
              of unnamed nodes.
  @param pos  Where the element was encountered.
*/
using parse_func_t = std::function<void(source_kind_t kind, const string &str, position_t pos)>;


/** The Sparse parser class. */
struct S_EXPORT parser_t
{
  /** Constructor. Takes parsing flags and a callback function. */
  parser_t(int options, parse_func_t callback);
  /** Copy constructor. */
  parser_t(const parser_t &other); // Copies parser state, callback, and options
  virtual ~parser_t();

  /**
    Function to add source to the parser. The source will be parsed as it is
    added.

    @param source The source to parse.
  */
  virtual void add_source(const string &source);
  /**
    Closes the parser, signalling that it end close any in-progress elements
    and send the SP_DONE message to the callback.

    If close() is not called, some data may not be fully parsed.
  */
  virtual void close();

  /** Returns whether the parser encountered an error. */
  inline virtual bool have_error() const { return !state_.error.empty(); }
  /** Returns the error string for the parser. */
  inline virtual const string &error() const { return state_.error; }
  /**
    Returns whether the parser is still open. If true, add_source() may still
    be called.
  */
  inline virtual bool is_open() const { return !state_.closed; }


private:
  using position_stack_t = std::stack<position_t>;


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


/** @} */


} // namespace sparse


} // namespace snow



#endif /* end __SNOW_COMMON__SPARSE_HH__ include guard */
