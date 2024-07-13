#include "load_character_frequencies.h"
#include "read_csv.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace data {


bool load_character_frequencies(std::array<double, 256>& freqs)
{
  const char* fn =
    DATA_DIR "/harry_potter/character_frequency.csv";

  auto [counter_vec, okay] = read_csv<int>(fn, true);
  if (not okay) {
      return false;
  }

  double total = 0.0;

  // Only one csv line
  auto& counter = counter_vec[0];

  for (size_t i = 0; i < counter.size(); ++i) {

    // Store all character data
    total += freqs[i] = counter[i];
  }

  // Divide by computed total number of
  // characters to get the frequencies
  std::transform(freqs.begin(), freqs.end(), freqs.begin(), [total](auto v) {
    return v / total;
  });

  return true;
}

} // namespace data
