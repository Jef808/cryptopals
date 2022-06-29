#include "bytes.h"
#include "bitwise_xor.h"

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>


int main(int argc, char *argv[]) {
    const std::string fn = DATA_DIR "/set_1/challenge_3.txt";
    std::ifstream ifs{ fn };
    if (not ifs) {
        std::cerr << "Failed to open input file " << fn << std::endl;
        return EXIT_FAILURE;
    }
    std::string hex_buf;
    ifs >> hex_buf;
    std::cout << "Input string:\n\n  " << hex_buf << std::endl;

    auto bs = bytes::hex::decode(hex_buf);
    auto [key, ok] = bytes::break_single_byte_xor(&bs[0], bs.size());

    if (not ok) {
        std::cerr << "Call to break_single_byte_xor failed" << std::endl;
        return EXIT_FAILURE;
    }

    auto dec = bytes::bitwise_xor(&key, 1, &bs[0], bs.size());

    std::cout << "\nKey: \033[1m" << (unsigned)key << "\033[0m"
              << "\nOutput string:\n\n  \033[1m";
    for (auto b : dec) { std::cout << b; }
    std::cout << "\033[0m\n" << std::endl;

    return EXIT_SUCCESS;
}
