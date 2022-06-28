#ifndef BITWISE_XOR_H_
#define BITWISE_XOR_H_


#include "bytes.h"

#include <cstdint>
#include <string>


namespace bytes {


/**
 * Return the result of byte-wise xoring a hex-encoded input string
 * against a hex-encoded key string.
 *
 * @Note  If the key is shorter than the string, repeat the key once
 * it is fully consumed.
 */
std::basic_string<uint8_t>
bitwise_xor(const uint8_t* /* key */,
            const size_t /* key length */,
            const uint8_t* /* input string */,
            const size_t /* input string length */);


/**
 * Helper function to work on hex-encoded strings
 */
inline std::string bitwise_xor(const std::string& key, const std::string& msg) {
  return hex::encode(
      bitwise_xor(
          &hex::decode(key)[0], size_t((key.size() + 1) / 2),
          &hex::decode(msg)[0], size_t((msg.size() + 1) / 2)));
}

/**
 * Perform bitwise xor but simply check all resulting characters are
 * printable.
 */
bool get_all_printable(const std::basic_string<uint8_t>& /* Input string to decode */,
                       const uint8_t* /* Key */,
                       const size_t /* Key lenght */);

} //  namespace bytes


#endif // BITWISE_XOR_H_
