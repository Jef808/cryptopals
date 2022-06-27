/**
 * For each possible byte (from 0 to 255), pretend the key
 * is that byte repeated ad-infinitum.
 * Then bitwise-xor the message with that key to get what would
 * be the decoded message, and compute a cost function for the resulting
 * string R as follows:
 *
 * 0) (0.0 <= Cost(R) <= 1.0)
 *
 * 1) Cost = 1.0 as soon as one character of R is not printable.
 *
 * 2) If all characters of R are printable, compute the manhattan
 * distance between M = "ETAOIN SHRDLU" and the the permuted sigma_R(M)
 * gotten by reordering M with respect to each of its character frequency
 * inside of R.
 * i.e. sigma_R(M) = "TEAOIN SHRDLU" if 'T' appears more often than 'E'
 * inside of R, etc...
 */
#include "bytes.h"
#include "bitwise_xor.h"
#include "english_text_metric.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>


int main(int argc, char *argv[]) {

    std::string encoded_hex;

    if (argc > 1) {
      encoded_hex = argv[1];
    }
    else {
      const std::string fn = DATA_DIR "/set_1/challenge_3.txt";
      std::ifstream ifs{ fn };
      if (not ifs) {
        std::cerr << "Failed to open input file " << fn << std::endl;
        return EXIT_FAILURE;
      }
      ifs >> encoded_hex;
    }

    std::vector<std::pair<uint8_t, double>> costs;

    std::cout << "Deciphering encoded string\n  "
              << encoded_hex << "..." << std::endl;

    std::basic_string<uint8_t> enc = bytes::hex::decode(encoded_hex);

    uint8_t best_key = 0;
    double lowest_cost = std::numeric_limits<double>::max();
    std::basic_string<uint8_t> best_dec;

    for (int k = 0; k < 256; ++k) {
      // Decode using k as key
      const uint8_t key = k;
      std::basic_string<uint8_t> dec =
        bytes::bitwise_xor(&key, 1, &enc[0], enc.size());

      double cost = bytes::metrics::english_text_cost(dec);

      // Debug
      if (cost < 1000.0)
        costs.push_back(std::make_pair(k, cost));

      // Update best-to-date.
      if (cost < lowest_cost) {
        best_key = key;
        lowest_cost = cost;
        std::swap(best_dec, dec);
      }
    }

    size_t min_col = 0;
    double min_cost = std::numeric_limits<double>::max();
    double max_cost = std::numeric_limits<double>::min();

    size_t col = 0;
    for (const auto& [k, c] : costs) {
      if (c < min_cost) {
        min_cost = c;
        min_col = col;
      }
      else if (c > max_cost) {
        max_cost = c;
      }
      ++col;
    }

    // Normalize costs
    const int max_height = 60;
    std::transform(costs.begin(), costs.end(), costs.begin(), [m=min_cost, M=max_cost](const auto& kc){
      return std::make_pair(kc.first, (1.0 + (kc.second - m) / (M - m)) * (max_height / 2));
    });

    std::cout << std::endl;

    for (size_t y = 0; y < max_height; ++y) {
      for (size_t x = 0; x < costs.size(); ++x) {
        std::cout << ' ';
        size_t x_space = max_height - costs[x].second;
        if (y < x_space) {
          std::cout << " ";
        }
        else {
          std::cout << (x == min_col ? "\033[1m" : "") << "|" << "\033[0m";
        }
        std::cout << "  ";
      }
      std::cout << '\n';
    }

    std::fill_n(std::ostream_iterator<char>(std::cout), costs.size() * 5, ' ');
    std::cout << '\n';

    for (size_t x = 0; x < costs.size(); ++x) {
      int key = (unsigned)costs[x].first;
      double logkey = std::log10(key);
      std::cout << ' '
                << (x == min_col ? "\033[1m" : "")
                << key << "\033[0m"
                << (1 < logkey && logkey < 3 ? " " : "");
    }

    std::cout << std::endl;

    // Output best result
    std::cout << "\nBest key: " << (unsigned)best_key << std::endl;
    std::cout << "Best cost: " << lowest_cost << std::endl;
    std::cout << "Decoded message:\n  ";
    for (uint8_t c : best_dec) {
        std::cout << c;
    }
    std::cout << std::endl;

    return 0;
}
