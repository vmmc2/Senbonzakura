#pragma once

#include <string>
#include <vector>

#include "senbonzakura/token.hpp"

class FileWriter {
private:
  const std::string output_directory_;
  const std::vector<std::string> &eta_programs_filepaths_;

public:
  FileWriter(const std::vector<std::string> &eta_programs_filepaths,
             const std::string output_directory = "");
  void WriteLexerOutput(const std::string &current_eta_filepath_str,
                        std::vector<Token> tokens) const;
};
