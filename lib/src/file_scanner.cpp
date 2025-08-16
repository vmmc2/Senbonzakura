#include "senbonzakura/file_scanner.hpp"
#include "senbonzakura/diagnostic_reporter.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>

FileScanner::FileScanner(std::string file_path,
                         DiagnosticReporter &diagnostic_reporter)
    : file_path_(std::move(file_path)),
      diagnostic_reporter_(diagnostic_reporter) {}

const DiagnosticReporter &FileScanner::GetDiagnosticReporter() const {
  return diagnostic_reporter_;
}

const std::string &FileScanner::GetFileContentBytes() const {
  return file_content_bytes_;
}

const std::string &FileScanner::GetFilePath() const { return file_path_; }

void FileScanner::ScanFile() {
  std::filesystem::path path{file_path_};
  if (path.extension() != ".eta") {
    diagnostic_reporter_.ReportSystemError(
        Severity::kFatal,
        std::format(
            "[E]: Could not read the provided file '{}' because it is not an "
            "Eta file. Eta files must have the '.eta' extension.",
            file_path_));
    return;
  }

  std::ifstream file{file_path_,
                     std::ios::in | std::ios::binary | std::ios::ate};
  if (!file) {
    diagnostic_reporter_.ReportSystemError(
        Severity::kFatal,
        std::format("[E]: Could not open the provided file '{}'.", file_path_));
    return;
  }

  file_content_bytes_.resize(file.tellg());
  file.seekg(0, std::ios::beg);
  file.read(file_content_bytes_.data(), file_content_bytes_.size());
  file.close();

  return;
}
