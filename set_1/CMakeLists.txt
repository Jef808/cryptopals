add_library(set_1_bytes STATIC
  src/bytes.h
  src/base64.h
  src/hex.h
  src/base64.cpp
  src/hex.cpp
  src/bitwise_xor.h
  src/bitwise_xor.cpp)

target_include_directories(set_1_bytes PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src)



add_executable(challenge_1
  challenge_1.cpp)
target_link_libraries(challenge_1 PUBLIC
  set_1_bytes)

add_executable(challenge_2
  challenge_2.cpp)
target_link_libraries(challenge_2 PUBLIC
  set_1_bytes)
