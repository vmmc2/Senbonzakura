#pragma once

#include <string>

#include "senbonzakura/diagnostic_reporter.hpp"

class FileScanner {
private:
  std::string file_path_;
  std::string file_content_bytes_;
  DiagnosticReporter &diagnostic_reporter_;

public:
  FileScanner(std::string file_path, DiagnosticReporter &diagnostic_reporter);

  const DiagnosticReporter &GetDiagnosticReporter() const;
  const std::string &GetFileContentBytes() const;
  const std::string &GetFilePath() const;
  void ScanFile();
};
