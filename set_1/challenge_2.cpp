#include "bitwise_xor.h"
#include "utils/printer.h"

#include <iostream>
#include <fstream>

int main(int argc, char *argv[]) {

    std::string string;
    std::string key;
    std::string expected_result;

    if (argc > 2) {
        std::cin >> key;
        std::cin >> string;
    }
    else {
        const std::string fn = DATA_DIR "/set_1/challenge_2.txt";
        std::ifstream ifs{ fn };
        if (not ifs) {
          std::cerr << "Failed to open input file '" << fn << '\'' << std::endl;
          return EXIT_FAILURE;
        }

        ifs >> key;
        ifs >> string;
        ifs >> expected_result;
    }

    cryptopals::print_bold(std::cout, "Inputs:\n  ")
      << string;
    cryptopals::print_bold(std::cout, "\nand\n  ")
      << key << std::endl;

    auto result = bytes::bitwise_xor(key, string);

    cryptopals::print_bold(std::cout, "\nBitwise-XOR:\n  ")
      << result << std::endl;

    if (result == expected_result) {
      cryptopals::print_success(std::cout) << std::endl;
      return EXIT_SUCCESS;
    } else {
      cryptopals::print_failure(std::cout, "\nExpected result:\n  ")
        << expected_result << std::endl;
      return EXIT_FAILURE;
    }
}
