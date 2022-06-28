#include <data_analysis/histogram.h>

#include <algorithm>
#include <cstdint>
#include <vector>
#include <iostream>
#include <iomanip>
#include <locale>
#include <numeric>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

#include <fmt/core.h>

const char* fn = DATA_DIR "/harry_potter/character_frequency.csv";

int main(int argc, char *argv[]) {
    std::ifstream ifs{ fn };
    if (not ifs) {
        std::cerr << "Failed to open data file " << fn << std::endl;
        return EXIT_FAILURE;
    }

    std::cerr << "Succesfully loaded data file " << fn << std::endl;

    std::vector<std::pair<uint8_t, double>> data;
    std::string buf;

    // Labels
    std::getline(ifs, buf);

    // Data
    std::getline(ifs, buf);

    std::cerr << "Successfully read csv line" << std::endl;
    size_t n_items = std::count(buf.begin(), buf.end(), ',') + 1;
    std::cerr << "Got " << n_items << " items" << std::endl;

    std::string_view sv{ buf };
    double total = 0.0;

    std::string int_buf;
    int punctuation_count = 0;
    int space_count = 0;

    for (size_t i = 0; i < n_items; ++i) {
      // Get the next value
      size_t comma = sv.find_first_of(',');
      int val = std::stoi(sv.substr(0, comma).data());
      sv.remove_prefix(comma + 1);

      // Discard non-printable and non-existent characters
      if (val == 0 || not std::isprint(i)) { continue; }

      // Store all other character data
      auto& [n, v] = data.emplace_back();
      n = static_cast<uint8_t>(i);

      // Adjust the total
      total += v = static_cast<double>(val);

      // Gather punctuation frequency
      if (std::ispunct(n)) {
          punctuation_count += val;
      }

      // Gather word data
      if (std::isspace(n)) {
          space_count += val;
      }
    }

    // Divide by computed total number of characters to get
    // the frequency
    std::transform(data.begin(), data.end(), data.begin(), [t=total](auto& kv) {
      return std::make_pair(kv.first, kv.second / t);
    });
    double punctuation_freq = punctuation_count / total;
    double space_freq = space_count / total;
    double avg_word_len = (total - space_count) / (space_count + 1);

    const size_t n_elements = 21;
    std::partial_sort(
        data.begin(),
        data.begin() + n_elements,
        data.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

    for (auto it = data.begin(); it < data.begin() + n_elements; ++it) {
      bool is_print = std::isprint(it->first);
      const char c = it->first;
      std::string s =
          is_print ? std::string(1, c) : std::to_string((unsigned)it->first);

      fmt::print("Char: {0}{1}{0}, Freq: {2:.2}%\n",
                 is_print ? "'" : "",
                 s,
                 100.0 * it->second);
    }
    fmt::print("\nPunctuation frequency: {0:.2f}%\n", 100.0 * punctuation_freq);
    fmt::print("Average word length: {0:.2f} characters\n", avg_word_len);
    fmt::print("Data sample consists of {0} characters\n", (int)total);

    //bytes::utils::show_histogram(data.begin(), data.begin() + N, false);
    bytes::utils::show_table(data.begin(), data.begin() + n_elements,
                             "Char", "Freq", false);

    return EXIT_SUCCESS;
}
