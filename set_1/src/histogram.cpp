#include "histogram.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <limits>

namespace bytes::utils {


void show_histogram(const std::vector<std::pair<uint8_t, double>>& data) {
  const int max_height = 60;
  size_t min_col = 0;
  double min_cost = std::numeric_limits<double>::max();
  double max_cost = std::numeric_limits<double>::min();

  size_t col = 0;
  for (const auto& [k, c] : data) {
    if (c < min_cost) {
      min_cost = c;
      min_col = col;
    }
    else if (c > max_cost) {
      max_cost = c;
    }
    ++col;
  }

  // Flush output stream.
  std::cout << std::endl;

  for (size_t y = 0; y < max_height; ++y) {
    for (size_t x = 0; x < data.size(); ++x) {

      // Normalize the data
      double rdata = (1.0 + (data[x].second - min_cost) / (max_cost - min_cost)) * (max_height / 2.0);
      std::cout << ' ';
      size_t x_space = max_height - rdata;
      if (y < x_space) {
        std::cout << " ";
      }
      else {
        // Use bold face for the min column.
        std::cout << (x == min_col ? "\033[1m" : "") << "|" << "\033[0m";
      }
      std::cout << "  ";
    }
    std::cout << '\n';
  }

  std::fill_n(std::ostream_iterator<char>(std::cout), data.size() * 5, ' ');
  std::cout << '\n';

  for (size_t x = 0; x < data.size(); ++x) {
    int key = (unsigned)data[x].first;
    double logkey = std::log10(key);
    std::cout << ' '
              << (x == min_col ? "\033[1m" : "")
              << key << "\033[0m"
              << (1 < logkey && logkey < 3 ? " " : logkey == 1 ? "  " : "");
  }

  std::cout << std::endl;
}

} // namespace bytes::utils
