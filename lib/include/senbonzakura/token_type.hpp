#pragma once

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
  kAmpersand,
  kPipe,

  // Single or Double Character Tokens.
  kBang,
  kBangEqual,
  kEqual,
  kEqualEqual,
  kGreater,
  kGreaterEqual,
  kLess,
  kLessEqual,

  // Triple-Charachter Tokens.
  kHighMul,

  // Multi-Character Tokens.
  kIdentifier,

  // Literal Tokens.
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
