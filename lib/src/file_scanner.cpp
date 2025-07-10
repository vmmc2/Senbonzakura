#include "../include/senbonzakura/file_scanner.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <stdexcept>

FileScanner::FileScanner(std::string file_path)
    : file_path_(std::move(file_path)) {}

void FileScanner::ScanFile() {
  std::filesystem::path path{file_path_};
  if (path.extension() != ".eta") {
    throw std::runtime_error{std::format(
        "[E]: Could not read the provided file '{}' because it is not an "
        "Eta file. Eta files must have the '.eta' extension.",
        file_path_)};
  }

  std::ifstream file{file_path_,
                     std::ios::in | std::ios::binary | std::ios::ate};
  if (!file) {
    throw std::runtime_error{
        std::format("[E]: Could not open the provided file '{}'.", file_path_)};
  }

  file_content_.resize(file.tellg());
  file.seekg(0, std::ios::beg);
  file.read(file_content_.data(), file_content_.size());
  file.close();

  return;
}

const std::string &FileScanner::GetFileContent() const { return file_content_; }
