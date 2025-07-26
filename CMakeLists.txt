cmake_minimum_required(VERSION 3.20)

project(senbonzakura LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_ROOT_DIR "${CMAKE_SOURCE_DIR}")

# Defines the compiler library.
add_subdirectory(lib)
# Defines the final executable.
add_subdirectory(app)

target_compile_definitions(senbonzakura PRIVATE
    "PROJECT_ROOT_DIR=\"${PROJECT_ROOT_DIR}\""
)

enable_testing()
option(BUILD_TESTING "Buind the tests" ON)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
