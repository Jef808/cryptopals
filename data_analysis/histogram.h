#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include <cstdint>
#include <string>
#include <vector>

namespace bytes::utils {

/**
 * Display a basic histogram on the console displaying data indexed by a uint8_t byte.
 */
void show_histogram(const std::vector<std::pair<uint8_t, double>>& /* indexed quantities */,
                    bool /* highlight_min */ = true);

/**
 * Display a basic histogram on the console displaying data indexed by a uint8_t byte.
 */
void show_table(const std::vector<std::pair<uint8_t, double>>& /* indexed quantities */,
                const std::string& /* keys */ = "",
                const std::string& /* values */ = "",
                bool /* highlight_min */ = true);

template<typename InputIter>
inline void show_histogram(InputIter beg, InputIter end, bool highlight_min = true) {
  show_histogram(std::vector<std::pair<uint8_t, double>>(beg, end), highlight_min);
}

template<typename InputIter>
inline void show_table(InputIter beg, InputIter end,
                       const std::string& keys = "",
                       const std::string& values = "",
                       bool minimize_value = true) {
  show_table(std::vector<std::pair<uint8_t, double>>(beg, end),
             keys, values,
             minimize_value);
}

}  // namespace bytes::utils

#endif // HISTOGRAM_H_
