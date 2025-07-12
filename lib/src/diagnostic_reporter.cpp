#include "../include/senbonzakura/diagnostic_reporter.hpp"

#include <format>
#include <iostream>

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

std::ostream &operator<<(std::ostream &os, const Diagnostic &diagnostic) {
  os << std::format(
      "[{}] - [Location]:{} - [Message]:{}", diagnostic.diagnostic_severity,
      diagnostic.source_code_location, diagnostic.diagnostic_message);
  return os;
}

void DiagnosticReporter::Report(SourceCodeLocation loc, Severity sev,
                                std::string msg) {
  diagnostics_.push_back(Diagnostic(std::move(msg), loc, sev));
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

void DiagnosticReporter::PrintDiagnostic() const {
  for (const Diagnostic &curr_diagnostic : diagnostics_) {
    std::cout << curr_diagnostic << std::endl;
  }

  return;
}
