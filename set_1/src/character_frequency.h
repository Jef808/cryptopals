#ifndef CHARACTER_FREQUENCY_H_
#define CHARACTER_FREQUENCY_H_

#include <array>
#include <cstdint>
#include <string>

namespace bytes {

/**
 * Collect the character frequency distribution of a given
 * byte string.
 */
void get_frequency_distribution(
  const std::basic_string<uint8_t>& /* Input byte string */,
  std::array<double, 256>& /* Ouput */);



} // namespace bytes

#endif // CHARACTER_FREQUENCY_H_
