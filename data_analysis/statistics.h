#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <array>
#include <iostream>
#include <fstream>
#include <string>

namespace data {


/**
 * Helper struct to read and collect statistics on text.
 */
struct CharFreq {
  explicit CharFreq(std::array<int, 256>& /* Counter */);

  /**
   * Read a file and store the character count in @m_count
   */
  bool load_data(const char* /* Input filename */);

  /**
   * Write the data of @m_count in a csv file
   */
  bool write_data(const char* /* Output filename */);

  /** Storage for each character frequency */
  std::array<int, 256>& m_count;

  /** Number of characters collected to date */
  size_t n_chars;
};


} // namespace data

#endif // STATISTICS_H_
