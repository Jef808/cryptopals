/**
 * FIXME: It is very possible that the only reason this doesn't
 * work is that the base64 decoding has a bug... The other
 * challenges only tested for base64 encoding!
 */

#include "bitwise_xor.h"
#include "bytes.h"
#include "metrics.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

/**
 * Compute the number of bits differing between the first two chunks
 * of length @keysize.
 *
 * @Note  The result is normalized by dividing by @keysize.
 */
double hamming_test(const std::basic_string<uint8_t> &s, size_t keysize, bool twice=false) {
  double result = bytes::metrics::hamming_distance(&s[0], &s[keysize], keysize);
  if (twice) {
      double result2 = bytes::metrics::hamming_distance(&s[2*keysize], &s[3*keysize], keysize);
      result = (result + result2) / 2;
  }
  return result / keysize;
}

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
                                     const size_t max_size=40,
                                     const bool twice=false)
{
  assert(n_sizes <= max_size - 1
         && "More sizes are asked for than the number of sizes tested for");
  assert(enc.size() >= 2 * max_size * (2*twice)
         && "Our discover_keysize will overflow");

  std::vector<size_t> keysizes(max_size - 1);
  std::iota(keysizes.begin(), keysizes.end(), 2);
  std::vector<double> results;
  results.reserve(max_size - 1);

  for (auto ks : keysizes) {
    results.push_back(hamming_test(enc, ks, twice));
  }

  std::partial_sort(keysizes.begin(), keysizes.begin() + n_sizes, keysizes.end(),
                    [&results](const auto& a, const auto& b){ return results[a - 2] < results[b - 2]; });

  for (size_t s : keysizes) {
      std::cout << "Keysize " << s << ", Hamming test score: " << results[s - 2] << '\n';
  }

  std::cout << std::endl;

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
  buf.reserve(4096);
  ifs.read(&buf[0], 4096);
  int len = ifs.gcount();

  std::basic_string<uint8_t> enc = bytes::base64::decode(&buf[0], len);

  const size_t n_keysizes = 6;

  // FIXME: There is a bug in one of my base64 decode/encode methods,
  // after decoding and reencoding, the single '=' padding character
  // is replaced by 'AA'
  //
  // Test
  // buf = bytes::base64::encode(enc);
  // std::cout << buf << std::endl;

  // Get the size of the key using hamming distance
  std::vector<size_t> keysizes = discover_keysize(enc, n_keysizes);
  std::cout << "Most likely keysizes \033[1m: ";
  for (auto ks : keysizes) {
      std::cout << ks << ' ';
  }
  std::cout << "\033[0m" << std::endl;

  std::basic_string<uint8_t> Key;
  bool searching = true;

  // For each keysize we want to try
  for (size_t k = 0; k < n_keysizes; ++k) {
      // TODO Keep all keys found and only return once we have compared them all
      if (not searching) {
          break;
      }

      size_t keysize = keysizes[k];
      std::cout << "Keysize: \033[1m" << keysize << "\033[0m" << std::endl;

      Key.clear();

      // For each character of the key
      for (size_t i = 0; i < keysize; ++i) {

          // Group together all bytes of the string that are encoded using that character
          std::basic_string<uint8_t> group;

          for (size_t p = i; p < enc.size(); p += keysize) {
              group.push_back(enc[p]);
          }

          std::cout << "Group: " << bytes::hex::encode(group) << std::endl;

          // Since our method for breaking single-byte binary-wise xor encodings
          // relies only on character frequency, it is perfectly applicable here
          // and we can use it to retrieve the character used to encode that group,
          // thus providing us with the next character of the hidden key.
          auto [kc, ok] = bytes::break_single_byte_xor(group.data(), group.size());

          // TODO Remove this possible I/O error by doing the statistics fetching at init
          if (not ok) {
              std::cerr << "I/O error during call to break_single_byte_xor" << std::endl;
              return EXIT_FAILURE;
          }

          // In case the algorithm didn't find a viable non-trivial key
          // TODO Implement a more robust way for the algorithm to indicate it failed
          if (kc == 0) {
              std::cout << "Giving up on character " << Key.size() << std::endl;
              break;
          }

          Key.push_back(kc);
          std::cout << "Found character " << Key.size() << " of " << keysize << std::endl;

          if (Key.size() == keysize) {
              searching = false;
          }
      }
  }

  if (Key.empty()) {
      std::cout << "Failed to find a key" << std::endl;
      return EXIT_FAILURE;
  }

  std::cout << "Key: \033[1m";
  for (uint8_t b : Key) {
    std::cout << (unsigned)b;
  }
  std::cout << "\033[0m" << std::endl;

  // Decode the message.
  auto Dec = bytes::bitwise_xor(Key.data(), Key.size(), enc.data(), enc.size());

  std::cout << "Output string:\n\n  \033[1m";
  for (auto b : Dec) {
      std::cout << b;
  }
  std::cout << "\033[0m\n" << std::endl;

  return EXIT_SUCCESS;
}
