#include "senbonzakura/parser.hpp"

Parser::Parser(const std::string &file_path, const std::vector<Token> &tokens,
               DiagnosticReporter &diagnostic_reporter)
    : file_path_{file_path}, tokens_{tokens},
      diagnostic_reporter_{diagnostic_reporter} {}

void Parser::Parse() {}
