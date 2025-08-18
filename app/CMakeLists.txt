find_package(CLI11 REQUIRED)

add_executable(senbonzakura main.cpp)

# Linka o executável com a sua biblioteca e com a CLI11
target_link_libraries(senbonzakura PRIVATE
    senbonzakura_lib
    CLI11::CLI11
)
