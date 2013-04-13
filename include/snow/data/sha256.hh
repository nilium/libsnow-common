// sha256.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__SHA256_HH__
#define __SNOW_COMMON__SHA256_HH__

#include <snow/config.hh>

#if HAS_SHA256

#include <openssl/sha.h>

namespace snow {


S_EXPORT void sha256(const char *data, size_t length, char hash[SHA256_DIGEST_LENGTH]);



template <typename T>
inline void sha256(const std::basic_string<T> &string, char hash[65])
{
  sha256((const char *)string.c_str(), string.size() * sizeof(T));
}


} // namespace snow

#endif

#endif /* end __SNOW_COMMON__SHA256_HH__ include guard */
