#include "bytes.h"
#include "utils/printer.h"

#include <iostream>
#include <fstream>


int main(int argc, char *argv[]) {

    std::string string;
    std::string expected_result;

    const auto input_file = DATA_DIR "/set_1/challenge_1.txt";
    std::ifstream ifs{ input_file };
    if (not ifs) {
      std::cerr << "Could not open input file: '" << input_file << '\'' << std::endl;
      return EXIT_FAILURE;
    }

    ifs >> string;
    ifs >> expected_result;

    auto result = bytes::hex_2_base64(string);

    cryptopals::print_bold(std::cout, "Input:\n  ") << string << std::endl;
    cryptopals::print_bold(std::cout, "\nHex->Base64:\n  ") << result << std::endl;

    if (result == expected_result) {
      cryptopals::print_success(std::cout) << std::endl;
      return EXIT_SUCCESS;
    } else {
      cryptopals::print_failure(std::cout, "\nExpected:\n  ") << expected_result << std::endl;
      return EXIT_FAILURE;
    }
}
