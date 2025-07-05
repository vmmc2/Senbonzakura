#pragma once

#include <string>

class FileScanner {
  private:
    std::string file_path_{""};
    std::string file_content_{""};
  public:
    explicit FileScanner(std::string file_path);

    void ScanFile();

    const std::string& GetFileContent() const;
};
