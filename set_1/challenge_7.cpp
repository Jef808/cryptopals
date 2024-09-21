#include "aes.h"
#include <argparse/argparse.hpp>
#include <cstring>
#include <iomanip>
#include <iostream>

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
    parser.add_argument("input");

    try {
        parser.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
        return EXIT_FAILURE;
    }

    const auto& key_value = parser.get<std::string>("-k");
    const auto& plaintext_value = parser.get<std::string>("input");

    unsigned char key[16 + 1] = {0};
    unsigned char plaintext[plaintext_value.size() + 1] = {0};

    std::strncpy(reinterpret_cast<char*>(key), key_value.c_str(), key_value.size());
    std::strncpy(reinterpret_cast<char*>(plaintext), plaintext_value.c_str(), plaintext_value.size());

    std::cout << "\nPlaintext: " << plaintext << std::endl;
    std::cout << "(hex):     ";
    printHex(plaintext, 16);

    std::cout << "\nKey:       " << key << std::endl;

    aesEncode(plaintext, key);
    std::cout << "\nEncrypted: ";
    printHex(plaintext, 16);

    aesDecode(plaintext, key);
    std::cout << "\nDecrypted: " << plaintext << std::endl;
    std::cout << "(hex):     ";
    printHex(plaintext, 16);

    return 0;
}
