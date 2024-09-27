#include "aes.h"
#include "bytes.h"
#include <argparse/argparse.hpp>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <fstream>

using namespace aes;

// Helper function to print hex values
void printHex(const unsigned char* data, int len) {
    for (int i = 0; i < len; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]) << ' ';
    }
    std::cout << std::endl;
}

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

    const auto& key_value = parser.get<std::string>("-k");
    const auto& input_file = parser.get<std::string>("file");

    std::ifstream ifs{input_file};
    if (not ifs) {
        std::cerr << "Failed to open input file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string buf;
    std::stringstream ss;
    while (std::getline(ifs, buf)) {
        ss << buf;
    }
    std::string string = ss.str();

    auto newline = std::find(string.begin(), string.end(), '\n');
    if (newline != string.end()) {
        std::cerr << "Found newline character in input string" << std::endl;
        return EXIT_FAILURE;
    }

    std::basic_string<uint8_t> enc = bytes::base64::decode(string.data(), string.size());

    unsigned char key[16 + 1] = {0};

    std::strncpy(reinterpret_cast<char*>(key), key_value.c_str(), key_value.size());

    float n_blocks_f = enc.size() / 16;
    int n_blocks = n_blocks_f;

    if (n_blocks < n_blocks_f) {
        std::cerr << "The last encoded block requires padding!" << std::endl;
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n_blocks; ++i) {
        aesDecode(enc.data() + 16 * i, key);
    }

    for (auto c : enc) {
        std::cout << c;
    }
    std::cout << std::endl;

    return 0;
}
