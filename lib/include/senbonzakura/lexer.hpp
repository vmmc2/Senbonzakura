#pragma once

#include <any>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "senbonzakura/diagnostic_reporter.hpp"
#include "senbonzakura/token.hpp"
#include "senbonzakura/token_type.hpp"

class Lexer {
private:
  void AddToken(TokenType token_type);
  void AddToken(TokenType token_type, std::any value);
  char32_t Advance();
  void Character();
  void Comment();
  void Identifier();
  void Integer();
  bool IsAlpha(char32_t c);
  bool IsAlphaNumeric(char32_t c);
  bool IsAtEnd();
  bool IsHexDigit(char32_t c);
  bool IsDigit(char32_t c);
  void LexToken();
  bool Match(char32_t expected);
  char32_t Peek(std::size_t offset);
  std::optional<char32_t>
  ProcessSingleEscape(char32_t current_backslash_char,
                      char32_t next_char_after_backslash);
  void ReportError(const std::string& message, unsigned int col_offset);
  void String();
  void UpdateColumnNumber();
  std::string U32StringToUtf8(const std::u32string &u32str);

  std::size_t current_ = 0;
  std::size_t start_ = 0;
  std::size_t line_ = 1;
  std::size_t column_ = 1;
  std::size_t column_delta_ = 1;
  std::string file_path_{""};
  std::u32string source_code_codepoints_;
  std::vector<Token> tokens_;
  std::map<std::string, TokenType> keywords_to_tokentypes_ = {
      {"int", TokenType::kInt},       {"bool", TokenType::kBool},
      {"if", TokenType::kIf},         {"else", TokenType::kElse},
      {"while", TokenType::kWhile},   {"return", TokenType::kReturn},
      {"true", TokenType::kTrue},     {"false", TokenType::kFalse},
      {"length", TokenType::kLength}, {"use", TokenType::kUse},
  };
  DiagnosticReporter &diagnostic_reporter_;

public:
  Lexer(const std::string &file_path,
        const std::u32string &source_code_codepoints,
        DiagnosticReporter &diagnostic_reporter);
  const std::vector<Token> &LexTokens();
};
