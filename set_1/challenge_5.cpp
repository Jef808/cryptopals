#include "bytes.h"
#include "bitwise_xor.h"

#include <algorithm>
#include <cstdint>
#include <string>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {

    std::basic_string<uint8_t> Key
        { 'I', 'C', 'E' };

    std::string Message =
        "Burning 'em, if you ain't quick and nimble\n"
        "I go crazy when I hear a cymbal";

    std::basic_string<uint8_t> RawMessage =
        reinterpret_cast<uint8_t*>(Message.data());

    std::basic_string<uint8_t> Encoded =
        bytes::bitwise_xor(
            Key.data(), Key.size(), RawMessage.data(), RawMessage.size());

    std::string ExpectedResult =
        "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272"
        "a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";

    std::string Result = bytes::hex::encode(Encoded);

    std::cout << "\nExpected result:\n  "
              << ExpectedResult
              << "\n\nResult:\n  "
              << Result << std::endl;

    std::cout << "\n" << (ExpectedResult == Result ? "Success!" : "Failed!") << std::endl;

    return 0;
}
