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
  if (sev == Severity::kError || sev == Severity::kFatal) {
    error_count_++;
  }

  return;
}

bool DiagnosticReporter::HasErrors() const { return error_count_ > 0; }

void DiagnosticReporter::Print() const {
  for (const Diagnostic &curr_diagnostic : diagnostics_) {
    std::cout << std::format("") << std::endl;
  }

  return;
}
