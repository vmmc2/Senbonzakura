find_package(utf8cpp CONFIG REQUIRED)

add_library(senbonzakura_lib STATIC)

# Adiciona os arquivos de implementação à biblioteca
target_sources(senbonzakura_lib PRIVATE
  src/diagnostic_reporter.cpp
  src/file_scanner.cpp  
  src/lexer.cpp
  src/token_type.cpp
  src/token.cpp
)

# Torna o diretório "include" público para que outros alvos possam encontrá-lo
target_include_directories(senbonzakura_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(senbonzakura_lib PUBLIC utf8::cpp)
