#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

void pad_pkcs7(unsigned char* block, uint8_t current_size, uint8_t target_size) {
    assert(current_size <= target_size && "current_size must not be larger than target_size");
    assert(target_size - current_size < 256 && "The difference between target_size and current_size must be less than 256");

    int padding_size = target_size - current_size;
    unsigned char padding_byte = static_cast<uint8_t>(padding_size);

    for (int i = 0; i < padding_size; ++i) {
        *(block + target_size - padding_size + i) = padding_byte;
    }
}

int main() {
    unsigned char block[20 + 1] = "YELLOW SUBMARINE";

    pad_pkcs7(block, 16, 20);

    if (not memcmp(block, "YELLOW SUBMARINE\x04\x04\x04\x04", 16)) {
        std::cout << "YELLOW SUBMARINE\\x04\\x04\\x04\\x04" << std::endl;
    } else {
        std::cout << "result different than expected" << std::endl;
    }
}
