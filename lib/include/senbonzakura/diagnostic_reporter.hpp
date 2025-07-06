#pragma once

#include <format>
#include <string>
#include <sstream>
#include <vector>

enum class Severity {
  // Scenarios: unused variable, unreachable code, always true/false condition.
  kWarning,
  // Scenarios: lexical error, syntax error, semantic error (usage of undeclared
  // variable, calling a function with wrong number of arguments), type error
  // (passing an incorrect type to a function parameter during a function call,
  // trying to apply an operation on values of different types).
  kError,
  // Scenarios: specified source code file was not found, lack of permission to
  // read or write to a file, not enough free disk storage, internal compiler
  // error (An assertion has failed. Something that should never happen).
  kFatal,
};

std::string SeverityToString(Severity severity);

template <>
struct std::formatter<Severity, char> : std::formatter<std::string, char> {
  auto format(Severity severity, std::format_context &ctx) const {
    return std::formatter<std::string, char>::format(
        SeverityToString(severity), ctx);
  }
};

struct SourceCodeLocation {
  std::string source_name;
  int line;
  int column;

  friend std::ostream &operator<<(std::ostream &os, const SourceCodeLocation &source_code_location);
};

template <>
struct std::formatter<SourceCodeLocation, char> : std::formatter<std::string, char> {
    auto format(const SourceCodeLocation& loc, auto& ctx) const {
        std::stringstream ss;
        ss << loc;
        return std::formatter<std::string, char>::format(ss.str(), ctx);
    }
};

struct Diagnostic {
  std::string diagnostic_message;
  SourceCodeLocation source_code_location;
  Severity diagnostic_severity;

  friend std::ostream &operator<<(std::ostream &os, const Diagnostic &diagnostic);
};

class DiagnosticReporter {
private:
  std::vector<Diagnostic> diagnostics_;
  int error_count_ = 0;

public:
  void Report(SourceCodeLocation loc, Severity sev, std::string msg);
  bool HasErrors() const;
  void Print() const;
};
