// hash.cc -- Noel Cower -- Public Domain

#include <snow/data/hash.hh>


namespace snow {


/*==============================================================================
  hash32(string, seed)

    Wrapper around hash64 to simplify using it with std::string.
==============================================================================*/
uint32_t hash32(const string &str, uint32_t seed)
{
  return hash32(str.c_str(), str.size(), seed);
}



/*==============================================================================
  hash32(cstring, length, seed)

    Hash function for general strings. Goes byte-by-byte, so probably not the
    fastest possible hash. That said, it won't differ between implementations,
    so it can be used (unlike std::hash<string>).
==============================================================================*/
uint32_t hash32(const char *str, const size_t length, uint32_t seed)
{
  static uint32_t mask_left[16] = {
    0x00000000U, 0x80000000U, 0xC0000000U, 0xE0000000U,
    0xF0000000U, 0xF8000000U, 0xFC000000U, 0xFE000000U,
    0xFF000000U, 0xFF800000U, 0xFFC00000U, 0xFFE00000U,
    0xFFF00000U, 0xFFF80000U, 0xFFFC0000U, 0xFFFE0000U,
  };
  uint32_t hash = seed;
  constexpr uint32_t hbits = sizeof(hash) * 8;
  size_t index = 0;
  for (; index < length; ++index) {
    const uint32_t curchar = str[index];
    hash = hash * 439U + curchar * 23U + (index + 257U);
    const uint32_t shift =
      ((curchar & 0x9) | ((curchar & 0x10) >> 2) | ((curchar & 0x40) >> 5)) ^
      ((curchar & 0xA) >> 5) | ((curchar & 0x2) << 2) | ((curchar & 0x4) >> 1);
    hash = (hash << shift) | (hash & mask_left[shift]) >> (hbits - shift);
  }
  return hash;
}



/*==============================================================================
  hash64(string, seed)

    Wrapper around hash64 to simplify using it with std::string.
==============================================================================*/
uint64_t hash64(const string &str, uint64_t seed)
{
  return hash64(str.c_str(), str.size(), seed);
}



/*==============================================================================
  hash64(cstring, length, seed)

    64-bit equivalent of the above hash32.
==============================================================================*/
uint64_t hash64(const char *str, const size_t length, uint64_t seed)
{
  static uint64_t mask_left[16] = {
    0x0000ULL << 48, 0x8000ULL << 48, 0xC000ULL << 48, 0xE000ULL << 48,
    0xF000ULL << 48, 0xF800ULL << 48, 0xFC00ULL << 48, 0xFE00ULL << 48,
    0xFF00ULL << 48, 0xFF80ULL << 48, 0xFFC0ULL << 48, 0xFFE0ULL << 48,
    0xFFF0ULL << 48, 0xFFF8ULL << 48, 0xFFFCULL << 48, 0xFFFEULL << 48,
  };
  uint64_t hash = seed;
  constexpr uint64_t hbits = sizeof(hash) * 8;
  size_t index = 0;
  for (; index < length; ++index) {
    const uint64_t curchar = str[index];
    hash = hash * 5741U + curchar * 23U + (index + 257U);
    const uint64_t shift =
      ((curchar & 0x9) | ((curchar & 0x10) >> 2) | ((curchar & 0x40) >> 5)) ^
      ((curchar & 0xA) >> 5) | ((curchar & 0x2) << 2) | ((curchar & 0x4) >> 1);
    hash = (hash << shift) | (hash & mask_left[shift]) >> (hbits - shift);
  }
  return hash;
}


} // namespace snow
