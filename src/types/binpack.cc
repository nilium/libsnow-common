// binpack.cc -- Noel Cower -- Public Domain

#include <snow/types/binpack.hh>

namespace snow
{

S_EXPORT
binpack_t::binpack_t(const recti_t &frame, binpack_t *right, binpack_t *bottom) :
  pack_right_(right), pack_bottom_(bottom), frame_(frame), loaded_(false)
{}

S_EXPORT
binpack_t::binpack_t(const recti_t &frame) :
  pack_right_(nullptr), pack_bottom_(nullptr), frame_(frame), loaded_(false)
{}

S_EXPORT
binpack_t::binpack_t(const binpack_t &other) :
  pack_right_(nullptr), pack_bottom_(nullptr), frame_(other.frame()), loaded_(other.loaded())
{
  if (other.pack_right_)
    pack_right_ = new binpack_t(*other.pack_right_);

  if (other.pack_bottom_)
    pack_bottom_ = new binpack_t(*other.pack_bottom_);
}

S_EXPORT
binpack_t::~binpack_t()
{
  if (pack_right_) delete pack_right_;
  if (pack_bottom_) delete pack_bottom_;
}

S_EXPORT
auto binpack_t::find_unused_bin(const dimensi_t &size) -> binpack_t*
{
  const int f_width = frame_.size.width;
  const int f_height = frame_.size.height;

  if (loaded_ || f_width < size.width || f_height < size.height) {
    binpack_t *result = nullptr;

    if (pack_right_ && pack_bottom_ && ! (pack_bottom_->loaded() || pack_right_->loaded())) {
      int delta, opposite;

      if (size.width >= size.height) {
        delta = pack_bottom_->width() - size.width;
        opposite = pack_right_->width();
      } else {
        delta = pack_bottom_->height() - size.height;
        opposite = pack_right_->height();
      }

      const bool bottom_first = (0 < delta && delta < opposite);


      if (! (result = (bottom_first ? pack_bottom_ : pack_right_)->find_unused_bin(size)))
        result = (bottom_first ? pack_right_ : pack_bottom_)->find_unused_bin(size);
    } else { // pack_right_ && pack_bottom_ && ...
      if (pack_right_)
        result = pack_right_->find_unused_bin(size);
      if ( ! result && pack_bottom_)
        result = pack_bottom_->find_unused_bin(size);
    } // pack_right_ && pack_bottom_ && ...

    return result;
  } // loaded_ || f_width < size.width || ...

  if (loaded_ && pack_right_ && pack_bottom_) {
    return nullptr;
  }

  if (pack_right_ || pack_bottom_) {
    const int bottom_delta = f_height - size.height;
    const int right_delta = f_width - size.width;

    if (bottom_delta > 0 && ! pack_bottom_) {
      pack_bottom_ = new binpack_t({ {0, 0}, {f_width, bottom_delta} }, nullptr, pack_bottom_);
    }

    if (right_delta > 0 && ! pack_right_) {
      pack_right_ = new binpack_t({ {0, 0}, {right_delta, f_height} }, pack_right_, nullptr);
    }
  } else { // pack_right_ || pack_bottom_
    const int width_delta = f_width - size.width;
    const int height_delta = f_height - size.height;
    recti_t right_rect = {{0,0}}, bottom_rect = {{0,0}};

    if (height_delta < width_delta) {
      right_rect.size = { width_delta, size.height };
      bottom_rect.size = { f_width, height_delta };
    } else {
      right_rect.size = { width_delta, f_height };
      bottom_rect.size = { size.width, height_delta };
    }

    pack_right_ = new binpack_t(right_rect);
    pack_bottom_ = new binpack_t(bottom_rect);
  } // pack_right_ || pack_bottom_

  if (pack_right_)
    pack_right_->frame_.origin = { frame_.origin.x + size.width, frame_.origin.y };

  if (pack_bottom_)
    pack_bottom_->frame_.origin = { frame_.origin.x, frame_.origin.y + size.height };

  frame_.size = size;
  loaded_ = true;

  return this;
}

S_EXPORT
void binpack_t::merge_empty_recursive()
{
  const int f_width = frame_.size.width;
  const int f_height = frame_.size.height;

  if (pack_right_)
    pack_right_->merge_empty_recursive();

  if (pack_bottom_)
    pack_bottom_->merge_empty_recursive();

  if (pack_right_) {
    if ( ! pack_right_->loaded() && pack_right_->height() == f_height) {
      frame_.size.width += pack_right_->width();
      binpack_t *old_right = pack_right_;
      pack_right_ = pack_right_->pack_right_;
      delete old_right;
    }

    if (pack_bottom_ && ! pack_bottom_->loaded() && pack_bottom_->width() == f_width) {
      frame_.size.height += pack_bottom_->height();
      binpack_t *old_bottom = pack_bottom_;
      pack_bottom_ = pack_bottom_->pack_bottom_;
      delete old_bottom;
    }
  }

  if (pack_bottom_) {
    if ( ! pack_bottom_->loaded() && pack_bottom_->width() == f_width) {
      frame_.size.height += pack_bottom_->height();
      binpack_t *old_bottom = pack_bottom_;
      pack_bottom_ = pack_bottom_->pack_bottom_;
      delete old_bottom;
    }

    if (pack_right_ && ! pack_right_->loaded() && pack_right_->height() == f_height) {
      frame_.size.width += pack_right_->width();
      binpack_t *old_right = pack_right_;
      pack_right_ = pack_right_->pack_right_;
      delete old_right;
    }
  }
}

S_EXPORT
void binpack_t::unload()
{
  loaded_ = false;
  merge_empty_recursive();
}

S_EXPORT
void binpack_t::reset()
{
  reset_recursive();
  merge_empty_recursive();
}

S_EXPORT
void binpack_t::reset_recursive()
{
  loaded_ = false;

  if (pack_right_)
    pack_right_->reset_recursive();

  if (pack_bottom_)
    pack_bottom_->reset_recursive();
}

}
