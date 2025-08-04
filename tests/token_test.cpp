#include "senbonzakura/token.hpp"
#include "senbonzakura/token_type.hpp"

#include <gtest/gtest.h>

TEST(TokenTest, TokenConstructorTest){
  Token identifier_token{1, 5, TokenType::kIdentifier, {}, "x"};
  EXPECT_EQ(identifier_token.line_, 1);
  EXPECT_EQ(identifier_token.column_, 5);
  EXPECT_EQ(identifier_token.token_type_, TokenType::kIdentifier);
  EXPECT_EQ(identifier_token.value_.has_value(), false);
  EXPECT_EQ(identifier_token.lexeme_, "x");
}
