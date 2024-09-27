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

#############
# Utilities #
#############
add_subdirectory(utils)
link_libraries(cryptopals::printer)

#################
# Data analysis #
#################
add_subdirectory(data_analysis)

#########
# Tests #
#########
enable_testing()
include(FetchContent)

# Fetch Catch2 library
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.6.0
)
FetchContent_MakeAvailable(Catch2)

add_subdirectory(tests)

#########
# Set 1 #
#########
add_subdirectory(set_1)
