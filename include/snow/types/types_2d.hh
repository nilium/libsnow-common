// types_2d.hh -- Noel Cower -- Public Domain

#ifndef __TYPES_2D_HH__
#define __TYPES_2D_HH__

#include <iostream>
#include <algorithm>

namespace snow
{

template <typename T>
struct rect_t;

template <typename T>
struct dimens_t;

template <typename T = int>
struct point_t
{

  T x;
  T y;

  auto max(const point_t &other) const -> point_t
  {
    return {
      std::max(x, other.x),
      std::max(y, other.y)
    };
  }

  auto min(const point_t &other) const -> point_t
  {
    return {
      std::min(x, other.x),
      std::min(y, other.y)
    };
  }

  operator rect_t<T> () const
  {
    return {
      *this,
      { T(), T() }
    };
  }

  bool operator == (const point_t &other)
  {
    return other.x == x && other.y == y;
  }

  template <typename Q>
  operator point_t<Q> () const
  {
    return {
      static_cast<Q>(x),
      static_cast<Q>(y)
    };
  }
};

template <typename T = int>
struct dimens_t
{
  T width;
  T height;

  auto max(const dimens_t &other) const -> dimens_t
  {
    return {
      std::max(width, other.width),
      std::max(height, other.height)
    };
  }

  auto min(const dimens_t &other) const -> dimens_t
  {
    return {
      std::min(width, other.width),
      std::min(height, other.height)
    };
  }

  operator rect_t<T> () const
  {
    return {
      { T(), T(), },
      *this
    };
  }

  bool operator == (const dimens_t &other)
  {
    return other.width == width && other.height == height;
  }

  template <typename Q>
  operator dimens_t<Q> () const
  {
    return {
      static_cast<Q>(width),
      static_cast<Q>(height)
    };
  }
};

template <typename T = int>
struct rect_t
{
  point_t<T> origin;
  dimens_t<T> size;

  auto intersects(const rect_t &other) const   -> bool
  {
    return !(other.right() < left() ||
             other.bottom() < top() ||
             right() < other.left() ||
             bottom() < other.top());
  }

  auto intersection(const rect_t &other) const -> rect_t
  {
    point_t<T> point_max = origin.max(other.origin);
    return {
      point_max,
      {
        std::max(T(), std::min(right(), other.right()) - point_max.x),
        std::max(T(), std::min(top(), other.top()) - point_max.y)
      }
    };
  }

  auto padded(T horizontal, T vertical) const -> rect_t
  {
    return {
      {
        origin.x - horizontal,
        origin.y - vertical
      },
      {
        size.width + horizontal * 2,
        size.height + vertical * 2
      }
    };
  }

  auto pad(T horizontal, T vertical) -> rect_t&
  {
    origin.x -= horizontal;
    origin.y -= vertical;
    size.width += horizontal * 2;
    size.height += vertical * 2;
    return *this;
  }

  auto right() const  -> T
  {
    if (size.width < T())
      return origin.x;
    else
      return origin.x + size.width;
  }

  auto left() const   -> T
  {
    if (size.width < T())
      return origin.x + size.width;
    else
      return origin.x;
  }

  auto top() const    -> T
  {
    if (size.height < T())
      return origin.y;
    else
      return origin.y + size.height;
  }

  auto bottom() const -> T
  {
    if (size.height < T())
      return origin.y + size.height;
    else
      return origin.y;
  }

  auto contains(const point_t<T> &point) const -> bool
  {
    return (left() <= point.x && point.x <= right() &&
            bottom() <= point.y && point.y <= top());
  }

  bool operator == (const rect_t &other)
  {
    return other.origin == origin && other.size == size;
  }

  template <typename Q>
  operator rect_t<Q> () const
  {
    return {
      origin,
      size
    };
  }
};

template <typename T = int>
auto make_point(T x, T y) -> point_t<T>
{
  return {
    x, y
  };
}

template <typename T = int>
auto make_dimens(T width, T height) -> dimens_t<T>
{
  return {
    width, height
  };
}

template <typename T = int>
auto make_rect(T x, T y, T width, T height) -> rect_t<T>
{
  return {
    { x, y },
    { width, height }
  };
}

template <typename T>
std::ostream &operator << (std::ostream &out, const point_t<T> &in)
{
  return out << "{ x: " << in.x << ", y: " << in.y << " }";
}

template <typename T>
std::ostream &operator << (std::ostream &out, const dimens_t<T> &in)
{
  return out << "{ width: " << in.width << ", height: " << in.height << " }";
}

template <typename T>
std::ostream &operator << (std::ostream &out, const rect_t<T> &in)
{
  return out << "{ origin: " << in.origin << ", size: " << in.size << " }";
}

typedef point_t<float> pointf_t;
typedef point_t<double> pointd_t;
typedef point_t<long> pointl_t;
typedef point_t<int> pointi_t;

typedef dimens_t<float> dimensf_t;
typedef dimens_t<double> dimensd_t;
typedef dimens_t<long> dimensl_t;
typedef dimens_t<int> dimensi_t;

typedef rect_t<float> rectf_t;
typedef rect_t<double> rectd_t;
typedef rect_t<long> rectl_t;
typedef rect_t<int> recti_t;

}

#endif /* end __TYPES_2D_HH__ include guard */
