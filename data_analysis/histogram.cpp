#include "histogram.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iterator>
#include <limits>
#include <numeric>

#include <fmt/core.h>
#include <fmt/color.h>
//#include <fmt/ostream.h>

#include <iostream>


namespace bytes::utils {

void show_table(const std::vector<std::pair<uint8_t, double>>& data,
                const std::string& keys,
                const std::string& values,
                bool minimize_value) {
  size_t highlighted_col = 0;
  double min_cost = std::numeric_limits<double>::max();
  double max_cost = std::numeric_limits<double>::min();

  size_t col = 0;
  for (const auto& [k, c] : data) {
    if (c < min_cost) {
      min_cost = c;
      if (minimize_value) {
        highlighted_col = col;
      }
    }
    else if (c > max_cost) {
      max_cost = c;
      if (not minimize_value) {
        highlighted_col = col;
      }
    }
    ++col;
  }


  auto flags_normal = fmt::emphasis::bold | fg(fmt::terminal_color::white);
  auto flags_hot    = fmt::emphasis::bold | fg(fmt::terminal_color::bright_yellow);
  auto flags_medium = fmt::emphasis::bold | fg(fmt::terminal_color::bright_blue);
  auto flags_cold   = fmt::emphasis::faint| fg(fmt::terminal_color::white);
  auto flags_titles = fmt::emphasis::bold | fg(fmt::terminal_color::bright_magenta);
  auto flags_bars   = fmt::emphasis::bold | fg(fmt::terminal_color::white);

  double total = std::accumulate(data.begin(), data.end(), 0.0, [](double s, const auto& d){
    return s + d.second;
  });

  auto get_heat = [&](double value) {
    const double val = (value - min_cost) / (max_cost - min_cost);
    bool flip = not minimize_value;
    if (flip) {
      return value > 0.09 ? flags_hot : value > 0.03 ? flags_medium : flags_cold;
    }
    return val < 0.1 ? flags_hot : val < 0.4 ? flags_medium : flags_cold;
  };

  fmt::print(flags_titles, "\n{0} ", not values.empty() ? values : "Value");
  fmt::print(flags_bars, "|");
  for (size_t x = 0; x < data.size(); ++x) {
    double rdata =
      // 100.0 * (data[x].second - min_cost) / (max_cost - min_cost);
      100.0 * data[x].second;
    auto flags = x == highlighted_col ? flags_hot : get_heat(data[x].second);
    fmt::print(flags,
               "{2}{0:.1f}{1}", rdata, rdata < 10.0 ? " " : "", rdata < 100.0 ? " " : "");
    fmt::print(flags_bars, "|");
  }

  fmt::print(flags_titles, "\n{0} ", not keys.empty() ? keys : "Keys");
  fmt::print(flags_bars, "|");
  for (size_t x = 0; x < data.size(); ++x) {
    bool is_print = std::isprint(data[x].first);
    int key = (unsigned)data[x].first;
    double logkey = 1 + std::log10(key);
    const char c = data[x].first;
    std::string s = is_print
      ? std::string(1, '\'') + std::string(1, c) + std::string(1, '\'')
      : std::to_string(key);
    size_t sz =
      is_print ? 3
      : std::floor(logkey);
    size_t s1 = sz < 4 ? 1 : 0;
    size_t s2 = 5 - sz - 1;
    auto flags = x == highlighted_col ? flags_hot : get_heat(data[x].second);

    fmt::print(flags,
               "{1}{0}{2}", s,
               std::string(s1, ' '),
               std::string(s2, ' '));
    fmt::print(flags_bars, "|");
  }
  fmt::print("\n");
}

void show_histogram(const std::vector<std::pair<uint8_t, double>>& data,
                    bool highlight_min) {
  const int max_height = 60;
  size_t highlighted_col = 0;
  double min_cost = std::numeric_limits<double>::max();
  double max_cost = std::numeric_limits<double>::min();

  size_t col = 0;
  for (const auto& [k, c] : data) {
    if (c < min_cost) {
      min_cost = c;
      if (highlight_min) {
        highlighted_col = col;
      }
    }
    else if (c > max_cost) {
      max_cost = c;
      if (not highlight_min) {
        highlighted_col = col;
      }
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
        // Use bold face for the min or max column.
        std::cout << (x == highlighted_col
                      ? "\033[1m" : "") << "|" << "\033[0m";
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
              << (x == highlighted_col ? "\033[1m" : "");
    if (std::isprint(data[x].first)) {
      std::cout << '\'' << data[x].first << "'\033[0m"
                << " ";
    } else {
      std::cout << (unsigned)data[x].first << "\033[0m"
                << (1 < logkey && logkey < 3 ? " " : logkey == 1 ? "  " : "");
    }

  }

  std::cout << std::endl;
}

} // namespace bytes::utils
