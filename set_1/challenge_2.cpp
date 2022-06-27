#include "bitwise_xor.h"

#include <iostream>


int main(int argc, char *argv[]) {

    std::string string;
    std::string key;

    if (argc > 2) {
        std::cin >> key;
        std::cin >> string;
    }
    else {
        std::cout << "Please input hex-encoded key..." << std::endl;
        std::cin >> key;
        std::cout << "Please input hex-encoded message..." << std::endl;
        std::cin >> string;
    }

    std::cout << "\nbitwise xor-ing message against key...\n" << std::endl;

    std::cout << "Result: "
              << bytes::bitwise_xor(key, string) << std::endl;

    return 0;
}
