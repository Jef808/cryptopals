#include "character_frequency.h"

#include <algorithm>
#include <array>
#include <string>

namespace bytes {


void get_frequency_distribution(
  const std::basic_string<uint8_t>& S,
  std::array<double, 256>& Freq)
{
  Freq.fill(0.0);

  for (auto b : S)
    ++Freq[b];

  std::transform(
    Freq.begin(), Freq.end(), Freq.begin(),
    [total=S.size()](auto c) {
      return c / total;
    });
}


} // namespace bytes
