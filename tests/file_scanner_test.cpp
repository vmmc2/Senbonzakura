#include "senbonzakura/file_scanner.hpp"

#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>

class FileScannerTest : public ::testing::Test {
protected:
  std::string temp_dir_path_;
  DiagnosticReporter diagnostic_reporter_;

  void SetUp() override {
    temp_dir_path_ = std::filesystem::temp_directory_path().string() +
                     "/test_filescanner_dir";
    std::filesystem::create_directory(temp_dir_path_);
  }

  void TearDown() override { std::filesystem::remove_all(temp_dir_path_); }

  void CreateTestFile(const std::string &filename, const std::string &content) {
    std::ofstream file{temp_dir_path_ + "/" + filename, std::ios::binary};
    file << content;
  }
};

TEST_F(FileScannerTest, FileScannerConstructorTest) {
  std::string file_path = "/some/path/to/a/file.eta";

  FileScanner file_scanner{file_path, diagnostic_reporter_};

  EXPECT_EQ(file_scanner.GetFilePath(), file_path);
}
