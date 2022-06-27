#ifndef BYTES_H_
#define BYTES_H_

#include "hex.h"
#include "base64.h"

#include <string>

namespace bytes {


inline std::string hex_2_base64(const std::string& string) {
  return base64::encode(hex::decode(string));
}

inline std::string base64_2_hex(const std::string& string) {
  return hex::encode(base64::decode(string));
}

} // namespace bytes


#endif // BYTES_H_
