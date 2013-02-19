// sparse.cc -- Noel Cower -- Public Domain

#include "snow/data/sparse.hh"
#include <sstream>

BEGIN_SNOW_NS
BEGIN_SPARSE_NS

#define CHECK_FLAG(FLAGS, FLAG) (((FLAGS)&(FLAG))==(FLAG))

/// Static function declarations

// Gets the escaped form of a given character code
static inline char escaped_char(char ch);
// Used for basic error messages
static inline string error_with_position(position_t pos, const string &str);



/// Constants

const size_t SP_INIT_BUFFER_CAPACITY = 64;



/// Static function definitions

static inline string error_with_position(position_t pos, const string &str)
{
  std::stringstream stream;
  stream << pos << ' ' << str;
  return stream.str();
}

static inline char escaped_char(char ch) {
  switch (ch) {
  case 'n': case 'N': return '\n';
  case 'r': case 'R': return '\r';
  case 'a': case 'A': return '\a';
  case 'b': case 'B': return '\b';
  case 'f': case 'F': return '\f';
  case 't': case 'T': return '\t';
  case '0':           return '\0';
  default:            return ch;
  }
}



/// position_t

position_t::operator string() const
{
  std::stringstream stream;
  stream << *this;
  return stream.str();
}

std::ostream &operator << (std::ostream &out, const position_t &pos)
{
  return (out << '[' << pos.line << ':' << pos.column << ']');
}



/// parser_t

const parser_t::state_t parser_t::DEFAULT_STATE = {
  false,                       // closed
  { 1, 1 },                    // pos
  { 1, 1 },                    // start
  0,                           // space_count
  parser_t::FIND_NAME,         // mode
  false,                       // escaped
  ' ',                         // last_char
  string(),                    // buffer
  string(),                    // error
  parser_t::position_stack_t() // openings
};

parser_t::parser_t(int options, parse_func_t callback)
  : func_(callback),
    options_({
      // consume_ws
      CHECK_FLAG(options, SP_CONSUME_WHITESPACE),
      // trim_spaces
      CHECK_FLAG(options, SP_TRIM_TRAILING_SPACES),
      // nameless_roots
      (CHECK_FLAG(options, SP_NAMELESS_ROOT_NODES) ||
       // Nameless nodes necessitates support for nameless roots
       CHECK_FLAG(options, SP_NAMELESS_NODES)),
      // nameless_nodes
      CHECK_FLAG(options, SP_NAMELESS_NODES)
    }),
    state_(DEFAULT_STATE)
{
  if (!callback) {
    state_.closed = true;
    state_.error = "Invalid parser function";
  } else {
    state_.buffer.reserve(SP_INIT_BUFFER_CAPACITY);
  }
}

parser_t::parser_t(const parser_t &other)
  : func_(other.func_), options_(other.options_), state_(other.state_)
{}

parser_t::~parser_t()
{
}

void parser_t::add_source(const string &source)
{
  for (const char current : source) {
    if (state_.mode == READ_COMMENT) {
      // If in a comment, read until the end of the line. In all cases, if an
      // end of line occurs, the next mode will necessarily be FIND_NAME (this
      // is because it's impossible to end a line with a comment without
      // also ending a value or name if one was being read).
      if (current == '\n')
        state_.mode = FIND_NAME;
    } else if (state_.escaped) {
      // Handle escaped character
      buffer_char(escaped_char(current));
      state_.escaped = false;
    } else {
      // Generic parsing
      switch (current) {
      case ' ':  // Whitespace
      case '\t': // Whitespace
        if ((options_.consume_ws && state_.last_char == current) ||
            state_.mode == FIND_NAME || state_.mode == FIND_VALUE) {
          // NOP
        } else if (state_.mode == READ_NAME) {
          send_buffer_and_reset(SP_NAME);
          state_.mode = FIND_VALUE;
        } else {
          buffer_char(current);
        }
        break; // END ' ' & '\t'

      case '{': // Start of node
        switch (state_.mode) {
        case READ_NAME:
          send_buffer_and_reset(SP_NAME);
        case FIND_VALUE:
          state_.openings.push(state_.pos);
          send_string(SP_OPEN_NODE, "{");
          break;

        case READ_VALUE:
          send_buffer_and_reset(SP_VALUE);
        case FIND_NAME:
          if ((options_.nameless_roots && state_.openings.size() == 0) ||
              options_.nameless_nodes) {
            state_.openings.push(state_.pos);
            send_string(SP_NAME, "");
          } else {
            default:
            close_with_error(error_with_position(state_.pos,
                             "Invalid character '{' - expected name."));
            return;
          }
        }
        state_.mode = FIND_NAME;
        break; // END '{'

      case '}':  // Ened of node
      case '\n': // End-line terminator
      case ';':  // Inline terminator
      case '#':  // Comment
        switch (state_.mode) {
        case READ_NAME:  send_buffer_and_reset(SP_NAME);
        case FIND_VALUE: send_string(SP_VALUE, ""); break;
        case READ_VALUE: send_buffer_and_reset(SP_VALUE); break;
        default: break;
        }

        if (current == '}') {
          // End of node
          if (state_.openings.size() == 0) {
            close_with_error(error_with_position(state_.pos,
                             "Unexpected '}' - no matching '{'."));
            return;
          }
          state_.openings.pop();
          state_.mode = FIND_NAME;
          send_string(SP_CLOSE_NODE, "}");
        } // if (current == '}')

        state_.mode = FIND_NAME << (4 * (current == '#'));
        break;

      case '\\': // Escape
        state_.escaped = true;
        break;

      default:
        if (state_.mode < READ_NAME) { // if mode is find_name or find_value
          state_.mode <<= 2;           // shift it to read_name or read_value
          state_.start = state_.pos;   // and store the token's starting pos
        }

        buffer_char(current);
        break;
      }
    }

    if (current == '\n') {
      state_.pos.line += 1;
      state_.pos.column = 1;
    } else {
      state_.pos.column += 1;
    }

    state_.last_char = current;
  }
}

void parser_t::close()
{
  if (state_.closed)
    throw std::runtime_error("Attempt to close already-closed parser.");

  switch (state_.mode) {
  case READ_NAME:  send_buffer_and_reset(SP_NAME);
  case FIND_VALUE: send_string(SP_VALUE, ""); break;
  case READ_VALUE: send_buffer_and_reset(SP_VALUE); break;
  default: break;
  }

  if (state_.openings.size() > 0) {
    std::stringstream stream;
    stream << state_.pos
      << " Unexpected end of document - expected '}' to match '{' at "
      << state_.openings.top();
    close_with_error(stream.str());
    return;
  }

  state_.buffer.resize(0);
  state_.closed = true;

  send_string(SP_DONE, "");
}

const string &parser_t::trimmed_buffer()
{
  if (options_.trim_spaces && state_.space_count > 0) {
    std::clog << "Trimming by " << state_.space_count << std::endl;
    state_.buffer.resize(state_.buffer.length() - state_.space_count);
    state_.space_count = 0;
  }
  return state_.buffer;
}

END_SPARSE_NS
END_SNOW_NS
