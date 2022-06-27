#ifndef ENGLISH_TEXT_METRIC_H_
#define ENGLISH_TEXT_METRIC_H_

#include <cstdint>
#include <string>


namespace bytes::metrics {


/**
 * Compare the character frequency ordering with ETAOIN SHRDLU.
 *
 * A lower returned value indicates a higher likelyhood of the
 * input string being real-world English text, and vice-versa.
 */
double english_text_cost(const std::basic_string<uint8_t>& /* Input string */);



} // namespace bytes::metrics


#endif // ENGLISH_TEXT_METRIC_H_
