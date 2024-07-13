#include "bytes.h"
#include "utils/printer.h"

#include <iostream>
#include <fstream>
#include <string>


void test(const std::string& string, const std::string& expected_result) {
  auto result = bytes::hex_2_base64(string);

  cryptopals::print_bold(std::cout, "Input:\n  ") << string << std::endl;
  cryptopals::print_bold(std::cout, "\nHex->Base64:\n  ") << result << std::endl;

  if (not expected_result.empty() and result != expected_result) {
    cryptopals::print_failure(std::cout, "\nExpected:\n  ") << expected_result << std::endl;
  } else if (not expected_result.empty()) {
    cryptopals::print_success(std::cout) << std::endl;
  }

  auto back = bytes::base64_2_hex(std::string(result));

  cryptopals::print_bold(std::cout, "\nBase64->Hex:\n  ") << back << std::endl;

  if (string != back) {
    cryptopals::print_failure(std::cout, "\nExpected:\n  ") << string << std::endl;
  } else {
    cryptopals::print_success(std::cout) << std::endl;
  }
}


int main(int argc, char *argv[]) {

    std::string string;
    std::string expected_result;

    if (argc > 1) {
      for (auto i = 0; i < argc-1; ++i) {
        string = argv[i+1];
        test(string, "");
      }
    } else {
      const auto input_file = DATA_DIR "/set_1/challenge_1.txt";
      std::ifstream ifs{ input_file };
      if (not ifs) {
        std::cerr << "Could not open input file: '" << input_file << '\'' << std::endl;
        return EXIT_FAILURE;
      }
      ifs >> string;
      ifs >> expected_result;

      test(string, expected_result);
    }
}
