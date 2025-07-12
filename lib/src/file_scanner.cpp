#include "../include/senbonzakura/file_scanner.hpp"
#include "senbonzakura/diagnostic_reporter.hpp"

#include <filesystem>
#include <format>
#include <fstream>

FileScanner::FileScanner(std::string file_path,
                         DiagnosticReporter &diagnostic_reporter)
    : file_path_(std::move(file_path)),
      diagnostic_reporter_(diagnostic_reporter) {}

void FileScanner::ScanFile() {
  std::filesystem::path path{file_path_};
  if (path.extension() != ".eta") {
    diagnostic_reporter_.Report(
        std::move(SourceCodeLocation{
            .source_name = file_path_, .line = 0, .column = 0}),
        Severity::kFatal,
        std::format(
            "[E]: Could not read the provided file '{}' because it is not an "
            "Eta file. Eta files must have the '.eta' extension.",
            file_path_));
  }

  std::ifstream file{file_path_,
                     std::ios::in | std::ios::binary | std::ios::ate};
  if (!file) {
    diagnostic_reporter_.Report(
        std::move(SourceCodeLocation{
            .source_name = file_path_, .line = 0, .column = 0}),
        Severity::kFatal,
        std::format("[E]: Could not open the provided file '{}'.", file_path_));
  }

  file_content_.resize(file.tellg());
  file.seekg(0, std::ios::beg);
  file.read(file_content_.data(), file_content_.size());
  file.close();

  return;
}

const std::string &FileScanner::GetFileContent() const { return file_content_; }
