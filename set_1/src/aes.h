#ifndef AES_H_
#define AES_H_

namespace aes {

using Byte = unsigned char;

void aesEncode(Byte* plaintext, const Byte* key);
void aesDecode(Byte* ciphertext, const Byte* key);

}  // namespace aes

#endif // AES_H_
