# Set 1 exports
add_library(set_1_bytes STATIC
  src/bytes.h
  src/base64.h
  src/hex.h
  src/base64.cpp
  src/hex.cpp
  src/bitwise_xor.h
  src/bitwise_xor.cpp
  )
target_include_directories(set_1_bytes PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src)


add_library(set_1_text_metrics STATIC
  src/english_text_metric.h
  src/english_text_metric.cpp
  )
target_include_directories(set_1_text_metrics PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src)


# Challenges
add_executable(challenge_1
  challenge_1.cpp)
target_link_libraries(challenge_1
  set_1_bytes)

add_executable(challenge_2
  challenge_2.cpp)
target_link_libraries(challenge_2
  set_1_bytes)

add_executable(challenge_3
  challenge_3.cpp)
target_link_libraries(challenge_3
  set_1_bytes
  set_1_text_metrics)
