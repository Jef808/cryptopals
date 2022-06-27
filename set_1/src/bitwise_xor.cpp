#include "bitwise_xor.h"

#include <algorithm>

namespace bytes {

namespace {


struct Xor {
  Xor(const uint8_t* key, const size_t key_len)
    : m_key{key}
    , m_key_len{key_len}
    , m_key_ndx{0}
  {}

  uint8_t operator()(const uint8_t b) const
  {
    return b ^ m_key[m_key_ndx++ % m_key_len];
  }

  const uint8_t* m_key;
  const size_t m_key_len;
  mutable size_t m_key_ndx;
};


} // namespace


std::basic_string<uint8_t>
bitwise_xor(const uint8_t* key,    const size_t key_len,
            const uint8_t* string, const size_t string_len)
{
  std::basic_string<uint8_t> ret;
  ret.reserve(string_len);

  const Xor unary_xor(key, key_len);

  std::transform(string,
                 string + string_len,
                 std::back_inserter(ret),
                 unary_xor);
  return ret;
}


} // namespace bytes
