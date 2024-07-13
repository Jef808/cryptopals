/**
 *  Illustration:
 *                         +--first octet--+-second octet--+--third octet--+
 * 3 binary octets:        |7 6 5 4 3 2 1 0|7 6 5 4 3 2 1 0|7 6 5 4 3 2 1 0|
 *        =                +-----------+---+-------+-------+---+-----------+
 * 4 base64 digits:        |5 4 3 2 1 0|5 4 3 2 1 0|5 4 3 2 1 0|5 4 3 2 1 0|
 *                         +--1.index--+--2.index--+--3.index--+--4.index--+
 *
 *
 * Examples:
 *
 * Input data: 0x14fb9c03d97e
 * Hex: 1 4 f b 9 c | 0 3 d 9 7 e
 * 8-bit: 00010100 11111011 10011100 | 00000011 11011001 01111110
 * 6-bit: 000101 001111 101110 011100 | 000000 111101 100101 111110
 * Decimal: 5 15 46 28 0 61 37 62
 * Output: F P u c A 9 l +
 *
 * Input data: 0x14fb9c03d9
 * Hex: 1 4 f b 9 c | 0 3 d 9
 * 8-bit: 00010100 11111011 10011100 | 00000011 11011001
 * pad with 00
 * 6-bit: 000101 001111 101110 011100 | 000000 111101 100100
 * Decimal: 5 15 46 28 0 61 36
 * pad with =
 * Output: F P u c A 9 k =
 *
 * Input data: 0x14fb9c03
 * Hex: 1 4 f b 9 c | 0 3
 * 8-bit: 00010100 11111011 10011100 | 00000011
 * pad with 0000
 * 6-bit: 000101 001111 101110 011100 | 000000 110000
 * Decimal: 5 15 46 28 0 48
 * pad with = =
 * Output: F P u c A w = =
 */
#include "base64.h"

#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <string>

#include <iostream>

namespace bytes::base64 {


namespace {

/**
 * The 64 characters in the alphabet.
 *
 * @Note  The array allocates one extra char
 * for the terminal '\0'.
 */
constexpr const char base64_alphabet[65] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"           // 0  - 25
    "abcdefghijklmnopqrstuvwxyz"           // 26 - 51
    "0123456789+/";                        // 52 - 61, 62, 63

/**
 * The character used for padding.
 */
constexpr const char PAD = '=';

/**
 * Usage: base64_index[(unsigned)c] gives
 * the 1-based index of c in base64_alphabet.
 *
 * In case of c not being a base64 digit, invalid value is 0.
 */
constexpr uint8_t base64_index[256] =
{
    0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 0  - 15
    0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   // 16 - 31
    0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 63,  0,  0,  0, 64,   // 32 - 47 (+ and /)
    53, 54, 55, 56, 57, 58, 59, 60, 61, 62,  0,  0,  0,  0,  0,  0,   // 48 - 63 (digits)
    0,   1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,   // 64 - 79 (uppercase)
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,  0,  0,  0,  0,  0,   // 80 - 95 (uppercase)
    0,  27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,   // 96 -111 (lowercase)
    42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52                        // 112-127 (lowercase)
};

}  // namespace


size_t find_invalid(const char* string, const size_t len) {
  return std::distance(
    string, std::find_if(string, string + len, [](auto c){
      return base64_index[(unsigned)c] == 0 and c != PAD;
    }));
}


std::string encode(const uint8_t* data, const size_t len) {
  const size_t olen = 4 * (len + 2) / 3;
  std::string ret;
  ret.reserve(olen);
  auto out = std::back_inserter(ret);

  const size_t i_end = 3 * int(len / 3);

  for (size_t i = 0; i < i_end; i += 3) {
    // Store three bytes into a 24 bits chunk at intervals of 8 bits.
    int n = int(data[i]) << 16 | int(data[i+1]) << 8 | data[i+2];

    // Output those 24 bits 6 bits at a time.
    out = base64_alphabet[n >> 18];
    out = base64_alphabet[n >> 12 & 0x3f];
    out = base64_alphabet[n >> 6 & 0x3f];
    out = base64_alphabet[n & 0x3f];
  }

  const int nb_last = len - i_end;

  if (nb_last > 0) {
    int n = nb_last == 2 ? data[i_end] << 8 | data[i_end+1] : data[i_end];

    // First 6 bits
    out = base64_alphabet[nb_last == 2 ? n >> (2+8) & 0x3f : n >> 2 & 0x3f];

    // [Next 6 bits] or [Last 2 bits, right-0000-padded]
    out = base64_alphabet[nb_last == 2 ? n >> 4 & 0x3f : n << 4 & 0x3f];

    // [Last 4 bits, right-00-padded] or [Padding character]
    out = nb_last == 2 ? base64_alphabet[n << 2 & 0x3f] : PAD;

    // Padding character
    out = PAD;
  }

  return ret;
}


std::basic_string<uint8_t> decode(const char* string, const size_t len) {
  {
    auto inv = find_invalid(string, len);
    if (inv < len) {
      std::stringstream ss;
      auto invalid_char = string + inv;
      ss << "base64::decode: Invalid char at index " << inv << ": " << *invalid_char;
      throw std::runtime_error(ss.str());
    }
  }

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
    int n = (base64_index[in[0]]-1) << 18 | (base64_index[in[1]]-1) << 12 | (base64_index[in[2]]-1) << 6 | (base64_index[in[3]]-1);

    // Consume that chunk of 24 bits as 3 bytes
    *out++ = n >> 16;
    *out++ = n >> 8 & 0xff;
    *out++ = n & 0xff;
  }

  // The last (short) packet
  if (pad1) {
    int n = (base64_index[in[0]]-1) << 18 | (base64_index[in[1]]-1) << 12;

    // Have at least one extra byte for two chars
    *out++ = n >> 16;

    // Construct second extra byte with third char if needed
    if (pad2) {
      n |= (base64_index[in[2]]-1) << 6;
      *out++ = n >> 8 & 0xff;
    }
  }

  return ret;
}

}  // namespace bytes::base64
