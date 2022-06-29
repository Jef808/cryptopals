#ifndef METRICS_H_
#define METRICS_H_

#include <array>
#include <cstdint>
#include <numeric>
#include <string>

namespace bytes::metrics {


/**
 * Check if all characters of string are printable or white-space.
 */
bool is_text(const std::basic_string<uint8_t>& /* Input string */);


/**
 * Compute the L2 distance squared between two mappings
 *
 * @Return Precisely $ Sum_x  |f(x) - g(x)|^2  /  256 $.
 */
double L2_distance_squared(const std::array<double, 256>& /* Input1 */,
                           const std::array<double, 256>& /* Input2 */);


/**
 * Count the number of differing bits between two strings.
 */
int hamming_distance(const uint8_t* /* string1 */,
                     const uint8_t* /* string2 */,
                     size_t         /* len */);


inline int hamming_distance(const std::basic_string<uint8_t>& string1,
                            const std::basic_string<uint8_t>& string2)
{
    return hamming_distance(
        &string1[0], &string2[0], std::min(string1.size(), string2.size()));
}


} // namesapce bytes::metrics

#endif // METRICS_H_
