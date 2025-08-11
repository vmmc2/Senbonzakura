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
