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
  std::stringstream captured_cout;
  std::streambuf *original_cout_buffer;

  void SetUp() override {
    original_cout_buffer = std::cout.rdbuf();
    std::cout.rdbuf(captured_cout.rdbuf());
  }

  void TearDown() override { std::cout.rdbuf(original_cout_buffer); }
};

TEST_F(DiagnosticReporterOutputTest, OutputCompilerErrorsPrintsCorrectFormat) {}

TEST_F(DiagnosticReporterOutputTest, OutputSystemErrorsPrintsCorrectFormat) {}
