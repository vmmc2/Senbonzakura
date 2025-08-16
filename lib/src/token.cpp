#include "senbonzakura/token.hpp"

#include <format>
#include <iostream>

Token::Token(int line, int column, TokenType token_type, std::any value,
             std::string lexeme)
    : line_(line), column_(column), token_type_(token_type),
      value_(std::move(value)), lexeme_(std::move(lexeme)) {}

bool Token::operator==(const Token &other) const {
  return line_ == other.line_ && column_ == other.column_ &&
         token_type_ == other.token_type_ && lexeme_ == other.lexeme_;
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
  os << std::format("[{:03}:{:03}] Type: {}", token.line_, token.column_,
                    token.token_type_);

  switch (token.token_type_) {
  case (TokenType::kCharacter):
    try {
      const char ch_value = std::any_cast<const char>(token.value_);
      os << std::format(" - Value: {}", ch_value);
      os << std::format(" - Lexeme: {}", token.lexeme_);
    } catch (const std::bad_any_cast &e) {
      os << " - Value: <Error: Bad Any Cast for string>";
    }
    break;
  case (TokenType::kString): {
    try {
      const std::string s_value =
          std::any_cast<const std::string>(token.value_);
      os << std::format(" - Value: {}", s_value);
      os << std::format(" - Lexeme: {}", token.lexeme_);
    } catch (const std::bad_any_cast &e) {
      os << " - Value: <Error: Bad Any Cast for string>";
    }
    break;
  }
  case (TokenType::kInteger): {
    try {
      int i_value = std::any_cast<int>(token.value_);
      os << std::format(" - Value: {}", i_value);
    } catch (const std::bad_any_cast &e) {
      os << " - Value: <Error: Bad Any Cast for integer>";
    }
    break;
  }
  case (TokenType::kTrue):
  case (TokenType::kFalse): {
    try {
      bool b_value = std::any_cast<bool>(token.value_);
      os << std::format(" - Value: {}", b_value ? "true" : "false");
    } catch (const std::bad_any_cast &e) {
      os << " - Value: <Error: Bad Any Cast for boolean>";
    }
    break;
  }
  default:
    os << std::format(" - Lexeme: {}",
                      token.lexeme_); // Mantém o lexema para outros tipos
    break;
  }
  return os;
}
