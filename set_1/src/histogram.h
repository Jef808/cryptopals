#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include <cstdint>
#include <vector>

namespace bytes::utils {

/**
 * Display a basic histogram on the console displaying data indexed by a uint8_t byte.
 */
void show_histogram(const std::vector<std::pair<uint8_t, double>>& /* indexed quantities */);

}  // namespace bytes::utils

#endif // HISTOGRAM_H_
