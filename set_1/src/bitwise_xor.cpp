#include "bitwise_xor.h"

#include <algorithm>

namespace bytes {

namespace {


struct Xor {
  const uint8_t operator()(const uint8_t a, const uint8_t b) {
    return a ^ b;
  }
};


} // namespace


std::string bitwise_xor(const std::string& key, const std::string& string)
{
    std::basic_string<uint8_t> string_bytes = hex::decode(string);
    std::basic_string<uint8_t> key_bytes = hex::decode(key);
    std::basic_string<uint8_t> ret_bytes;
    ret_bytes.reserve(std::min(key_bytes.size(), string_bytes.size()));

    std::transform(string_bytes.begin(),
                   std::min(string_bytes.end(), string_bytes.begin() + key_bytes.size()),
                   key_bytes.begin(),
                   std::back_inserter(ret_bytes),
                   Xor{});

    return hex::encode(ret_bytes);
}


} // namespace bytes
