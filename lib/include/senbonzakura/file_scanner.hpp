#pragma once

#include "diagnostic_reporter.hpp"
#include <string>

class FileScanner {
  private:
    std::string file_path_{""};
    std::string file_content_{""};
    DiagnosticReporter& diagnostic_reporter_;

  public:
    FileScanner(std::string file_path, DiagnosticReporter& diagnostic_reporter);

    void ScanFile();

    const std::string& GetFileContent() const;
};
