#include "hex.h"

#include <cassert>
#include <sstream>
#include <string>

namespace bytes::hex {

namespace {

constexpr char hex_alphabet[17] =
    "0123456789"
    "abcdef";

/**
 * Usage: hex_index[(unsigned char*)(&c)] gives
 * the index of the character c in hex_alphabet.
 *
 * In case of c not being a hex digit, fallback value is 0.
 *
 * Note: Both first 6 lowercase and uppercase letters
 * correspond to last 6 hex digits.
 */
constexpr uint8_t hex_index[256] =
{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  0,  0,  0,  0,  0,  0,
    0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

} // namespace


std::string encode(const uint8_t* bytes, const size_t len) {
    std::string ret;
    ret.reserve(2 * len);
    auto out = std::back_inserter(ret);

    const uint8_t* bytes_end = bytes + len;

    while (bytes < bytes_end) {
      int n = *bytes++;
      out = hex_alphabet[n >> 4 & 0xf];
      out = hex_alphabet[n & 0xf];
    }

    return ret;
}

/**
 * Transform a hex-encoded stream into a raw binary string.
 */
std::basic_string<uint8_t> decode(const char* string, const size_t len) {
    std::basic_string<uint8_t> ret;
    ret.reserve(len / 2);
    auto out = std::back_inserter(ret);

    const char* in = string;
    const char* in_end = in + len;

    for (; in + 1 < in_end; in += 2) {
      int n = hex_index[unsigned(in[0])] << 4 | hex_index[unsigned(in[1])];
      out = n & 0xff;
    }

    return ret;
}


} // namespace bytes::hex
