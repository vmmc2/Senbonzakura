#pragma once

#include <any>
#include <map>
#include <string>
#include <vector>

#include "diagnostic_reporter.hpp"
#include "token.hpp"
#include "token_type.hpp"

class Lexer {
private:
  void AddToken(TokenType token_type);
  void AddToken(TokenType token_type, std::any value);
  char Advance();
  void Comment();
  void Identifier();
  void Integer();
  bool IsAlpha(char c);
  bool IsAlphaNumeric(char c);
  bool IsAtEnd();
  bool IsDigit(char c);
  void LexToken();
  bool Match(char expected);
  char Peek(int offset);
  void String();
  void UpdateColumnNumber();

  int current_ = 0;
  int start_ = 0;
  int line_ = 1;
  int column_ = 1;
  int column_delta_ = 1;
  std::string source_code_;
  std::vector<Token> tokens_;
  std::map<std::string, TokenType> keywords_to_tokentypes_ = {
      {"int", TokenType::kInt},       {"bool", TokenType::kBool},
      {"if", TokenType::kIf},         {"else", TokenType::kElse},
      {"while", TokenType::kWhile},   {"return", TokenType::kReturn},
      {"true", TokenType::kTrue},     {"false", TokenType::kFalse},
      {"length", TokenType::kLength}, {"use", TokenType::kUse},
  };
  DiagnosticReporter& diagnostic_reporter_;

public:
  Lexer(const std::string& source_code, DiagnosticReporter& diagnostic_reporter);
  const std::vector<Token> &LexTokens();
};
