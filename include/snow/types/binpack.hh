// binpack.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__BINPACK_HH__
#define __SNOW_COMMON__BINPACK_HH__

#include <snow/config.hh>
#include "types_2d.hh"

namespace snow {

class S_EXPORT binpack_t
{
private:
  binpack_t *pack_right_;
  binpack_t *pack_bottom_;

  recti_t frame_;
  bool loaded_;

  binpack_t(const recti_t &frame, binpack_t *right, binpack_t *bottom);

  virtual
  void reset_recursive();
  virtual
  void merge_empty_recursive();

public:
  // This is the constructor you want.
  binpack_t(const recti_t &frame);
  binpack_t(const binpack_t &other); // copy
  virtual
  ~binpack_t();

  virtual
  auto find_unused_bin(const dimensi_t &size) -> binpack_t*;
  // After calling unload(), one should discard any pointers to the binpack_t
  // it's called from.
  virtual
  void unload();

  // Reset to empty state
  virtual
  void reset();

  inline
  auto bottom() const -> const binpack_t*
  {
    return pack_bottom_;
  }

  inline
  auto right() const -> const binpack_t*
  {
    return pack_right_;
  }

  inline
  auto loaded() const -> bool
  {
    return loaded_;
  }

  inline
  auto origin() const -> pointi_t
  {
    return frame_.origin;
  }

  inline
  auto dimens() const -> dimensi_t
  {
    return frame_.size;
  }

  inline
  auto frame() const -> recti_t
  {
    return frame_;
  }

  inline
  auto x() const -> int
  {
    return frame_.origin.x;
  }

  inline
  auto y() const -> int
  {
    return frame_.origin.y;
  }

  inline
  auto width() const -> int
  {
    return frame_.size.width;
  }

  inline
  auto height() const -> int
  {
    return frame_.size.height;
  }

};

} // namespace snow

#endif /* end __SNOW_COMMON__BINPACK_HH__ include guard */
