/**
 * Encode/decode binary strings to/from base64 strings.
 *
 * The base64 alphabet uses the uppercase A-Z,
 * lowercase a-z, and the digits 0-9 for its first
 * 62 possible values, with the characters '+' and '/'
 * for the last two values.
 * base64 is thus a 6-bit encoding, i.e. each base64 digit
 * contains 6 bit worth of information.
 *
 * A binary stream is a contiguous sequence of bytes, i.e. 8-bit
 * octets.
 * Since base64 is a 6-bit encoding, we consider byte packets of
 * size 24: the least common multiple of 6 and 8.
 * Thus, packets of 3 bytes correspond to packets of 4 base64
 * digits and vice-versa.
 *
 * To enforce base64 packets of 4 digits, the extra character '='
 * is used for padding.
 */
#ifndef BASE64_H_
#define BASE64_H_

#include <cstdint>
#include <string>

namespace bytes::base64 {


/**
 * Transform binary data into a base64-encoded string.
 *
 * The resulting string is padded so that its length is a multiple of 4.
 */
std::string encode(const uint8_t* /* Unencoded byte stream */,
                   const size_t   /* Size of data in bytes */);

/**
 * Transform a base64-encoded stream into a raw binary string.
 */
std::basic_string<uint8_t> decode(const char*  /* base64 encoded char stream */,
                                  const size_t /* Number of characters in the encoded string */);

/**
 * Encode a byte string into a base64 string.
 */
inline std::string encode(const std::basic_string<uint8_t>& bytes) {
  return encode(&bytes[0], bytes.size());
}

/**
 * Decode a base64 string to a byte string.
 */
inline std::basic_string<uint8_t> decode(const std::string& string) {
  return decode(&string[0], string.size());
}


} // namespace bytes::base64


#endif // BASE64_H_
