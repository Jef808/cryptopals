#include "bytes.h"
#include "bitwise_xor.h"
#include "english_text_metric.h"

#include <fstream>
#include <iostream>
#include <vector>


int main(int argc, char *argv[]) {

  std::vector<std::string> encoded_hex;

  const std::string fn = DATA_DIR "/set_1/challenge_3.txt";
  std::ifstream ifs{ fn };
  if (not ifs) {
    std::cerr << "Failed to open input file " << fn << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<std::basic_string<uint8_t>> raw_strings;

  while (std::getline(ifs, encoded_hex.emplace_back())) {
    raw_strings.push_back(bytes::hex::decode(encoded_hex.back()));
  }



  return EXIT_SUCCESS;
}
