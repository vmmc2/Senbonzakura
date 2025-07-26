#include "senbonzakura/token_type.hpp"

std::string TokenTypeToString(TokenType token_type) {
  static const std::string token_type_strings[] = {
      "kLeftParen",    "kRightParen",   "kLeftBrace",  "kRightBrace",
      "kLeftBracket",  "kRightBracket", "kComma",      "kColon",
      "kSemiColon",    "kPlus",         "kMinus",      "kRemainder",
      "kStar",         "kSlash",        "kUnderscore", "kBang",
      "kBangEqual",    "kEqual",        "kEqualEqual", "kGreater",
      "kGreaterEqual", "kLess",         "kLessEqual",  "kLogicalAnd",
      "kLogicalOr",    "kHighMul",      "kIdentifier", "kCharacter",
      "kInteger",      "kString",       "kInt",        "kBool",
      "kIf",           "kElse",         "kWhile",      "kReturn",
      "kTrue",         "kFalse",        "kLength",     "kUse",
      "kFileEnd"};

  return token_type_strings[static_cast<int>(token_type)];
}
