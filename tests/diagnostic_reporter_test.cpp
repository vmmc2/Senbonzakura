#include "senbonzakura/diagnostic_reporter.hpp"

#include <sstream>

#include <gtest/gtest.h>

TEST(DiagnosticReporterTest, ReportCompilerErrorTest) {
  DiagnosticReporter diagnostic_reporter;

  SourceCodeLocation src_code_location{
      .source_name = "/path/to/file.eta", .line = 1, .column = 1};

  EXPECT_FALSE(diagnostic_reporter.HasWarnings());
  EXPECT_FALSE(diagnostic_reporter.HasNormalErrors());
  EXPECT_FALSE(diagnostic_reporter.HasFatalErrors());

  diagnostic_reporter.ReportCompilerError(
      src_code_location, Severity::kWarning,
      "A variable was declared but not used in the program.");
  EXPECT_TRUE(diagnostic_reporter.HasWarnings());
  EXPECT_FALSE(diagnostic_reporter.HasNormalErrors());
  EXPECT_FALSE(diagnostic_reporter.HasFatalErrors());

  diagnostic_reporter.ReportCompilerError(
      src_code_location, Severity::kError,
      "Missing right operand during the usage of the '+' operator.");
  EXPECT_TRUE(diagnostic_reporter.HasWarnings());
  EXPECT_TRUE(diagnostic_reporter.HasNormalErrors());
  EXPECT_FALSE(diagnostic_reporter.HasFatalErrors());

  diagnostic_reporter.ReportCompilerError(
      src_code_location, Severity::kFatal,
      "Usage of unknown symbol present inside the source file: '@'.");
  EXPECT_TRUE(diagnostic_reporter.HasWarnings());
  EXPECT_TRUE(diagnostic_reporter.HasNormalErrors());
  EXPECT_TRUE(diagnostic_reporter.HasFatalErrors());
}

class DiagnosticReporterOutputTest : public ::testing::Test {
protected:
  std::stringstream captured_cout_;
  std::streambuf *original_cout_buffer_;

  void SetUp() override {
    original_cout_buffer_ = std::cout.rdbuf();
    std::cout.rdbuf(captured_cout_.rdbuf());
  }

  void TearDown() override { std::cout.rdbuf(original_cout_buffer_); }
};

TEST_F(DiagnosticReporterOutputTest, OutputCompilerErrorsPrintsCorrectFormat) {
  DiagnosticReporter diagnostic_reporter;

  diagnostic_reporter.ReportCompilerError(
      SourceCodeLocation{
          .source_name = "/some/path/to/file_1.eta", .line = 1, .column = 2},
      Severity::kWarning, "The variable 'foo' was declared but not used.");
  diagnostic_reporter.ReportCompilerError(
      SourceCodeLocation{
          .source_name = "/some/path/to/file_2.eta", .line = 1, .column = 1},
      Severity::kError, "Invalid symbol present within the source code: '@'.");
  diagnostic_reporter.ReportCompilerError(
      SourceCodeLocation{
          .source_name = "/some/path/to/file_3.eta", .line = 10, .column = 23},
      Severity::kFatal, "Expected a right operand of the binary operator '+'.");

  diagnostic_reporter.OutputCompilerErrors();

  std::string expected_output =
      "\033[31m[Warning] - [Location]:[Source File: '/some/path/to/file_1.eta' "
      "- Line: 1 - Column: 2] - [Message]:The variable 'foo' was declared but "
      "not used.\033[0m\n"
      "\033[31m[Error] - [Location]:[Source File: '/some/path/to/file_2.eta' "
      "- Line: 1 - Column: 1] - [Message]:Invalid symbol present within the "
      "source code: '@'.\033[0m\n"
      "\033[31m[Fatal] - [Location]:[Source File: '/some/path/to/file_3.eta' "
      "- Line: 10 - Column: 23] - [Message]:Expected a right operand of the "
      "binary operator '+'.\033[0m\n";

  EXPECT_EQ(captured_cout_.str(), expected_output);
}

TEST_F(DiagnosticReporterOutputTest, OutputSystemErrorsPrintsCorrectFormat) {}
