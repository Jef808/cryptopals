###########################
# Bytes encoding/decoding #
###########################
add_library(bytes STATIC
  src/bytes.h
  src/base64.h
  src/hex.h
  src/base64.cpp
  src/hex.cpp
  src/bitwise_xor.h
  src/bitwise_xor.cpp
  src/metrics.h
  src/metrics.cpp
  src/character_frequency.h
  src/character_frequency.cpp
  src/aes.h
  src/aes.cpp)

target_include_directories(bytes PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(bytes PRIVATE
  cryptopals::statistics)

set_property(TARGET bytes
  PROPERTY CXX_STANDARD_REQUIRED 20)

add_library(cryptopals::bytes ALIAS bytes)

add_library(text_metrics STATIC
  src/english_text_metric.h
  src/english_text_metric.cpp)

target_include_directories(text_metrics PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(cryptopals::text_metrics ALIAS text_metrics)

# ############################################################
# # Methods to encode/decode and break bitwise-xor encodings #
# ############################################################
# add_library(xor STATIC
#   src/bitwise_xor.h
#   src/bitwise_xor.cpp
#   )
# target_include_directories(xor PUBLIC
#   ${CMAKE_CURRENT_SOURCE_DIR}/src)
# target_link_libraries(xor PUBLIC
#   character_frequency
#   metrics
#   cryptopals::statistics)



#################################
# CHALLENGES SOLUTIONS PROGRAMS #
#################################
add_executable(challenge_1
  challenge_1.cpp)
target_link_libraries(challenge_1
  cryptopals::bytes)

add_executable(challenge_2
  challenge_2.cpp)
target_link_libraries(challenge_2
  cryptopals::bytes)

add_executable(challenge_3
  challenge_3.cpp)
target_link_libraries(challenge_3
  cryptopals::bytes)

add_executable(challenge_4
  challenge_4.cpp)
target_link_libraries(challenge_4
  cryptopals::bytes)

add_executable(challenge_5
  challenge_5.cpp)
target_link_libraries(challenge_5
  cryptopals::bytes)

add_executable(challenge_6
  challenge_6.cpp)
target_link_libraries(challenge_6
  cryptopals::bytes)

add_executable(challenge_7
  challenge_7.cpp
  src/aes.cpp)
target_link_libraries(challenge_7
  cryptopals::bytes)


add_executable(test_aes
  src/test_aes.cc
  src/aes.cpp)
target_link_libraries(test_aes PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(test_aes)
