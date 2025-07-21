#pragma once

#include <string>

#include "senbonzakura/diagnostic_reporter.hpp"

class FileScanner {
private:
  std::string file_path_;
  std::string file_content_bytes_;
  std::u32string file_content_codepoints_;
  DiagnosticReporter &diagnostic_reporter_;

public:
  FileScanner(std::string file_path, DiagnosticReporter &diagnostic_reporter);

  void ScanFile();

  const std::string &GetFileContentBytes() const;
  const std::u32string &GetFileContentCodepoints() const;
};
