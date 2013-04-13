// hash.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__HASH_HH__
#define __SNOW_COMMON__HASH_HH__

#include "../config.hh"
#include <cstdint>


namespace snow {


// Default seed is more or less random garbage that produces better results
// than some other random garbage.
const uint32_t DEFAULT_HASH_SEED_32 = 0x9E2030F1U;
const uint64_t DEFAULT_HASH_SEED_64 = 0x9E2030F19E2030F1ULL;



// 32-bit
// Wrapper around the other hash function.
uint32_t hash32(const string &str, uint32_t seed = DEFAULT_HASH_SEED_32);
uint32_t hash32(const char *str, const size_t length,
                uint32_t seed = DEFAULT_HASH_SEED_32);



// 64-bit
uint64_t hash64(const string &str, uint64_t seed = DEFAULT_HASH_SEED_64);
uint64_t hash64(const char *str, const size_t length,
                uint64_t seed = DEFAULT_HASH_SEED_64);


} // namespace snow

#endif /* end __SNOW_COMMON__HASH_HH__ include guard */
