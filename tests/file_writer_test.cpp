#include "senbonzakura/file_writer.hpp"
#include "senbonzakura/token.hpp"
#include "senbonzakura/token_type.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <gtest/gtest.h>

class FileWriterTest : public ::testing::Test {
protected:
  // Temporary directory for the tests.
  std::string temp_output_dir_path_;

  void SetUp() override {
    // Unique name for the temporary directory.
    temp_output_dir_path_ =
        std::filesystem::temp_directory_path().string() + "/test_output_dir";
    std::filesystem::create_directory(temp_output_dir_path_);
  }

  void TearDown() override {
    // Clean the temporary directory and its contents.
    std::filesystem::remove_all(temp_output_dir_path_);
  }
};

TEST_F(FileWriterTest, FileWriterConstructorTest) {
  std::vector<std::string> programs_filepaths{
      "/some/path/to/file_1.eta", "/this/is/another/path/to/a/file_2.eta",
      "/a/third/path/to/yet/another/file_3.eta"};
  FileWriter file_writer{programs_filepaths, temp_output_dir_path_};

  EXPECT_EQ(file_writer.GetProgramFilepaths(), programs_filepaths);
  EXPECT_EQ(file_writer.GetOutputDirectory(), temp_output_dir_path_);
}

TEST_F(FileWriterTest, WriteLexerOutputSuccessTest) {
  std::vector<std::string> programs_filepaths{"/home/user/test_1.eta"};
  FileWriter file_writer{programs_filepaths, temp_output_dir_path_};

  std::vector<Token> tokens{
      Token{1, 1, TokenType::kIdentifier, {}, "foo"},
      Token{1, 4, TokenType::kColon, {}, ":"},
      Token{1, 5, TokenType::kInt, {}, "int"},
      Token{1, 8, TokenType::kEqual, {}, "="},
      Token{1, 9, TokenType::kInteger, 9, "9"},
      Token{1, 10, TokenType::kFileEnd, {}, ""},
  };

  file_writer.WriteLexerOutput(programs_filepaths[0], tokens);

  std::filesystem::path output_file_path =
      std::filesystem::path(temp_output_dir_path_) / "test_1.eta.lexed";

  ASSERT_TRUE(std::filesystem::exists(output_file_path));

  std::ifstream input_file_stream{output_file_path};
  std::string produced_output{
      (std::istreambuf_iterator<char>(input_file_stream)),
      std::istreambuf_iterator<char>()};

  std::string expected_output = "[001:001] Type: kIdentifier - Lexeme: "
                                "foo\n[001:004] Type: kColon - Lexeme: "
                                ":\n[001:005] Type: kInt - Lexeme: "
                                "int\n[001:008] Type: kEqual - Lexeme: "
                                "=\n[001:009] Type: kInteger - Value: "
                                "9\n[001:010] Type: kFileEnd - Lexeme: \n";

  EXPECT_EQ(produced_output, expected_output);
}

TEST_F(FileWriterTest, WriteLexerOutputFailureOutputDirPathDoesntExistTest) {
  std::vector<std::string> programs_filepaths{"test_1.eta"};

  FileWriter file_writer{programs_filepaths,
                         "/output/dir/path/that/doesnt/exist"};

  std::vector<Token> tokens;

  EXPECT_THROW(file_writer.WriteLexerOutput(programs_filepaths[0], tokens),
               std::runtime_error);
}

TEST_F(FileWriterTest, WriteLexerOutputFailureOutputDirPathIsNotDirectoryTest) {
  std::string temp_file_path =
      std::filesystem::temp_directory_path().string() + "/temp_file.txt";
  std::ofstream(temp_file_path).close();

  std::vector<std::string> programs_filepaths{"file.eta"};
  FileWriter file_writer{programs_filepaths, temp_file_path};
  std::vector<Token> tokens;

  ASSERT_THROW(file_writer.WriteLexerOutput(programs_filepaths[0], tokens),
               std::runtime_error);

  std::filesystem::remove(temp_file_path);
}
