#include <iostream>

#include "../lib/include/senbonzakura/diagnostic_reporter.hpp"
#include "../lib/include/senbonzakura/file_scanner.hpp"
#include "../lib/include/senbonzakura/lexer.hpp"

#include <CLI/CLI.hpp>

int main(int argc, const char **argv) {
  // CLI::App app;
  // CLI11_PARSE(app, argc, argv);

  DiagnosticReporter diagnostic_reporter;

  FileScanner file_scanner{"/home/vmmc2/Senbonzakura/test.eta", diagnostic_reporter};
  file_scanner.ScanFile();
  const std::string &file_content = file_scanner.GetFileContent();

  Lexer lexer{file_content, diagnostic_reporter};
  const std::vector<Token> tokens = lexer.LexTokens();

  for (const auto &token : tokens) {
    if (token.token_type_ != TokenType::kFileEnd) {
      std::cout << token << std::endl;
    }
  }

  return 0;
}