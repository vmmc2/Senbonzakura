#include "senbonzakura/token.hpp"
#include "senbonzakura/token_type.hpp"

#include <any>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

TEST(TokenHelpersTest, U32StringToUtf8CorversionTest) {
  // Test with a 'Smiley Face' emoji.
  std::u32string u32_str = U"Hello, World! \U0001F60A";
  std::string expected_utf8_str = "Hello, World! 😊";
  EXPECT_EQ(U32StringToUtf8(u32_str), expected_utf8_str);
}

TEST(TokenHelpersTest,
     EscapeU32StringForDisplayCorrectlyEscapeSpecialCharsTest) {
  std::u32string u32str = U"A string with \"\n\t\r quotes and line breaks.";
  std::u32string expected_escaped =
      U"A string with \\\"\\n\\t\\r quotes and line breaks.";

  EXPECT_EQ(EscapeU32StringForDisplay(u32str), expected_escaped);
}

TEST(TokenHelpersTest, EscapeU32StringForDisplayCorrectlyFormatUnicodeTest) {
  // Unicode character outside of the ASCII range (Example: Copyright symbol).
  std::u32string u32str = U"Hello, \u00A9!";
  // Expected result is the string: 'Hello, ' + escape for the copyright symbol
  // + '!'
  std::u32string expected_escaped = U"Hello, \\x{a9}!";
  EXPECT_EQ(EscapeU32StringForDisplay(u32str), expected_escaped);

  // Emoji (A UTF-32 character of 4 bytes).
  std::u32string emoji_str = U"\U0001F60A";
  std::u32string expected_emoji_escaped = U"\\x{1f60a}";
  EXPECT_EQ(EscapeU32StringForDisplay(emoji_str), expected_emoji_escaped);
}

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

  Token file_end_token{500, 1, TokenType::kFileEnd, {}, ""};
  EXPECT_EQ(file_end_token.line_, 500);
  EXPECT_EQ(file_end_token.column_, 1);
  EXPECT_EQ(file_end_token.token_type_, TokenType::kFileEnd);
  EXPECT_EQ(file_end_token.value_.has_value(), false);
  EXPECT_EQ(file_end_token.lexeme_, "");

  Token string_literal_token{1, 5, TokenType::kString, "Hello World!",
                             "Hello, Worl\\x{64}!"};
  EXPECT_EQ(string_literal_token.line_, 1);
  EXPECT_EQ(string_literal_token.column_, 5);
  EXPECT_EQ(string_literal_token.token_type_, TokenType::kString);
  EXPECT_EQ(string_literal_token.value_.has_value(), true);
  EXPECT_EQ(string_literal_token.lexeme_, "Hello, Worl\\x{64}!");
}

TEST(TokenTest, OperatorPrintsIntegerCorrectlyTest) {
  Token integer_token{1, 3, TokenType::kInteger, 12345, "12345"};

  std::stringstream ss;
  ss << integer_token;

  std::string expected_output{"[001:003] Type: kInteger - Value: 12345"};
  EXPECT_EQ(ss.str(), expected_output);
}

TEST(TokenTest, OperatorPrintsBooleanCorrectlyTest) {
  Token truthy_boolean_token{10, 4, TokenType::kTrue, true, "true"};
  Token falsey_boolean_token{11, 4, TokenType::kFalse, false, "false"};

  std::stringstream ss;
  ss << truthy_boolean_token;

  std::string expected_truthy_boolean_output =
      "[010:004] Type: kTrue - Value: true";
  EXPECT_EQ(ss.str(), expected_truthy_boolean_output);

  ss.str("");
  ss.clear();
  ss << falsey_boolean_token;

  std::string expected_falsey_boolean_output =
      "[011:004] Type: kFalse - Value: false";
  EXPECT_EQ(ss.str(), expected_falsey_boolean_output);
}

TEST(TokenTest, OperatorPrintsStringLiteralCorrectlyTest) {
  std::u32string u32_str = U"Hello, World!\n";
  Token string_literal_token{23, 2, TokenType::kString, u32_str,
                             "Hello, Worl\x{64}!\n"};

  std::stringstream ss;
  ss << string_literal_token;

  std::string expected_output =
      "[023:002] Type: kString - Value: Hello, World!\\n";

  EXPECT_EQ(ss.str(), expected_output);
}

TEST(TokenTest, OperatorPrintsCharLiteralCorrectlyTest) {
  std::u32string u32_ch = U"d";
  Token character_literal_token{1, 1, TokenType::kCharacter, u32_ch, "\x{64}"};

  std::stringstream ss;
  ss << character_literal_token;

  std::string expected_output = "[001:001] Type: kCharacter - Value: d";

  EXPECT_EQ(ss.str(), expected_output);
}

TEST(TokenTest, OperatorPrintsDefaultLexemeCorrectlyTest) {
  Token single_character_token{1, 1, TokenType::kColon, {}, ":"};
  Token double_character_token{2, 1, TokenType::kEqualEqual, {}, "=="};
  Token triple_character_token{3, 1, TokenType::kHighMul, {}, "*>>"};
  Token identifier_token{4, 1, TokenType::kIdentifier, {}, "foo"};
  Token primitive_type_token{5, 1, TokenType::kInt, {}, "int"};
  Token keyword_token{6, 1, TokenType::kIf, {}, "if"};
  Token file_end_token{7, 1, TokenType::kFileEnd, {}, ""};

  std::stringstream ss;

  ss << single_character_token;
  std::string expected_output = "[001:001] Type: kColon - Lexeme: :";
  EXPECT_EQ(ss.str(), expected_output);

  ss.str("");
  ss << double_character_token;
  expected_output = "[002:001] Type: kEqualEqual - Lexeme: ==";
  EXPECT_EQ(ss.str(), expected_output);

  ss.str("");
  ss << triple_character_token;
  expected_output = "[003:001] Type: kHighMul - Lexeme: *>>";
  EXPECT_EQ(ss.str(), expected_output);

  ss.str("");
  ss << identifier_token;
  expected_output = "[004:001] Type: kIdentifier - Lexeme: foo";
  EXPECT_EQ(ss.str(), expected_output);

  ss.str("");
  ss << primitive_type_token;
  expected_output = "[005:001] Type: kInt - Lexeme: int";
  EXPECT_EQ(ss.str(), expected_output);

  ss.str("");
  ss << keyword_token;
  expected_output = "[006:001] Type: kIf - Lexeme: if";
  EXPECT_EQ(ss.str(), expected_output);

  ss.str("");
  ss << file_end_token;
  expected_output = "[007:001] Type: kFileEnd - Lexeme: ";
  EXPECT_EQ(ss.str(), expected_output);
}
