#include "senbonzakura/token.hpp"
#include "senbonzakura/token_type.hpp"

#include <any>

#include <gtest/gtest.h>

TEST(TokenTest, TokenConstructorTest) {
  Token identifier_token{1, 5, TokenType::kIdentifier, {}, "x"};
  EXPECT_EQ(identifier_token.line_, 1);
  EXPECT_EQ(identifier_token.column_, 5);
  EXPECT_EQ(identifier_token.token_type_, TokenType::kIdentifier);
  EXPECT_EQ(identifier_token.value_.has_value(), false);
  EXPECT_EQ(identifier_token.lexeme_, "x");

  Token integer_token{10, 23, TokenType::kInteger, 100, "100"};
  EXPECT_EQ(integer_token.line_, 10);
  EXPECT_EQ(integer_token.column_, 23);
  EXPECT_EQ(integer_token.token_type_, TokenType::kInteger);
  EXPECT_EQ(integer_token.value_.has_value(), true);
  EXPECT_EQ(std::any_cast<int>(integer_token.value_), 100);
  EXPECT_EQ(integer_token.lexeme_, "100");
}
