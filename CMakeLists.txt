cmake_minimum_required(VERSION 3.22)
project(cryptopals)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)


add_subdirectory(set_1)

add_library(cryptopals::bytes ALIAS
  set_1_bytes)
