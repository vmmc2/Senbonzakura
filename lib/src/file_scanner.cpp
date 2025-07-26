#include "senbonzakura/file_scanner.hpp"
#include "senbonzakura/diagnostic_reporter.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>

#include <utf8.h>

FileScanner::FileScanner(std::string file_path,
                         DiagnosticReporter &diagnostic_reporter)
    : file_path_(std::move(file_path)),
      diagnostic_reporter_(diagnostic_reporter) {}

const std::string &FileScanner::GetFileContentBytes() const {
  return file_content_bytes_;
}

const std::u32string &FileScanner::GetFileContentCodepoints() const {
  return file_content_codepoints_;
}

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

  file_content_codepoints_.clear();

  try {
    // Used to convert a UTF-8 byte sequence to a char32_t (UTF-32) sequence.
    utf8::utf8to32(file_content_bytes_.begin(), file_content_bytes_.end(),
                   std::back_inserter(file_content_codepoints_));
  } catch (const utf8::invalid_utf8 &e) {
    // The byte sequence is not valid UTF-8.
    diagnostic_reporter_.ReportSystemError(
        Severity::kFatal,
        std::format("[E]: Invalid UTF-8 sequence in file '{}'. "
                    "Ensure your source file is saved with UTF-8 encoding.",
                    file_path_));
  } catch (const utf8::not_enough_room &e) {
    // The UTF-8 byte sequence is incomplete (example: The file terminates in
    // the middle of a character).
    diagnostic_reporter_.ReportSystemError(
        Severity::kFatal,
        std::format("[E]: Incomplete UTF-8 sequence at end of the file '{}'."
                    "Ensure your source file is saved with UTF-8 encoding.",
                    file_path_));
  } catch (const std::exception &e) {
    diagnostic_reporter_.ReportSystemError(
        Severity::kFatal,
        std::format("[E]: An unknown error has occurred while trying to decode "
                    "the UTF-8 content of the file '{}' to UTF-32 code points.",
                    file_path_));
  }

  return;
}
