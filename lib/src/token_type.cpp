#include "senbonzakura/token_type.hpp"

#include <unordered_map>

std::string TokenTypeToString(TokenType token_type) {
  static const std::unordered_map<TokenType, std::string>
      token_type_to_str_map = {{TokenType::kLeftParen, "kLeftParen"},
                               {TokenType::kRightParen, "kRightParen"},
                               {TokenType::kLeftBrace, "kLeftBrace"},
                               {TokenType::kRightBrace, "kRightBrace"},
                               {TokenType::kLeftBracket, "kLeftBracket"},
                               {TokenType::kRightBracket, "kRightBracket"},
                               {TokenType::kComma, "kComma"},
                               {TokenType::kColon, "kColon"},
                               {TokenType::kSemiColon, "kSemiColon"},
                               {TokenType::kPlus, "kPlus"},
                               {TokenType::kMinus, "kMinus"},
                               {TokenType::kRemainder, "kRemainder"},
                               {TokenType::kStar, "kStar"},
                               {TokenType::kSlash, "kSlash"},
                               {TokenType::kUnderscore, "kUnderscore"},
                               {TokenType::kBang, "kBang"},
                               {TokenType::kBangEqual, "kBangEqual"},
                               {TokenType::kEqual, "kEqual"},
                               {TokenType::kEqualEqual, "kEqualEqual"},
                               {TokenType::kGreater, "kGreater"},
                               {TokenType::kGreaterEqual, "kGreaterEqual"},
                               {TokenType::kLess, "kLess"},
                               {TokenType::kLessEqual, "kLessEqual"},
                               {TokenType::kLogicalAnd, "kLogicalAnd"},
                               {TokenType::kLogicalOr, "kLogicalOr"},
                               {TokenType::kHighMul, "kHighMul"},
                               {TokenType::kIdentifier, "kIdentifier"},
                               {TokenType::kCharacter, "kCharacter"},
                               {TokenType::kInteger, "kInteger"},
                               {TokenType::kString, "kString"},
                               {TokenType::kInt, "kInt"},
                               {TokenType::kBool, "kBool"},
                               {TokenType::kIf, "kIf"},
                               {TokenType::kElse, "kElse"},
                               {TokenType::kWhile, "kWhile"},
                               {TokenType::kReturn, "kReturn"},
                               {TokenType::kTrue, "kTrue"},
                               {TokenType::kFalse, "kFalse"},
                               {TokenType::kLength, "kLength"},
                               {TokenType::kUse, "kUse"},
                               {TokenType::kFileEnd, "kFileEnd"}};

  if (token_type_to_str_map.contains(token_type)) {
    return token_type_to_str_map.at(token_type);
  }

  return "Unknown TokenType";
}
