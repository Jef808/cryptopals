#include "base64.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

template <typename InputIter, typename VT = typename InputIter::value_type>
std::ostream &print_seq(std::ostream &out, InputIter beg, InputIter end,
                        bool bold = false) {
  using OStreamIter = std::ostream_iterator<VT>;
  if (bold) {
    out << "\033[1m";
  }
  std::copy(beg, end, OStreamIter(out));
  return out << "\033[0m";
}

int main(int argc, char *argv[]) {

  assert(bytes::base64::encode("") == "");
  assert(bytes::base64::encode("f") == "Zg==");
  assert(bytes::base64::encode("fo") == "Zm8=");
  assert(bytes::base64::encode("foo") == "Zm9v");
  assert(bytes::base64::encode("foob") == "Zm9vYg==");
  assert(bytes::base64::encode("fooba") == "Zm9vYmE=");
  assert(bytes::base64::encode("foobar") == "Zm9vYmFy");

  constexpr std::array<const char *, 3> inputs = {
      TESTS_DIR "/data/test_base64_input1",
      TESTS_DIR "/data/test_base64_input2",
      TESTS_DIR "/data/test_base64_input3"};
  constexpr std::array<const char *, 3> expected_outputs = {
      TESTS_DIR "/data/test_base64_expected_output1.txt",
      TESTS_DIR "/data/test_base64_expected_output2.txt",
      TESTS_DIR "/data/test_base64_expected_output3.txt"};
  std::array<std::vector<unsigned int>, 3> test_inputs;
  std::array<std::string, 3> test_expected_outputs;
  std::array<std::string, 3> test_outputs;
  std::string buf_in;
  std::basic_string<uint8_t> bbuf_in;
  for (size_t i = 0; i < 3; ++i) {
    int len = 1022 + i;
    {
      std::ifstream ifs_in{inputs[i], std::ios_base::binary};
      if (not ifs_in) {
        std::cerr << "Failed to open input file " << inputs[i] << std::endl;
        return EXIT_FAILURE;
      }
      std::ifstream ifs_out{expected_outputs[i]};
      if (not ifs_out) {
        std::cerr << "Failed to open input file " << expected_outputs[i]
                  << std::endl;
        return EXIT_FAILURE;
      }
      buf_in.resize(len);
      ifs_in.read(buf_in.data(), len);
      ifs_out >> test_expected_outputs[i];
    }
    bbuf_in = reinterpret_cast<uint8_t *>(buf_in.data());
    std::transform(bbuf_in.begin(), bbuf_in.end(),
                   std::back_inserter(test_inputs[i]),
                   [](auto c) { return (unsigned int)(c); });
    test_outputs[i] = bytes::base64::encode(bbuf_in);
  }
  std::cout << "TEST " << __FILE_NAME__ << ":\n";
  for (size_t i = 0; i < 3; ++i) {
    bool passed = test_outputs[i] == test_expected_outputs[i];
    std::cout << "\n  test " << i + 1 << ": \033[1m"
              << (passed ? "PASSED" : "FAILED") << std::endl;
    if (not passed) {
      size_t m =
          std::min(test_expected_outputs[i].size(), test_outputs[i].size());
      std::cout << "\033[0m  Expected output: (length "
                << test_expected_outputs[i].size() << ")\n";
      print_seq(std::cout, test_expected_outputs[i].begin(),
                test_expected_outputs[i].begin() + m);
      if (m < test_expected_outputs[i].size()) {
        print_seq(std::cout, test_expected_outputs[i].begin() + m,
                  test_expected_outputs[i].end(), true);
      }
      std::cout << "\033[0m\n  Output: (length " << test_outputs[i].size()
                << ")\n    ";
      bool first_bad_found = false;
      size_t b = m;
      for (size_t j = 0; j < m; ++j) {
        if (test_outputs[i][j] != test_expected_outputs[i][j]) {
          if (not first_bad_found) {
            b = j;
            first_bad_found = true;
          }
          std::cout << "\033[1m" << test_outputs[i][j] << "\033[0m";
        } else {
          std::cout << test_outputs[i][j];
        }
      }
      if (m < test_outputs[i].size()) {
        print_seq(std::cout, test_outputs[i].begin() + m, test_outputs[i].end(),
                  true);
      }
      std::cout << "\n  Input: (length " << test_inputs[i].size() << ")\n\\";
      std::copy(test_inputs[i].begin(), test_inputs[i].end(),
                std::ostream_iterator<unsigned int>(std::cout, "\\"));
      std::cout << std::endl;
    }
  }

  return EXIT_SUCCESS;
}
