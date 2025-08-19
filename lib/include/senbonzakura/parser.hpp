#pragma once

#include <vector>

#include "senbonzakura/diagnostic_reporter.hpp"
#include "senbonzakura/token.hpp"

class Parser {
private:
  std::size_t current_ = 0;
  std::size_t start_ = 0;
  std::string file_path_{""};
  const std::vector<Token> &tokens_;
  DiagnosticReporter &diagnostic_reporter_;

public:
  Parser(const std::string &file_path, const std::vector<Token> &tokens,
         DiagnosticReporter &diagnostic_reporter);
  void Parse();
};
