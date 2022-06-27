/**
 * Encode/decode binary strings to/from hex strings.
 *
 * The hex alphabet uses the digits 0-9 for its first 10
 * possible values, and letters (lowercase or uppercase) a-f
 * for its last 6.
 * hex is thus a 4-bit encoding, i.e. each hex digit
 * contains 4 bit worth of information.
 *
 * A binary stream is a contiguous sequence of bytes, i.e. 8-bit
 * octets.
 * Since hex is a 4-bit encoding, we consider packets of 2 hex digits
 * corresponding to exactly one byte and vice-versa.
 */
#ifndef HEX_H_
#define HEX_H_

#include <cstdint>
#include <string>

namespace bytes::hex {


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


} // namespace bytes::hex


#endif // HEX_H_
