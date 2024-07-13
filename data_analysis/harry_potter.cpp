/**
/* Generate the character frequency data from the Harry Potter books
*/

#include "data_analysis/statistics.h"

#include <array>
#include <cassert>
#include <fstream>
#include <string>


constexpr std::array<const char*, 7> fns = {
  DATA_DIR "/harry_potter/Book1.txt",
  DATA_DIR "/harry_potter/Book2.txt",
  DATA_DIR "/harry_potter/Book3.txt",
  DATA_DIR "/harry_potter/Book4.txt",
  DATA_DIR "/harry_potter/Book5.txt",
  DATA_DIR "/harry_potter/Book6.txt",
  DATA_DIR "/harry_potter/Book7.txt"
};

constexpr const char* fn_out =
  DATA_DIR "/harry_potter/character_frequency.csv";


int main(int argc, char *argv[]) {
  std::array<int, 256> count;
  count.fill(0);

  data::CharFreq cfreq(count);

  std::cerr << "Initialized CharFreq object" << std::endl;

  for (size_t i = 0; i < 7; ++i) {
    bool okay = cfreq.load_data(fns[i]);
    std::cerr << "Loaded file " << fns[i] << ": "
              << std::boolalpha << okay << std::endl;
  }

  cfreq.write_data(fn_out);

  std::cerr << "Wrote to " << fn_out << std::endl;

  return EXIT_SUCCESS;
}
