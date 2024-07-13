#include "data_analysis/statistics.h"

#include <algorithm>
#include <cassert>
#include <cstdint>

namespace data {


CharFreq::CharFreq(std::array<int, 256> &count) : m_count{count}, n_chars{0} {}

bool CharFreq::load_data(const char *fn) {
  std::ifstream ifs{fn};
  const size_t buf_size = 6 * 1024 * 1024;
  std::string buf;
  buf.resize(buf_size, '\0');
  if (not ifs) {
    std::cerr << "Failed to load input file " << fn << std::endl;
    return false;
  }

  while (ifs) {
    ifs.read(&buf[0], buf_size);
    const char *p = &buf[0];
    size_t chars_read = ifs.gcount();

    assert(buf.size() > chars_read &&
           "Buffer is not bigger than number of chars read");

    buf.resize(chars_read);

    std::cerr << "Read " << chars_read << " chars into buffer" << std::endl;

    for (size_t i = 0; i < chars_read; ++i) {
      auto c = static_cast<uint8_t>(buf[i]);
      ++m_count[c];
    }
    n_chars += chars_read;
  }

  return true;
}

bool CharFreq::write_data(const char *fn) {
  std::ofstream ofs{fn};
  if (not ofs) {
    std::cerr << "Failed to open output file " << fn << std::endl;
    return false;
  }
  for (size_t i = 0; i < m_count.size() - 1; ++i) {
    ofs << i << ',';
  }
  ofs << m_count.size() - 1 << '\n';
  for (auto it = m_count.begin(); it < m_count.end() - 1; ++it) {
    ofs << *it << ',';
  }
  ofs << m_count.back() << std::endl;
  return true;
}


} // namespace data
