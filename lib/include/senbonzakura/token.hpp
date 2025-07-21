#pragma once

#include "senbonzakura/token_type.hpp"

#include <any>
#include <string>

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
