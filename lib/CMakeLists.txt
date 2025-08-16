add_library(senbonzakura_lib STATIC)

# Adiciona os arquivos de implementação à biblioteca
target_sources(senbonzakura_lib PRIVATE
  src/diagnostic_reporter.cpp
  include/senbonzakura/diagnostic_reporter.hpp
  src/file_scanner.cpp
  include/senbonzakura/file_scanner.hpp
  src/file_writer.cpp
  include/senbonzakura/file_writer.hpp
  src/lexer.cpp
  include/senbonzakura/lexer.hpp
  src/parser.cpp
  include/senbonzakura/parser.hpp
  src/token_type.cpp
  include/senbonzakura/token_type.hpp
  src/token.cpp
  include/senbonzakura/token.hpp
)

# Torna o diretório "include" público para que outros alvos possam encontrá-lo
target_include_directories(senbonzakura_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
