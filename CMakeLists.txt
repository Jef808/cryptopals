cmake_minimum_required(VERSION 3.22)
project(cryptopals)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})
add_library(data_dir INTERFACE)
target_compile_definitions(data_dir INTERFACE
  "-DDATA_DIR=\"${CMAKE_SOURCE_DIR}/data\"")
link_libraries(data_dir)


find_package(fmt REQUIRED)

#########
# Set 1 #
#########
add_subdirectory(set_1)

#################
# Data analysis #
#################
add_subdirectory(data_analysis)

###################
# Library aliases #
###################
add_library(cryptopals::io_utils ALIAS
  io_utils)
add_library(cryptopals::data_utils ALIAS
  data_utils)
add_library(cryptopals::bytes ALIAS
  set_1_bytes)
add_library(cryptopals::statistics ALIAS
  data_statistics)
add_library(cryptopals::text_metrics ALIAS
  set_1_text_metrics)
