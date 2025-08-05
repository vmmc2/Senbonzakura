#pragma once

#include "senbonzakura/token_type.hpp"

#include <any>
#include <string>

std::string U32StringToUtf8(const std::u32string &u32str);

std::u32string EscapeU32StringForDisplay(const std::u32string &u32str);

class Token {
public:
  Token(int line, int column, TokenType token_type, std::any value,
        std::string lexeme);

  friend std::ostream &operator<<(std::ostream &os, const Token &token);

  const int line_;
  const int column_;
  const TokenType token_type_;
  const std::any value_;
  const std::string lexeme_;
};
