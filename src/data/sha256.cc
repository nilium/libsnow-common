// sha256.cc -- Noel Cower -- Public Domain

#include <snow/data/sha256.hh>
#include <stdexcept>

namespace snow {

#ifdef S_PLATFORM_APPLE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

void sha256(const char *data, size_t length, char hash[SHA256_DIGEST_LENGTH])
{
  SHA256_CTX context;
  if (!SHA256_Init(&context))
    throw std::runtime_error("Failed to initialize SHA-256 context");
  else if (!SHA256_Update(&context, data, length))
    throw std::runtime_error("Failed to hash data");
  else if (!SHA256_Final((unsigned char *)hash, &context))
    throw std::runtime_error("Failed to get message digest from SHA-256 context");
}

#ifdef S_PLATFORM_APPLE
#pragma GCC diagnostic pop
#endif

}
