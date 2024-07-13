add_library(io_utils INTERFACE read_csv.h)
add_library(cryptopals::io_utils ALIAS io_utils)

add_library(data_utils STATIC
  histogram.h
  histogram.cpp)
target_link_libraries(data_utils PUBLIC
  fmt)
add_library(cryptopals::data_utils ALIAS data_utils)

add_library(statistics STATIC
  statistics.h
  statistics.cpp
  load_character_frequencies.h
  load_character_frequencies.cpp)
add_library(cryptopals::statistics ALIAS statistics)


#####################################################################
# Generate the character frequency data from the Harry Potter books #
#####################################################################
add_executable(harry_potter_data
  harry_potter.cpp)
target_link_libraries(harry_potter_data
  cryptopals::io_utils
  cryptopals::statistics)

#####################################################################
# View the character frequency data from the Harry Potter books     #
#####################################################################
add_executable(view_hp_data
  view_harry_potter_data.cpp)
target_link_libraries(view_hp_data
  cryptopals::data_utils
  fmt)
