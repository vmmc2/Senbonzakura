#include <iostream>

#include "senbonzakura/diagnostic_reporter.hpp"
#include "senbonzakura/file_scanner.hpp"
#include "senbonzakura/lexer.hpp"

#include <CLI/CLI.hpp>

int main(int argc, const char **argv) {
  // CLI::App app;
  // CLI11_PARSE(app, argc, argv);

  DiagnosticReporter diagnostic_reporter;
  std::string file_path = "/home/vmmc2/Senbonzakura/test.eta";
  FileScanner file_scanner{file_path, diagnostic_reporter};

  file_scanner.ScanFile();
  const std::u32string &file_content = file_scanner.GetFileContentCodepoints();

  Lexer lexer{file_path, file_content, diagnostic_reporter};
  const std::vector<Token> tokens = lexer.LexTokens();

  for (const auto &token : tokens) {
    if (token.token_type_ != TokenType::kFileEnd) {
      std::cout << token << std::endl;
    }
  }
  if (diagnostic_reporter.HasFatalErrors()) {
    diagnostic_reporter.PrintDiagnostic();
    // TODO: Verify if this is the correct exit code for this specific scenario.
    std::exit(64);
  }

  return 0;
}