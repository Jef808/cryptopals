#ifndef AES_H_
#define AES_H_

namespace aes {

using Byte = unsigned char;

void subBytes(Byte* state);
void invSubBytes(Byte* state);
void shiftRows(Byte* state);
void invShiftRows(Byte* state);
void mixColumns(Byte* state);
void invMixColumns(Byte* state);
void addRoundKey(Byte* state, const Byte* roundKey);
void keyExpansion(const Byte* key, Byte* expandedKey);

void aesEncode(Byte* plaintext, const Byte* key);
void aesDecode(Byte* ciphertext, const Byte* key);

}  // namespace aes

#endif // AES_H_
