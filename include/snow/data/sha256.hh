// sha256.hh -- Noel Cower -- Public Domain

#ifndef __SNOW_COMMON__SHA256_HH__
#define __SNOW_COMMON__SHA256_HH__

#include <snow/config.hh>

#if HAS_SHA256

#include <openssl/sha.h>

namespace snow {


/**
  @addtogroup HashFunction
  @{
*/

/**
  Function to get the SHA256 of an arbitrary chunk of data. Requires OpenSSL to
  be linked in application code if used.
  @param data The data to hash.
  @param length The length of the data to hash.
  @param hash An output buffer at least SHA256_DIGEST_LENGTH bytes in length to
  store the resulting hash.
*/
S_EXPORT void sha256(const char *data, size_t length, char hash[SHA256_DIGEST_LENGTH]);



/**
  Function to get the SHA256 of an arbitrary chunk of data. T must provide a
  T::value_type typedef, a `data()` member function that returns a pointer, and
  a `size()` member function to return the number of elements in the data.
  @param data The data to hash.
  @param hash An output buffer at least SHA256_DIGEST_LENGTH bytes in length to
  store the resulting hash.
*/
template <typename T>
inline void sha256(const T &data, char hash[SHA256_DIGEST_LENGTH])
{
  sha256((const char *)data.data(), data.size() * sizeof(T::value_type));
}


/** @} */


} // namespace snow

#endif

#endif /* end __SNOW_COMMON__SHA256_HH__ include guard */
