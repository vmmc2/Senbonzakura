find_package(GTest REQUIRED)

# Cria um executável para os testes
add_executable(senbonzakura_tests
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