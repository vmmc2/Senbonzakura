find_package(GTest REQUIRED)

# Cria um executável para os testes
add_executable(senbonzakura_tests
  diagnostic_reporter_test.cpp
  file_scanner_test.cpp
  file_writer_test.cpp
  lexer_test.cpp
  parser_test.cpp
  token_test.cpp
  token_type_test.cpp
)

# Linka os testes com a sua biblioteca e com o GTest
target_link_libraries(senbonzakura_tests PRIVATE
    senbonzakura_lib
    GTest::GTest
    GTest::Main
)

# Integra os testes com o CTest (ferramenta de testes do CMake)
include(GoogleTest)
gtest_discover_tests(senbonzakura_tests)
