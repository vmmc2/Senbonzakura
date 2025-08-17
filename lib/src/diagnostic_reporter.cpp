#include "senbonzakura/diagnostic_reporter.hpp"

#include <format>
#include <iostream>
#include <string>

std::string SeverityToString(Severity severity) {
  static const std::string severity_strings[] = {"Warning", "Error", "Fatal"};

  return severity_strings[static_cast<int>(severity)];
}

std::ostream &operator<<(std::ostream &os,
                         const SourceCodeLocation &source_code_location) {
  os << std::format("[Source File: '{}' - Line: {} - Column: {}]",
                    source_code_location.source_name, source_code_location.line,
                    source_code_location.column);
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const CompilerError &compiler_error) {
  os << std::format("[{}] - [Location]:{} - [Message]:{}",
                    compiler_error.compiler_error_severity,
                    compiler_error.source_code_location,
                    compiler_error.compiler_error_message);
  return os;
}

std::ostream &operator<<(std::ostream &os, const SystemError &system_error) {
  os << std::format("[{}] - [Message]:{}", system_error.system_error_severity,
                    system_error.system_error_message);
  return os;
}

void DiagnosticReporter::ReportCompilerError(SourceCodeLocation loc,
                                             Severity sev, std::string msg) {
  compiler_errors_.emplace_back(CompilerError(std::move(msg), loc, sev));
  if (sev == Severity::kWarning) {
    warning_count_++;
  } else if (sev == Severity::kError) {
    normal_error_count_++;
  } else {
    fatal_error_count_++;
  }

  return;
}

void DiagnosticReporter::ReportSystemError(Severity sev, std::string msg) {
  system_errors_.emplace_back(SystemError(std::move(msg), sev));
  if (sev == Severity::kWarning) {
    warning_count_++;
  } else if (sev == Severity::kError) {
    normal_error_count_++;
  } else {
    fatal_error_count_++;
  }

  return;
}

bool DiagnosticReporter::HasFatalErrors() const {
  return fatal_error_count_ > 0;
}

bool DiagnosticReporter::HasNormalErrors() const {
  return normal_error_count_ > 0;
}

bool DiagnosticReporter::HasWarnings() const { return warning_count_ > 0; }

void DiagnosticReporter::OutputCompilerErrors() const {
  for (const CompilerError &curr_compiler_error : compiler_errors_) {
    std::cout << kAnsiRed << curr_compiler_error << kAnsiReset << std::endl;
  }

  return;
}

void DiagnosticReporter::OutputSystemErrors() const {
  for (const SystemError &curr_system_error : system_errors_) {
    std::cout << kAnsiRed << curr_system_error << kAnsiReset << std::endl;
  }

  return;
}
