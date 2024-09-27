#include "aes.h"
#include <catch2/catch_test_macros.hpp>

using namespace aes;

unsigned char* create_state(const char* hex_string) {
    static unsigned char state[16];
    for (int i = 0; i < 16; ++i) {
        sscanf(hex_string + 2*i, "%2hhx", &state[i]);
    }
    return state;
}

TEST_CASE("AES-128 SubBytes", "[aes128]") {
    unsigned char* state = create_state("00102030405060708090a0b0c0d0e0f0");
    subBytes(state);
    REQUIRE(memcmp(state, "\x63\xca\xb7\x04\x09\x53\xd0\x51\xcd\x60\xe0\xe7\xba\x70\xe1\x8c", 16) == 0);
}

TEST_CASE("AES-128 SubBytes and InvSubBytes", "[aes128]") {
    unsigned char* state = create_state("8e9ff1c64ddce1c7a158d1c8bc9dc1c9");
    subBytes(state);
    REQUIRE(memcmp(state, "\x19\xdb\xa1\xb4\xe3\x86\xf8\xc6\x32\x6a\x3e\xe8\x65\x5e\x78\xdd", 16) == 0);
    invSubBytes(state);
    REQUIRE(memcmp(state, "\x8e\x9f\xf1\xc6\x4d\xdc\xe1\xc7\xa1\x58\xd1\xc8\xbc\x9d\xc1\xc9", 16) == 0);
}

TEST_CASE("AES-128 ShiftRows", "[aes128]") {
    unsigned char* state = create_state("63cab7040953d051cd60e0e7ba70e18c");
    shiftRows(state);
    REQUIRE(memcmp(state, "\x63\x53\xe0\x8c\x09\x60\xe1\x04\xcd\x70\xb7\x51\xba\xca\xd0\xe7", 16) == 0);
}

TEST_CASE("AES-128 MixColumns", "[aes128]") {
    unsigned char* state = create_state("6353e08c0960e104cd70b751bacad0e7");
    mixColumns(state);
    REQUIRE(memcmp(state, "\x5f\x72\x64\x15\x57\xf5\xbc\x92\xf7\xbe\x3b\x29\x1d\xb9\xf9\x1a", 16) == 0);
}

TEST_CASE("AES-128 InvMixColums", "[aes128]") {
    unsigned char* state = create_state("8e9ff1c64ddce1c7a158d1c8bc9dc1c9");
    mixColumns(state);
    invMixColumns(state);
    REQUIRE(memcmp(state, "\x8e\x9f\xf1\xc6\x4d\xdc\xe1\xc7\xa1\x58\xd1\xc8\xbc\x9d\xc1\xc9", 16) == 0);
}

TEST_CASE("AES-128 AddRoundKey", "[aes128]") {
    unsigned char* state = create_state("5f72641557f5bc92f7be3b291db9f91a");
    unsigned char round_key[16] = {
        0xd6, 0xaa, 0x74, 0xfd, 0xd2, 0xaf, 0x72, 0xfa,
        0xda, 0xa6, 0x78, 0xf1, 0xd6, 0xab, 0x76, 0xfe
    };
    addRoundKey(state, round_key);
    REQUIRE(memcmp(state, "\x89\xd8\x10\xe8\x85\x5a\xce\x68\x2d\x18\x43\xd8\xcb\x12\x8f\xe4", 16) == 0);
}

TEST_CASE("AES-128 KeyExpansion", "[aes128]") {
    unsigned char key[16] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    unsigned char expanded_key[176];
    keyExpansion(key, expanded_key);

    // Check the first round key (original key)
    REQUIRE(memcmp(expanded_key, key, 16) == 0);

    // Check the second round key
    unsigned char expected_second_round_key[16] = {
        0xa0, 0xfa, 0xfe, 0x17, 0x88, 0x54, 0x2c, 0xb1,
        0x23, 0xa3, 0x39, 0x39, 0x2a, 0x6c, 0x76, 0x05
    };
    REQUIRE(memcmp(expanded_key + 16, expected_second_round_key, 16) == 0);
}

TEST_CASE("AES-128 Encode", "[aes128]") {
    unsigned char key[17] = "YELLOW SUBMARINE";
    unsigned char plaintext[17] = "hello this is me";

    aesEncode(plaintext, key);
    REQUIRE(memcmp(plaintext, "\xa6\x5b\x96\x48\x1b\xf5\x52\x9d\x20\x59\xb0\xb7\x15\x25\xd0\x7c", 16) == 0);
}
