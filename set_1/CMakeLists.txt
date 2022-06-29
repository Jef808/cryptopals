###########################
# Bytes encoding/decoding #
###########################
add_library(set_1_bytes STATIC
  src/bytes.h
  src/base64.h
  src/hex.h
  src/base64.cpp
  src/hex.cpp
  )
target_include_directories(set_1_bytes PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src)

########################################
# Metrics used to compare byte strings #
########################################
add_library(set_1_metrics STATIC
  src/metrics.h
  src/metrics.cpp
  )
target_include_directories(set_1_metrics PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_property(TARGET set_1_metrics PROPERTY CXX_STANDARD_REQUIRED 20)

##############################################################
# Utility functions for algorithms using character frequency #
##############################################################
add_library(set_1_character_frequency STATIC
  src/character_frequency.h
  src/character_frequency.cpp
  )
target_include_directories(set_1_character_frequency PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src)




add_library(set_1_text_metrics STATIC
  src/english_text_metric.h
  src/english_text_metric.cpp
  )
target_include_directories(set_1_text_metrics PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src)

############################################################
# Methods to encode/decode and break bitwise-xor encodings #
############################################################
add_library(set_1_xor STATIC
  src/bitwise_xor.h
  src/bitwise_xor.cpp
  )
target_include_directories(set_1_xor PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(set_1_xor PUBLIC
  set_1_character_frequency
  set_1_metrics
  cryptopals::statistics)








#################################
# CHALLENGES SOLUTIONS PROGRAMS #
#################################
add_executable(challenge_1
  challenge_1.cpp)
target_link_libraries(challenge_1
  set_1_bytes)

add_executable(challenge_2
  challenge_2.cpp)
target_link_libraries(challenge_2
  set_1_bytes
  set_1_xor)

add_executable(challenge_3
  challenge_3.cpp)
target_link_libraries(challenge_3
  set_1_bytes
  set_1_xor)

add_executable(challenge_4
  challenge_4.cpp)
target_link_libraries(challenge_4
  set_1_bytes
  set_1_xor
  set_1_character_frequency)

add_executable(challenge_5
  challenge_5.cpp)
target_link_libraries(challenge_5
  set_1_bytes
  set_1_xor)

add_executable(challenge_6
  challenge_6.cpp)
target_link_libraries(challenge_6
  set_1_bytes
  set_1_metrics
  set_1_xor
  set_1_character_frequency)
