#include "bitwise_xor.h"

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

    std::cout << "\nbitwise xor-ing string\n" << "  " << string
              << "\nagainst key\n" << "  " << key << "..." << std::endl;

    auto res = bytes::bitwise_xor(key, string);

    std::cout << "Result:\n  "
              << res
              << "\nExpected result:\n  "
              << expected_result
              << "\n\n"
              << (res == expected_result ? "SUCCESS!" : "FAIL") << std::endl;

    return EXIT_SUCCESS;
}
