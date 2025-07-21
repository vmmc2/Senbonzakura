#pragma once

#include <format>
#include <string>

enum class TokenType {
  // Single-Character Tokens.
  kLeftParen,
  kRightParen,
  kLeftBrace,
  kRightBrace,
  kLeftBracket,
  kRightBracket,
  kComma,
  kColon,
  kSemiColon,
  kPlus,
  kMinus,
  kRemainder,
  kStar,
  kSlash,

  // Single or Double Character Tokens.
  kBang,
  kBangEqual,
  kEqual,
  kEqualEqual,
  kGreater,
  kGreaterEqual,
  kLess,
  kLessEqual,
  kLogicalAnd,
  kLogicalOr,

  // Triple-Charachter Tokens.
  kHighMul,

  // Multi-Character Tokens.
  kIdentifier,

  // Literal Tokens.
  kCharacter,
  kInteger,
  kString,

  // Primitive Type Tokens.
  kInt,
  kBool,

  // Keyword Tokens.
  kIf,
  kElse,
  kWhile,
  kReturn,
  kTrue,
  kFalse,
  kLength,
  kUse,

  // End of File Token.
  kFileEnd
};

std::string TokenTypeToString(TokenType token_type);

template <>
struct std::formatter<TokenType, char> : std::formatter<std::string, char> {
  auto format(TokenType token_type, std::format_context &ctx) const {
    return std::formatter<std::string, char>::format(
        TokenTypeToString(token_type), ctx);
  }
};
