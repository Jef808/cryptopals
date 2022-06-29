#include "metrics.h"

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <bit>

namespace bytes::metrics {

bool is_text(const std::basic_string<uint8_t>& s)
{
  return std::all_of(s.begin(), s.end(), [](uint8_t c) {
    return std::isprint(c) || std::isspace(c);
  });
}


double L2_distance_squared(const std::array<double, 256>& F,
                           const std::array<double, 256>& G)
{
  constexpr auto ReductionOp = std::plus<>{};
  constexpr auto TransformOp = [](auto Fx, auto Gx) {
    return (Fx - Gx) * (Fx - Gx);
  };

  return 0.00380625 * std::transform_reduce(
    F.begin(), F.end(), G.begin(), 0.0, ReductionOp, TransformOp);
}


int hamming_distance(const uint8_t* s, const uint8_t* t, const size_t len)
{
  return std::transform_reduce(s, s + len, t, 0, std::plus<>{},
    [](auto a, auto b) {
      //uint8_t c = a ^ b;
      return std::popcount(static_cast<uint8_t>(a ^ b));
    });
}



}  // namespace bytes::metrics
