add_library(io_utils INTERFACE
  read_csv.h
)

add_library(data_statistics STATIC
  statistics.h
  statistics.cpp
  load_character_frequencies.h
  load_character_frequencies.cpp
)
target_link_libraries(data_statistics PUBLIC
  io_utils
)

add_library(data_utils STATIC
  histogram.h
  histogram.cpp
  )
target_link_libraries(data_utils PUBLIC
  fmt)


add_executable(harry_potter_data
  harry_potter.cpp)
target_link_libraries(harry_potter_data
  data_statistics)

add_executable(view_hp_data
  view_harry_potter_data.cpp)
target_link_libraries(view_hp_data
  data_utils
  fmt)
