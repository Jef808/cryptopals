#include "bytes.h"

#include <iostream>


int main(int argc, char *argv[]) {

    std::string string;

    if (argc > 1) {
      string = argv[1];
    }
    else {
      std::cout << "Please enter hex string..." << std::endl;
      std::cin >> string;
    }

    std::cout << "Converting from hex to base64..." << std::endl;
    std::cout << bytes::hex_2_base64(string) << std::endl;

    return 0;
}
