#pragma once

#include <any>
#include <string>
#include <vector>

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

  int current_ = 0;
  int start_ = 0;
  int line_ = 1;
  int column_ = 1;
  std::string source_code_;
  std::vector<Token> tokens_;

public:
  Lexer(std::string source_code);
  const std::vector<Token> &LexTokens();
};
