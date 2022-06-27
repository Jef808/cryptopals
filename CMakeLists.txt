cmake_minimum_required(VERSION 3.22)
project(cryptopals)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_library(data_dir INTERFACE)
target_compile_definitions(data_dir INTERFACE
  "-DDATA_DIR=\"${CMAKE_SOURCE_DIR}/data\"")
link_libraries(data_dir)

add_subdirectory(set_1)

add_library(cryptopals::bytes ALIAS
  set_1_bytes)
add_library(cryptopals::text_metrics ALIAS
  set_1_text_metrics)
