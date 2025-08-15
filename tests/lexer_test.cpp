#include "senbonzakura/file_scanner.hpp"
#include "senbonzakura/lexer.hpp"
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

    Lexer lexer{temp_lexer_filepath_, file_scanner.GetFileContentCodepoints(),
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
  std::vector<Token> tokens =
      LexSourceCode("test_single_character_tokens.eta", file_content_bytes);

  EXPECT_EQ(tokens.size(), 20);
  EXPECT_EQ(tokens.back().token_type_, TokenType::kFileEnd);
}
