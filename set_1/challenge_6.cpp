#include "bitwise_xor.h"
#include "bytes.h"
#include "metrics.h"
#include "data_analysis/load_character_frequencies.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>

/**
 * We compare larger and larger contiguous chunks
 * under the Hamming distance. Only for chunks
 * of the same size as the hidden key will the key
 * not repeat itself at all, it is to be expected that
 * the number of differing bits drops at that size.
 *
 * @Param n_sizes  The number of keysizes to be returned
 * @Param max_size  The upper bound for sizes to test
 */
std::vector<size_t> discover_keysize(const std::basic_string<uint8_t> &enc,
                                     const size_t n_sizes=1,
                                     const size_t max_size=40)
{
  assert(enc.size() >= 4 * max_size
         && "Our discover_keysize will overflow");

  std::vector<size_t> keysizes(max_size - 1);
  std::iota(keysizes.begin(), keysizes.end(), 2);
  std::vector<double> results;
  results.reserve(max_size - 1);

  for (auto ks : keysizes) {
    std::basic_string_view<uint8_t> sv1(enc.data(), ks);
    std::basic_string_view<uint8_t> sv2(enc.data() + ks, ks);
    std::basic_string_view<uint8_t> sv3(enc.data() + 2*ks, ks);
    std::basic_string_view<uint8_t> sv4(enc.data() + 3*ks, ks);
    std::basic_string_view<uint8_t> sv5(enc.data() + 4*ks, ks);

    double distance12 = bytes::metrics::hamming_distance(sv1.data(), sv2.data(), ks);
    double distance13 = bytes::metrics::hamming_distance(sv1.data(), sv3.data(), ks);
    double distance14 = bytes::metrics::hamming_distance(sv1.data(), sv4.data(), ks);
    double distance15 = bytes::metrics::hamming_distance(sv1.data(), sv5.data(), ks);
    double distance23 = bytes::metrics::hamming_distance(sv2.data(), sv3.data(), ks);
    double distance24 = bytes::metrics::hamming_distance(sv2.data(), sv4.data(), ks);
    double distance25 = bytes::metrics::hamming_distance(sv2.data(), sv5.data(), ks);
    double distance34 = bytes::metrics::hamming_distance(sv3.data(), sv4.data(), ks);
    double distance35 = bytes::metrics::hamming_distance(sv3.data(), sv5.data(), ks);
    double distance45 = bytes::metrics::hamming_distance(sv4.data(), sv5.data(), ks);

    double avg_distance =
      (distance12 + distance13 + distance14 + distance15 + distance23 +
       distance24 + distance25 + distance34 + distance35 + distance45) / 10.0;
    results.push_back(avg_distance / ks);
  }
  std::partial_sort(keysizes.begin(), keysizes.begin() + n_sizes, keysizes.end(),
                    [&results](auto a, auto b){
                      return results[a - 2] < results[b - 2];
                    });
  keysizes.resize(n_sizes);
  return keysizes;
}


int main(int argc, char *argv[]) {
  const char *fn = DATA_DIR "/set_1/challenge_6.txt";
  std::ifstream ifs{fn};
  if (not ifs) {
    std::cerr << "Failed to open input file" << std::endl;
    return EXIT_FAILURE;
  }

  std::string buf;
  buf.reserve(256);
  std::stringstream ss;
  while (std::getline(ifs, buf)) {
    ss << buf;
  }
  std::string string = ss.str();

  auto newline = std::find(string.begin(), string.end(), '\n');
  if (newline != string.end()) {
    std::cout << "Found newline character\n" << std::endl;
    return EXIT_FAILURE;
  }

  std::basic_string<uint8_t> enc = bytes::base64::decode(string.data(), string.size());
  std::cout << "\033[1mEncoded string size:\033[0m\n  " << enc.size() << " bytes." << std::endl;

  // Get likely sizes of the key using the hamming distance
  // of consecutive chunks of the encoded string.
  const size_t n_keysizes = 6;
  std::vector<size_t> keysizes = discover_keysize(enc, n_keysizes);

  std::array<double, 256> freqs;
  data::load_character_frequencies(freqs);

  // For each keysize, collect the encoded bytes which would
  // have been encoded using the same character of the key.
  // We can then break the single-byte xor encoding of that
  // group of bytes to find the character of the key.
  std::basic_string<uint8_t> best_key;
  double best_key_score = std::numeric_limits<double>::min();
  for (size_t keysize : keysizes) {
      double key_score = 0.0;
      std::basic_string<uint8_t> key;

      // For each possible byte value, xor the group with that byte
      // and compare the resulting character frequency with the expected
      // frequency of characters in the English language.
      for (size_t i = 0; i < keysize; ++i) {
          std::basic_string<uint8_t> group;
          for (size_t p = i; p < enc.size(); p += keysize) {
              group.push_back(enc[p]);
          }
          int best_byte = -1;
          double best_byte_score = std::numeric_limits<double>::min();
          for (int _b = 1; _b < 256; ++_b) {
              uint8_t b = static_cast<uint8_t>(_b);
              if (not (std::isprint(b) or std::isspace(b))) {
                continue;
              }
              bool search = true;
              std::array<double, 256> counts;
              counts.fill(0.0);
              std::basic_string<uint8_t> xor_group = bytes::bitwise_xor(&b, 1, group.data(), group.size());
              for (auto c : xor_group) {
                if (not (std::isprint(c) or std::isspace(c))) {
                  search = false;
                  break;
                }
                counts[c] += 1;
              }
              if (not search) {
                continue;
              }
              double score = bytes::metrics::dot(counts, freqs) / xor_group.size();
              if (score > best_byte_score) {
                  best_byte_score = score;
                  best_byte = b;
              }
          }
          if (best_byte == -1) {
            key.clear();
            break;
          }
          key.push_back(best_byte);
          key_score += best_byte_score;
      }
      if (key.empty()) {
        continue;
      }
      key_score /= keysize;
      if (key_score > best_key_score) {
          best_key = key;
          best_key_score = key_score;
      }
  }

  if (best_key.empty()) {
    std::cout << "\nFailed to find a key"
      << "\nThe cypher text may not have been encoded with repeating key xor"
      << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "\n\033[1mKey:\033[0m\n  ";
  for (uint8_t b : best_key) {
      std::cout << b;
  }
  std::cout << std::endl;

  // Decode the message.
  auto dec = bytes::bitwise_xor(best_key.data(), best_key.size(), enc.data(), enc.size());

  std::cout << "\n\033[1mDecoded content:\033[0m\n  ";
  for (auto b : dec) {
      std::cout << b;
  }
  std::cout << std::endl;

  return EXIT_SUCCESS;
}
