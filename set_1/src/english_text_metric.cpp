#include "english_text_metric.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <locale>

#include <iostream>

namespace bytes::metrics {

namespace {


// constexpr const std::array<unsigned char, 14> ETAOIN_SHRDLU =
// { '_', 'E', 'T', 'A', 'O', 'I', 'N', ' ', 'S', 'H', 'R', 'D', 'L', 'U' };

/**
 * The expected relative frequencies
 * real-world English text
 */
constexpr const std::array<double, 14> EXPECTED_FREQ =
{ 0.0, 0.13, 0.091, 0.082, 0.075, 0.07, 0.067, 0.0, 0.063, 0.061, 0.06, 0.043, 0.04, 0.028 };

/**
 * Usage: char_index[c] gives the index of
 * the (unsigned) ASCII character c in the
 * string "_ETAOIN SHRDLU" (or 0 for characters
 * not present in the string).
 */
constexpr uint8_t char_index[256] =
{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  3,  0,  0, 11,  1,  0,  0,  9,  5,  0,  0, 12,  0,  6,  4,
    0,  0, 10,  8,  2, 13,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  3,  0,  0, 11,  1,  0,  0,  9,  5,  0,  0, 12,  0,  6,  4,
    0,  0, 10,  8,  2, 13,  0,  0,  0,  0,  0
};

} // namespace


double english_text_cost(const std::basic_string<uint8_t>& bytes) {
  // The input string's character frequency histogram
  std::array<int, 14> freq_count =
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  int letter_count = 0;

  double cost_char_penalty = 0.0;
  double cost_letter_freq = 0.0;
  double cost_word_len = 0.0;

  // Compute the cost function
  for (uint8_t c : bytes) {
    // Early return if we find a non-printable character
    if (not std::isprint(c)) {
      return 1000.0;
    }

    // Count spaces
    if (c == 32) {
      ++freq_count[char_index[c]];
      continue;
    }

    // Accumulate letter frequency
    if (std::isalpha(c)) {
      ++freq_count[char_index[c]];
      ++letter_count;
      continue;
    }

    // Penalty for printable non-space, non-letter, non-punctuation
    if (not std::ispunct(c)) {
      cost_char_penalty += 20.0;
      continue;
    }
  }

  // Add the mean squared errors for the letter frequencies
  for (size_t i = 1; i < 14; ++i) {
    if (i == 7) { continue; }
    cost_letter_freq += std::pow(
      100.0 * (EXPECTED_FREQ[i] - static_cast<double>(freq_count[i]) / letter_count), 2);
  }
  cost_letter_freq /= letter_count;

  // Add a penalty when average word length is far from 4.5
  int spaces_count = freq_count[7];
  double average_word_len = static_cast<double>(bytes.size() - spaces_count) / (1 + spaces_count);
  cost_word_len = std::abs(average_word_len - 4.5);

  return cost_char_penalty + cost_letter_freq + cost_word_len;;
}

bool is_all_printable(const std::basic_string<uint8_t>& bytes) {
  return std::all_of(bytes.begin(), bytes.end(), [](auto b) {
    return std::isprint(b) || b == 10;
  });
}


} // namespace bytes::metrics
