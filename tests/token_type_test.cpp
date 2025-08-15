#include "senbonzakura/token_type.hpp"

#include <gtest/gtest.h>

TEST(TokenTypeTest, AllTokenTypesReturnCorrectString) {
  // Single-Character Tokens
  EXPECT_EQ(TokenTypeToString(TokenType::kLeftParen), "kLeftParen");
  EXPECT_EQ(TokenTypeToString(TokenType::kRightParen), "kRightParen");
  EXPECT_EQ(TokenTypeToString(TokenType::kLeftBracket), "kLeftBracket");
  EXPECT_EQ(TokenTypeToString(TokenType::kRightBracket), "kRightBracket");
  EXPECT_EQ(TokenTypeToString(TokenType::kLeftSquareBracket),
            "kLeftSquareBracket");
  EXPECT_EQ(TokenTypeToString(TokenType::kRightSquareBracket),
            "kRightSquareBracket");
  EXPECT_EQ(TokenTypeToString(TokenType::kComma), "kComma");
  EXPECT_EQ(TokenTypeToString(TokenType::kColon), "kColon");
  EXPECT_EQ(TokenTypeToString(TokenType::kSemiColon), "kSemiColon");
  EXPECT_EQ(TokenTypeToString(TokenType::kPlus), "kPlus");
  EXPECT_EQ(TokenTypeToString(TokenType::kMinus), "kMinus");
  EXPECT_EQ(TokenTypeToString(TokenType::kRemainder), "kRemainder");
  EXPECT_EQ(TokenTypeToString(TokenType::kStar), "kStar");
  EXPECT_EQ(TokenTypeToString(TokenType::kSlash), "kSlash");
  EXPECT_EQ(TokenTypeToString(TokenType::kUnderscore), "kUnderscore");

  // Single or Double Character Tokens
  EXPECT_EQ(TokenTypeToString(TokenType::kBang), "kBang");
  EXPECT_EQ(TokenTypeToString(TokenType::kBangEqual), "kBangEqual");
  EXPECT_EQ(TokenTypeToString(TokenType::kEqual), "kEqual");
  EXPECT_EQ(TokenTypeToString(TokenType::kEqualEqual), "kEqualEqual");
  EXPECT_EQ(TokenTypeToString(TokenType::kGreater), "kGreater");
  EXPECT_EQ(TokenTypeToString(TokenType::kGreaterEqual), "kGreaterEqual");
  EXPECT_EQ(TokenTypeToString(TokenType::kLess), "kLess");
  EXPECT_EQ(TokenTypeToString(TokenType::kLessEqual), "kLessEqual");
  EXPECT_EQ(TokenTypeToString(TokenType::kLogicalAnd), "kLogicalAnd");
  EXPECT_EQ(TokenTypeToString(TokenType::kLogicalOr), "kLogicalOr");

  // Triple-Charachter Tokens
  EXPECT_EQ(TokenTypeToString(TokenType::kHighMul), "kHighMul");

  // Multi-Character Tokens
  EXPECT_EQ(TokenTypeToString(TokenType::kIdentifier), "kIdentifier");

  // Literal Tokens
  EXPECT_EQ(TokenTypeToString(TokenType::kCharacter), "kCharacter");
  EXPECT_EQ(TokenTypeToString(TokenType::kInteger), "kInteger");
  EXPECT_EQ(TokenTypeToString(TokenType::kString), "kString");

  // Primitive Type Tokens
  EXPECT_EQ(TokenTypeToString(TokenType::kInt), "kInt");
  EXPECT_EQ(TokenTypeToString(TokenType::kBool), "kBool");

  // Keyword Tokens
  EXPECT_EQ(TokenTypeToString(TokenType::kIf), "kIf");
  EXPECT_EQ(TokenTypeToString(TokenType::kElse), "kElse");
  EXPECT_EQ(TokenTypeToString(TokenType::kWhile), "kWhile");
  EXPECT_EQ(TokenTypeToString(TokenType::kReturn), "kReturn");
  EXPECT_EQ(TokenTypeToString(TokenType::kTrue), "kTrue");
  EXPECT_EQ(TokenTypeToString(TokenType::kFalse), "kFalse");
  EXPECT_EQ(TokenTypeToString(TokenType::kLength), "kLength");
  EXPECT_EQ(TokenTypeToString(TokenType::kUse), "kUse");

  // End of File Token
  EXPECT_EQ(TokenTypeToString(TokenType::kFileEnd), "kFileEnd");
}
