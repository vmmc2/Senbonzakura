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

TEST_F(FileScannerTest, FileScannerScanFileSuccessTest) {
  std::string filename = "test_1.eta";
  std::string file_content = "x: int = 42;";

  CreateTestFile(filename, file_content);

  FileScanner file_scanner{temp_dir_path_ + "/" + filename,
                           diagnostic_reporter_};

  file_scanner.ScanFile();

  auto diagnostic_reporter = file_scanner.GetDiagnosticReporter();

  EXPECT_FALSE(diagnostic_reporter.HasWarnings());
  EXPECT_FALSE(diagnostic_reporter.HasNormalErrors());
  EXPECT_FALSE(diagnostic_reporter.HasFatalErrors());
}

TEST_F(FileScannerTest, FileScannerScanFileFailureFileExtensionTest) {
  std::string invalid_filename = "test_1.txt";
  std::string file_content = "foo bar";

  CreateTestFile(invalid_filename, file_content);

  FileScanner file_scanner{temp_dir_path_ + "/" + invalid_filename,
                           diagnostic_reporter_};

  file_scanner.ScanFile();

  auto diagnostic_reporter = file_scanner.GetDiagnosticReporter();

  EXPECT_TRUE(diagnostic_reporter.HasFatalErrors());
}

TEST_F(FileScannerTest, FileScannerScanFileFailureOpenFileFTest) {
  std::string non_existent_file = "non_existent.eta";

  FileScanner file_scanner{non_existent_file, diagnostic_reporter_};
  file_scanner.ScanFile();

  auto diagnostic_reporter = file_scanner.GetDiagnosticReporter();

  EXPECT_TRUE(diagnostic_reporter.HasFatalErrors());
}
