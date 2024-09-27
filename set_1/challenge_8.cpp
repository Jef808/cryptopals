#include "aes.h"
#include "hex.h"
#include <argparse/argparse.hpp>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    argparse::ArgumentParser parser("Challenge 7 - AES128");

    parser.add_argument("-k")
        .help("A 16 character key for encoding plaintext");
    parser.add_argument("file");

    try {
        parser.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
        return EXIT_FAILURE;
    }

    const auto& input_file = parser.get<std::string>("file");

    std::ifstream ifs{input_file};
    if (not ifs) {
        std::cerr << "Failed to open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::basic_string<unsigned char>> ciphers;
    std::string buf;
    while (std::getline(ifs, buf)) {
        auto cipher = bytes::hex::decode(buf.data(), buf.size());
        ciphers.push_back(cipher);

    }

    // Find ciphers with repeating blocks of 16 bytes
    std::vector<size_t> candidates;
    std::basic_string<unsigned char> block1;
    std::basic_string<unsigned char> block2;
    for (size_t i = 0; i < ciphers.size(); ++i) {
        const auto& cipher = ciphers[i];
        size_t n_blocks = cipher.size() / 16;

        for (size_t j = 0; j < n_blocks - 1; ++j) {
            bool found = false;
            for (size_t k = j + 1; k < n_blocks; ++k) {
                block1.clear();
                block2.clear();
                std::copy(cipher.begin() + 16*j, cipher.begin() + 16*(j+1), std::back_inserter(block1));
                std::copy(cipher.begin() + 16*k, cipher.begin() + 16*(k+1), std::back_inserter(block2));
                if (block1 == block2) {
                    candidates.push_back(i);
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
    }

    std::cout << "Found \033[1m" << candidates.size() << " \033[0mcipher(s) having repeating 16-byte blocks:" << std::endl;
    for (auto i : candidates) {
        std::cout << "\n  Index \033[1m" << i << "\033[0m:\n";
        std::cout << bytes::hex::encode(ciphers[i]) << std::endl;
    }
}
