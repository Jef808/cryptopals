set(cryptopals_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test_base64
  test_base64.cc)
target_link_libraries(test_base64 PUBLIC
  cryptopals::bytes)
target_compile_definitions(test_base64 PRIVATE
  "-DTESTS_DIR=\"${cryptopals_TESTS_DIR}\"")
