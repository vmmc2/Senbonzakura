#include "senbonzakura/file_scanner.hpp"
#include "senbonzakura/lexer.hpp"
#include "senbonzakura/token.hpp"
#include "senbonzakura/token_type.hpp"

#include <filesystem>
#include <fstream>
#include <string>

#include <gtest/gtest.h>

class LexerTest : public ::testing::Test {
protected:
  std::string temp_dir_path_;
  std::string temp_lexer_filepath_;
  DiagnosticReporter diagnostic_reporter_;

  void SetUp() override {
    temp_dir_path_ =
        std::filesystem::temp_directory_path().string() + "/test_lexer_dir";
    std::filesystem::create_directory(temp_dir_path_);
  }

  std::vector<Token> LexSourceCode(const std::string &filename,
                                   const std::string &file_content_bytes) {
    temp_lexer_filepath_ = temp_dir_path_ + "/" + filename;
    std::ofstream file{temp_lexer_filepath_};
    file << file_content_bytes;
    file.close();

    FileScanner file_scanner{temp_lexer_filepath_, diagnostic_reporter_};

    file_scanner.ScanFile();

    Lexer lexer{temp_lexer_filepath_, file_scanner.GetFileContentBytes(),
                diagnostic_reporter_};

    return lexer.LexTokens();
  }
};

TEST_F(LexerTest, LexesWithEmptySourceCode) {
  std::string file_content_bytes = "";
  std::vector<Token> tokens =
      LexSourceCode("test_empty_source_code.eta", file_content_bytes);

  EXPECT_EQ(tokens.size(), 1);
  EXPECT_EQ(tokens[0].token_type_, TokenType::kFileEnd);
}

TEST_F(LexerTest, LexesWithOnlySingleCharacterTokensInSourceCode) {
  std::string file_content_bytes = "(){}[],:;+-%*/_=!><";
  std::vector<Token> output_tokens =
      LexSourceCode("test_single_character_tokens.eta", file_content_bytes);

  std::vector<Token> expected_tokens{
      Token{1, 1, TokenType::kLeftParen, {}, "("},
      Token{1, 2, TokenType::kRightParen, {}, ")"},
      Token{1, 3, TokenType::kLeftBracket, {}, "{"},
      Token{1, 4, TokenType::kRightBracket, {}, "}"},
      Token{1, 5, TokenType::kLeftSquareBracket, {}, "["},
      Token{1, 6, TokenType::kRightSquareBracket, {}, "]"},
      Token{1, 7, TokenType::kComma, {}, ","},
      Token{1, 8, TokenType::kColon, {}, ":"},
      Token{1, 9, TokenType::kSemiColon, {}, ";"},
      Token{1, 10, TokenType::kPlus, {}, "+"},
      Token{1, 11, TokenType::kMinus, {}, "-"},
      Token{1, 12, TokenType::kRemainder, {}, "%"},
      Token{1, 13, TokenType::kStar, {}, "*"},
      Token{1, 14, TokenType::kSlash, {}, "/"},
      Token{1, 15, TokenType::kUnderscore, {}, "_"},
      Token{1, 16, TokenType::kEqual, {}, "="},
      Token{1, 17, TokenType::kBang, {}, "!"},
      Token{1, 18, TokenType::kGreater, {}, ">"},
      Token{1, 19, TokenType::kLess, {}, "<"},
      Token{1, 20, TokenType::kFileEnd, {}, ""},
  };

  EXPECT_EQ(output_tokens.size(), expected_tokens.size());
  EXPECT_EQ(output_tokens, expected_tokens);
}

TEST_F(LexerTest, LexesWithOnlyDoubleCharacterTokensInSourceCode) {
  std::string file_content_bytes = "!===>=<=&|";
  std::vector<Token> output_tokens =
      LexSourceCode("test_double_character_tokens.eta", file_content_bytes);

  std::vector<Token> expected_tokens{
      Token{1, 1, TokenType::kBangEqual, {}, "!="},
      Token{1, 3, TokenType::kEqualEqual, {}, "=="},
      Token{1, 5, TokenType::kGreaterEqual, {}, ">="},
      Token{1, 7, TokenType::kLessEqual, {}, "<="},
      Token{1, 9, TokenType::kLogicalAnd, {}, "&"},
      Token{1, 10, TokenType::kLogicalOr, {}, "|"},
      Token{1, 11, TokenType::kFileEnd, {}, ""},
  };

  EXPECT_EQ(output_tokens.size(), expected_tokens.size());
  EXPECT_EQ(output_tokens, expected_tokens);
}

TEST_F(LexerTest, LexesWithOnlyTripleCharacterTokensInSourceCode) {
  std::string file_content_bytes = "*>>*>";
  std::vector<Token> output_tokens =
      LexSourceCode("test_triple_character_tokens.eta", file_content_bytes);

  std::vector<Token> expected_tokens{
      Token{1, 1, TokenType::kHighMul, {}, "*>>"},
      Token{1, 4, TokenType::kStar, {}, "*"},
      Token{1, 5, TokenType::kGreater, {}, ">"},
      Token{1, 6, TokenType::kFileEnd, {}, ""},
  };

  EXPECT_EQ(output_tokens.size(), expected_tokens.size());
  EXPECT_EQ(output_tokens, expected_tokens);
}

TEST_F(LexerTest, LexesWithOnlyWhitespaceCharactersInSourceCode) {
  std::string file_content_bytes = "  \r \t\r \t";
  std::vector<Token> output_tokens =
      LexSourceCode("test_whitespace_characters.eta", file_content_bytes);

  std::vector<Token> expected_tokens{Token{1, 9, TokenType::kFileEnd, {}, ""}};

  EXPECT_EQ(output_tokens.size(), expected_tokens.size());
  EXPECT_EQ(output_tokens, expected_tokens);
}

TEST_F(LexerTest, LexesWithOnlyCommentsInSourceCode) {
  std::string file_content_bytes = "// Just a comment here. There are no other "
                                   "tokens beside the 'kFileEnd' one.";
  std::vector<Token> output_tokens =
      LexSourceCode("test_comment.eta", file_content_bytes);

  std::vector<Token> expected_tokens{Token{1, 77, TokenType::kFileEnd, {}, ""}};

  EXPECT_EQ(output_tokens.size(), expected_tokens.size());
  EXPECT_EQ(output_tokens, expected_tokens);
}

TEST_F(LexerTest, LexesWithOnlyIdentifiersInSourceCode) {
  std::string file_content_bytes = "foo bar A a_Ab23\na1'c2r2d2";

  std::vector<Token> output_tokens =
      LexSourceCode("test_identifiers.eta", file_content_bytes);

  std::vector<Token> expected_tokens{
      Token{1, 1, TokenType::kIdentifier, {}, "foo"},
      Token{1, 5, TokenType::kIdentifier, {}, "bar"},
      Token{1, 9, TokenType::kIdentifier, {}, "A"},
      Token{1, 11, TokenType::kIdentifier, {}, "a_Ab23"},
      Token{2, 1, TokenType::kIdentifier, {}, "a1'c2r2d2"},
      Token{2, 10, TokenType::kFileEnd, {}, ""},
  };

  EXPECT_EQ(output_tokens.size(), expected_tokens.size());
  EXPECT_EQ(output_tokens, expected_tokens);
}

TEST_F(LexerTest, LexesWithOnlyKeywordsInSourceCode) {
  std::string file_content_bytes =
      "else if while return\ntrue false use   length";

  std::vector<Token> output_tokens =
      LexSourceCode("test_keywords.eta", file_content_bytes);

  std::vector<Token> expected_tokens{
      Token{1, 1, TokenType::kElse, {}, "else"},
      Token{1, 6, TokenType::kIf, {}, "if"},
      Token{1, 9, TokenType::kWhile, {}, "while"},
      Token{1, 15, TokenType::kReturn, {}, "return"},
      Token{2, 1, TokenType::kTrue, true, "true"},
      Token{2, 6, TokenType::kFalse, false, "false"},
      Token{2, 12, TokenType::kUse, {}, "use"},
      Token{2, 18, TokenType::kLength, {}, "length"},
      Token{2, 24, TokenType::kFileEnd, {}, ""},
  };

  EXPECT_EQ(output_tokens.size(), expected_tokens.size());
  EXPECT_EQ(output_tokens, expected_tokens);
}

TEST_F(LexerTest, LexesWithOnlyPrimitiveTypesInSourceCode) {
  std::string file_content_bytes = "int bool\n bool bool\nint";

  std::vector<Token> output_tokens =
      LexSourceCode("test_primitive_types.eta", file_content_bytes);

  std::vector<Token> expected_tokens{
      Token{1, 1, TokenType::kInt, {}, "int"},
      Token{1, 5, TokenType::kBool, {}, "bool"},
      Token{2, 2, TokenType::kBool, {}, "bool"},
      Token{2, 7, TokenType::kBool, {}, "bool"},
      Token{3, 1, TokenType::kInt, {}, "int"},
      Token{3, 4, TokenType::kFileEnd, {}, ""},
  };

  EXPECT_EQ(output_tokens.size(), expected_tokens.size());
  EXPECT_EQ(output_tokens, expected_tokens);
}

TEST_F(LexerTest, LexesWithOnlyLiteralsInSourceCode) {
  std::string file_content_bytes = " 2 23 \"Hello, World!\"";

  std::vector<Token> output_tokens =
      LexSourceCode("test_literal_values.eta", file_content_bytes);

  std::vector<Token> expected_tokens{Token{1, 2, TokenType::kInteger, 2, "2"},
                                     Token{1, 4, TokenType::kInteger, 23, "23"},
                                     Token{1, 7, TokenType::kString,
                                           std::string{"Hello, World!"},
                                           "\"Hello, World!\""},
                                     Token{1, 22, TokenType::kFileEnd, {}, ""}};

  EXPECT_EQ(output_tokens.size(), expected_tokens.size());
  EXPECT_EQ(output_tokens, expected_tokens);
}

TEST_F(LexerTest, LexesWithUnterminatedStringLiteralOnSourceCode) {
  std::string file_content_bytes = "s: int[] = \"Hello,";

  std::vector<Token> output_tokens =
      LexSourceCode("test_unterminated_string_literal.eta", file_content_bytes);

  std::vector<Token> expected_tokens{
      Token{1, 1, TokenType::kIdentifier, {}, "s"},
      Token{1, 2, TokenType::kColon, {}, ":"},
      Token{1, 4, TokenType::kInt, {}, "int"},
      Token{1, 7, TokenType::kLeftSquareBracket, {}, "["},
      Token{1, 8, TokenType::kRightSquareBracket, {}, "]"},
      Token{1, 10, TokenType::kEqual, {}, "="},
      Token{1, 20, TokenType::kFileEnd, {}, ""},
  };

  EXPECT_EQ(output_tokens.size(), expected_tokens.size());
  EXPECT_EQ(output_tokens, expected_tokens);

  EXPECT_FALSE(diagnostic_reporter_.HasWarnings());
  EXPECT_TRUE(diagnostic_reporter_.HasNormalErrors());
  EXPECT_FALSE(diagnostic_reporter_.HasFatalErrors());
}

TEST_F(LexerTest, LexesWithStringLiteralSpanningMultipleLinesOnSourceCode) {}

TEST_F(LexerTest, LexesWithUnterminatedCharLiteralOnSourceCode) {
  std::string file_content_bytes = "s: int = 'a";

  std::vector<Token> output_tokens =
      LexSourceCode("test_unterminated_char_literal.eta", file_content_bytes);

  std::vector<Token> expected_tokens{
      Token{1, 1, TokenType::kIdentifier, {}, "s"},
      Token{1, 2, TokenType::kColon, {}, ":"},
      Token{1, 4, TokenType::kInt, {}, "int"},
      Token{1, 8, TokenType::kEqual, {}, "="},
      Token{1, 12, TokenType::kFileEnd, {}, ""},
  };

  EXPECT_EQ(output_tokens.size(), expected_tokens.size());
  EXPECT_EQ(output_tokens, expected_tokens);

  EXPECT_FALSE(diagnostic_reporter_.HasWarnings());
  EXPECT_TRUE(diagnostic_reporter_.HasNormalErrors());
  EXPECT_FALSE(diagnostic_reporter_.HasFatalErrors());
}

TEST_F(LexerTest, LexesWithEmptyCharLiteralOnSourceCode) {
  std::string file_content_bytes = "s: int = '';";

  std::vector<Token> output_tokens =
      LexSourceCode("test_empty_char_literal.eta", file_content_bytes);

  std::vector<Token> expected_tokens{
      Token{1, 1, TokenType::kIdentifier, {}, "s"},
      Token{1, 2, TokenType::kColon, {}, ":"},
      Token{1, 4, TokenType::kInt, {}, "int"},
      Token{1, 8, TokenType::kEqual, {}, "="},
      Token{1, 12, TokenType::kSemiColon, {}, ";"},
      Token{1, 13, TokenType::kFileEnd, {}, ""},
  };

  EXPECT_EQ(output_tokens.size(), expected_tokens.size());
  EXPECT_EQ(output_tokens, expected_tokens);

  EXPECT_FALSE(diagnostic_reporter_.HasWarnings());
  EXPECT_TRUE(diagnostic_reporter_.HasNormalErrors());
  EXPECT_FALSE(diagnostic_reporter_.HasFatalErrors());
}

TEST_F(LexerTest, LexesWithMultiCharLiteralOnSourceCode) {}
