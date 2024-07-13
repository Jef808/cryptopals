/**
/* Generate the character frequency data from the Harry Potter books
*/

#include "data_analysis/statistics.h"

#include <array>
#include <cassert>
#include <fstream>
#include <string>


constexpr const char* fn_in =
  DATA_DIR "/harry_potter/Harry_Potter_all_books_preprocessed.txt";

constexpr const char* fn_out =
  DATA_DIR "/harry_potter/character_frequency.csv";


int main(int argc, char *argv[]) {
  std::array<int, 256> count;
  count.fill(0);

  data::CharFreq cfreq(count);

  std::cerr << "Initialized CharFreq object" << std::endl;

  bool okay = cfreq.load_data(fn_in);
  std::cerr << "Loaded file " << fn_in << ": "
    << std::boolalpha << okay << std::endl;

  cfreq.write_data(fn_out);

  std::cerr << "Wrote to " << fn_out << std::endl;

  return EXIT_SUCCESS;
}
