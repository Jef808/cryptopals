#include "bitwise_xor.h"
#include "metrics.h"
#include "character_frequency.h"
#include "data_analysis/load_character_frequencies.h"

#include <algorithm>
#include <limits>
#include <iostream>

namespace bytes {

namespace {


struct Xor {
  Xor(const uint8_t* key, const size_t key_len)
    : m_key{key}
    , m_key_len{key_len}
    , m_key_ndx{0}
  {}

  uint8_t operator()(const uint8_t b) const
  {
    return b ^ m_key[m_key_ndx++ % m_key_len];
  }

  const uint8_t* m_key;
  const size_t m_key_len;
  mutable size_t m_key_ndx;
};

} // namespace


std::basic_string<uint8_t>
bitwise_xor(const uint8_t* key,    const size_t key_len,
            const uint8_t* string, const size_t string_len)
{
  std::basic_string<uint8_t> ret;
  ret.reserve(string_len);

  const Xor unary_xor(key, key_len);

  std::transform(string,
                 string + string_len,
                 std::back_inserter(ret),
                 unary_xor);
  return ret;
}


std::pair<uint8_t, bool>
break_single_byte_xor(
  const uint8_t* enc, const size_t len, const size_t stride)
{
  std::array<double, 256> Freq;
  bool okay = data::load_character_frequencies(Freq);

  if (not okay) {
    return std::make_pair(0, false);
  }

  uint8_t best_key = 0;
  double lowest_cost = std::numeric_limits<double>::max();

  for (size_t k = 0; k < 256; ++k) {

    std::basic_string<uint8_t> Dec =
      bitwise_xor(reinterpret_cast<uint8_t*>(&k), 1, enc, len);

    if (not metrics::is_text(Dec)) {
      continue;
    }

    std::array<double, 256> DecFreq;
    get_frequency_distribution(Dec, DecFreq);

    double cost = metrics::L2_distance_squared(DecFreq, Freq);
    if (cost < lowest_cost) {
      lowest_cost = cost;
      best_key = k;
    }
  }

  return std::make_pair(best_key, true);
}


} // namespace bytes
