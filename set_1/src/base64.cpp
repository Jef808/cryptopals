#include "base64.h"

#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <string>

namespace bytes::base64 {


namespace {

constexpr char base64_alphabet[65] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

/**
 * Usage: base64_index[reinterpret_cast<unsigned char*>(c)] gives
 * the index of c in base64_alphabet.
 *
 * In case of c not being a base64 digit, fallback value is 0.
 *
 * Note: ')', '+' and ',' are all interpreted as the 62nd character,
 * while '*', '-' and '/' are interpreted as the 63rd character.
 */
constexpr uint8_t base64_index[256] =
{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  62, 63, 62, 62, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0,  0,  0,  0,
    0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0,  0,  0,  0,  63,
    0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
};

}  // namespace


std::string encode(const uint8_t* data, const size_t len) {
  const size_t olen = 4 * ((len + 2) / 3);
  std::string ret(olen, '=');

  const uint8_t* data_end = data + len;
  char* out = &ret[0];

  for (; data + 2 < data_end; data += 3) {
    // Store three bytes into a 24 bits chunk at intervals of 8 bits.
    int n = int(data[0]) << 16 | int(data[1]) << 8 | data[2];

    // Consume the same 24 bits as four packets of 6 bits.
    *out++ = base64_alphabet[n >> 18];
    *out++ = base64_alphabet[n >> 12 & 0x3f];
    *out++ = base64_alphabet[n >> 6 & 0x3f];
    *out++ = base64_alphabet[n & 0x3f];
  }

  // 0, 1 or 2 bytes remaining in input buffer
  // (corresponding to 0, 2, or 3 base64 digits)
  const int n_bytes = data_end - data;
  if (n_bytes > 0) {
    int n = n_bytes > 1 ? int(data[0]) << 8 | data[1] : data[0];
    const bool two_bytes = n_bytes > 1;

    // Encoded first 6 bits
    *out++ = base64_alphabet[two_bytes ? n >> 10 & 0x3f : n >> 2];

    // Encoded next 6 bits or encoded
    // last 2 bits, 0-padded on the right
    *out++ = base64_alphabet[two_bytes ? n >> 4 & 0x3f : n << 4 & 0x3f];

    // Encoded last 4 bits, 0-padded on the right
    // or padding character
    *out++ = two_bytes ? base64_alphabet[n << 2 & 0x3f] : '=';
  }

  return ret;
}


std::basic_string<uint8_t> decode(const char* string, const size_t len) {
  std::basic_string<uint8_t> ret;
  if (len == 0) {
    return ret;
  }
  const unsigned char* in = (unsigned char*)string;

  // pad1 if there is at least one padding '='
  // (At least one byte added at the end)
  const bool pad1 = len % 4 || in[len - 1] == '=';

  // pad2 if there is ast most one padding '='
  // (Two bytes added at the end)
  const bool pad2 = pad1 && (len % 4 > 2 || in[len - 2] != '=');

  // pointer to the beginning of the last packet of 4 chars
  const unsigned char* in_last_packet = in + ((len - pad1) / 4 << 2);
  ret.resize(3 * ((in_last_packet - in) / 4) + pad1 + pad2, '\0');

  uint8_t* out = &ret[0];

  for (; in < in_last_packet; in += 4) {
    // Store four base64 digits into a 24 bits chunk
    int n = base64_index[in[0]] << 18 | base64_index[in[1]] << 12 | base64_index[in[2]] << 6 | base64_index[in[3]];

    // Consume that chunk of 24 bits as 3 bytes
    *out++ = n >> 16;
    *out++ = n >> 8 & 0xff;
    *out++ = n & 0xff;
  }

  // The last (short) packet
  if (pad1) {
    int n = base64_index[in[0]] << 18 | base64_index[in[1]] << 12;

    // Have at least one extra byte for two chars
    *out++ = n >> 16;

    // Construct second extra byte with third char if needed
    if (pad2) {
      n |= base64_index[in[2]] << 6;
      *out++ = n >> 8 & 0xff;
    }
  }

  return ret;
}

}  // namespace bytes::base64
