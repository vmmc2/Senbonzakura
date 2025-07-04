#include "../include/senbonzakura/token.hpp"

#include <format>
#include <iostream>

Token::Token(int line, int column, TokenType token_type, std::any value,
             std::string lexeme)
    : line_(line), column_(column), token_type_(token_type),
      value_(std::move(value)), lexeme_(std::move(lexeme)) {}

std::ostream &operator<<(std::ostream &os, const Token &token) {
  os << std::format("[{}:{} Type:{} - Lexeme: {}]", token.line_, token.column_,
                    token.token_type_, token.lexeme_);
  return os;
}
